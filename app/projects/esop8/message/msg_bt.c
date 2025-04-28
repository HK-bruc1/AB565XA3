#include "include.h"
#include "func.h"
#include "func_bt.h"

bool bt_tws_pair_mode(u8 method);
u16 get_user_def_lr_msg(u16 msg_l, u16 msg_r);
u16 get_user_def_vol_msg(u8 func_sel);
void user_def_track_msg(u16 msg);
bool user_def_play_pause_msg(void);
bool user_def_func_is_ready(u8 func_sel);
bool user_def_key_msg(u8 func_sel);
void bt_hid_vol_msg(u8 sel);
extern u8 bt_alg_dbb_on;

///检查USER_DEF按键消息处理
bool user_def_key_msg(u8 func_sel)
{
    if (func_sel == UDK_SIRI_REDIALING) {
        func_sel = get_user_def_lr_msg(UDK_SIRI, UDK_REDIALING);
    }
    if (!user_def_func_is_ready(func_sel)) {
        return false;
    }

    if (func_sel == UDK_REDIALING) {
        bt_call_redial_last_number();                   //回拨电话
        sys_warning_play(T_WARNING_REDIALING, PIANO_REDIALING);
    } else if (func_sel == UDK_SIRI) {                  //SIRI
        bt_siri_switch();
    } else if (func_sel == UDK_NR) {                    //NR
        bt_ctl_nr_sta_change();                         //发消息通知手机
#if ANC_EN
        sys_cb.anc_user_mode++;
        if (sys_cb.anc_user_mode > 2) {
            sys_cb.anc_user_mode = 0;
        }
        bsp_anc_set_mode(sys_cb.anc_user_mode);
#endif
    } else if (func_sel == UDK_PREV) {                  //PREV
        user_def_track_msg(get_user_def_lr_msg(KU_PREV, KU_NEXT));
    } else if (func_sel == UDK_NEXT) {                  //NEXT
        user_def_track_msg(get_user_def_lr_msg(KU_NEXT, KU_PREV));
    } else if (func_sel == UDK_PHOTO) {
        return bsp_bt_hid_photo(HID_KEY_VOL_UP);        //拍照
    } else if (func_sel == UDK_HOME) {
        return bt_hid_consumer(HID_KEY_IOS_HOME);       //IOS Home按键功能
    } else if (func_sel == UDK_LANG) {
        bt_switch_voice_lang();                         //中英文切换
    } else if (func_sel == UDK_ATMOS_LED) {             //氛围灯开/关
        if (atmos_gpio.sfr) {
            if (atmosphere_led_is_on()) {
                tws_res_play(TWS_ATMOS_LED_OFF);
            } else {
                tws_res_play(TWS_ATMOS_LED_ON);
            }
            return true;
        }
        return false;
    } else if (func_sel == UDK_PLAY_PAUSE) {
        return user_def_play_pause_msg();
    } else if (func_sel == UDK_GAME_SWITCH) {
        if (bt_is_low_latency()) {
            tws_res_play(TWS_RES_MUSIC_MODE);           //提示音播放的地方更新sys_cb.game_mode变量
        } else {
            tws_res_play(TWS_RES_GAME_MODE);
        }
    } else if (func_sel == UDK_MODE) {                  //MODE
        func_message(KU_MODE);
    } else {                                            //VOL+, VOL-
        func_message(get_user_def_vol_msg(func_sel));
    }
    return true;
}

static bool user_def_lkey_tone_is_enable(u8 func_sel)
{
    if (!xcfg_cb.user_def_kl_tone_en) {
        return false;
    }
    return user_def_func_is_ready(func_sel);
}

bool bt_tws_pair_mode(u8 method)
{
    if ((xcfg_cb.bt_tws_en) && (xcfg_cb.bt_tws_pair_mode == method) && (!bt_nor_is_connected())) {
        if(bt_tws_is_connected()) {
            bt_tws_disconnect();
        } else {
//            bt_tws_set_scan(0x3);
            bt_tws_search_slave();
        }
        return true;
    }
    return false;
}

