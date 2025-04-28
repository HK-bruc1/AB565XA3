#ifndef _BSP_BT_H
#define _BSP_BT_H

//标准HID键
#define HID_KEY_ENTER       0x28

//自定义HID键
#define HID_KEY_VOL_DOWN    0x00EA
#define HID_KEY_VOL_UP      0x00E9
#define HID_KEY_IOS_HOME    0x0040      //ios home
#define HID_KEY_IOS_POWER   0x0030      //ios 锁屏
#define HID_KEY_IOS_LAYOUT  0x01AE      //ios Keyboard Layout

extern char bt_name_buf[32];
extern char bthid_name_buf[32];
extern char redial_buf[2][32];
extern u8 bt_clkn_rst;
extern uint8_t cfg_bt_work_mode;
extern uint8_t cfg_bt_hid_feature;
extern uint8_t cfg_bt_link_filter;

void bsp_bt_init(void);
void bsp_bt_close(void);
void bt_long_wait(u32 delay);
void bsp_bt_vol_change(void);
bool bsp_bt_hid_photo(u16 keycode);
bool bsp_bt_pwrkey5s_check(void);
void bsp_bt_pwrkey5s_clr(void);
void bsp_bt_hid_tog_conn(void);
void bt_switch_voice_lang(void);
bool bt_tws_is_sync_voice(void);
void bt_tws_new_name_set(u8 val);
bool bt_chk_new_local_name(char *ptr);
void bt_get_local_bd_addr(u8 *addr);
uint bt_get_bat_level(u32 div);

void bsp_bt_redial_init(void);
void bsp_bt_redial_reset(uint8_t index);
void bsp_bt_update_redial_number(uint8_t index, char *buf, u32 len);
const char *bsp_bt_get_last_call_number(uint8_t index);

bool bsp_bt_hid_vol_set(u16 hid_code);

void bsp_tws_sync_info_eq_aec(void);

uint bt_hfp_get_sys_vol(uint hfp_vol);

void ble_popup_update_state(uint step);
void ble_popup_delay_process(void);
void bt_tws_location_adv_process(void);

bool bt_tws_get_channel(uint8_t *channel);
void bt_clr_all_link_info(u8 flag);
void bt_tws_clr_link_info(u8 flag);

void bt_set_ear_sta(bool ear_sta);
void bt_update_ear_sta(void);
uint8_t bt_get_right_ear_sta(void);
uint8_t bt_get_left_ear_sta(void);
u8 bt_get_loc_nr_sta(void);
void bt_set_menu_ctrl_sta(u8 *ptr, uint len);
u8 bt_change_nr_sta(void);
void bsp_bt_msg_nr_sta_change(void);
bool bsp_bt_w4_connect(void);
bool bt_get_master_addr(u8 *addr);

void bt_tws_set_operation(uint8_t *cmd);
void bt_emit_notice_hid_con(u32 param);
uint hfp_get_bat_level(void);
void bt_tws_report_dgb(void);
void bt_store_a2dp_volume(u8 vol);

void bt_work_mode_init(void);
bool bt_tws_set_lr_same_ch_mac_is_en(void);
#endif //_BSP_BT_H
