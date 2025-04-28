#ifndef _PLUGIN_H
#define _PLUGIN_H

#include "multi_lang.h"
#include "port_led.h"
#include "port_key.h"
#include "port_tkey.h"
#include "port_motor.h"
#include "port_linein.h"
#include "port_hall_sensor.h"
#include "port_ear_in.h"
#include "port_breathe_led.h"

void plugin_init(void);
void plugin_var_init(void);
void plugin_tmr5ms_isr(void);
void maxvol_tone_play(void);
void minvol_tone_play(void);

void loudspeaker_mute_init(void);
void loudspeaker_mute(void);
void loudspeaker_unmute(void);
void loudspeaker_disable(void);
void loudspeaker_gpiode_set(void);
void plugin_music_eq(void);

void atmosphere_led_init(void);
bool atmosphere_led_toggle(void);
bool atmosphere_led_is_on(void);
void atmosphere_led_sta_set(u8 sta);
void atmosphere_led_gpiode_set(void);

void sleep_wakeup_config(void);
void clkgate_configure(void);
void plugin_popup_auto_config(void);

extern volatile int pwrkey_detect_flag;
extern gpio_t atmos_gpio;
#endif
