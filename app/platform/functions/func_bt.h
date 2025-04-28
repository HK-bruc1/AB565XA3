#ifndef _FUNC_BT_H
#define _FUNC_BT_H

#define BT_WARN_CON             BIT(0)
#define BT_WARN_HID_CON         BIT(1)
#define BT_WARN_TWS_MCON        BIT(2)
#define BT_WARN_TWS_SCON        BIT(3)
#define BT_WARN_DISCON          BIT(4)
#define BT_WARN_HID_DISCON      BIT(5)
#define BT_WARN_PAIRING         BIT(6)
#define BT_WARN_TWS_DISCON      BIT(7)
#define BT_WARN_TWS_CON         BIT(8)

#define INCALL_FLAG_SCO         BIT(0)
#define INCALL_FLAG_CALL        BIT(1)
#define INCALL_FLAG_FADE        (INCALL_FLAG_SCO|INCALL_FLAG_CALL)

///对应USER_DEF键自定义的按键功能列表(与配置项对应)
enum _user_def_key {
    UDK_NONE            = 0,        //0
    UDK_REDIALING,                  //1
    UDK_SIRI,                       //2
    UDK_PREV,                       //3
    UDK_NEXT,                       //4
    UDK_VOL_UP,                     //5
    UDK_VOL_DOWN,                   //6
    UDK_SIRI_REDIALING,             //7
    UDK_PHOTO,                      //8
    UDK_HOME,                       //9
    UDK_LANG,                       //10
    UDK_ATMOS_LED,                  //11
    UDK_PLAY_PAUSE,                 //12
    UDK_NR,                         //13
    UDK_TOGGLE_POPUP,               //14
    UDK_GAME_SWITCH,                //15
    UDK_MODE,                       //16
};

enum {
    AEC_PARAM_MIC_ANL_GAIN,
    AEC_PARAM_MIC_DIG_GAIN,
    AEC_PARAM_MIC_POST_GAIN,
    AEC_PARAM_NOISE,
    AEC_PARAM_LEVEL,
    AEC_PARAM_OFFSET,
    AEC_PARAM_FULL_THD,
    AEC_PARAM_HALF_THD,
    AEC_PARAM_SMOOTH_GAIN,
};

///请勿修改func_bt_t结构体
typedef struct{
    u16 warning_status;
    u8 disp_status;
    u8 hid_menu_flag;
    u8 hid_discon_flag;
    u8 siri_kl_flag;
    u8 user_kl_flag;
    u8 tws_status;
    u8 need_pairing;
    u8 w4_tws_warning;
    u8 autoplay;            //蓝牙连接后自动播放控制
volatile u8 ring_stop;
volatile u8 ring_sta;
    u8 bt_is_inited;
    u8 quick_test_flag;     //蓝牙快速测试控制变量
    u8 popup_delay_dis;     //关窗后需要延时关广播
    u32 tick;
    u32 tick_vbat;
    u8 loc_vbat;
    u8 rem_vbat;
} func_bt_t;

extern func_bt_t f_bt;

typedef struct {
    u8 cur;
    s8 gain_offset;
    u8 avol_level;
    u8 ring_cnt;
    u8 inband_ring_sta;
    u8 ring_first;
    u16 w4_sco_tick;
    u32 ios_ring_tick;
    u32 ring_tick;
} t_play_cb;

typedef struct {
    u8 buf[25];
    u8 len;
    t_play_cb play;
    u8 sysclk_bak;
    u8 dac_sta;
} f_bt_ring_cb;
extern f_bt_ring_cb f_bt_ring;

typedef struct
{
    u8 esbc_flag;
    u8 sbc_flag;

}f_bt_ring1_cb;

extern f_bt_ring1_cb f_bt_ring1;

extern int sfunc_bt_call_flag;

void sfunc_bt_ring(void);
void sfunc_bt_ring_message(u16 msg);
void sfunc_bt_ring_message_m(u16 msg);

void sfunc_bt_call(void);
void sfunc_bt_call_message(u16 msg);
void sfunc_bt_call_message_m(u16 msg);
void bt_call_volume_change(u16 msg);

void func_bt_status(void);
void func_bt_message(u16 msg);
void func_bt_message_m(u16 msg);
u16 func_bt_chkclr_warning(u16 bits);
void func_bt_display(void);
void func_bt_enter_display(void);
void func_bt_exit_display(void);
void func_bt_mp3_res_play(u32 addr, u32 len);
void sfunc_bt_ring_sync(void);
void bt_msg_tws_ring_sync(void);

void bt_ring_var_init(void);
void bt_ring_stop(void);
bool bt_ring_inband_is_active(void);                //是否正在播放手机铃声
void func_bt_dut(void);

void func_bt_init(void);
void func_bt_exit(void);
void func_bt_chk_off(void);
void func_bt_set_dac(u8 enable);
u8 func_bt_tws_get_channel(void);
void func_bt_sub_process(void);

bool user_def_key_msg(u8 func_sel);
void bt_exit_wait_ticks(u32 tick);
void func_bt_tws_set_channel(void);

void sco_set_incall_flag(u8 bit);
bool sco_clr_incall_flag(u8 bit);

void func_bt_fcc(void);
void fcc_init_hook(void);
void fcc_exit_hook(void);

void bt_sco_magic_voice_switch(void);

#endif // _FUNC_BT_H
