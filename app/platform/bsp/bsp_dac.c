#include "include.h"
#include "bsp_dac.h"

extern const u8 *dac_dvol_table;
extern const u16 dig_vol_tbl[61];
extern const u8 dac_dvol_tbl_16[16 + 1];
extern const u8 dac_dvol_tbl_32[32 + 1];
void dac_channel_configure(void);
bool music_soft_gain_check(void);

AT(.text.bsp.dac)
void bsp_change_volume_hook(u8 vol)
{
    u8 level = 0;
    sys_cb.vol_set = vol;
#if VUSB_SMART_VBAT_HOUSE_EN
    if (vhouse_cb.ear_mute_flag) {
        return;
    }
#endif

#if BT_ALG_DBB_EN
    if (music_soft_gain_check()) {
        return;
    }
#endif
    if (vol <= VOL_MAX) {
        level = dac_dvol_table[vol] + sys_cb.gain_offset;
        if (level > 60) {
            level = 60;
        }
        dac_set_dvol(dig_vol_tbl[level]);
    }
}


AT(.text.dac)
void dac_set_anl_offset(u8 bt_call_flag)
{
    if (bt_call_flag) {
        sys_cb.gain_offset = BT_CALL_MAX_GAIN;
    } else {
        sys_cb.gain_offset = DAC_MAX_GAIN;
    }
}

AT(.text.bsp.dac)
void dac_init(void)
{
    u32 cic_gain = xcfg_cb.dac_cic_gain + 1;

    if (sys_cb.vol_max == 16) {
        dac_dvol_table = dac_dvol_tbl_16;
    } else {
        dac_dvol_table = dac_dvol_tbl_32;
    }
    dac_set_anl_offset(0);
    DACDIGCON1 = (DACDIGCON1 & ~(0x3f << 26)) | (cic_gain << 26);

//    printf("[%s] vol_max:%d, offset: %d\n", __func__, sys_cb.vol_max, sys_cb.gain_offset);
#if LINEIN_2_PWRDOWN_EN
    if (sys_cb.sleep_dac_en) {
        dac_power_off_restore_daclr();
    }
#endif
    dac_obuf_init();
    dac_power_on();
    if (DAC_OUT_SPR == DAC_OUT_48K) {
        DACDIGCON0 |= BIT(1);                                                   //dac out sample 48K
    }
    if (xcfg_cb.dac_channel_exchange_en) {
        dac_channel_exchange();
    }
#if WARING_WAV_VOL_OWN_EN
    dac_msc_vol_en();
#endif

    plugin_music_eq();

#if DAC_DNR_EN
    dac_dnr_init(2, 0x18, 90, 0x10);
#endif
}

////处理audio bypass时淡出收得不好的问题
//void dac_fade_out_wait(void)
//{
//    dac_unmute_set_delay(30);
//    dac_wait_vol_match();
//}

