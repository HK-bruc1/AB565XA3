#include "include.h"

u8 get_piano_index(u8 num);
bool bt_ring_msbc_play(uint index, u32 tws_sync);
void bt_tws_ring_tone_stop(void);
void tws_set_sta_for_res(uint32_t res);
void dac_digvol_fade(u32 flag, u32 step);
int bt_ring_esbc_play(u8 index, const u8 *ptr, const u8 *sptr, u32 size, u32 tws_sync);
int bt_ring_sbc_play(uint index, u32 tws_sync);
bool esbc_inner_res_play(uint index, u32 tws_sync);

AT(.text.music.res)
bool sys_warning_play_do(uint index, uint type, u32 tws_sync)
{
    bool res_play = false;

#if VUSB_TBOX_QTEST_EN
    if (!get_qtest_mode())
#endif
    {
        if (type == 1) {
            //tone音
            piano_warning_play(index, get_piano_index(index), tws_sync);
            res_play = true;
        } else if (type == 2) {
            //esbc
            if (!esbc_res_play(index, (const u8 *)sbcplay_cb.buf, NULL, sbcplay_cb.len, tws_sync)) {
				res_play = true;
			}
        } else if (type == 3) {
            //sbc
            sbc_res_play(index, tws_sync);
            res_play = true;
        } else if (type == 4) {
            //wav提示音
            wav_res_play_do((u32)sbcplay_cb.buf, sbcplay_cb.len, tws_sync);
            res_play = true;
        } else if (type == 14) {
            //ring
            res_play = bt_ring_tone_play(tws_sync);
        } else if (type == 15) {
            //call number
            res_play = bt_ring_msbc_play(index, tws_sync);
        } else if (type == 16) {
            //ring esbc
            if (!bt_ring_esbc_play(index, (const u8 *)sbcplay_cb.buf, NULL, sbcplay_cb.len, tws_sync)) {
				res_play = true;
			}
        } else if (type == 17) {
            //ring sbc
            if (!bt_ring_sbc_play(index, tws_sync)) {
				res_play = true;
			}
        } else if (type == 18) {
            //播放rom内置esbc提示音
            res_play = esbc_inner_res_play(index, tws_sync);
        } else {
            res_play = false;
        }
    }

    return res_play;
}

AT(.text.music.res)
void tws_res_get_addr(u32 index, u32 *res_index, u32 *res_type)
{
    *res_index = 0;
    *res_type = 0;
    switch(index) {

    case TWS_RES_CONNECTED:
        f_bt.w4_tws_warning = 0;
        *res_index = T_WARNING_BT_CONNECT;
        *res_type = sbcplay_set(T_WARNING_BT_CONNECT, PIANO_TWS_CH);
        break;

    case TWS_RES_NOR_CONNECTED:
        *res_index = T_WARNING_BT_CONNECT;
        *res_type = sbcplay_set(T_WARNING_BT_CONNECT, PIANO_BT_CONNECT);
        break;

    case TWS_RES_DISCONNECT:
        *res_index = T_WARNING_BT_DISCONNECT;
        *res_type = sbcplay_set(T_WARNING_BT_DISCONNECT, PIANO_BT_DISCONNECT);
        break;

    case TWS_RES_PAIRING:
        *res_index = T_WARNING_PAIRING;
        *res_type = sbcplay_set(T_WARNING_PAIRING, PIANO_PAIR);
        if (*res_type == 0) {
            *res_type = 18;     //播放rom内置esbc提示音
        }
        break;

    case TWS_RES_MAX_VOL:
        *res_index = T_WARNING_MAX_VOL;
        *res_type = sbcplay_set(T_WARNING_MAX_VOL, PIANO_MAX_VOL);
        break;

    case TWS_RES_MIN_VOL:
        *res_index = T_WARNING_MIN_VOL;
        *res_type = sbcplay_set(T_WARNING_MIN_VOL, PIANO_MIN_VOL);
        break;

	case TWS_RES_TONE:
        *res_index = T_WARNING_NEXT_TRACK;
        *res_type = 1;
        break;

    case TWS_RES_LANGUAGE:
        *res_index = T_WARNING_SWITCH_LANG;
        *res_type = sbcplay_set(T_WARNING_SWITCH_LANG, PIANO_SWITCH_LANG);
        if (*res_type == 0) {
            *res_type = 18;     //播放rom内置esbc提示音
        }
        break;

    case TWS_RES_MUSIC_MODE:
        *res_index = T_WARNING_MUSIC_MODE;
        *res_type = sbcplay_set(T_WARNING_MUSIC_MODE, 2);
        if (*res_type == 0) {
            *res_type = 18;     //播放rom内置esbc提示音
        }
        break;

    case TWS_RES_GAME_MODE:
        *res_index = T_WARNING_GAME_MODE;
        *res_type = sbcplay_set(T_WARNING_GAME_MODE, 2);
        if (*res_type == 0) {
            *res_type = 18;     //播放rom内置esbc提示音
        }
        break;

    case TWS_RES_WAV_TEST:
        *res_index = T_WARNING_TEST_WAV;
        *res_type = sbcplay_set(T_WARNING_TEST_WAV, 4);
        break;

#if LED_BREATHE_EN
    case TWS_BRE_LED_START:
        breathe_led_start(T_BRE_TWS_CON, sys_led.tws_con_bre_cfg);
        break;

    case TWS_BRE_LED_START1:
        breathe_led_start(T_BRE_TWS_PLAY, sys_led.tws_play_bre_cfg);
        break;

    case TWS_BRE_LED_STOP:
        breathe_led_stop();
        break;
#endif // LED_BREATHE_EN

    case TWS_ATMOS_LED_ON:
        atmosphere_led_sta_set(1);
        break;
    case TWS_ATMOS_LED_OFF:
        atmosphere_led_sta_set(0);
        break;

    case TWS_RES_RING:
        *res_index = T_WARNING_BT_RING;
        *res_type = sbcplay_set(T_WARNING_BT_RING, 2);
        if (*res_type == 2) {
            *res_type = 16;
        } else if (*res_type == 0) {
            *res_type = 14;
        } else if (*res_type == 3) {
            *res_type = 17;
        }
        break;

    default:
        if (index >= TWS_RES_NUM_0 && index <= TWS_RES_NUM_9) {
            *res_index = T_WARNING_NUM_0 + index - TWS_RES_NUM_0;
            *res_type = 15;
        }
        break;
    }
}

AT(.text.music.res)
void tws_res_proc(void)
{
    u32 res_index, res_type;
    u32 res = tws_res_sync(800);
    if (res == -1L) {
        return;
    } else if (res == -2L) {
        tws_res_done();
        return;
    }
    res &= 0xff;
    sys_cb.tws_res_brk = 0;
    tws_res_get_addr(res, &res_index, &res_type);
#if BT_LOW_LATENCY_QUICK_FIX
    tws_set_sta_for_res(res);
#endif
    if (res == TWS_RES_RING_STOP) {
        bt_tws_ring_tone_stop();
    } else {
        if (!sys_warning_play_do(res_index, res_type, 1)) {
            tws_res_done();
        }
    }
#if !BT_LOW_LATENCY_QUICK_FIX
    tws_set_sta_for_res(res);
#endif
}

void wav_res_play(u32 addr, u32 len)
{
#if VUSB_TBOX_QTEST_EN
    if(get_qtest_mode()){
        return;
    }
#endif
    wav_res_play_do(addr, len, 0);
}

