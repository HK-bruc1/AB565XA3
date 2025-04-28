#ifndef _BSP_BLE_H
#define _BSP_BLE_H

#define POPUP_DELAY_MS          4000

extern uint (*ble_get_adv_data_ptr)(u8 *buf);

uint ble_get_bat_level(void);
void ble_get_adv_addr_type(u8 *buf);
uint ble_get_adv1_data(u8 *buf);        //不可连接广播
uint ble_get_adv2_data(u8 *buf);        //可连接广播
void ble_house_adv_init(void);
bool ble_get_house_key_state(void);
u8 ble_get_house_state(void);
u8 ble_get_house_bat(void);
u8 ble_get_left_bat_level(void);
u8 ble_get_right_bat_level(void);
void ble_bat_house_set_key_state(bool key_down);
void ble_bat_house_set_bat(u8 bat, u8 hbat);
void ble_bat_house_set_state(bool is_open);
void ble_bat_house_dis_adv(void);
void ble_bat_house_en_adv(void);
void ble_bat_tws_sync(void);
void ble_bat_ear_update(void);
void ble_popup_delay_process(void);
void ble_popup_3bat_once_set(void);
#endif // _BSP_BLE_H
