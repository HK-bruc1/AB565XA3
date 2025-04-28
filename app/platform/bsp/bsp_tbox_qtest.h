#ifndef _BSP_TBOX_QTEST_H
#define _BSP_TBOX_QTEST_H

enum{
    QTEST_STA_INBOX,
    QTEST_STA_OUTBOX,
    QTEST_STA_DCIN,
};

void bsp_qtest_init(void);
bool qtest_other_usage_process(void);
bool qtest_is_loading_local_addr(void);
u8* qtest_other_usage_alloc_txbuf(u32 *buf_len);
bool bsp_qtest_other_usage(void);
void bsp_qtest_var_init(void);
bool qtest_feature_is_btdisconnet_pwroff(void);
uint ble_get_bat_level(void);
bool update_set_qcheck_code(void);
void bt_get_ext_link_info(void *buf, u16 addr, u16 size);
void bt_put_ext_link_info(void *buf, u16 addr, u16 size);
#endif // _BSP_TBOX_QTEST_H

