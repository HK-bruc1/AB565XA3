#include "include.h"
#include "func.h"
#include "func_bt.h"

#if FUNC_BT_EN

f_bt_ring1_cb f_bt_ring1;

#if WARNING_BT_INCALL

void bt_ring_var_init(void)
{
    f_bt_ring.len = 0;
}

void bt_ring_stop(void)
{
    f_bt.ring_stop = 1;
    bt_tws_user_key(TWS_USER_KEY_RING_STOP);
}


#endif

bool bt_ring_esbc_stop(u8 force)
{
    if(f_bt_ring1.esbc_flag == 0){
        return false;
    }
    if(force || (music_cb.sta == MUSIC_STOP) ){
        if(music_cb.sta != MUSIC_STOP) {            //避免来电响铃/报号未完成，影响get_music_dec_sta()状态
            music_control(MUSIC_MSG_STOP);
        }
        sys_clk_free(INDEX_RES_PLAY);
        f_bt_ring1.esbc_flag = 0;
        return true;
    }
    return false;
}

int bt_ring_esbc_play(u8 index, const u8 *ptr, const u8 *sptr, u32 size, u32 tws_sync)
{
    if ((size == 0) || (ptr == 0)) {
        return -1;
    }
    if (ptr[0] != 0x8B) {                       //头不匹配
        return -2;
    }
    sys_clk_req(INDEX_RES_PLAY, SYS_52M);

    if(music_cb.sta != MUSIC_STOP) {            //避免来电响铃/报号未完成，影响get_music_dec_sta()状态
        music_control(MUSIC_MSG_STOP);
    }
    f_bt_ring1.esbc_flag = 1;

    esbc_init(ptr, size);
    res_play_start_do(index);
    if (tws_sync) {
        tws_res_play_sync(1);
    } else {
        music_control(MUSIC_MSG_PLAY);
    }

    return 0;
}


void sfunc_bt_ring_process(void)
{
    u32 ticks = 50 + 500 * (u32)xcfg_cb.bt_ring_ticks_sel;

    if (f_bt_ring.play.ring_first) {
        f_bt_ring.play.ring_first = 0;
        ticks = 300;
    }
    func_process();
    func_bt_sub_process();
    if (f_bt.disp_status != BT_STA_INCOMING || func_cb.sta != FUNC_BT) {
        if (bt_tws_is_connected()) {
            if (tone_is_playing()) {
                if (f_bt.ring_sta == 1) {
                    if (!bt_tws_is_slave()) {
                        tws_res_play(TWS_RES_RING_STOP);
                    }
                    f_bt.ring_sta = 3;                                                      //等提示同步结束再退出
                }
            }
#if !BT_CALL_RING_EXIT_FIX
            else if (get_music_dec_sta() != MUSIC_STOP) {
                f_bt.ring_sta = 2;
            }
#endif
            else {
                f_bt.ring_sta = 0;
            }
        } else {
            f_bt.ring_sta = 0;
        }
    }
    if (bt_ring_tone_stop(0) || bt_ring_esbc_stop(0) || bt_ring_sbc_stop(0)) {                                                             //是否响铃一次结束？
        f_bt_ring.play.ring_tick = tick_get();
        f_bt_ring.play.ring_cnt++;
    }
#if WARNING_BT_INCALL
    if (bt_is_ring() && f_bt.ring_sta == 1 && !f_bt.ring_stop) {                            //是否播放来电提示音？
    #if BT_HFP_RING_NUMBER_EN
        if ((xcfg_cb.bt_hfp_ring_number_en) && (f_bt_ring.play.ring_cnt >= BT_HFP_RINGS_BEFORE_NUMBER) && (f_bt_ring.play.cur < f_bt_ring.len)) {
            if (get_music_dec_sta() == MUSIC_STOP && !tws_res_is_busy() && tick_check_expire(f_bt_ring.play.ring_tick, 500)) {
                if (f_bt_ring.buf[f_bt_ring.play.cur] != 0xff) {
                    if (!bt_tws_is_slave()) {
                        tws_res_play(f_bt_ring.buf[f_bt_ring.play.cur] + TWS_RES_NUM_0);
                    }
                }
                f_bt_ring.play.cur++;
                f_bt_ring.play.ring_tick = tick_get();
            }
        } else if (get_music_dec_sta() == MUSIC_STOP)                                       //等报号先播放完
    #endif
        {
    #if BT_HFP_INBAND_RING_EN
            if (xcfg_cb.bt_hfp_inband_ring_en) {                                            //播放手机铃声
                if (sco_is_connected()) {
                    if (f_bt_ring.play.inband_ring_sta != 2) {
                        f_bt_ring.play.inband_ring_sta = 2;                                 //已建立SCO,播放手机铃声
                        bt_audio_enable();
                        if (sys_cb.hfp_vol < 6) {                                           //解决某水果手机来电铃声无声的问题
                            sys_cb.hfp_vol = 6;
                            bsp_change_volume(bt_hfp_get_sys_vol(sys_cb.hfp_vol));
                        }
                        dac_fade_in();
                        //printf("ios ring\n");
                    }
                } else if (!tick_check_expire(f_bt_ring.play.ios_ring_tick, f_bt_ring.play.w4_sco_tick)) {
                    if (f_bt_ring.play.inband_ring_sta != 2) {
                        f_bt_ring.play.inband_ring_sta = 1;                                 //等待建立SCO播放手机铃声
                    }
                } else {
                    if (f_bt_ring.play.inband_ring_sta == 1) {
                        f_bt_ring.play.inband_ring_sta = 0;                                 //超时还未建立过SCO清状态播放本地ring
                    }
                }
            }
    #endif
            if (!f_bt_ring.play.inband_ring_sta && !tone_is_playing() && !tws_res_is_busy() && tick_check_expire(f_bt_ring.play.ring_tick, ticks)) {
                if (PIANO_BT_RING && !bt_tws_is_slave()) {
                    tws_res_play(TWS_RES_RING);
                    f_bt_ring.play.ring_tick = tick_get();
                }
            }
        }

        motor_ring_process();
    }
#endif
}

