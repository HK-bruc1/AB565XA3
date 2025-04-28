#ifndef _BSP_IODM_H
#define _BSP_IODM_H





//返回结果
#define IODM_RESULT_OK              0x00
#define IODM_RESULT_FAIL            0x01
#define IODM_RESULT_COMPLETE        0x02
#define IODM_RESULT_NO_RSP          0xFF


#define IODM_HARDWARE_VER               "\x01\x00"
#define IODM_SOFTWARE_VER               "\x01\x00"

#define RST_FLAG_MAGIC_VALUE        0x5c       //复位标志值


void iodm_reveice_data_deal(vh_packet_t *packet,u8 entrance);
void bt_save_qr_addr(u8 *addr);
bool bt_get_qr_addr(u8 *addr);
bool iodm_reflash_bt_name(void);
bool iodm_save_bt_new_name(char *name,u8 len);
u8 vhouse_custom_test(vh_packet_t *packet,u8* cmd_rsp_param_len);
void bt_spp_iodm_event(void);
bool iodm_get_bt_name(u8 *buf);
void iodm_init(void);

#endif
