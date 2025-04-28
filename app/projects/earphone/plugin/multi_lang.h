#ifndef _MULTI_LANG_H
#define _MULTI_LANG_H

#define LANG_EN         0
#define LANG_ZH         1
#define LANG_EN_ZH      2
#define LANG_SELECT     LANG_EN_ZH

enum {
    IDX_LANGUAGE_SBC,
    IDX_LOW_BATTERY_SBC,
    IDX_POWERON_SBC,
    IDX_POWEROFF_SBC,
    IDX_MAX_VOL_SBC,
    IDX_SDCARD_MODE_SBC,
    IDX_USB_MODE_SBC,
    IDX_AUX_MODE_SBC,
    IDX_CLOCK_MODE_SBC,
    IDX_FM_MODE_SBC,
    IDX_SPK_MODE_SBC,
    IDX_PC_MODE_SBC,
    IDX_BT_MODE_SBC,
    IDX_CAMERA_MODE_SBC,
    IDX_LEFT_CH_SBC,
    IDX_RIGHT_CH_SBC,
    IDX_PAIRING_SBC,
    IDX_WAIT4CONN_SBC,
    IDX_CONNECTED_SBC,
    IDX_DISCONNECT_SBC,
    IDX_CAMERA_ON_SBC,
    IDX_CAMERA_OFF_SBC,
    IDX_CALL_HANGUP_SBC,
    IDX_CALL_REJECT_SBC,
    IDX_REDIALING_SBC,
    IDX_MUSIC_MODE_SBC,
    IDX_GAME_MODE_SBC,
    IDX_MIN_VOL_SBC,
    IDX_NUM_0_SBC,
    IDX_NUM_1_SBC,
    IDX_NUM_2_SBC,
    IDX_NUM_3_SBC,
    IDX_NUM_4_SBC,
    IDX_NUM_5_SBC,
    IDX_NUM_6_SBC,
    IDX_NUM_7_SBC,
    IDX_NUM_8_SBC,
    IDX_NUM_9_SBC,
    IDX_MAX_SBC,
};

typedef struct {
    u32 *ptr;
    u32 *len;
} res_addr_t;

const res_addr_t *res_get_ring_num(u8 index);

#if (LANG_SELECT == LANG_EN_ZH)

