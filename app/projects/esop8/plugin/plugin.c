#include "include.h"

u8 huart_rxbuf[512] AT(.huart_buf);
extern uint8_t cfg_vbat_capless_en;

void huart_init_do(u8 tx_port, u8 rx_port, u32 baud_rate, u8 *buf, u16 buf_size);
void plugin_var_init_m(void);

void plugin_init(void)
{

}

AT(.text.eq)
void plugin_music_eq(void)
{
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
    plugin_var_init_m();

#if IODM_TEST_MODE
    xcfg_cb.chbox_smart_en             = 1;
#endif

#if ANC_EN
    xcfg_cb.dac_dnr_en = 0;
#endif

    EARPHONE_DETECT_INIT();

    led_func_init();

    if (xcfg_cb.user_cap) {
        xcfg_cb.osc_cap = xcfg_cb.uosc_cap | ((u8)xcfg_cb.uosc_both_cap << 6);
    }
}

