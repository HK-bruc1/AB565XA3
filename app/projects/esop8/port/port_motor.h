#ifndef _PORT_MOTOR_H
#define _PORT_MOTOR_H

#if BT_MOTOR_EN
enum {
    MOTOR_NONE,
    MOTOR_PWRON,
    MOTOR_PWROFF,
    MOTOR_RING,
    MOTOR_CONN,
    MOTOR_DISCONN,
};

#define T_MOTOR_PWRON_SEL           xcfg_cb.motor_pwron_sel
#define T_MOTOR_PWROFF_SEL          xcfg_cb.motor_pwroff_sel
#define T_MOTOR_CONN_SEL            xcfg_cb.motor_conn_sel
#define T_MOTOR_DISCONN_SEL         xcfg_cb.motor_disconn_sel
#define T_MOTOR_RING_SEL            xcfg_cb.motor_ring_sel


typedef struct {
    volatile u8  sta;        //马达状态
    volatile u8  type;
    volatile u16 cnt;        //振动时间控制
    u16 time;
} motor_cb_t;
extern motor_cb_t motor_cb;

void motor_init(void);
void motor_enable(u8 type, u8 time_sel);
void motor_disable(void);
void pwroff_motor_enable(void);
void motor_tmr20ms_process(void);
void motor_ring_process(void);
#else
#define motor_init()
#define motor_enable(type, time_sel)
#define motor_disable()
#define pwroff_motor_enable()
#define motor_tmr20ms_process()
#define motor_ring_process()

#endif // BT_MOTOR_EN

#endif // _PORT_MOTOR_H
