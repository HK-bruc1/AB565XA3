#ifndef  _BSP_PIANO_H_
#define  _BSP_PIANO_H_

//          1/8  4/4   3/4   2/4      1/4  4/4   3/4   2/4
//Rhythm = [    0.062 0.094 0.125         0.125 0.187 0.250];
//           Do     Re     Mi     Fa     Sol    La    Si
//Fin    = [261.63 293.67 329.63 349.23 391.99 440 493.88 0 0 0];
#define PIANO_BASEKEYDLY        (int)(48000*0.125)
#define TONE_DELAY(x)           (int)(48*x)
#define TONE_FREQUENCY(a, b)    (u32)((a << 16) | (u16)((1 << 15) / (48000 / b)))

struct tone_tbl_t {
    u32 res;
    u16 last_time;  //持续时间
}__attribute__ ((packed));

///请勿修改piano_cb_t结构体
typedef struct {
    u8 len;
    u8 type;
    u8 spr;
    u8 base_key;                //音阶：CDEFGAB
    u8 harmen;                  //泛音
    u8 fade_sta;
    u8 warning_index;
    u8 dnr_sta;
    s16 base_delay;             //音节持续时间
    volatile u16 write_cnt;
    const u32 *piano_buf;
    const struct tone_tbl_t *tone_buf;
    u8 dac_sta;
    u8 mix_flag;                //是否SRC0 + SRC1叠加播放
    volatile u8 play_flag;      //是否正在播放
} piano_cb_t;
extern piano_cb_t piano_cb;

enum {
    WARNING_PIANO,
    WARNING_TONE,
};

///与piano_index_tbl[]一一对应，注意同步修改
enum {
    ///此处请勿新增或修改
    T_WARNING_POWER_ON,
    T_WARNING_POWER_OFF,
    T_WARNING_BT_DISCONNECT,
    T_WARNING_BT_CONNECT,
    T_WARNING_RIGHT_CH,
    T_WARNING_LEFT_CH,
    T_WARNING_PAIRING,
    T_WARNING_SWITCH_LANG,
    T_WARNING_MUSIC_MODE,
    T_WARNING_GAME_MODE,
    T_WARNING_LOW_BATTERY,
    T_WARNING_EAR_IN,           //入耳语音

    //后半部分无esbc语音
    T_WARNING_REDIALING,
    T_WARNING_HANGUP,
    T_WARNING_REJECT,
    T_WARNING_MAX_VOL,
    T_WARNING_MIN_VOL,
    T_WARNING_BT_RING,
    T_WARNING_CAMERA_ON,
    T_WARNING_CAMERA_OFF,
    T_WARNING_BT_MODE,
    T_WARNING_CAMERA_MODE,
    T_WARNING_AUX_MODE,
    T_WARNING_TAKE_PHOTO,
    T_WARNING_NEXT_TRACK,
    T_WARNING_BT_WAIT4CONN,

    //语音播号
    T_WARNING_NUM_0,
    T_WARNING_NUM_1,
    T_WARNING_NUM_2,
    T_WARNING_NUM_3,
    T_WARNING_NUM_4,
    T_WARNING_NUM_5,
    T_WARNING_NUM_6,
    T_WARNING_NUM_7,
    T_WARNING_NUM_8,
    T_WARNING_NUM_9,

    T_WARNING_FAST_TEST,

    ///新增项放到此处后面
    T_WARNING_TEST_WAV,
    //新增提示音
    T_WARNING_BALABALA,

    T_WARNING_ALL,
};

#define MAX_LANG_VOICE_NUM      T_WARNING_LOW_BATTERY

#define PIANO_POWER_ON          xcfg_cb.v_power_sel
#define PIANO_POWER_OFF         xcfg_cb.v_power_sel
#define PIANO_PAIR              xcfg_cb.v_pairing_sel
#define PIANO_BT_DISCONNECT     xcfg_cb.v_connect_sel
#define PIANO_BT_CONNECT        xcfg_cb.v_connect_sel
#define PIANO_LOW_BATTERY       xcfg_cb.v_lowbattery_sel
#define PIANO_REDIALING         xcfg_cb.v_redialing_sel
#define PIANO_HANGUP            xcfg_cb.v_redialing_sel
#define PIANO_REJECT            xcfg_cb.v_redialing_sel
#define PIANO_TWS_CH            xcfg_cb.v_channel_sel
#define PIANO_MAX_VOL           xcfg_cb.v_maxvol_sel
#define PIANO_MIN_VOL           xcfg_cb.v_minvol_sel
#define PIANO_BT_RING           xcfg_cb.v_ring_sel
#define PIANO_CAMERA_ON         xcfg_cb.v_cameraon_sel
#define PIANO_CAMERA_OFF        xcfg_cb.v_cameraon_sel
#define PIANO_BT_MODE           xcfg_cb.v_btmode_sel
#define PIANO_CAMERA_MODE       0
#define PIANO_AUX_MODE          xcfg_cb.v_auxmode_sel
#define PIANO_TAKE_PHOTO        xcfg_cb.v_take_photo_sel
#define PIANO_SWITCH_LANG       xcfg_cb.v_lang_switch_sel
#define PIANO_GAME_MODE         xcfg_cb.v_game_mode_sel

void sys_warning_play(uint index, uint type);       //type: 0->无提示音, 1->piano, 2->语音
bool sys_warning_play_do(uint index, uint type, u32 tws_sync);
void piano_warning_play(uint index, uint tone_index, u32 tws_sync);
u8 get_piano_tone_sta(void);
void piano_var_init(void);
bool tone_is_playing(void);
void tone_play_start(void);
void tone_play_kick(void);
void tone_play_end(void);
bool bt_ring_tone_play(u32 tws_sync);
bool bt_ring_tone_stop(u32 force);
bool warning_is_playing(void);
u8 warning_volume_get(u32 sys_vol, u8 index);
#endif
