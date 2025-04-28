#include "include.h"

#if LED_BREATHE_EN

#define USER_BREATHE_LED_EN        0            //关闭时用回rom的pwm参数

//定时器3模拟PWM实现呼吸灯，IO口PB4
#define  PWM_TIME                  6                                                       //呼吸灯调节亮度的时间间隔，值越大，呼吸灯变化的越慢.
#define  PWM_PERIOD                120                                                     //定时器周期，值越小，呼吸灯亮度变化的越快，建议在20~200之间
#define  PWM_LEVEL                 90                                                      //呼吸灯不同亮度调节参数，值越大，呼吸灯亮度划分的越明显，变化的速率越慢

// TODO 打开2个灯的呼吸灯效果
#define  BREATHE_IO_INIT()         {LED_SET_OFF(); LED_PWR_SET_OFF();}
#define  BREATHE_IO_H()            {LED_SET_ON();  LED_PWR_SET_ON();}
#define  BREATHE_IO_L()            {LED_SET_OFF(); LED_PWR_SET_OFF();}

void breathe_led_timer2_isr(void);

//breathe led start
AT(.com_text.led_disp)
uint breathe_led_start_hook(u32 type, u32 mode)
{
#if USER_BREATHE_LED_EN
    if ((sys_cb.breathe_led_sta != type) || (bre_led_cb.mode != mode)) {
        memset(&bre_led_cb, 0, sizeof(bre_led_cb));
        bre_led_cb.duty = 1;
        bre_led_cb.pwm_time = PWM_TIME;
        bre_led_cb.pwm_period = PWM_PERIOD;
        bre_led_cb.pwm_level = PWM_LEVEL;
        bre_led_cb.mode = mode;
        bre_led_cb.pwm_low_en = !xcfg_cb.led_on_bre_only_en;
        sys_cb.breathe_led_sta = type;
        TMR2CON = 0;
        BREATHE_IO_INIT();
        register_isr(IRQ_TMR2_VECTOR, breathe_led_timer2_isr);
        PICPR &= ~BIT(IRQ_TMR2_VECTOR);
        PICEN |= BIT(IRQ_TMR2_VECTOR);
        TMR2CNT = 0;
        if (XO26MCON & BIT(10)) {                       //低功耗充电时会关掉晶振
            breathe_led_timer_start(1);
        } else {
            breathe_led_timer_start(0);
        }
    }
    return 1;
#else
    return 0;       //跑回rom程序实现
#endif // USER_BREATHE_LED_EN
}

#else
AT(.com_text.led_disp)
uint breathe_led_start_hook(u32 type, u32 mode)
{
    return 1;
}
#endif // LED_BREATHE_EN
