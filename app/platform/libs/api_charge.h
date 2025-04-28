#ifndef _API_CHARGE_H
#define _API_CHARGE_H

//充电状态
enum {
    CHAG_STA_UNINIT = 0,        //未初始化
    CHAG_STA_OFF,               //充电关闭
    CHAG_STA_OFF_VBUS_PATH,     //充电关闭，但是还是插着5V电源
    CHAG_STA_ON_TRICKLE,        //正在涓流充电
    CHAG_STA_ON_CONSTANT,       //正在恒流充电
};

//充电配置
typedef struct {
    u8 chag_sta;                //充电状态
    u8 dcin_reset;              //DC插入复位
    u8 const_curr;              //恒流充电电流
    u8 trick_curr;              //涓流充电电流
    u8 stop_curr;               //充电截止电流
    u8 stop_volt;               //充电截止电压
    u8 trick_curr_en;           //是否使能涓流充电
    u8 bled_on_pr;
    u8 stop_curr_thd;           //截止电流计数阈值
    u8 volt_follow;             //电压跟随快充
    u16 stop_time;              //电压满足
    u32 limited_time;           //充电时间限制
} charge_cfg_t;
extern charge_cfg_t charge_cfg;

extern bool cfg_vio_chg_en;



#endif
