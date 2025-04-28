#include "include.h"

#if BT_MAP_EN
int nibble_for_char(char c);
void bt_map_start(void);
void ble_charge_box_cmd_set(u8 cmd);
void tws_send_map_time(u8* buf);

u32 map_start_tick;
u8 map_time_data[7] = {0};
extern bool bt_time_get_flag;

void bt_map_kick(void)
{
    memset(map_time_data, 0, 7);
    map_start_tick = tick_get();
    bt_map_start();
}

int nibble_for_char(char c){
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

void bt_get_time(char *ptr)
{
    bool ios_flag = bt_is_ios_device();
    u16 year = ios_flag? 2000 + nibble_for_char(ptr[2])*10 + nibble_for_char(ptr[3]) : nibble_for_char(ptr[0])*1000 + nibble_for_char(ptr[1])*100 + nibble_for_char(ptr[2])*10 + nibble_for_char(ptr[3]);
    u8 month = nibble_for_char(ptr[4])*10 + nibble_for_char(ptr[5]);
    u8 day = nibble_for_char(ptr[6])*10 + nibble_for_char(ptr[7]);
    u8 hour = nibble_for_char(ptr[9 - ios_flag])*10 + nibble_for_char(ptr[10 - ios_flag]);
    u8 min = nibble_for_char(ptr[11 - ios_flag])*10 + nibble_for_char(ptr[12 - ios_flag]);
    u8 sec = nibble_for_char(ptr[13 - ios_flag])*10 + nibble_for_char(ptr[14 - ios_flag]);
    if((year >= 2000) && (year <= 3000) && (month <= 12) && (day <= 31) && (hour <= 24) && (min <= 60) && sec <= 60){
        printf("date:%04d.%02d.%02d time:%02d:%02d:%02d\n",year,month,day,hour,min,sec);
        bt_time_get_flag = true;
        memcpy(&map_time_data[0], &year, 2);
        map_time_data[2] = month;
        map_time_data[3] = day;
        map_time_data[4] = hour;
        map_time_data[5] = min;
        map_time_data[6] = sec;
    }
}

void bt_map_data_callback(uint8_t *packet)
{
    char *ptr = (char *)(packet+13);
    bt_get_time(ptr);
}


const char * hfp_get_at_cmd(void) {
    return "AT+CCLK?\r";//获取IOS手机时间（安卓暂不支持），获取回调函数hfp_get_time
}

u32 hfp_start_tick;
u8 hfp_time_data[7] = {0};

void hfp_at_kick(void)
{
    memset(hfp_time_data, 0, 7);
    hfp_start_tick = tick_get();

    //printf("hfp_at_kick\n");
    bt_send_msg(BT_MSG_HFP_AT_CMD);
    delay_5ms(10); //延迟一下，等它发送完毕
}

bool hfp_at_time_get(u8 *buf, u8 len)
{
    if (hfp_time_data[0] == 0) {
        printf("hfp time empty!\n");
        return false;
    }
    if (tick_check_expire(hfp_start_tick, 30000)) {
        printf("hfp time outdated!\n");   //超过30s，数据过时
        return false;
    }
    memcpy(buf, hfp_time_data,7);
    return true;
}

int nibble_for_char(char c);
void hfp_get_time(char *ptr)
{
   // my_printf("-->(hfp)set time\n");
    u16 year = 2000 + nibble_for_char(ptr[0])*10 + nibble_for_char(ptr[1]);
    u8 month = nibble_for_char(ptr[2])*10 + nibble_for_char(ptr[3]);
    u8 day = nibble_for_char(ptr[4])*10 + nibble_for_char(ptr[5]);
    u8 hour = nibble_for_char(ptr[6])*10 + nibble_for_char(ptr[7]);
    u8 min = nibble_for_char(ptr[8])*10 + nibble_for_char(ptr[9]);
    u8 sec = nibble_for_char(ptr[10])*10 + nibble_for_char(ptr[11]);
    if((year >= 2000) && (year <= 3000) && (month <= 12) && (day <= 31) && (hour <= 24) && (min <= 60) && sec <= 60){
        printf("IOS get time:\ndate:%04d.%02d.%02d time:%02d:%02d:%02d\n", year, month, day, hour, min, sec);
        memcpy(&hfp_time_data[0], &year, 2);
        hfp_time_data[2] = month;
        hfp_time_data[3] = day;
        hfp_time_data[4] = hour;
        hfp_time_data[5] = min;
        hfp_time_data[6] = sec;
        bt_time_get_flag = true;
    }
}

void hfp_notice_network_time(u8 *buf, u16 len)
{
//    char cache[16] = "";
//    memcpy(cache, buf, min(len, 15));
//    printf("hfp_notice_network_time:%s\n", cache);

    if (bt_is_ios_device()) {
        hfp_get_time((char *)buf);
    }
}
#endif