void sfunc_bt_ring_enter(void)
{
#if WARNING_BT_INCALL
    memset(&f_bt_ring.play, 0, sizeof(t_play_cb));
    memset(&f_bt_ring1, 0, sizeof(f_bt_ring1));
    sbc_res_var_init();
    f_bt_ring.play.gain_offset = sys_cb.gain_offset;
    f_bt_ring.play.ios_ring_tick = tick_get();
    f_bt_ring.play.w4_sco_tick = 3000;
    if (bt_is_ios_device()) {
        f_bt_ring.play.w4_sco_tick = 6500;
    }
    f_bt.ring_stop = 0;
    dac_set_anl_offset(0);
    bt_audio_bypass();
    music_control(MUSIC_MSG_STOP);
    f_bt_ring.play.ring_tick = tick_get();
    f_bt_ring.play.ring_first = 1;
#endif // WARNING_BT_INCALL

#if BT_HFP_CALL_EARPHONE_EN
    sys_cb1.call_switch = 1;
#endif

    f_bt.ring_sta = 1;
    if (bt_tws_is_connected()) {
        if (!bt_tws_sync_ring_tone()) {
            f_bt_ring.play.ring_tick = tick_get();
        }
    } else {
        //先报号码，再响铃
        if (xcfg_cb.bt_hfp_ring_number_en) {
            int i;
            for(i = 0; i < 70; i++) {
                func_bt_status();
                if((f_bt_ring.len != 0) || !bt_is_ring() || (f_bt.disp_status != BT_STA_INCOMING)) {
                    break;
                }
                delay_5ms(10);
            }
        }
    }
    tws_res_reset();
#if DAC_DNR_EN
    f_bt_ring.dac_sta = dac_dnr_get_sta();
    dac_dnr_set_sta(0);
#endif
}

void sfunc_bt_ring_exit(void)
{
    f_bt.ring_sta = 0;
    bt_ring_tone_stop(1);
    bt_ring_esbc_stop(1);
    bt_ring_sbc_stop(1);
    bt_tws_clr_ring_tone();
#if BT_HFP_INBAND_RING_EN
    if (f_bt_ring.play.inband_ring_sta == 2) {
        f_bt_ring.play.inband_ring_sta = 0;
        bt_audio_bypass();
    }
#endif
#if WARNING_BT_INCALL
    motor_disable();
    f_bt_ring.len = 0;
    music_control(MUSIC_MSG_STOP);
#if DAC_DNR_EN
    dac_dnr_set_sta(f_bt_ring.dac_sta);
#endif
    sys_cb.gain_offset = f_bt_ring.play.gain_offset;
    bsp_change_volume(sys_cb.vol);
    bt_audio_enable();
#endif
}

AT(.text.func.btring)
void sfunc_bt_ring(void)
{
    printf("%s\n", __func__);

    sfunc_bt_ring_enter();

    while (f_bt.ring_sta) {
        sfunc_bt_ring_process();
        sfunc_bt_ring_message(msg_dequeue());
//        func_bt_display();
    }

    sfunc_bt_ring_exit();
}

#endif //FUNC_BT_EN