extern const res_addr_t mul_lang_tbl[2][IDX_MAX_SBC];
#define MUTIL_LANG_TBL_EN           1
#define RES_BUF_LANGUAGE_SBC        *mul_lang_tbl[sys_cb.lang_id][IDX_LANGUAGE_SBC   ].ptr
#define RES_LEN_LANGUAGE_SBC        *mul_lang_tbl[sys_cb.lang_id][IDX_LANGUAGE_SBC   ].len
#define RES_BUF_LOW_BATTERY_SBC     *mul_lang_tbl[sys_cb.lang_id][IDX_LOW_BATTERY_SBC].ptr
#define RES_LEN_LOW_BATTERY_SBC     *mul_lang_tbl[sys_cb.lang_id][IDX_LOW_BATTERY_SBC].len
#define RES_BUF_POWERON_SBC         *mul_lang_tbl[sys_cb.lang_id][IDX_POWERON_SBC    ].ptr
#define RES_LEN_POWERON_SBC         *mul_lang_tbl[sys_cb.lang_id][IDX_POWERON_SBC    ].len
#define RES_BUF_POWEROFF_SBC        *mul_lang_tbl[sys_cb.lang_id][IDX_POWEROFF_SBC   ].ptr
#define RES_LEN_POWEROFF_SBC        *mul_lang_tbl[sys_cb.lang_id][IDX_POWEROFF_SBC   ].len
//#define RES_BUF_SDCARD_MODE_SBC     *mul_lang_tbl[sys_cb.lang_id][IDX_SDCARD_MODE_SBC].ptr
//#define RES_LEN_SDCARD_MODE_SBC     *mul_lang_tbl[sys_cb.lang_id][IDX_SDCARD_MODE_SBC].len
//#define RES_BUF_USB_MODE_SBC        *mul_lang_tbl[sys_cb.lang_id][IDX_USB_MODE_SBC   ].ptr
//#define RES_LEN_USB_MODE_SBC        *mul_lang_tbl[sys_cb.lang_id][IDX_USB_MODE_SBC   ].len
#define RES_BUF_AUX_MODE_SBC        *mul_lang_tbl[sys_cb.lang_id][IDX_AUX_MODE_SBC   ].ptr
#define RES_LEN_AUX_MODE_SBC        *mul_lang_tbl[sys_cb.lang_id][IDX_AUX_MODE_SBC   ].len
//#define RES_BUF_CLOCK_MODE_SBC      *mul_lang_tbl[sys_cb.lang_id][IDX_CLOCK_MODE_SBC ].ptr
//#define RES_LEN_CLOCK_MODE_SBC      *mul_lang_tbl[sys_cb.lang_id][IDX_CLOCK_MODE_SBC ].len
//#define RES_BUF_FM_MODE_SBC         *mul_lang_tbl[sys_cb.lang_id][IDX_FM_MODE_SBC    ].ptr
//#define RES_LEN_FM_MODE_SBC         *mul_lang_tbl[sys_cb.lang_id][IDX_FM_MODE_SBC    ].len
//#define RES_BUF_SPK_MODE_SBC        *mul_lang_tbl[sys_cb.lang_id][IDX_SPK_MODE_SBC   ].ptr
//#define RES_LEN_SPK_MODE_SBC        *mul_lang_tbl[sys_cb.lang_id][IDX_SPK_MODE_SBC   ].len
//#define RES_BUF_PC_MODE_SBC         *mul_lang_tbl[sys_cb.lang_id][IDX_PC_MODE_SBC    ].ptr
//#define RES_LEN_PC_MODE_SBC         *mul_lang_tbl[sys_cb.lang_id][IDX_PC_MODE_SBC    ].len
#define RES_BUF_MAX_VOL_SBC         *mul_lang_tbl[sys_cb.lang_id][IDX_MAX_VOL_SBC    ].ptr
#define RES_LEN_MAX_VOL_SBC         *mul_lang_tbl[sys_cb.lang_id][IDX_MAX_VOL_SBC    ].len
#define RES_BUF_MIN_VOL_SBC         *mul_lang_tbl[sys_cb.lang_id][IDX_MIN_VOL_SBC    ].ptr
#define RES_LEN_MIN_VOL_SBC         *mul_lang_tbl[sys_cb.lang_id][IDX_MIN_VOL_SBC    ].len
#define RES_BUF_BT_MODE_SBC         *mul_lang_tbl[sys_cb.lang_id][IDX_BT_MODE_SBC    ].ptr
#define RES_LEN_BT_MODE_SBC         *mul_lang_tbl[sys_cb.lang_id][IDX_BT_MODE_SBC    ].len
#define RES_BUF_CAMERA_MODE_SBC     *mul_lang_tbl[sys_cb.lang_id][IDX_CAMERA_MODE_SBC].ptr
#define RES_LEN_CAMERA_MODE_SBC     *mul_lang_tbl[sys_cb.lang_id][IDX_CAMERA_MODE_SBC].len
#define RES_BUF_LEFT_CH_SBC         *mul_lang_tbl[sys_cb.lang_id][IDX_LEFT_CH_SBC    ].ptr
#define RES_LEN_LEFT_CH_SBC         *mul_lang_tbl[sys_cb.lang_id][IDX_LEFT_CH_SBC    ].len
#define RES_BUF_RIGHT_CH_SBC        *mul_lang_tbl[sys_cb.lang_id][IDX_RIGHT_CH_SBC   ].ptr
#define RES_LEN_RIGHT_CH_SBC        *mul_lang_tbl[sys_cb.lang_id][IDX_RIGHT_CH_SBC   ].len
#define RES_BUF_PAIRING_SBC         *mul_lang_tbl[sys_cb.lang_id][IDX_PAIRING_SBC    ].ptr
#define RES_LEN_PAIRING_SBC         *mul_lang_tbl[sys_cb.lang_id][IDX_PAIRING_SBC    ].len
#define RES_BUF_WAIT4CONN_SBC       *mul_lang_tbl[sys_cb.lang_id][IDX_WAIT4CONN_SBC  ].ptr
#define RES_LEN_WAIT4CONN_SBC       *mul_lang_tbl[sys_cb.lang_id][IDX_WAIT4CONN_SBC  ].len
#define RES_BUF_CONNECTED_SBC       *mul_lang_tbl[sys_cb.lang_id][IDX_CONNECTED_SBC  ].ptr
#define RES_LEN_CONNECTED_SBC       *mul_lang_tbl[sys_cb.lang_id][IDX_CONNECTED_SBC  ].len
#define RES_BUF_DISCONNECT_SBC      *mul_lang_tbl[sys_cb.lang_id][IDX_DISCONNECT_SBC ].ptr
#define RES_LEN_DISCONNECT_SBC      *mul_lang_tbl[sys_cb.lang_id][IDX_DISCONNECT_SBC ].len
#define RES_BUF_CAMERA_ON_SBC       *mul_lang_tbl[sys_cb.lang_id][IDX_CAMERA_ON_SBC  ].ptr
#define RES_LEN_CAMERA_ON_SBC       *mul_lang_tbl[sys_cb.lang_id][IDX_CAMERA_ON_SBC  ].len
#define RES_BUF_CAMERA_OFF_SBC      *mul_lang_tbl[sys_cb.lang_id][IDX_CAMERA_OFF_SBC ].ptr
#define RES_LEN_CAMERA_OFF_SBC      *mul_lang_tbl[sys_cb.lang_id][IDX_CAMERA_OFF_SBC ].len
#define RES_BUF_CALL_HANGUP_SBC     *mul_lang_tbl[sys_cb.lang_id][IDX_CALL_HANGUP_SBC].ptr
#define RES_LEN_CALL_HANGUP_SBC     *mul_lang_tbl[sys_cb.lang_id][IDX_CALL_HANGUP_SBC].len
#define RES_BUF_CALL_REJECT_SBC     *mul_lang_tbl[sys_cb.lang_id][IDX_CALL_REJECT_SBC].ptr
#define RES_LEN_CALL_REJECT_SBC     *mul_lang_tbl[sys_cb.lang_id][IDX_CALL_REJECT_SBC].len
#define RES_BUF_REDIALING_SBC       *mul_lang_tbl[sys_cb.lang_id][IDX_REDIALING_SBC  ].ptr
#define RES_LEN_REDIALING_SBC       *mul_lang_tbl[sys_cb.lang_id][IDX_REDIALING_SBC  ].len
#define RES_BUF_MUSIC_MODE_SBC      *mul_lang_tbl[sys_cb.lang_id][IDX_MUSIC_MODE_SBC  ].ptr
#define RES_LEN_MUSIC_MODE_SBC      *mul_lang_tbl[sys_cb.lang_id][IDX_MUSIC_MODE_SBC  ].len
#define RES_BUF_GAME_MODE_SBC       *mul_lang_tbl[sys_cb.lang_id][IDX_GAME_MODE_SBC  ].ptr
#define RES_LEN_GAME_MODE_SBC       *mul_lang_tbl[sys_cb.lang_id][IDX_GAME_MODE_SBC  ].len