//来电响铃时拒接操作(长按/双击)
void bt_ring_reject(void)
{
    bt_ring_stop();
    bt_call_terminate();                //拒接
    sys_warning_play(T_WARNING_REJECT, PIANO_REJECT);
}

void func_bt_message_do(u16 msg)
{
    int klu_flag = 0;
    u8 ku_sel = xcfg_cb.user_def_ks_sel;

    switch (msg) {
    case KU_PLAY:
        ku_sel = UDK_PLAY_PAUSE;
    case KU_PLAY_USER_DEF:
    case KU_PLAY_PWR_USER_DEF:
//        key_voice_play(501, 100, 3);                                        //按键音
        if (!bt_nor_is_connected()) {
            bt_tws_pair_mode(3);                                            //单击PLAY按键手动配对
            break;
        }
        user_def_key_msg(ku_sel);
        break;

    case KL_PLAY_PWR_USER_DEF:
        if (!xcfg_cb.kl_pwrdwn_dis) {
            klu_flag = 1;                                                   //长按抬键的时候呼SIRI
        }
    case KL_PLAY_USER_DEF:
        f_bt.user_kl_flag = 0;
        if (xcfg_cb.user_def_kl_sel == UDK_GAME_SWITCH) {
            klu_flag = 0;
        }
        if (!bt_tws_pair_mode(4)) {                                         //是否长按配对功能
            if (user_def_lkey_tone_is_enable(xcfg_cb.user_def_kl_sel)) {
                sys_warning_play(T_WARNING_NEXT_TRACK, 1);                  //长按“滴”一声
//                tws_res_play(TWS_RES_TONE);                                 //tws同步播放
            }
            if (klu_flag) {
                f_bt.user_kl_flag = user_def_func_is_ready(xcfg_cb.user_def_kl_sel);     //长按抬键的时候再处理
            } else {
                user_def_key_msg(xcfg_cb.user_def_kl_sel);
            }
        }
        break;

        //SIRI, NEXT, PREV在长按抬键的时候响应,避免关机前切歌或呼SIRI了
    case KLU_PLAY_PWR_USER_DEF:
        if (f_bt.user_kl_flag) {
            user_def_key_msg(xcfg_cb.user_def_kl_sel);
            f_bt.user_kl_flag = 0;
        }
        break;

        //长按调音量
    case KH_PLAY_PWR_USER_DEF:
        if (!xcfg_cb.kl_pwrdwn_dis) {
            break;
        }
    case KH_PLAY_USER_DEF:
        func_message(get_user_def_vol_msg(xcfg_cb.user_def_kl_sel));
        break;

    ///双击按键处理
    case KD_PLAY_USER_DEF:
    case KD_PLAY_PWR_USER_DEF:
        if (xcfg_cb.user_def_kd_tone_en) {
            sys_warning_play(T_WARNING_NEXT_TRACK, 1);                  //2击“滴”一声
        }
        if ((xcfg_cb.user_def_kd_lang_en) && (!bt_nor_is_connected())) {
            bt_switch_voice_lang();
        } else if (user_def_key_msg(xcfg_cb.user_def_kd_sel)) {
#if BT_TWS_EN
        } else if(bt_tws_pair_mode(2)) {
#endif
        }
        break;

    ///三击按键处理
    case KTH_PLAY_USER_DEF:
    case KTH_PLAY_PWR_USER_DEF:
#if BT_ALG_DBB_KEY_EN
        if(bt_alg_dbb_on) {
            bt_alg_dbb_on = 0;
        }else {
            bt_alg_dbb_on = 1;
        }
#else
        if (xcfg_cb.user_def_kt_tone_en) {
            sys_warning_play(T_WARNING_NEXT_TRACK, 1);                  //3击“滴”一声
        }
        user_def_key_msg(xcfg_cb.user_def_kt_sel);
#endif
        break;

    ///四击按键处理
    case KFO_PLAY_USER_DEF:
    case KFO_PLAY_PWR_USER_DEF:
        if (xcfg_cb.user_def_kfour_tone_en) {
            sys_warning_play(T_WARNING_NEXT_TRACK, 1);                  //4击“滴”一声
        }
        user_def_key_msg(xcfg_cb.user_def_kfour_sel);
        if (xcfg_cb.user_def_kfour_clr_pair_en) {                       //四击清配对信息
            if (!bt_nor_is_connected() && !bt_tws_is_connected()) {
                bt_clr_all_link_info('k');
            }
        }
        break;

    ///五击按键处理
    case KFI_PLAY_USER_DEF:
    case KFI_PLAY_PWR_USER_DEF:
        if (xcfg_cb.bb_dut_test_k5s_en && !bt_nor_is_connected()) {
            func_cb.sta = FUNC_BT_DUT;          //五击进DUT测试模式
        } else if (xcfg_cb.user_def_kfive_sel) {
            user_def_key_msg(xcfg_cb.user_def_kfive_sel);
        }
        break;

#if BT_LIGHTNINIG_EN
    case KU_SIRI:
    case KL_SIRI:
    case KL_HOME:
        if (bt_nor_is_connected()) {
            bt_siri_switch();
        }
        break;
#endif

//    case KL_PLAY:
//        if (xcfg_cb.bt_key_discon_en) {
//            bt_disconnect();
//            break;
//        }

    case KU_PREV_VOL_DOWN:
    case KU_PREV_VOL_UP:
    case KL_VOL_DOWN_PREV:
    case KL_VOL_UP_PREV:
    case KU_PREV:
        bt_music_prev();
        break;

    case KU_NEXT:
    case KU_NEXT_VOL_UP:
    case KU_NEXT_VOL_DOWN:
    case KL_VOL_UP_NEXT:
    case KL_VOL_DOWN_NEXT:
        bt_music_next();
        break;

#if USER_ADKEY
    case KL_PREV:
        bt_music_rewind();
        break;
    case KLU_PREV:
        bt_music_rewind_end();
        break;
    case KL_NEXT:
        bt_music_fast_forward();
        break;
    case KLU_NEXT:
        bt_music_fast_forward_end();
        break;

#if FUNC_AUX_EN
    case KD_MODE:
    case KD_MODE_PWR:
        bt_tws_pair_mode(5);
        break;
#endif

    case KD_HSF:
        if (bt_nor_is_connected()) {
            bt_call_redial_last_number();           //回拨电话
            sys_warning_play(T_WARNING_REDIALING, PIANO_REDIALING);
        }
        break;
#endif // USER_ADKEY

#if BT_HID_EN
    case KU_HOME:
        bt_hid_consumer(HID_KEY_IOS_HOME);
        break;

    case KD_NEXT_VOL_UP:
    case KD_PREV_VOL_UP:
    case KD_VOL_UP_NEXT:
    case KD_VOL_UP_PREV:
    case KD_VOL_UP:
        bt_hid_vol_msg(xcfg_cb.bt_hid_volup_sel);
        break;

    case KD_PREV_VOL_DOWN:
    case KD_NEXT_VOL_DOWN:
    case KD_VOL_DOWN_PREV:
    case KD_VOL_DOWN_NEXT:
    case KD_VOL_DOWN:
        bt_hid_vol_msg(xcfg_cb.bt_hid_voldown_sel);
        break;
#endif // BT_HID_EN

    case MSG_SYS_1S:
        bt_send_msg(BT_MSG_HFP_REPORT_BAT);

        //蓝牙连接后延时2秒发送PLAY命令实现自动播放
        if (f_bt.autoplay > 0) {
            f_bt.autoplay++;
            if (f_bt.autoplay >= 3) {
                f_bt.autoplay = 0;
                if ((xcfg_cb.bt_autoplay_en) && (bt_get_status() == BT_STA_CONNECTED)) {
                    bt_music_play();
//                    printf("AutoPlay\n");
                }
            }
        }
#if BT_TWS_DBG_EN
        //BT-Assistant工具分析信号质量
        if (xcfg_cb.bt_tws_dbg_en) {
            bt_tws_report_dgb();
        }
#endif
        break;

    case EVT_A2DP_MUSIC_PLAY:
        func_bt_set_dac(1);
        dac_fade_in();
        break;

    case EVT_A2DP_MUSIC_STOP:
        dac_fade_out();
        break;

#if BT_A2DP_DEFAULT_VOLUME_EN
    case EVT_A2DP_DEFAULT_VOLUME:
        if (bt_is_ios_device()) {
            bsp_set_volume(BT_A2DP_IOS_DEFAULT_VOLUME);
        } else {
            bsp_set_volume(BT_A2DP_DEFAULT_VOLUME);
        }
        bsp_bt_vol_change();
        break;
#endif

#if IODM_TEST_MODE
    case EVT_BT_SPP_IODM:
        bt_spp_iodm_event();
        break;
#endif

    default:
        func_message(msg);
        break;
    }
}

