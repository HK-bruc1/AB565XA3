#ifndef _API_DEVICE_H
#define _API_DEVICE_H

enum {
    DEV_SDCARD = 0,
    DEV_EARPHONE,
    DEV_LINEIN,
    DEV_EARIN,

    DEV_TOTAL_NUM = 7,
};

typedef struct {
    u8 sta;
    u8 online;
    u8 delay[DEV_TOTAL_NUM];
    u8 cnt[DEV_TOTAL_NUM];
} dev_cb_t;

void device_init(void);
bool device_is_online(u8 dev_num);
bool device_insert_event(u8 dev_num);
bool device_remove_event(u8 dev_num);

#endif
