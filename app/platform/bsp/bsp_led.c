#include "include.h"
#include "bsp_led.h"

#if LED_DISP_EN
extern led_cb_t led_cb;
extern led_cb_t led_bak;
extern sys_led_t sys_led;
void bt_set_sync_info(u8 *buf);
void speaker_unmute_charge_rled_on(void);
void port_2bled_on(gpio_t *g);
void port_2rled_on(gpio_t *g);
void port_2led_off(gpio_t *g);
void pwm_led_func_set(void);
void pwm_led_stop(void);

////50ms调用周期
//AT(.com_text.led_disp)
//void led_scan(void)
//{
//    uint bcnt;
//
//    if (sys_led.scan_dis) {                         //关LED扫描
//        return;
//    }
//    if (led_cb.wait) {
//        led_cb.wait--;
//        return;
//    }
//
//    //等待间隔时间
//    if (led_cb.unit) {
//        bcnt = led_cb.cnt / led_cb.unit;
//    } else {
//        bcnt = led_cb.cnt;
//    }
//    if (bcnt <= 7) {
//        if (led_cb.bled_sta & BIT(bcnt)) {
//            bled_set_on();
//        } else {
//            bled_set_off();
//        }
//        if (led_cb.rled_sta & BIT(bcnt)) {
//            rled_set_on();
//        } else {
//            rled_set_off();
//        }
//    }
//    if (led_cb.cnt < 0xffff) {
//        led_cb.cnt++;
//        if (led_cb.cnt > led_cb.circle) {
//            led_cb.cnt = 0;
//        }
//        if (led_cb.period == 0xff) {            //只循环一次
//            if (bcnt && bcnt <= 8) {
//                led_cb.bled_sta &= ~BIT(bcnt - 1);
//                led_cb.rled_sta &= ~BIT(bcnt - 1);
//            }
//        }
//    }
//}

#if USER_PWM_LED_EN
AT(.com_text.led_disp)
void led_cfg_pwm_init(gpio_t *g)
{
    sys_cb1.pwm_bled_on = 0;
    pwm_led_func_set();
}

AT(.com_text.led_disp)
void led_cfg_pwm_set_on(gpio_t *g)
{
    sys_cb1.pwm_bled_on = 1;
    pwm_led_start();
}

AT(.com_text.led_disp)
void led_cfg_pwm_set_off(gpio_t *g)
{
    sys_cb1.pwm_bled_on = 0;
    pwm_led_stop();
}

AT(.com_text.led_disp)
void rled_cfg_pwm_init(gpio_t *g)
{
    sys_cb1.pwm_rled_on = 0;
    pwm_led_func_set();
}

AT(.com_text.led_disp)
void rled_cfg_pwm_set_on(gpio_t *g)
{
    sys_cb1.pwm_rled_on = 1;
    pwm_led_start();
}

AT(.com_text.led_disp)
void rled_cfg_pwm_set_off(gpio_t *g)
{
    sys_cb1.pwm_rled_on = 0;
    pwm_led_stop();
}
#endif

#if PORT_2LED_FIX_EN
AT(.com_text.led_disp)
void bled2_set_on_fix(gpio_t *g)
{
    if ((g == NULL) || (g->sfr == NULL)) {
        return;
    }

    if (!(sys_led.port2led_sta & BIT(0))) {
        port_2bled_on(g);
        sys_led.port2led_sta |= BIT(0);
    }
}

AT(.com_text.led_disp)
void bled2_set_off_fix(gpio_t *g)
{
    if ((g == NULL) || (g->sfr == NULL)) {
        return;
    }

    if (sys_led.port2led_sta & BIT(0)) {
        port_2led_off(g);
        sys_led.port2led_sta &= ~BIT(0);
    }
}

AT(.com_text.led_disp)
void rled2_set_on_fix(gpio_t *g)
{
    if ((g == NULL) || (g->sfr == NULL)) {
        return;
    }

    if (!(sys_led.port2led_sta & BIT(1))) {
        port_2rled_on(g);
        sys_led.port2led_sta |= BIT(1);
    }
}

