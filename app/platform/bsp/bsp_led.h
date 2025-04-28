#ifndef  _BSP_LED_H_
#define  _BSP_LED_H_

#define LED_SCAN_TIME       50  //单位ms

///请勿修改led_cfg_t结构体
typedef struct {
    u8 redpat;
    u8 bluepat;
    u8 unit;
    u8 cycle;
} led_cfg_t;

///请勿修改led_cb_t结构体
typedef struct {
    volatile u8 rled_sta;   //红灯 亮-灭控制
    volatile u8 bled_sta;   //蓝灯 亮-灭控制
    u8 unit;        //50ms单位 (范围：1~255)
    u8 period;      //间隔周期 (范围：0~255)


    u16 circle;     //一个周期(n-1)
    u16 cnt;        //已运行
    u8 wait;

    //u8 cnt;
    //u8 bcnt;
    //u8 space_cnt;

    u8 flag;
} led_cb_t;
extern led_cb_t led_cb;

typedef struct {
    volatile u8 port2led_sta;
    volatile u8 scan_dis;           //关闭LED灯扫描
    volatile u8 mic2led_bak_sta;
    volatile u8 dac2led_bak_sta;
    u8 port2led_en;                 //是否1个GPIO推2个灯
    u8 led_type;
    u8 mic2led_en;                  //复用MIC脚推蓝灯
    u8 dac2led_en;                  //复用DAC脚推蓝灯

    u8 charge_rled_en   : 1,
       charge_bled_en   : 1,
       charge_led_2s_en : 1,
       charge_bre_en    : 1,        //呼吸灯模式使能
       charge_bre_cfg   : 2,        //呼吸灯模式选择
       lowbat_led_en    : 1;        //低电led独立控制

    u8 tws_con_bre_en   : 1,
       tws_play_bre_en  : 1,
       tws_con_bre_cfg  : 2,
       tws_play_bre_cfg : 2;

    volatile u8 crbf_run;
} sys_led_t;
extern sys_led_t sys_led;

enum
{
    LED_TOG_LBAT    = 0x03,         //低电LED闪烁
    LED_CRITICAL    = 0x80,         //临界处理
};

void led_cfg_port_init(gpio_t *g);
void led_cfg_set_on(gpio_t *g);
void led_cfg_set_off(gpio_t *g);
void led2_port_init(gpio_t *g);
void bled2_set_on(gpio_t *g);
void bled2_set_off(gpio_t *g);
void rled2_set_on(gpio_t *g);
void rled2_set_off(gpio_t *g);
void bled2_set_on_fix(gpio_t *g);
void bled2_set_off_fix(gpio_t *g);
void rled2_set_on_fix(gpio_t *g);
void rled2_set_off_fix(gpio_t *g);

#if LED_DISP_EN

    void led_set_sta(u8 rled_sta, u8 bled_sta, u8 unit, u8 period);
    void led_cfg_set_sta(led_cfg_t *cfg_cb);

    void rled_lowbat(void);
    void rled_lowbat_recover(void);

    void port_2led_scan(void);
    bool port_2led_is_sleep_en(void);

    void bled_set_off(void);
    void bled_set_on(void);

    void led_init(void);
    void led_on(void);
    void rled_on(void);
    void led_off(void);
    void rled_off(void);
    void led_aux_idle(void);
    void led_scan(void);
    void led_sync(void);
    void led_bt_init(void);
    void led_bt_idle(void);
    void led_bt_connected(void);
    void led_bt_connectting(void);
    void led_bt_reconnect(void);
    void led_bt_play(void);
    u32 led_bt_sleep(u8 cfg_val);
    void led_bt_ring(void);
    void led_bt_call(void);
    void led_bt_scan(void);
    void led_ctrl_configure(u8 cfg_val);
    void led_power_up(void);
    void led_power_down(void);
    void charge_led_off(u8 sta);
    void charge_led_on(void);
    void rled_set_off(void);
    void rled_set_tog(void);
    void rled_set_on(void);

    void led_aux_play(void);
    void led_aux_idle(void);
    #define led_bt_pb_sleep(x)  led_ctrl_configure(x)
    #define charge_bled_on()    LED_SET_ON()                    //充满蓝灯常亮
    #define charge_bled_off()   LED_SET_OFF()                   //充满蓝灯灭
#else
    #define led_set_sta(x)
    #define led_cfg_set_sta(x)

    #define led_init()
    #define led_on()
    #define rled_on()
    #define led_off()
    #define rled_off()
    #define led_aux_idle()
    #define led_scan()
    #define led_sync()
    #define led_bt_init()
    #define led_bt_idle()
    #define led_aux_play()
    #define led_bt_connected()
    #define led_bt_reconnect()
    #define led_bt_connectting()
    #define led_bt_play()
    #define led_bt_sleep(x)                  (-1L)
    #define led_bt_ring()
    #define led_bt_call()
    #define led_bt_scan()
    #define led_bt_pb_sleep(x)
    #define led_power_up()
    #define led_power_down()
    #define charge_led_off(x)
    #define charge_led_on()
    #define rled_lowbat()
    #define rled_lowbat_recover()

    #define port_2led_scan()
    #define port_2led_is_sleep_en()         1
    #define charge_bled_on()
#endif

#endif //_BSP_LED_H_