AT(.text.bfunc.bt)
void func_bt_message(u16 msg)
{
    if (msg == NO_MSG || msg == MSG_SYS_1S) {       //减少flash缺页
        func_bt_message_m(msg);
        if (msg == MSG_SYS_1S) {
        }
    } else {
        func_bt_message_do(msg);
    }
}

void sfunc_bt_ring_message_do(u16 msg)
{
    u8 ku_sel = xcfg_cb.user_def_ks_sel;

    switch (msg) {
    case KU_HSF:                //接听
        ku_sel = UDK_PLAY_PAUSE;
    case KU_PLAY_USER_DEF:
    case KU_PLAY_PWR_USER_DEF:
        user_def_key_msg(ku_sel);
        break;

    case KL_PLAY_PWR_USER_DEF:
        //PWRKEY松开前不产生KLH_PLAY_PWR_USER_DEF消息。按键松开自动清此标志。
        if (!xcfg_cb.user_def_kl_call_pwrdwn_en) {
            sys_cb.poweron_flag = 1;
        }
    case KL_PLAY_USER_DEF:
    case KL_HSF:
        if (xcfg_cb.user_def_kl_reject_en || msg == KL_HSF) {
            bt_ring_reject();
        } else {
            user_def_key_msg(xcfg_cb.user_def_kl_sel);
        }
        break;

    case KD_PLAY_USER_DEF:
    case KD_PLAY_PWR_USER_DEF:
        if (xcfg_cb.user_def_kd_reject_en) {
            bt_ring_reject();
        } else {
            user_def_key_msg(xcfg_cb.user_def_kd_sel);
        }
        break;

    ///三击按键处理
    case KTH_PLAY_USER_DEF:
    case KTH_PLAY_PWR_USER_DEF:
        user_def_key_msg(xcfg_cb.user_def_kt_sel);
        break;

	    ///四击按键处理
    case KFO_PLAY_USER_DEF:
    case KFO_PLAY_PWR_USER_DEF:
        user_def_key_msg(xcfg_cb.user_def_kfour_sel);
        break;

    ///五击按键处理
    case KFI_PLAY_USER_DEF:
    case KFI_PLAY_PWR_USER_DEF:
        user_def_key_msg(xcfg_cb.user_def_kfive_sel);
        break;

    case MSG_SYS_1S:
        bt_send_msg(BT_MSG_HFP_REPORT_BAT);
        break;

    case KLH_PLAY_PWR_USER_DEF:
        //ring不响应关机消息，解决关机时间1.5时长按拒接偶尔触发关机的问题。
        if (!xcfg_cb.user_def_kl_call_pwrdwn_en) {
            break;
        }
    default:
        func_message(msg);
        break;
    }
}

