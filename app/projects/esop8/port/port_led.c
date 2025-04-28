#include "include.h"

extern led_func_t bled_func;
extern led_func_t rled_func;
extern gpio_t bled_gpio;
extern gpio_t rled_gpio;

void user_led_cfg_set_on(gpio_t *g);
void user_led_cfg_set_off(gpio_t *g);
void user_led2_port_init(gpio_t *g);
void user_bled2_set_on(gpio_t *g);
void user_bled2_set_off(gpio_t *g);
void user_rled2_set_on(gpio_t *g);
void user_rled2_set_off(gpio_t *g);
void led_xcfg_init(void);

//AT(.text.led_disp)
//void led_power_down(void)
//{
//    led_cb.flag &= ~LED_TOG_LBAT;               //关掉低电闪灯状态
//    sys_led.led_type = T_LED_PWROFF;
//#if LINEIN_2_PWRDOWN_EN
//    if ((xcfg_cb.linein_2_pwrdown_en) && (!LINEIN_2_PWRDOWN_TONE_EN)) {
//        if (device_is_online(DEV_LINEIN)) {
//            led_set_sta(0x00, 0x00, 1, 0);      //蓝灯灭， 红灯灭
//            return;
//        }
//    }
//#endif // LINEIN_2_PWRDOWN_EN
//
//    led_cfg_set_sta((led_cfg_t *)&xcfg_cb.led_poweroff);
//}
//
////等待关机闪灯结束再关机
//AT(.text.led_disp)
//void led_power_down_w4_done(void)
//{
//    u32 timeout, ticks;
//    if (sys_led.led_type == T_LED_PWROFF && led_cb.period == 0xff) {
//        timeout = (u32)led_cb.unit * 50 * 8;   //最长需要多长时间一轮
//        ticks = tick_get();
//        while (led_cb.rled_sta || led_cb.bled_sta) {
//            if (tick_check_expire(ticks, timeout)) {
//                break;
//            }
//            WDT_CLR();
//        }
//    }
//}

void led_func_init(void)
{
//    printf("%s\n", __func__);
    u32 bled_io_sel = xcfg_cb.bled_io_sel;
    u32 rled_io_sel = xcfg_cb.rled_io_sel;

    memset(&sys_led, 0, sizeof(sys_led));
    if (bled_io_sel == 1) {                     //MICIN, 非通话状态点蓝灯
        bled_io_sel = 7;                        //PF2控制灯
        sys_led.mic2led_en = 1;
    }
    if (rled_io_sel == 1) {                     //DACL充电状态点红灯
        rled_io_sel = 12;
        sys_led.dac2led_en = 1;
    }
    led_xcfg_init();
    if (rled_io_sel == xcfg_cb.mute_io_sel && xcfg_cb.spk_mute_en) {   //充电红灯与MUTE脚同一个IO
        if (sys_led.charge_rled_en) {
            sys_cb.rled_mux_mute_en = 1;
        }
        rled_io_sel = 0;
    }
#if !LED_PWR_EN
    rled_io_sel = 0;
#endif
    bsp_gpio_cfg_init(&bled_gpio, configure_gpio_tbl[bled_io_sel]);
    bsp_gpio_cfg_init(&rled_gpio, configure_gpio_tbl[rled_io_sel]);
    if (bled_io_sel != rled_io_sel) {
        if (sys_led.mic2led_en) {
            bled_func.port_init = bled_micl_set_off;
            bled_func.set_on = bled_micl_set_on;
            bled_func.set_off = bled_micl_set_off;
        } else {
#if USER_LED_GPIO
            bled_func.port_init = user_led_cfg_set_off;
            bled_func.set_on = user_led_cfg_set_on;
            bled_func.set_off = user_led_cfg_set_off;
#else
            bled_func.port_init = led_cfg_set_off;
            bled_func.set_on = led_cfg_set_on;
            bled_func.set_off = led_cfg_set_off;
#endif // USER_LED_GPIO
        }
        if (sys_led.dac2led_en) {
            rled_func.port_init = rled_dac_set_off;
            rled_func.set_on = rled_dac_set_on;
            rled_func.set_off = rled_dac_set_off;
        } else {
#if USER_LED_GPIO
            rled_func.port_init = user_led_cfg_set_off;
            rled_func.set_on = user_led_cfg_set_on;
            rled_func.set_off = user_led_cfg_set_off;
#else
            rled_func.port_init = led_cfg_set_off;
            rled_func.set_on = led_cfg_set_on;
            rled_func.set_off = led_cfg_set_off;
#endif // USER_LED_GPIO
        }
    } else {
        //1个IO推两个灯
        if (bled_gpio.sfr != NULL) {
            sys_led.port2led_en = 1;
        }
#if USER_LED_GPIO
        bled_func.port_init = user_led2_port_init;
        bled_func.set_on = user_bled2_set_on;
        bled_func.set_off = user_bled2_set_off;

        rled_func.port_init = user_led2_port_init;
        rled_func.set_on = user_rled2_set_on;
        rled_func.set_off = user_rled2_set_off;
#else
        bled_func.port_init = led2_port_init;
        bled_func.set_on = bled2_set_on;
        bled_func.set_off = bled2_set_off;

        rled_func.port_init = led2_port_init;
        rled_func.set_on = rled2_set_on;
        rled_func.set_off = rled2_set_off;
#endif // USER_LED_GPIO
    }
}