#define RES_BUF_NUM_0_SBC           *mul_lang_tbl[sys_cb.lang_id][IDX_NUM_0_SBC      ].ptr
#define RES_LEN_NUM_0_SBC           *mul_lang_tbl[sys_cb.lang_id][IDX_NUM_0_SBC      ].len
#define RES_BUF_NUM_1_SBC           *mul_lang_tbl[sys_cb.lang_id][IDX_NUM_1_SBC      ].ptr
#define RES_LEN_NUM_1_SBC           *mul_lang_tbl[sys_cb.lang_id][IDX_NUM_1_SBC      ].len
#define RES_BUF_NUM_2_SBC           *mul_lang_tbl[sys_cb.lang_id][IDX_NUM_2_SBC      ].ptr
#define RES_LEN_NUM_2_SBC           *mul_lang_tbl[sys_cb.lang_id][IDX_NUM_2_SBC      ].len
#define RES_BUF_NUM_3_SBC           *mul_lang_tbl[sys_cb.lang_id][IDX_NUM_3_SBC      ].ptr
#define RES_LEN_NUM_3_SBC           *mul_lang_tbl[sys_cb.lang_id][IDX_NUM_3_SBC      ].len
#define RES_BUF_NUM_4_SBC           *mul_lang_tbl[sys_cb.lang_id][IDX_NUM_4_SBC      ].ptr
#define RES_LEN_NUM_4_SBC           *mul_lang_tbl[sys_cb.lang_id][IDX_NUM_4_SBC      ].len
#define RES_BUF_NUM_5_SBC           *mul_lang_tbl[sys_cb.lang_id][IDX_NUM_5_SBC      ].ptr
#define RES_LEN_NUM_5_SBC           *mul_lang_tbl[sys_cb.lang_id][IDX_NUM_5_SBC      ].len
#define RES_BUF_NUM_6_SBC           *mul_lang_tbl[sys_cb.lang_id][IDX_NUM_6_SBC      ].ptr
#define RES_LEN_NUM_6_SBC           *mul_lang_tbl[sys_cb.lang_id][IDX_NUM_6_SBC      ].len
#define RES_BUF_NUM_7_SBC           *mul_lang_tbl[sys_cb.lang_id][IDX_NUM_7_SBC      ].ptr
#define RES_LEN_NUM_7_SBC           *mul_lang_tbl[sys_cb.lang_id][IDX_NUM_7_SBC      ].len
#define RES_BUF_NUM_8_SBC           *mul_lang_tbl[sys_cb.lang_id][IDX_NUM_8_SBC      ].ptr
#define RES_LEN_NUM_8_SBC           *mul_lang_tbl[sys_cb.lang_id][IDX_NUM_8_SBC      ].len
#define RES_BUF_NUM_9_SBC           *mul_lang_tbl[sys_cb.lang_id][IDX_NUM_9_SBC      ].ptr
#define RES_LEN_NUM_9_SBC           *mul_lang_tbl[sys_cb.lang_id][IDX_NUM_9_SBC      ].len

