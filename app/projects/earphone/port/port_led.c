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
u32 bt_tws_master_clock_get(void);
void led_cfg_pwm_init(gpio_t *g);
void led_cfg_pwm_set_on(gpio_t *g);
void led_cfg_pwm_set_off(gpio_t *g);
void rled_cfg_pwm_init(gpio_t *g);
void rled_cfg_pwm_set_on(gpio_t *g);
void rled_cfg_pwm_set_off(gpio_t *g);

#if BT_TWS_SLEEP_LED_SYNC_EN

#define SYNC_TINE             40        //休眠闪灯，单位500ms

AT(.text.sleep_com.timer)
bool led_bt_sleep_tws_sync(void)
{
    if (bt_tws_is_connected()) {
        u32 clock = bt_tws_master_clock_get() / 1600;
        if ((clock % SYNC_TINE) == 0) {
            LED_INIT();
            bled_set_on();
            return true;
        } else {
            bled_set_off();
            return false;
        }
    } else {
        static u8 cnt = 0;
        cnt++;
        if (cnt >= SYNC_TINE) {
            cnt = 0;
            LED_INIT();
            bled_set_on();
            return true;
        } else {
            bled_set_off();
            return false;
        }
    }
    return false;
}
#endif

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
#if USER_PWM_LED_EN
            bled_func.port_init = led_cfg_pwm_init;
            bled_func.set_on = led_cfg_pwm_set_on;
            bled_func.set_off = led_cfg_pwm_set_off;
#endif
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
#if USER_PWM_LED_EN
            rled_func.port_init = rled_cfg_pwm_init;
            rled_func.set_on = rled_cfg_pwm_set_on;
            rled_func.set_off = rled_cfg_pwm_set_off;
#endif
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
#if PORT_2LED_FIX_EN
        bled_func.set_on = bled2_set_on_fix;
        bled_func.set_off = bled2_set_off_fix;
#else
        bled_func.set_on = bled2_set_on;
        bled_func.set_off = bled2_set_off;
#endif

        rled_func.port_init = led2_port_init;
#if PORT_2LED_FIX_EN
        rled_func.set_on = rled2_set_on_fix;
        rled_func.set_off = rled2_set_off_fix;
#else
        rled_func.set_on = rled2_set_on;
        rled_func.set_off = rled2_set_off;
#endif
#endif // USER_LED_GPIO
    }
}

#if USER_PWM_LED_EN

#define  PWM_LED_INIT()            {led_cfg_set_off(&bled_gpio); led_cfg_set_off(&rled_gpio);}
#define  PWM_LED_TO_H()            {led_cfg_set_on(&bled_gpio);  led_cfg_set_on(&rled_gpio);}
#define  PWM_LED_TO_L()            {led_cfg_set_off(&bled_gpio); led_cfg_set_off(&rled_gpio);}
#define  PWM_BLED_ON()             led_cfg_set_on(&bled_gpio);
#define  PWM_BLED_OFF()            led_cfg_set_off(&bled_gpio);
#define  PWM_RLED_ON()             led_cfg_set_on(&rled_gpio);
#define  PWM_RLED_OFF()            led_cfg_set_off(&rled_gpio);

AT(.com_text.timer)
void pwm_led_timer2_isr(void)
{
    TMR2CPND = BIT(9);                                                  //Clear overflow Pending

    if (bre_led_cb.count <= bre_led_cb.duty){                           //调节高低电平占空比
        if (sys_cb1.pwm_rled_on) {
            PWM_RLED_ON();                                              //红灯亮
        } else {
            PWM_RLED_OFF();
        }

        if (sys_cb1.pwm_bled_on) {
            PWM_BLED_ON();                                              //蓝灯亮
        } else {
            PWM_BLED_OFF();
        }

    } else {
        PWM_LED_TO_L();
    }

    if(bre_led_cb.count == bre_led_cb.pwm_level) {                      //呼吸灯调节参数
        bre_led_cb.count = 0;
    }

    bre_led_cb.count++;
}

AT(.com_text.led_disp)
void pwm_led_stop(void)
{
    if (sys_cb1.pwm_bled_on || sys_cb1.pwm_rled_on) {
        return;
    }

    if (!sys_cb1.pwm_led_sta) {
        return;
    }

    sys_cb1.pwm_led_sta = 0;
}

//复用呼吸灯参数
AT(.com_text.led_disp)
void pwm_led_start(void)
{
    if (sys_cb1.pwm_led_sta) {
        return;
    }

    memset(&bre_led_cb, 0, sizeof(bre_led_cb));

    bre_led_cb.duty = USER_PWM_LED_LEVEL;
    bre_led_cb.pwm_period = 120;
    bre_led_cb.pwm_level = 10;

    sys_cb1.pwm_led_sta = 1;

    PWM_LED_INIT();

    TMR2CON = 0;
    register_isr(IRQ_TMR2_VECTOR, pwm_led_timer2_isr);
    PICPR &= ~BIT(IRQ_TMR2_VECTOR);
    PICEN |= BIT(IRQ_TMR2_VECTOR);
    TMR2CNT = 0;
    if (XO26MCON & BIT(10)) {                       //低功耗充电时会关掉晶振
        breathe_led_timer_start(1);
    } else {
        breathe_led_timer_start(0);
    }
}

AT(.com_text.led_disp)
void pwm_led_func_set(void)
{
    bled_func.set_on = led_cfg_pwm_set_on;
    bled_func.set_off = led_cfg_pwm_set_off;

    rled_func.set_on = rled_cfg_pwm_set_on;
    rled_func.set_off = rled_cfg_pwm_set_off;
}

AT(.com_text.led_disp)
void pwm_led_func_recover(void)
{
    bled_func.set_on = led_cfg_set_on;
    bled_func.set_off = led_cfg_set_off;

    rled_func.set_on = led_cfg_set_on;
    rled_func.set_off = led_cfg_set_off;
}
#endif