AT(.text.bfunc.bt)
void sfunc_bt_ring_message(u16 msg)
{
    if (msg == NO_MSG || msg == MSG_SYS_1S) {       //减少flash缺页
        sfunc_bt_ring_message_m(msg);
    } else {
        sfunc_bt_ring_message_do(msg);
    }
}

void sfunc_bt_call_message_do(u16 msg)
{
    u8 call_status;
    u8 ku_sel = xcfg_cb.user_def_ks_sel;

    switch (msg) {
#if BT_LIGHTNINIG_EN
    case KU_HOME:
    case KL_HOME:
        if (bt_get_siri_status()) {
            bt_call_terminate();                        //结束SIRI
        }
        break;
#endif

    ///挂断当前通话，或者结束当前通话并接听第2路通
    case KU_HSF:
        ku_sel = UDK_PLAY_PAUSE;
    case KU_PLAY_USER_DEF:
    case KU_PLAY_PWR_USER_DEF:
        user_def_key_msg(ku_sel);
        break;

    ///拒接第2路通话, 或私密接听切换
    case KL_PLAY_PWR_USER_DEF:
        if (!xcfg_cb.user_def_kl_call_pwrdwn_en) {
            sys_cb.poweron_flag = 1;                    //PWRKEY松开前不产生KLH_PLAY_PWR_USER_DEF消息。按键松开自动清此标志。
        }
    case KL_PLAY_USER_DEF:
        if (user_def_key_msg(xcfg_cb.user_def_kl_sel)) {
            break;
        }
    case KL_HSF:
        call_status = bt_get_call_indicate();
        if(call_status == BT_CALL_INCOMING) {
            bt_call_terminate();                        //拒接第2路通话
            sys_warning_play(T_WARNING_REJECT, PIANO_REJECT);
        } else if (xcfg_cb.bt_hfp_private_switch_en) {
            bt_call_private_switch();                   //私密接听切换
        }
        break;

    ///保持当前通话并接通第2路通话，或者2路通话切换
    case KD_PLAY_PWR_USER_DEF:
    case KD_PLAY_USER_DEF:
        if (user_def_key_msg(xcfg_cb.user_def_kd_sel)) {
            break;
        }
    case KD_HSF:
        call_status = bt_get_call_indicate();
        if (call_status == BT_CALL_INCOMING) {
            set_mic_mute(0);
            bt_call_answer_incoming();                  //接听第2路通话
        } else if(call_status == BT_CALL_3WAY_CALL) {
            set_mic_mute(0);
            bt_call_swap();                             //切换两路通话
        } else if (xcfg_cb.bt_hfp_mute_switch_en) {
            set_mic_mute(2);                            //MIC left channel mute toggle
        }
        break;

    ///长按调音量
    case KH_PLAY_PWR_USER_DEF:
    case KH_PLAY_USER_DEF:
        func_message(get_user_def_vol_msg(xcfg_cb.user_def_kl_sel));
        break;

    ///三击按键处理
    case KTH_PLAY_USER_DEF:
    case KTH_PLAY_PWR_USER_DEF:
        user_def_key_msg(xcfg_cb.user_def_kt_sel);
        break;

    ///四击按键处理
    case KFO_PLAY_USER_DEF:
    case KFO_PLAY_PWR_USER_DEF:
        user_def_key_msg(xcfg_cb.user_def_kfour_sel);
        break;

    ///五击按键处理
    case KFI_PLAY_USER_DEF:
    case KFI_PLAY_PWR_USER_DEF:
        user_def_key_msg(xcfg_cb.user_def_kfive_sel);
        break;

    case EVT_A2DP_MUSIC_PLAY:
        dac_fade_in();
        break;

    case MSG_SYS_1S:
        bt_send_msg(BT_MSG_HFP_REPORT_BAT);
        break;

    default:
        func_message(msg);
        break;
    }
}

AT(.text.bfunc.bt)
void sfunc_bt_call_message(u16 msg)
{
    if (msg == NO_MSG || msg == MSG_SYS_1S) {       //减少flash缺页
        sfunc_bt_call_message_m(msg);
    } else {
        sfunc_bt_call_message_do(msg);
    }
}

//void func_bthid_message(u16 msg)
//{
//    if (sys_hook.bthid_message) {
//        msg = sys_hook.bthid_message(msg);
//        if (msg == NO_MSG) {
//            return;
//        }
//    }
//
//    switch (msg) {
//    case KU_PLAY:
//    case KU_PLAY_USER_DEF:
//    case KU_PLAY_PWR_USER_DEF:
//        if (bt_get_status() < BT_STA_DISCONNECTING) {
//            bt_connect();
//        } else {
//            bsp_bt_hid_photo(HID_KEY_VOL_UP);
//        }
//        break;
//
//    case KL_PLAY:
//        //bt_disconnect();
//        break;
//
//    default:
//        func_message(msg);
//        break;
//    }
//}