#elif (LANG_SELECT == LANG_ZH)
#define RES_BUF_LOW_BATTERY_SBC         RES_BUF_ZH_LOW_BATTERY_SBC
#define RES_LEN_LOW_BATTERY_SBC         RES_LEN_ZH_LOW_BATTERY_SBC
#define RES_BUF_SDCARD_MODE_SBC         RES_BUF_ZH_SDCARD_MODE_SBC
#define RES_LEN_SDCARD_MODE_SBC         RES_LEN_ZH_SDCARD_MODE_SBC
#define RES_BUF_POWERON_SBC             RES_BUF_ZH_POWERON_SBC
#define RES_LEN_POWERON_SBC             RES_LEN_ZH_POWERON_SBC
#define RES_BUF_POWEROFF_SBC            RES_BUF_ZH_POWEROFF_SBC
#define RES_LEN_POWEROFF_SBC            RES_LEN_ZH_POWEROFF_SBC
#define RES_BUF_USB_MODE_SBC            RES_BUF_ZH_USB_MODE_SBC
#define RES_LEN_USB_MODE_SBC            RES_LEN_ZH_USB_MODE_SBC
#define RES_BUF_AUX_MODE_SBC            RES_BUF_ZH_AUX_MODE_SBC
#define RES_LEN_AUX_MODE_SBC            RES_LEN_ZH_AUX_MODE_SBC
#define RES_BUF_BT_MODE_SBC             RES_BUF_ZH_BT_MODE_SBC
#define RES_LEN_BT_MODE_SBC             RES_LEN_ZH_BT_MODE_SBC
#define RES_BUF_CLOCK_MODE_SBC          RES_BUF_ZH_CLOCK_MODE_SBC
#define RES_LEN_CLOCK_MODE_SBC          RES_LEN_ZH_CLOCK_MODE_SBC
#define RES_BUF_CAMERA_MODE_SBC         RES_BUF_ZH_CAMERA_MODE_SBC
#define RES_LEN_CAMERA_MODE_SBC         RES_LEN_ZH_CAMERA_MODE_SBC
#define RES_BUF_LEFT_CH_SBC             RES_BUF_ZH_LEFT_CH_SBC
#define RES_LEN_LEFT_CH_SBC             RES_LEN_ZH_LEFT_CH_SBC
#define RES_BUF_RIGHT_CH_SBC            RES_BUF_ZH_RIGHT_CH_SBC
#define RES_LEN_RIGHT_CH_SBC            RES_LEN_ZH_RIGHT_CH_SBC

