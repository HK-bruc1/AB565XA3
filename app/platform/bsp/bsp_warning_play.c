#include "include.h"

#define TRACE_EN                0

#if TRACE_EN
#define TRACE(...)              printf(__VA_ARGS__)
#else
#define TRACE(...)
#endif

sbc_res_t sbc_res;
sbcplay_cb_t sbcplay_cb;

uint bt_is_w4_pwrkey5s(void);
bool sys_warning_play_do(uint index, uint type, u32 tws_sync);
void sfunc_bt_ring_sync(void);
int sfunc_bt_ring_res_play(u8 num);
void bt_warning_set_playing(u8 flag);

AT(.text.sbc)
bool sbc_res_dec_frame(void)
{
    uint len;
    sbc_t *sbc = &sbc_dec.sbc;
    if (sbcplay_cb.len) {
        len = sbcplay_cb.len > 256 ? 256 : sbcplay_cb.len;
        memcpy(sbc_dec.buf, sbcplay_cb.buf, len);
        //TRACE("D: %x %d\n", sbcplay_cb.buf, len);
        if (sbc_decode(sbc, sbc_dec.buf, len) < 0) {
            sbcdec_end();
            return false;
        }
        sbcplay_cb.buf += len;
        sbcplay_cb.len -= len;
        return true;
    }
    //TRACE("End\n");
    return false;
}


//获取音乐文件
AT(.text.warning)
u32 sbcplay_set(uint index, uint type)
{
    sbcplay_cb.buf = NULL;
    sbcplay_cb.len = 0;

    switch (index) {
    case T_WARNING_POWER_ON:
        sbcplay_cb.buf = (u8 *)RES_BUF_POWERON_SBC;
        sbcplay_cb.len = RES_LEN_POWERON_SBC;
        break;

    case T_WARNING_POWER_OFF:
        sbcplay_cb.buf = (u8 *)RES_BUF_POWEROFF_SBC;
        sbcplay_cb.len = RES_LEN_POWEROFF_SBC;
        break;

    case T_WARNING_PAIRING:
        sbcplay_cb.buf = (u8 *)RES_BUF_PAIRING_SBC;
        sbcplay_cb.len = RES_LEN_PAIRING_SBC;
        break;

    case T_WARNING_BT_DISCONNECT:
        sbcplay_cb.buf = (u8 *)RES_BUF_DISCONNECT_SBC;
        sbcplay_cb.len = RES_LEN_DISCONNECT_SBC;
        break;

    case T_WARNING_BT_CONNECT:
        sbcplay_cb.buf = (u8 *)RES_BUF_CONNECTED_SBC;
        sbcplay_cb.len = RES_LEN_CONNECTED_SBC;
        break;

    case T_WARNING_BT_RING:
        sbcplay_cb.buf = (u8 *)RES_BUF_RING_SBC;
        sbcplay_cb.len = RES_LEN_RING_SBC;
        break;

    case T_WARNING_MAX_VOL:
        sbcplay_cb.buf = (u8 *)RES_BUF_MAX_VOL_SBC;
        sbcplay_cb.len = RES_LEN_MAX_VOL_SBC;
        break;

    case T_WARNING_SWITCH_LANG:
        sbcplay_cb.buf = (u8 *)RES_BUF_LANGUAGE_SBC;
        sbcplay_cb.len = RES_LEN_LANGUAGE_SBC;
        break;

    case T_WARNING_LOW_BATTERY:
        sbcplay_cb.buf = (u8 *)RES_BUF_LOW_BATTERY_SBC;
        sbcplay_cb.len = RES_LEN_LOW_BATTERY_SBC;
        break;

    case T_WARNING_CAMERA_ON:
        sbcplay_cb.buf = (u8 *)RES_BUF_CAMERA_ON_SBC;
        sbcplay_cb.len = RES_LEN_CAMERA_ON_SBC;
        break;

    case T_WARNING_CAMERA_OFF:
        sbcplay_cb.buf = (u8 *)RES_BUF_CAMERA_OFF_SBC;
        sbcplay_cb.len = RES_LEN_CAMERA_OFF_SBC;
        break;

    case T_WARNING_REDIALING:
        sbcplay_cb.buf = (u8 *)RES_BUF_REDIALING_SBC;
        sbcplay_cb.len = RES_LEN_REDIALING_SBC;
        break;

    case T_WARNING_HANGUP:
        sbcplay_cb.buf = (u8 *)RES_BUF_CALL_HANGUP_SBC;
        sbcplay_cb.len = RES_LEN_CALL_HANGUP_SBC;
        break;

    case T_WARNING_REJECT:
        sbcplay_cb.buf = (u8 *)RES_BUF_CALL_REJECT_SBC;
        sbcplay_cb.len = RES_LEN_CALL_REJECT_SBC;
        break;

    case T_WARNING_BT_MODE:
        sbcplay_cb.buf = (u8 *)RES_BUF_BT_MODE_SBC;
        sbcplay_cb.len = RES_LEN_BT_MODE_SBC;
        break;

    case T_WARNING_CAMERA_MODE:
        sbcplay_cb.buf = (u8 *)RES_BUF_CAMERA_MODE_SBC;
        sbcplay_cb.len = RES_LEN_CAMERA_MODE_SBC;
        break;

    case T_WARNING_AUX_MODE:
        sbcplay_cb.buf = (u8 *)RES_BUF_AUX_MODE_SBC;
        sbcplay_cb.len = RES_LEN_AUX_MODE_SBC;
        break;

    case T_WARNING_MIN_VOL:
        sbcplay_cb.buf = (u8 *)RES_BUF_MIN_VOL_SBC;
        sbcplay_cb.len = RES_LEN_MIN_VOL_SBC;
        break;

    case T_WARNING_TAKE_PHOTO:
        break;

    case T_WARNING_RIGHT_CH:
        sbcplay_cb.buf = (u8 *)RES_BUF_RIGHT_CH_SBC;
        sbcplay_cb.len = RES_LEN_RIGHT_CH_SBC;
        break;

    case T_WARNING_LEFT_CH:
        sbcplay_cb.buf = (u8 *)RES_BUF_LEFT_CH_SBC;
        sbcplay_cb.len = RES_LEN_LEFT_CH_SBC;
        break;

    case T_WARNING_EAR_IN:
        sbcplay_cb.buf = (u8 *)RES_BUF_DU_SBC;
        sbcplay_cb.len = RES_LEN_DU_SBC;
        break;

    case T_WARNING_MUSIC_MODE:
        sbcplay_cb.buf = (u8 *)RES_BUF_MUSIC_MODE_SBC;
        sbcplay_cb.len = RES_LEN_MUSIC_MODE_SBC;
        break;

    case T_WARNING_GAME_MODE:
        sbcplay_cb.buf = (u8 *)RES_BUF_GAME_MODE_SBC;
        sbcplay_cb.len = RES_LEN_GAME_MODE_SBC;
        break;

    case T_WARNING_TEST_WAV:
        sbcplay_cb.buf = (u8 *)RES_BUF_TEST_WAV;
        sbcplay_cb.len = RES_LEN_TEST_WAV;
        break;

    default:
        break;
    }
    if (type == 1) {                    //配置中选择了tone音
        return 1;
    }
    if ((sbcplay_cb.buf != NULL) && (sbcplay_cb.len != 0)) {
        if (sbcplay_cb.buf[0] == 0x8B) {
            return 2;                   //esbc 8k samplerate
        } else if (sbcplay_cb.buf[0] == 0x9C) {
            return 3;                   //sbc 16k samplerate
        } else if (GET_LE32(sbcplay_cb.buf) == 0x46464952) {  //"RIFF"
            return 4;                   //wave提示音
        }
    }
    return 0;
}



