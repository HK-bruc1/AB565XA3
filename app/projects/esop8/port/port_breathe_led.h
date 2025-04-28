#ifndef _PORT_BREATHE_LED_H
#define _PORT_BREATHE_LED_H

///呼吸灯类型
enum {
    T_BRE_NONE,
    T_BRE_CHARGE,                       //充电呼吸灯
    T_BRE_TWS_CON,                      //TWS连接且未连接手机状态呼吸灯
    T_BRE_TWS_PLAY,                     //音乐播放/响铃/通话状态呼吸灯(已连接手机)
    T_BRE_WAIT_DONE,                    //临时状态, 等TWS命令
};

typedef struct{
    s8 count;
    s8 duty;
    s8 dir;
    s8 time;
    u8 pwm_time;
    u8 pwm_period;
    u8 pwm_level;
    u8 mode;                    //呼吸灯模式：0:红蓝灯同时呼吸, 1: 红蓝灯交替呼吸,  2: 蓝灯呼吸, 3: 红灯呼吸
    u8 toggle;
    u8 pwm_low_en;
} breathe_led_t;
extern breathe_led_t bre_led_cb;

void breathe_led_start(u32 type, u32 mode);
void breathe_led_timer_start(u32 ref_xosc);
void breathe_led_stop(void);

#endif // _PORT_BREATHE_LED_H
