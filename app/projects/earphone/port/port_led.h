#ifndef _PORT_LED_H
#define _PORT_LED_H

enum {
    T_LED_NONE,
    T_LED_PWRON,
    T_LED_PWROFF,
    T_LED_BT_INIT,
    T_LED_BT_IDLE,
    T_LED_BT_RECONN,
    T_LED_BT_CONNECTED,
    T_LED_BT_PLAY,
    T_LED_BT_RING,
    T_LED_BT_CALL,
    T_LED_BT_PB,
    T_LED_AUX_PLAY,
    T_LED_AUX_PAUSE,
    T_LED_LOW_BATTERY,
};

typedef struct {
    void (*port_init)(gpio_t *g);
    void (*set_on)(gpio_t *g);
    void (*set_off)(gpio_t *g);
} led_func_t;

extern led_func_t rled_func;
extern led_func_t bled_func;
extern gpio_t bled_gpio;
extern gpio_t rled_gpio;

void led_func_init(void);

void led_bt_w4pwrkey_idle(void);
void led_aux_play(void);
void led_aux_idle(void);

u32 led_bt_sleep(u8 cfg_val);
void rled_dac_set_on(gpio_t *g);
void rled_dac_set_off(gpio_t *g);
void rled_dac_set_sta(u8 dac_start);
void bled_micl_set_on(gpio_t *g);
void bled_micl_set_off(gpio_t *g);
void bled_micl_set_sta(u8 mic_start);
void led_power_down_w4_done(void);

void pwm_led_start(void);
#endif