AT(.text.warning)
void sbc_res_var_init(void)
{
    memset(&sbc_res,0,sizeof(sbc_res));
}

AT(.text.warning)
bool bt_ring_sbc_stop(u8 force)
{
    if(f_bt_ring1.sbc_flag == 0){
        return false;
    }

    if((get_music_dec_sta() != MUSIC_STOP)&&(sys_cb.tws_res_brk == 0) && (!force)){
        return false;
    }

    if (sys_cb.tws_res_brk) {
        sys_cb.tws_res_brk = 0;
    }

    res_play_stop(sbc_res.res_dnr_sta);
    bt_decode_exit();
    bt_set_sbc_playing(0);
    if (sbc_res.res_bypass == 1) {
        bt_audio_enable();
    }
#if FUNC_AUX_EN
    else if (sbc_res.res_bypass == 2) {
        func_aux_start();
    }
#endif // FUNC_AUX_EN
    func_bt_set_dac(sbc_res.res_dac_sta);

    f_bt_ring1.sbc_flag = 0;

    return true;

}

//播放提示音接口
AT(.text.warning)
bool bt_ring_sbc_play(uint index, u32 tws_sync)
{

    WDT_CLR();
    bt_ring_sbc_stop(1);
    sbc_res.res_dac_sta = sys_cb.dac_sta;
    func_bt_set_dac(1);

    if (bt_get_status() != BT_STA_OFF) {
        if (!bt_is_w4_pwrkey5s()) {
            bt_audio_bypass();
//            bypass = 1;
            sbc_res.res_bypass = 1;
        }
    }
#if FUNC_AUX_EN
    else if (aux_is_start()) {
        func_aux_stop();
//        bypass = 2;
        sbc_res.res_bypass = 2;
    }
#endif // FUNC_AUX_EN

    if(music_cb.sta != MUSIC_STOP) {            //避免来电响铃/报号未完成，影响get_music_dec_sta()状态
        music_control(MUSIC_MSG_STOP);
    }

    bt_set_sbc_playing(1);
    sbc_decode_init();
    sbcdec_frame = sbc_res_dec_frame;
    sbc_res.res_dnr_sta = res_play_start_do(index);
    if (tws_sync) {
        tws_res_play_sync(1);
    } else {
        music_control(MUSIC_MSG_PLAY);
    }
    f_bt_ring1.sbc_flag = 1;
    return 0;
}