#define RES_BUF_FM_MODE_SBC             RES_BUF_ZH_FM_MODE_SBC
#define RES_LEN_FM_MODE_SBC             RES_LEN_ZH_FM_MODE_SBC
#define RES_BUF_SPK_MODE_SBC            RES_BUF_ZH_SPK_MODE_SBC
#define RES_LEN_SPK_MODE_SBC            RES_LEN_ZH_SPK_MODE_SBC
#define RES_BUF_PC_MODE_SBC             RES_BUF_ZH_PC_MODE_SBC
#define RES_LEN_PC_MODE_SBC             RES_LEN_ZH_PC_MODE_SBC
#define RES_BUF_MAX_VOL_SBC             RES_BUF_ZH_MAX_VOL_SBC
#define RES_LEN_MAX_VOL_SBC             RES_LEN_ZH_MAX_VOL_SBC

#define RES_BUF_PAIRING_SBC             RES_BUF_ZH_PAIRING_SBC
#define RES_LEN_PAIRING_SBC             RES_LEN_ZH_PAIRING_SBC
#define RES_BUF_WAIT4CONN_SBC           RES_BUF_ZH_WAIT4CONN_SBC
#define RES_LEN_WAIT4CONN_SBC           RES_LEN_ZH_WAIT4CONN_SBC
#define RES_BUF_CONNECTED_SBC           RES_BUF_ZH_CONNECTED_SBC
#define RES_LEN_CONNECTED_SBC           RES_LEN_ZH_CONNECTED_SBC
#define RES_BUF_DISCONNECT_SBC          RES_BUF_ZH_DISCONNECT_SBC
#define RES_LEN_DISCONNECT_SBC          RES_LEN_ZH_DISCONNECT_SBC
#define RES_BUF_CAMERA_ON_SBC           RES_BUF_ZH_CAMERA_ON_SBC
#define RES_LEN_CAMERA_ON_SBC           RES_LEN_ZH_CAMERA_ON_SBC
#define RES_BUF_CAMERA_OFF_SBC          RES_BUF_ZH_CAMERA_OFF_SBC
#define RES_LEN_CAMERA_OFF_SBC          RES_LEN_ZH_CAMERA_OFF_SBC
#define RES_BUF_CALL_HANGUP_SBC         RES_BUF_ZH_CALL_HANGUP_SBC
#define RES_LEN_CALL_HANGUP_SBC         RES_LEN_ZH_CALL_HANGUP_SBC
#define RES_BUF_CALL_REJECT_SBC         RES_BUF_ZH_CALL_REJECT_SBC
#define RES_LEN_CALL_REJECT_SBC         RES_LEN_ZH_CALL_REJECT_SBC
#define RES_BUF_REDIALING_SBC           RES_BUF_ZH_REDIALING_SBC
#define RES_LEN_REDIALING_SBC           RES_LEN_ZH_REDIALING_SBC
#define RES_BUF_MUSIC_MODE_SBC          RES_BUF_ZH_MUSIC_MODE_SBC
#define RES_LEN_MUSIC_MODE_SBC          RES_LEN_ZH_MUSIC_MODE_SBC
#define RES_BUF_GAME_MODE_SBC           RES_BUF_ZH_GAME_MODE_SBC
#define RES_LEN_GAME_MODE_SBC           RES_LEN_ZH_GAME_MODE_SBC

#define RES_BUF_NUM_0_SBC               RES_BUF_ZH_NUM_0_SBC
#define RES_LEN_NUM_0_SBC               RES_LEN_ZH_NUM_0_SBC
#define RES_BUF_NUM_1_SBC               RES_BUF_ZH_NUM_1_SBC
#define RES_LEN_NUM_1_SBC               RES_LEN_ZH_NUM_1_SBC
#define RES_BUF_NUM_2_SBC               RES_BUF_ZH_NUM_2_SBC
#define RES_LEN_NUM_2_SBC               RES_LEN_ZH_NUM_2_SBC
#define RES_BUF_NUM_3_SBC               RES_BUF_ZH_NUM_3_SBC
#define RES_LEN_NUM_3_SBC               RES_LEN_ZH_NUM_3_SBC
#define RES_BUF_NUM_4_SBC               RES_BUF_ZH_NUM_4_SBC
#define RES_LEN_NUM_4_SBC               RES_LEN_ZH_NUM_4_SBC
#define RES_BUF_NUM_5_SBC               RES_BUF_ZH_NUM_5_SBC
#define RES_LEN_NUM_5_SBC               RES_LEN_ZH_NUM_5_SBC
#define RES_BUF_NUM_6_SBC               RES_BUF_ZH_NUM_6_SBC
#define RES_LEN_NUM_6_SBC               RES_LEN_ZH_NUM_6_SBC
#define RES_BUF_NUM_7_SBC               RES_BUF_ZH_NUM_7_SBC
#define RES_LEN_NUM_7_SBC               RES_LEN_ZH_NUM_7_SBC
#define RES_BUF_NUM_8_SBC               RES_BUF_ZH_NUM_8_SBC
#define RES_LEN_NUM_8_SBC               RES_LEN_ZH_NUM_8_SBC
#define RES_BUF_NUM_9_SBC               RES_BUF_ZH_NUM_9_SBC
#define RES_LEN_NUM_9_SBC               RES_LEN_ZH_NUM_9_SBC

