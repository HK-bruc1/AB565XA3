#include "include.h"

u8 huart_rxbuf[512] AT(.huart_buf);
extern uint8_t cfg_vbat_capless_en;

void huart_init_do(u8 tx_port, u8 rx_port, u32 baud_rate, u8 *buf, u16 buf_size);
void plugin_var_init_m(void);

void plugin_init(void)
{
#if CHARGE_DELAY_OPEN_LEAKAGE_EN
    xcfg_cb.chg_inbox_pwrdwn_en = 0;           //关闭入仓关机
    bsp_charge_leakage_set(0, 0);               //开机关掉漏电流
#endif
}

AT(.text.eq)
void plugin_music_eq(void)
{
    eq_check_res();
    sys_cb.eq_mode = 0;
    music_set_eq_by_num(sys_cb.eq_mode);
    music_set_drc_by_res(RES_BUF_EQ_DAC_DRC, RES_LEN_EQ_DAC_DRC);
}

#if BT_FCC_TEST_EN
//fcc huart init
void huart_init(u8 io_sel)
{
    huart_init_do(io_sel, io_sel, 1500000, huart_rxbuf, 512);
}
#endif

void plugin_var_init(void)
{
#if (USER_TKEY_PB5_DISABLE || USER_TKEY_PWRKEY_EN)
    xcfg_cb.user_tkey = 0;                          //关掉wko的TouchKey功能
#endif

    plugin_var_init_m();

#if IODM_TEST_MODE
    xcfg_cb.chbox_smart_en             = 1;
#endif

#if ANC_EN
    xcfg_cb.dac_dnr_en = 0;
#endif

    EARPHONE_DETECT_INIT();

    led_func_init();

#if !SYS_SUPPORT_DONGLE_EN
    if (xcfg_cb.user_cap) {
        xcfg_cb.osc_cap = xcfg_cb.uosc_cap | ((u8)xcfg_cb.uosc_both_cap << 6);
    }

#else
    if (xcfg_cb.user_cap) {
        xcfg_cb.mosc_cap = xcfg_cb.uosc_cap | ((u8)xcfg_cb.uosc_both_cap << 6);
    }else{
        xcfg_cb.mosc_cap = xcfg_cb.osc_cap;
    }
#endif

}