//播放提示音接口
AT(.text.warning)
bool sbc_res_play(uint index, u32 tws_sync)
{
    u8 dnr_sta, bypass = 0;

    WDT_CLR();
    u8 dac_sta = sys_cb.dac_sta;
    func_bt_set_dac(1);

#if BT_TWS_WARNING_SLAVE_BYPASS_FIX
    if (!tws_sync) {
        bt_warning_set_playing(1);
    }
#endif

    if (bt_get_status() != BT_STA_OFF) {
        if (!bt_is_w4_pwrkey5s()) {
            bt_audio_bypass();
            bypass = 1;
        }
    }
#if FUNC_AUX_EN
    else if (aux_is_start()) {
        func_aux_stop();
        bypass = 2;
    }
#endif // FUNC_AUX_EN

    if(music_cb.sta != MUSIC_STOP) {            //避免来电响铃/报号未完成，影响get_music_dec_sta()状态
        music_control(MUSIC_MSG_STOP);
    }
    bt_set_sbc_playing(1);
    sbc_decode_init();
    sbcdec_frame = sbc_res_dec_frame;
    dnr_sta = res_play_start_do(index);
    if (tws_sync) {
        tws_res_play_sync(1);
    } else {
        music_control(MUSIC_MSG_PLAY);
    }
    delay_5ms(2);
    while (get_music_dec_sta() != MUSIC_STOP) {
        WDT_CLR();
        if (sys_cb.tws_res_brk) {
            sys_cb.tws_res_brk = 0;
            break;
        }
#if VUSB_SMART_VBAT_HOUSE_EN
        if (bt_get_status() >= BT_STA_IDLE) {
            bsp_smart_vhouse_process(0);
        }
#endif
    }
    res_play_stop(dnr_sta);
    bt_decode_exit();
    bt_set_sbc_playing(0);
#if BT_TWS_WARNING_SLAVE_BYPASS_FIX
    bt_warning_set_playing(0);
#endif
    if (bypass == 1) {
        bt_audio_enable();
    }
#if FUNC_AUX_EN
    else if (bypass == 2) {
        func_aux_start();
    }
#endif // FUNC_AUX_EN
    func_bt_set_dac(dac_sta);
    TRACE("Done\n");
    return true;
}

AT(.text.warning)
void wav_res_play_do(u32 addr, u32 len, bool sync)
{
    if (len == 0) {
        return;
    }
#if WARNING_WAV_EN
#if DAC_DNR_EN
    u8 sta = dac_dnr_get_sta();
    dac_dnr_set_sta(0);
#endif
    u8 dac_sta = sys_cb.dac_sta;
    func_bt_set_dac(1);
    u32 src1volcon = SRC1VOLCON;                    //还原SRC1音量
    wav_res_play_kick(addr, len, sync);
    while (wav_res_is_play()) {
        wav_res_dec_process();
        WDT_CLR();
    }
    wav_res_stop();
    SRC1VOLCON = src1volcon;
    SRC1VOLCON |= BIT(20);

    func_bt_set_dac(dac_sta);
#if DAC_DNR_EN
    dac_dnr_set_sta(sta);
#endif
#endif // WARNING_WAV_EN
}

//type: 0->无提示音, 1->piano, 2->语音,
AT(.text.warning)
bool warning_play(uint index, uint type)
{
    TRACE("%s: %d, %d\n", __func__, index, type);

    if(!type){
        return false;
    }

    type = sbcplay_set(index, type);
    if (type) {
        sys_warning_play_do(index, type, 0);
        return true;
    }
    return false;               //资源文件为空时继续使用ROM内置的语音播放
}

void key_voice_play(u32 freq, u32 play_ms, u32 gain)
{
    u32 fade_bits = 1, min;
    u32 tone_size = (DACDIGCON0 & BIT(1)) ? 48 : 44;
    u32 spl = (DACDIGCON0 & BIT(1)) ? 48000 : 44100;

    tone_size *= play_ms;
    min = (1 << fade_bits) * 512;
    if (tone_size < min) {
        tone_size = min;
    }
    KEYTONECON1 = (u64)0xfffffffful * (u64)freq / spl;
    KEYTONECON0 = (tone_size << 16) | (gain << 4) | (fade_bits << 2) | BIT(0);
//    printf("key voice: %x, %x\n", KEYTONECON1, KEYTONECON0);
    KEYTONECON0 |= BIT(7);                  //kick start
}