#elif (LANG_SELECT == LANG_EN)
#define RES_BUF_LOW_BATTERY_SBC         RES_BUF_EN_LOW_BATTERY_SBC
#define RES_LEN_LOW_BATTERY_SBC         RES_LEN_EN_LOW_BATTERY_SBC
#define RES_BUF_SDCARD_MODE_SBC         RES_BUF_EN_SDCARD_MODE_SBC
#define RES_LEN_SDCARD_MODE_SBC         RES_LEN_EN_SDCARD_MODE_SBC
#define RES_BUF_POWERON_SBC             RES_BUF_EN_POWERON_SBC
#define RES_LEN_POWERON_SBC             RES_LEN_EN_POWERON_SBC
#define RES_BUF_POWEROFF_SBC            RES_BUF_EN_POWEROFF_SBC
#define RES_LEN_POWEROFF_SBC            RES_LEN_EN_POWEROFF_SBC
#define RES_BUF_USB_MODE_SBC            RES_BUF_EN_USB_MODE_SBC
#define RES_LEN_USB_MODE_SBC            RES_LEN_EN_USB_MODE_SBC
#define RES_BUF_AUX_MODE_SBC            RES_BUF_EN_AUX_MODE_SBC
#define RES_LEN_AUX_MODE_SBC            RES_LEN_EN_AUX_MODE_SBC
#define RES_BUF_BT_MODE_SBC             RES_BUF_EN_BT_MODE_SBC
#define RES_LEN_BT_MODE_SBC             RES_LEN_EN_BT_MODE_SBC
#define RES_BUF_CLOCK_MODE_SBC          RES_BUF_EN_CLOCK_MODE_SBC
#define RES_LEN_CLOCK_MODE_SBC          RES_LEN_EN_CLOCK_MODE_SBC
#define RES_BUF_CAMERA_MODE_SBC         RES_BUF_EN_CAMERA_MODE_SBC
#define RES_LEN_CAMERA_MODE_SBC         RES_LEN_EN_CAMERA_MODE_SBC
#define RES_BUF_LEFT_CH_SBC             RES_BUF_EN_LEFT_CH_SBC
#define RES_LEN_LEFT_CH_SBC             RES_LEN_EN_LEFT_CH_SBC
#define RES_BUF_RIGHT_CH_SBC            RES_BUF_EN_RIGHT_CH_SBC
#define RES_LEN_RIGHT_CH_SBC            RES_LEN_EN_RIGHT_CH_SBC

#define RES_BUF_FM_MODE_SBC             RES_BUF_EN_FM_MODE_SBC
#define RES_LEN_FM_MODE_SBC             RES_LEN_EN_FM_MODE_SBC
#define RES_BUF_SPK_MODE_SBC            RES_BUF_EN_SPK_MODE_SBC
#define RES_LEN_SPK_MODE_SBC            RES_LEN_EN_SPK_MODE_SBC
#define RES_BUF_PC_MODE_SBC             RES_BUF_EN_PC_MODE_SBC
#define RES_LEN_PC_MODE_SBC             RES_LEN_EN_PC_MODE_SBC
#define RES_BUF_MAX_VOL_SBC             RES_BUF_EN_MAX_VOL_SBC
#define RES_LEN_MAX_VOL_SBC             RES_LEN_EN_MAX_VOL_SBC

