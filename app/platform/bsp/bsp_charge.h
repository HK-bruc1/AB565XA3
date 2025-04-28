#ifndef _BSP_CHARGE_H
#define _BSP_CHARGE_H

typedef struct {
    u16 on_cnt;
    u16 off_cnt;
    u16 on_delay;
    u16 off_delay;
} inbox_cb_t;
extern inbox_cb_t inbox_cb;

typedef struct {
    u8  out2pwr_en;              //拿起开机
    u8  leakage_sta;
    u8  dc_in;
volatile u8  inbox_rtt_check;    //是否中断进行inbox实时检测
    u32 counter;
    u32 ticks;
    u8  pwrsave_en;
} ch_box_cb_t;
extern ch_box_cb_t chbox_cb;

void bsp_charge_off(void);
void bsp_charge_init(void);
void charge_detect(void);
void bsp_charge_sta(u8 sta);
void bsp_set_stop_time(u16 stop_time);
u8 charge_dc_detect(void);
void bsp_charge_bled_ctrl(void);
u8 bsp_charge_box_sta(void);
void bsp_charge_inbox_wakeup_enable(void);
u8 bsp_charge_inbox_check(void);
void bsp_charge_leakage_set(u8 sta, u8 val);
int bsp_charge_outbox_stable_check(void);
void bsp_charge_inbox_chk_init(u16 off_delay, u16 on_delay);
void bsp_charge_inbox_process(void);
void bsp_charge_power_save(void);
void bsp_charge_power_recover(void);
void bsp_charge_box_exit(void);
void bsp_charge_box_enter(u32 out_auto_pwron);
u32 bsp_charge_box_process(void);
bool bsp_chbox_type_is_maintain_volt(void);
void bsp_charge_box_reinit(void);
bool bsp_charge_inbox_leakage_check(void);
void charge_box_var_init(void);
u32 bsp_charge_get_leakage_level(void);
void charge_tsen_check(u8 change_cnt);

#define vusb_delay_rst_en(nsec)                     {VBRSTCON = ((nsec * 33) << 20) | BIT(30);}         //vusb delay n second reset enable
#define vusb_delay_rst_dis()                        {RTCCPND = 3 << 28;}                                //clr counter and pending, disable
#define vusb_delay_rst_kick()                       {RTCCPND = BIT(30);}                                //kick start vusb reset delay counter


#endif
