#include "include.h"

extern param_cb_t param_cb;

u32 bsp_get_rand_key(void);
void cm_param_init(void);
void param_rtcram_init(void);
void param_bt_new_name_clr(void);

AT(.text.bsp.param)
void param_init(bool reset)
{
    sys_cb.rand_seed = bsp_get_rand_key();
    param_rtcram_init();
    cm_param_init();

    param_sys_vol_read();
    if ((SYS_RECOVER_INITVOL == 0) || (sys_cb.vol > VOL_MAX)) {
        sys_cb.vol = SYS_INIT_VOLUME + 1;                   //上电还原到默认音量
    } else {
        if (sys_cb.vol < ((u8)SYS_RECOVER_INITVOL + 1)) {
            sys_cb.vol = SYS_RECOVER_INITVOL + 1;
        }
    }
    sys_cb.hfp_vol = 15;

    if (xcfg_cb.lang_id >= LANG_EN_ZH) {
       param_lang_read();
    } else {
        sys_cb.lang_id = xcfg_cb.lang_id;
    }

#if BT_LOCAL_ADDR
    param_local_random_addr_read();
#endif

    param_bt_new_name_clr();
}

void param_bt_new_name_read(char *buf, uint size)
{
	param_read((u8 *)buf, PARAM_NEW_LOCAL_NAME, 64);
}

void param_bt_new_name_write(char *buf, uint size)
{
	param_write((u8 *)buf, PARAM_NEW_LOCAL_NAME, 64);
	param_sync();
}

void param_bt_new_name_clr(void)
{
    u16 temp = 0;
    param_write((u8 *)&temp, PARAM_NEW_LOCAL_NAME, 2);
    param_sync();
}

WEAK void param_bt_qpdn_boat_flag_write(u8 flag)
{
	param_write(&flag, PARAM_BT_QPDN_BOAT_FLAG, 1);
	param_sync();
}

WEAK void param_bt_qpdn_boat_flag_read(void)
{
	param_read(&sys_cb.pdn_boat_flag, PARAM_BT_QPDN_BOAT_FLAG, 1);
}

WEAK void param_bt_qpaird_id_write(u8* buf)
{
    param_write(buf, PARAM_BT_QPAIRED_ID, 5);
    param_sync();
}


WEAK u8 param_bt_qpaird_id_read(void)
{
    u8 buf[5];
    u8* ptr = buf;
    param_read(ptr, PARAM_BT_QPAIRED_ID, 5);
    u32 bt_tws_ext_name_key = GET_LE32(ptr + 1);
    if((ptr[0] == 1) && ((xcfg_cb.bt_tws_ext_nane_en) || (bt_tws_ext_name_key == 0x11223344))){
        xcfg_cb.bt_tws_ext_nane_en = 1;
        xcfg_cb.bt_tws_ext_name = GET_LE16(ptr + 1);
        return 0;
    }
   return 0;
}

WEAK void pram_bt_set_tws_qtest_info(u8 *buf)
{
    param_write(buf, PAGE2(PARAM_BT_TWS_QTEST_INFO), 28);
    param_sync();
}


WEAK u8 pram_bt_get_tws_qtest_info(u8 *buf)
{
     param_read(buf, PAGE2(PARAM_BT_TWS_QTEST_INFO), 28);
     return true;
}