#define RES_BUF_PAIRING_SBC             RES_BUF_EN_PAIRING_SBC
#define RES_LEN_PAIRING_SBC             RES_LEN_EN_PAIRING_SBC
#define RES_BUF_WAIT4CONN_SBC           RES_BUF_EN_WAIT4CONN_SBC
#define RES_LEN_WAIT4CONN_SBC           RES_LEN_EN_WAIT4CONN_SBC
#define RES_BUF_CONNECTED_SBC           RES_BUF_EN_CONNECTED_SBC
#define RES_LEN_CONNECTED_SBC           RES_LEN_EN_CONNECTED_SBC
#define RES_BUF_DISCONNECT_SBC          RES_BUF_EN_DISCONNECT_SBC
#define RES_LEN_DISCONNECT_SBC          RES_LEN_EN_DISCONNECT_SBC
#define RES_BUF_CAMERA_ON_SBC           RES_BUF_EN_CAMERA_ON_SBC
#define RES_LEN_CAMERA_ON_SBC           RES_LEN_EN_CAMERA_ON_SBC
#define RES_BUF_CAMERA_OFF_SBC          RES_BUF_EN_CAMERA_OFF_SBC
#define RES_LEN_CAMERA_OFF_SBC          RES_LEN_EN_CAMERA_OFF_SBC
#define RES_BUF_CALL_HANGUP_SBC         RES_BUF_EN_CALL_HANGUP_SBC
#define RES_LEN_CALL_HANGUP_SBC         RES_LEN_EN_CALL_HANGUP_SBC
#define RES_BUF_CALL_REJECT_SBC         RES_BUF_EN_CALL_REJECT_SBC
#define RES_LEN_CALL_REJECT_SBC         RES_LEN_EN_CALL_REJECT_SBC
#define RES_BUF_REDIALING_SBC           RES_BUF_EN_REDIALING_SBC
#define RES_LEN_REDIALING_SBC           RES_LEN_EN_REDIALING_SBC
#define RES_BUF_MUSIC_MODE_SBC          RES_BUF_EN_MUSIC_MODE_SBC
#define RES_LEN_MUSIC_MODE_SBC          RES_LEN_EN_MUSIC_MODE_SBC
#define RES_BUF_GAME_MODE_SBC           RES_BUF_EN_GAME_MODE_SBC
#define RES_LEN_GAME_MODE_SBC           RES_LEN_EN_GAME_MODE_SBC

#define RES_BUF_NUM_0_SBC               RES_BUF_EN_NUM_0_SBC
#define RES_LEN_NUM_0_SBC               RES_LEN_EN_NUM_0_SBC
#define RES_BUF_NUM_1_SBC               RES_BUF_EN_NUM_1_SBC
#define RES_LEN_NUM_1_SBC               RES_LEN_EN_NUM_1_SBC
#define RES_BUF_NUM_2_SBC               RES_BUF_EN_NUM_2_SBC
#define RES_LEN_NUM_2_SBC               RES_LEN_EN_NUM_2_SBC
#define RES_BUF_NUM_3_SBC               RES_BUF_EN_NUM_3_SBC
#define RES_LEN_NUM_3_SBC               RES_LEN_EN_NUM_3_SBC
#define RES_BUF_NUM_4_SBC               RES_BUF_EN_NUM_4_SBC
#define RES_LEN_NUM_4_SBC               RES_LEN_EN_NUM_4_SBC
#define RES_BUF_NUM_5_SBC               RES_BUF_EN_NUM_5_SBC
#define RES_LEN_NUM_5_SBC               RES_LEN_EN_NUM_5_SBC
#define RES_BUF_NUM_6_SBC               RES_BUF_EN_NUM_6_SBC
#define RES_LEN_NUM_6_SBC               RES_LEN_EN_NUM_6_SBC
#define RES_BUF_NUM_7_SBC               RES_BUF_EN_NUM_7_SBC
#define RES_LEN_NUM_7_SBC               RES_LEN_EN_NUM_7_SBC
#define RES_BUF_NUM_8_SBC               RES_BUF_EN_NUM_8_SBC
#define RES_LEN_NUM_8_SBC               RES_LEN_EN_NUM_8_SBC
#define RES_BUF_NUM_9_SBC               RES_BUF_EN_NUM_9_SBC
#define RES_LEN_NUM_9_SBC               RES_LEN_EN_NUM_9_SBC

#else
    #error "请选择提示音语言\n"
#endif

#endif