AT(.com_text.led_disp)
void rled2_set_off_fix(gpio_t *g)
{
    if ((g == NULL) || (g->sfr == NULL)) {
        return;
    }

    if (sys_led.port2led_sta & BIT(1)) {
        port_2led_off(g);
        sys_led.port2led_sta &= ~BIT(1);
    }
}
#endif // PORT_2LED_FIX_EN

//充电红灯亮
AT(.com_text.led_disp)
void charge_led_on(void)
{
    if (sys_led.charge_rled_en) {
        speaker_unmute_charge_rled_on();
        LED_PWR_SET_ON();
    }
    if (sys_led.charge_bled_en) {
        LED_SET_ON();                   //充电过程中同时亮红,蓝灯
    }
#if LED_BREATHE_EN
    if (sys_led.charge_bre_en) {
        breathe_led_start(T_BRE_CHARGE, sys_led.charge_bre_cfg);
    }
#endif
}

#if USER_LED_GPIO

#define USER_GPIO_PU_SEL    GPIOxPU             //可选上拉电阻: GPIOxPU, GPIOxPU200K, GPIOxPU300
#define USER_GPIO_PD_SEL    GPIOxPD             //可选上拉电阻: GPIOxPD, GPIOxPD200K, GPIOxPD300

AT(.com_text.led_disp)
void user_led_cfg_set_on(gpio_t *g)
{
    led_cfg_set_on(g);
    if (!(g->type)) {
        g->sfr[GPIOxPU300] &= ~BIT(g->num);     //固定去掉上拉300K，不要修改
        g->sfr[USER_GPIO_PU_SEL] |= BIT(g->num);
    }
}

AT(.com_text.led_disp)
void user_led_cfg_set_off(gpio_t *g)
{
    led_cfg_set_off(g);
    if (!(g->type)) {
        g->sfr[USER_GPIO_PU_SEL] &= ~BIT(g->num);
    }
}

//一个IO口推两个LED灯
void user_led2_port_init(gpio_t *g)
{
    led2_port_init(g);
    g->sfr[USER_GPIO_PU_SEL] &= ~BIT(g->num);
    g->sfr[USER_GPIO_PD_SEL] &= ~BIT(g->num);
}

AT(.com_text.led_disp)
void user_led2_set_off(gpio_t *g)
{
    if (!(g->type)) {
        g->sfr[USER_GPIO_PU_SEL] &= ~BIT(g->num);
        g->sfr[USER_GPIO_PD_SEL] &= ~BIT(g->num);
    }
}

//蓝灯亮，红灯灭
AT(.com_text.led_disp)
void user_bled2_set_on(gpio_t *g)
{
#if PORT_2LED_FIX_EN
    bled2_set_on_fix(g);
#else
    bled2_set_on(g);
#endif
    if (!(g->type)) {
        g->sfr[GPIOxPD300] &= ~BIT(g->num);     //固定去掉下拉300K，不要修改
        g->sfr[USER_GPIO_PD_SEL] |= BIT(g->num);
    }
}

AT(.com_text.led_disp)
void user_bled2_set_off(gpio_t *g)
{
#if PORT_2LED_FIX_EN
    bled2_set_off_fix(g);
#else
    bled2_set_off(g);
#endif
    user_led2_set_off(g);
}

AT(.com_text.led_disp)
void user_rled2_set_on(gpio_t *g)
{
#if PORT_2LED_FIX_EN
    rled2_set_on_fix(g);
#else
    rled2_set_on(g);
#endif
    if (!(g->type)) {
        g->sfr[GPIOxPU300] &= ~BIT(g->num);     //固定去掉上拉300K，不要修改
        g->sfr[USER_GPIO_PU_SEL] |= BIT(g->num);
    }
}

AT(.com_text.led_disp)
void user_rled2_set_off(gpio_t *g)
{
#if PORT_2LED_FIX_EN
    rled2_set_off_fix(g);
#else
    rled2_set_off(g);
#endif
    user_led2_set_off(g);
}
#endif // USER_LED_GPIO
#endif
