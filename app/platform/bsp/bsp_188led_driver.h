#ifndef __BSP_188LED_DRIVE_H
#define __BSP_188LED_DRIVE_H

typedef struct {
    u16 disp_value;
    int16_t curr_disp_value;
    u8  charge_full;
    u16 charge_full_cnt;
    u8  disp_on;
    u8  disp_sta;
    u8  led_0;      //对应真值表五组LED IO输出
    u8  led_1;
    u8  led_2;
    u8  led_3;
    u8  led_4;
    u16 delay_off_cnt;
    u32 cnt;
}sys_188led_t;
sys_188led_t sys_188led;

u32 anc_pow10_cal(int index);
void bsp_anc_dbg_eq_param(u8 packet, u8 band_cnt, u32 *eq_buf);
void bsp_anc_start(void);
void bsp_anc_stop(void);
void bsp_anc_set_mode(u8 mode);
void bsp_anc_init(void);
void bsp_anc_exit(void);
void bsp_anc_parse_cmd_process(u8 *rx_buf);
#endif
