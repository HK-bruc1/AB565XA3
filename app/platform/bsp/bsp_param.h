#ifndef __BSP_PARAM_H
#define __BSP_PARAM_H

#define PARAM_BT                    param_cb.bt_of
#define PARAM_BT_SIZE               param_cb.sys_of         //前面112byte存放蓝牙信息
#define PARAM_SYS_VOL               PARAM_BT_SIZE + 0
#define PARAM_HOUSE_IDX             PARAM_BT_SIZE + 1       //bit0~bit2, 3bits
#define PARAM_LANG_ID               PARAM_BT_SIZE + 1       //bit3, 1bit
#define PARAM_QUICK_TEST_PAIR_HIST  PARAM_BT_SIZE + 1       //bit4, 1bit
#define PARAM_BT_XOSC_CAP           PARAM_BT_SIZE + 2       //1Byte xosc value
#define PARAM_VUSB_CH_VALID         PARAM_BT_SIZE + 3       //bit0~bit1, 2bits  保存智能仓固定的左右声道
#define PARAM_VUSB_POPUP_FLAG       PARAM_BT_SIZE + 3       //bit2~bit3, 2bits  保存POPUP使能状态
#define PARAM_QTEST_CH_VALID        PARAM_BT_SIZE + 3       //bit4~bit5, 2bits  保存测试盒快配时固定的左右声道信息
#define PARAM_RESV                  PARAM_BT_SIZE + 4       //保留2bytes
#define PARAM_RANDOM_KEY            PARAM_BT_SIZE + 6       //4Byte随机数
#define PARAM_ANC_MIC_VAL           PARAM_BT_SIZE + 10      //1byte
#define PARAM_ANC_TP_MIC_VAL        PARAM_BT_SIZE + 11      //1byte
#define PARAM_NEW_LOCAL_NAME0       PARAM_BT_SIZE + 12      //ios menu bt_name, 2Byte

#define PARAM_CRC                   param_cb.crc_of         //crc16保存地址, 2bytes
#define PARAM_CRC_LEN               param_cb.crc_len        //多少bytes计算crc16
#define PARAM_TWS_LINK_ADDR         param_cb.tws_link_of    //TWS LINK KEY保存地址

///偏移地址 >= 128只能用于flash记忆
#define PARAM_NEW_LOCAL_NAME        128                     //64bytes
#define PARAM_QR_ADDR_VALID         192                     //1byte
#define PARAM_QR_ADDR               193                     //6byte
#define PARAM_BT_NAME_LEN           199                     //记录蓝牙长度最大值32 1bytes
#define PARAM_BT_NAME               200                     //使用新的蓝牙名字32 byte
#define PARAM_BT_QPDN_BOAT_FLAG     232                     //用于记录快测船运关机标志 1 byte
#define PARAM_BT_QPAIRED_ID         233                     //用于记录快测设置的配对ID或者拓展字 5byte

//TWS_INFO
#define PARAM_BT_TWS_QTEST_INFO           0                       //用于记录快测TWS信息

typedef struct {
    u8  bt_of;                                              //蓝牙回连信息起始位置
    u8  sys_of;                                             //PARAM_BT_SIZE
    u8  crc_of;                                             //PARAM_CRC
    u8  crc_len;                                            //PARAM_CRC_LEN
    u8  tws_link_of;                                        //PARAM_TWS_LINK_ADDR
    u8  new_name_of;                                        //PARAM_NEW_LOCAL_NAME
    void (*write)(u8 *buf, u32 addr, uint len);
    void (*read)(u8 *buf, u32 addr, uint len);
    void (*sync)(void);
} param_cb_t;
extern param_cb_t param_cb;

#define param_read(a, b, c)     param_cb.read(a, b, c)
#define param_write(a, b, c)    param_cb.write(a, b, c)
#define param_sync()            param_cb.sync()

void param_init(bool reset);
void param_sys_vol_write(void);
void param_sys_vol_read(void);
void param_random_key_write(void);
void param_random_key_read(u8 *key);
void param_lang_write(void);
void param_lang_read(void);
void param_sync_do(void);
void rtcram_write(u8 *buf, u32 addr, uint len);
void rtcram_read(u8 *buf, u32 addr, uint len);

void param_bt_xosc_write(u8 cap);
u8 param_bt_xosc_read(void);

void param_local_random_addr_read(void);
void param_vuart_force_channel_read(void);
void param_vuart_force_channel_write(u8 channel);
void param_vuart_popup_flag_read(void);
void param_vuart_popup_flag_write(u8 data);
void param_toggle_popup_write(void);
void param_house_index_write(void);
void param_qtest_force_channel_write(u8 channel);
void param_qtest_force_channel_read(void);

void param_bt_new_name_read(char *buf, uint size);
void param_bt_new_name_write(char *buf, uint size);
void param_bt_new_name_clr(void);

void param_tkey_temp_cnt_write(u16 temp_cnt, u16 tkcnt);
void param_tkey_temp_cnt_read(u16 *temp_cnt, u16 *tkcnt);

void param_bt_qpaird_id_write(u8* buf);
u8 param_bt_qpaird_id_read(void);
#endif // __BSP_PARAM_H

