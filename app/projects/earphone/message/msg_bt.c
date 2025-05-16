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
u8 bt_low_latency_is_busy(void);
void bt_low_latency_set_busy(void);

extern u8 bt_alg_dbb_on;

//检查USER_DEF按键消息处理
//根据传入的按键类型消息调用对应的处理函数
bool user_def_key_msg(u8 func_sel)
{
    //当功能选择为UDK_SIRI_REDIALING时，会通过get_user_def_lr_msg进行预处理转换。
    if (func_sel == UDK_SIRI_REDIALING) {
        func_sel = get_user_def_lr_msg(UDK_SIRI, UDK_REDIALING);
    }
    
    //猜测：
    //蓝牙状态依赖：多个功能需要蓝牙连接（如SIRI、回拨需要bt_nor_is_connected()）
    //硬件资源依赖：氛围灯功能需要atmos_gpio.sfr硬件存在
    //功能配置依赖：ANC模式切换需要xcfg_cb.anc_en配置启用
    //电源状态保护：通过sys_cb.poweron_flag防止在关机过程中执行操作
    //全局功能开关：通过xcfg_cb.user_def_en控制是否启用用户自定义功能
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

        sys_cb.anc_user_mode++;
        if (sys_cb.anc_user_mode > 2) {
            sys_cb.anc_user_mode = 0;
        }
#if ANC_EN
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
#if BT_LOW_LATENCY_QUICK_FIX
        if (bt_low_latency_is_busy()) {
            return true;
        }
        bt_low_latency_set_busy();
#endif
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
        //如果TWS连接了，则断开连接
        if(bt_tws_is_connected()) {
            bt_tws_disconnect();
        } else {
            //如果TWS没有连接，则搜索从机
//            bt_tws_set_scan(0x3);
            //搜索从机源码不可见
            bt_tws_search_slave();
        }
        return true;
    }
    return false;
}

//来电响铃时拒接操作(长按/双击，配置工具只有这两个配置)
//跟通话的挂断和接听是分开的逻辑
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
            //如果蓝牙未连接，则单击PLAY按键手动配对
            //TWS连接则断开重新搜索从耳，没有连接就直接搜索从耳
            //这样设置很让人怀疑当TWS连接后，主耳才会会自动主动广播，让手机蓝牙可见
            bt_tws_pair_mode(3);                                            //单击PLAY按键手动配对
            break;
        }
        //传递一个播放和暂停的按键事件启动对应的处理
        user_def_key_msg(ku_sel);
        break;

    case KL_PLAY_PWR_USER_DEF:
        if (!xcfg_cb.kl_pwrdwn_dis) {
            klu_flag = 1;                                                   //长按抬键的时候呼SIRI
        }
    case KL_PLAY_USER_DEF:
        f_bt.user_kl_flag = 0;
        if (xcfg_cb.user_def_kl_sel == UDK_GAME_SWITCH) {
            //如果配置项中配置的是长按切换游戏模式的话
            klu_flag = 0;
        }
        if (!bt_tws_pair_mode(4)) {                                         //是否长按配对功能
            if (user_def_lkey_tone_is_enable(xcfg_cb.user_def_kl_sel)) {
                sys_warning_play(T_WARNING_NEXT_TRACK, 1);                  //长按“滴”一声
//                tws_res_play(TWS_RES_TONE);                                 //tws同步播放-------》前者只播放单耳？？？
            }
            if (klu_flag) {
                f_bt.user_kl_flag = user_def_func_is_ready(xcfg_cb.user_def_kl_sel);     //长按抬键的时候再处理
            } else {
                //经过一系列判断才处理的
                user_def_key_msg(xcfg_cb.user_def_kl_sel);
            }
        }
        break;

        //SIRI, NEXT, PREV在长按抬键的时候响应,避免关机前切歌或呼SIRI了
        //不然跟长按关机可能会冲突，那么可以推理出长按关机是不松开到达时间就会触发。
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
        if ((xcfg_cb.user_def_kd_lang_en) && (!bt_nor_is_connected())) {//没有连接手机的情况下才会切换语言
            bt_switch_voice_lang();
        } else if (user_def_key_msg(xcfg_cb.user_def_kd_sel)) {//里面会有校验，估计是需要连接蓝牙才会执行用户自定义的双击操作
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

/**
 * @brief 蓝牙来电状态下的具体按键消息处理函数
 *
 * 该函数负责处理蓝牙来电状态下的各种按键消息，实现接听、拒接等功能。
 * 函数通过配置工具中的设置实现按键功能的自定义，具体工作原理：
 *
 * 1. 配置工具关联：
 *    - xcfg_cb.user_def_ks_sel：配置工具中设置的短按功能选择（如接听、挂断等）
 *    - xcfg_cb.user_def_kl_sel：配置工具中设置的长按功能选择（如拒接、语音助手等）
 *    - xcfg_cb.user_def_kd_sel：配置工具中设置的双击功能选择
 *    - xcfg_cb.user_def_kt_sel：配置工具中设置的三击功能选择
 *    - xcfg_cb.user_def_kfour_sel：配置工具中设置的四击功能选择
 *    - xcfg_cb.user_def_kfive_sel：配置工具中设置的五击功能选择
 *    - xcfg_cb.user_def_kl_reject_en：配置工具中设置的长按是否拒接电话
 *    - xcfg_cb.user_def_kd_reject_en：配置工具中设置的双击是否拒接电话
 *
 * 2. 按键功能映射：
 *    - 当接收到按键消息(如KU_HSF)时，根据消息类型选择对应的配置值
 *    - 通过user_def_key_msg()函数将配置值(如UDK_PLAY_PAUSE)转换为具体功能
 *    - UDK_常量定义了各种可能的功能（如UDK_PLAY_PAUSE表示接听/挂断）
 *
 * 3. 按键处理流程：
 *    - 短按：默认映射为接听电话(UDK_PLAY_PAUSE)
 *    - 长按：根据user_def_kl_reject_en配置决定是拒接电话还是执行user_def_kl_sel指定的功能
 *    - 双击：根据user_def_kd_reject_en配置决定是拒接电话还是执行user_def_kd_sel指定的功能
 *    - 三击/四击/五击：分别执行user_def_kt_sel/user_def_kfour_sel/user_def_kfive_sel指定的功能
 *
 * 4. 按键复用机制：
 *    - 蓝牙耳机通过状态感知实现按键复用，同一个按键在不同状态下有不同功能
 *    - 特别是UDK_PLAY_PAUSE(值为12)这个功能值在不同状态下的行为：
 *      * 在音乐播放状态：控制音乐的播放/暂停
 *      * 在来电状态：控制接听电话
 *      * 在通话状态：控制挂断电话
 *    - 这种设计使得配置工具中的"PLAY/PAUSE"选项实际上是一个多功能选项
 *    - 系统会根据当前状态(音乐/来电/通话)自动选择合适的功能
 *    - 因此，通话中的接听/挂断与音乐的播放/暂停共用同一个逻辑值(UDK_PLAY_PAUSE)
 *
 * @param msg 待处理的按键消息，如KU_HSF(短按接听键)、KL_PLAY_USER_DEF(长按多功能键)等
 */
void sfunc_bt_ring_message_do(u16 msg)
{
    // 获取配置工具中设置的短按功能选择值
    // 这个值对应配置工具中"USER_DEF键单按"的设置，如"PLAY/PAUSE"(12)
    u8 ku_sel = xcfg_cb.user_def_ks_sel;

    switch (msg) {
    // 短按接听键处理 - 强制设置为接听电话功能
    case KU_HSF:                // 短按接听键
        ku_sel = UDK_PLAY_PAUSE; // 强制设置为接听/挂断功能(UDK_PLAY_PAUSE=12)，覆盖配置工具的设置
                                // 重要：这里体现了蓝牙耳机按键复用的核心机制 - UDK_PLAY_PAUSE(值为12)在不同状态下有不同功能：
                                // 1. 在音乐播放状态：控制音乐的播放/暂停
                                // 2. 在来电状态：控制接听电话
                                // 3. 在通话状态：控制挂断电话
                                // 这种设计使得同一个按键可以在不同状态下实现不同功能，系统会先判断当前状态(音乐/来电/通话)
                                // 然后将同一个按键消息分发到不同的处理函数中
    // 短按多功能键处理
    case KU_PLAY_USER_DEF:      // 短按多功能键
    case KU_PLAY_PWR_USER_DEF:  // 短按电源/多功能组合键
        // 调用user_def_key_msg函数执行对应功能，传入配置值(ku_sel)
        // 在来电状态下，这里通常会执行接听电话的功能
        // user_def_key_msg函数是按键功能实现的核心，它会根据当前系统状态和传入的功能值执行不同操作：
        // - 当ku_sel=UDK_PLAY_PAUSE(12)时：
        //   * 在音乐播放状态：调用music_play_pause()控制音乐播放/暂停
        //   * 在来电状态：调用bt_call_answer_incoming()接听电话
        //   * 在通话状态：调用bt_call_terminate()挂断电话
        // 这种状态感知的设计使得同一个配置值(UDK_PLAY_PAUSE)可以在不同状态下执行不同功能
        // 配置工具中设置的"PLAY/PAUSE"(12)选项实际上是一个多功能选项，会根据当前状态自动适配
        user_def_key_msg(ku_sel);
        break;

    // 长按电源键处理
    case KL_PLAY_PWR_USER_DEF:  // 长按电源/多功能组合键
        // 如果配置了长按电源键不关机(user_def_kl_call_pwrdwn_en=0)，则设置标志防止关机
        // PWRKEY松开前不产生KLH_PLAY_PWR_USER_DEF消息。按键松开自动清此标志。
        if (!xcfg_cb.user_def_kl_call_pwrdwn_en) {
            sys_cb.poweron_flag = 1; // 设置标志防止关机
        }
    // 长按多功能键或接听键处理
    case KL_PLAY_USER_DEF:      // 长按多功能键
    case KL_HSF:                // 长按接听键
        // 根据配置决定是拒接电话还是执行自定义功能
        // 如果配置了长按拒接(user_def_kl_reject_en=1)或按键是接听键(KL_HSF)，则拒接电话
        if (xcfg_cb.user_def_kl_reject_en || msg == KL_HSF) {
            bt_ring_reject(); // 拒接来电
        } else {
            // 否则执行配置工具中设置的长按功能
            user_def_key_msg(xcfg_cb.user_def_kl_sel);
        }
        break;

    // 双击多功能键处理
    case KD_PLAY_USER_DEF:      // 双击多功能键
    case KD_PLAY_PWR_USER_DEF:  // 双击电源/多功能组合键
        // 根据配置决定是拒接电话还是执行自定义功能
        // 如果配置了双击拒接(user_def_kd_reject_en=1)，则拒接电话
        if (xcfg_cb.user_def_kd_reject_en) {
            bt_ring_reject(); // 拒接来电
        } else {
            // 否则执行配置工具中设置的双击功能
            user_def_key_msg(xcfg_cb.user_def_kd_sel);
        }
        break;

    // 三击按键处理
    case KTH_PLAY_USER_DEF:     // 三击多功能键
    case KTH_PLAY_PWR_USER_DEF: // 三击电源/多功能组合键
        // 执行配置工具中设置的三击功能
        user_def_key_msg(xcfg_cb.user_def_kt_sel);
        break;

    // 四击按键处理
    case KFO_PLAY_USER_DEF:     // 四击多功能键
    case KFO_PLAY_PWR_USER_DEF: // 四击电源/多功能组合键
        // 执行配置工具中设置的四击功能
        user_def_key_msg(xcfg_cb.user_def_kfour_sel);
        break;

    // 五击按键处理
    case KFI_PLAY_USER_DEF:     // 五击多功能键
    case KFI_PLAY_PWR_USER_DEF: // 五击电源/多功能组合键
        // 执行配置工具中设置的五击功能
        user_def_key_msg(xcfg_cb.user_def_kfive_sel);
        break;

    // 1秒定时消息处理
    case MSG_SYS_1S:
        // 每秒向手机报告电池电量
        bt_send_msg(BT_MSG_HFP_REPORT_BAT);
        break;

    // 长按电源键超时消息处理
    case KLH_PLAY_PWR_USER_DEF:
        // 来电状态下特殊处理长按电源键，避免与拒接功能冲突
        // 如果配置了长按电源键不关机(user_def_kl_call_pwrdwn_en=0)，则忽略此消息
        // ring不响应关机消息，解决关机时间1.5时长按拒接偶尔触发关机的问题。
        if (!xcfg_cb.user_def_kl_call_pwrdwn_en) {
            break;
        }
    // 其他消息处理
    default:
        // 将消息传递给通用消息处理函数
        func_message(msg);
        break;
    }
}

/**
 * @brief 蓝牙来电状态下的按键消息处理函数
 *
 * 该函数负责处理蓝牙来电状态下的各种按键消息，包括接听、拒接等操作。
 * 为了减少flash缺页，将常见的空消息和1秒定时消息单独处理。
 *
 * 主要功能：
 * 1. 处理来电时的按键操作，如接听、拒接电话
 * 2. 根据配置工具设置的按键功能执行相应操作
 * 3. 支持多种按键组合（单击、长按、双击、三击等）
 *
 * 按键处理逻辑：
 * - 短按：默认接听电话
 * - 长按：根据配置可拒接电话或执行其他功能
 * - 双击：根据配置可拒接电话或执行其他功能
 * - 三击/四击/五击：执行配置的自定义功能
 *
 * @param msg 待处理的按键消息
 */
AT(.text.bfunc.bt)
void sfunc_bt_ring_message(u16 msg)
{
    if (msg == NO_MSG || msg == MSG_SYS_1S) {       //减少flash缺页
        //不可见的消息处理函数，可能在其他文件中实现
        sfunc_bt_ring_message_m(msg);
    } else {
        //针对蓝牙模式下的来电时，根据获取的消息来走不同的处理分支
        sfunc_bt_ring_message_do(msg);
    }
}

/**
 * @brief 蓝牙通话状态下的具体按键消息处理函数
 *
 * 该函数负责处理蓝牙通话状态下的各种按键消息，实现挂断、切换通话、麦克风静音等功能。
 * 函数通过配置工具中的设置实现按键功能的自定义，支持多路通话场景。
 *
 * 主要功能：
 * 1. 挂断当前通话：短按多功能键
 * 2. 私密接听切换：长按多功能键（在手机和耳机之间切换通话）
 * 3. 多路通话控制：
 *    - 拒接第二路来电：长按多功能键
 *    - 接听第二路来电：双击多功能键
 *    - 在两路通话间切换：双击多功能键
 * 4. 麦克风静音控制：双击多功能键（在非多路通话状态下）
 * 5. 音量调节：长按保持多功能键
 *
 * 按键复用机制：
 * - 与来电状态和音乐播放状态类似，通话状态下的按键也采用状态感知的复用机制
 * - 同一个UDK_PLAY_PAUSE(值为12)在通话状态下表示挂断电话
 * - 系统会根据当前通话状态(单路通话/多路通话/来电等)自动选择合适的功能
 *
 * @param msg 待处理的按键消息，如KU_HSF(短按接听键)、KL_PLAY_USER_DEF(长按多功能键)等
 */
void sfunc_bt_call_message_do(u16 msg)
{
    // 用于存储当前通话状态(单路通话/多路通话/来电等)
    u8 call_status;
    // 获取配置工具中设置的短按功能选择值
    u8 ku_sel = xcfg_cb.user_def_ks_sel;

    switch (msg) {
#if BT_LIGHTNINIG_EN
    // Lightning接口设备的HOME键处理（如苹果设备）
    case KU_HOME:    // 短按HOME键
    case KL_HOME:    // 长按HOME键
        if (bt_get_siri_status()) {
            bt_call_terminate();    // 如果Siri正在活动，结束Siri
        }
        break;
#endif

    // 短按处理：挂断当前通话，或者结束当前通话并接听第二路来电
    case KU_HSF:    // 短按接听键
        // 强制设置为挂断功能(UDK_PLAY_PAUSE=12)，覆盖配置工具的设置
        // 这里体现了按键复用机制：UDK_PLAY_PAUSE在通话状态下表示挂断电话
        ku_sel = UDK_PLAY_PAUSE;
    case KU_PLAY_USER_DEF:    // 短按多功能键
    case KU_PLAY_PWR_USER_DEF:    // 短按电源/多功能组合键
        // 调用user_def_key_msg函数执行对应功能
        // 在通话状态下，这里通常会执行挂断电话的功能
        // 如果有第二路来电，则会结束当前通话并接听第二路来电
        user_def_key_msg(ku_sel);
        break;

    // 长按处理：拒接第二路来电或私密接听切换
    case KL_PLAY_PWR_USER_DEF:    // 长按电源/多功能组合键
        // 如果配置了长按电源键不关机(user_def_kl_call_pwrdwn_en=0)，则设置标志防止关机
        if (!xcfg_cb.user_def_kl_call_pwrdwn_en) {
            // 设置标志防止关机，PWRKEY松开前不产生KLH_PLAY_PWR_USER_DEF消息
            // 按键松开时会自动清除此标志
            sys_cb.poweron_flag = 1;
        }
    case KL_PLAY_USER_DEF:    // 长按多功能键
        // 尝试执行配置工具中设置的长按功能
        // 如果user_def_key_msg返回true，表示已处理该消息，直接退出
        if (user_def_key_msg(xcfg_cb.user_def_kl_sel)) {
            break;
        }
    case KL_HSF:    // 长按接听键
        // 获取当前通话状态
        call_status = bt_get_call_indicate();
        if(call_status == BT_CALL_INCOMING) {
            // 如果有第二路来电，则拒接该来电
            bt_call_terminate();    // 拒接第二路来电
            // 播放拒接提示音
            sys_warning_play(T_WARNING_REJECT, PIANO_REJECT);
        } else if (xcfg_cb.bt_hfp_private_switch_en) {
            // 如果没有第二路来电且启用了私密接听切换功能
            // 则在手机和耳机之间切换通话
            bt_call_private_switch();    // 私密接听切换
        }
        break;

    // 双击处理：接听第二路来电，切换通话，或麦克风静音控制
    case KD_PLAY_PWR_USER_DEF:    // 双击电源/多功能组合键
    case KD_PLAY_USER_DEF:    // 双击多功能键
        // 尝试执行配置工具中设置的双击功能
        // 如果user_def_key_msg返回true，表示已处理该消息，直接退出
        if (user_def_key_msg(xcfg_cb.user_def_kd_sel)) {
            break;
        }
    case KD_HSF:    // 双击接听键
        // 获取当前通话状态
        call_status = bt_get_call_indicate();
        if (call_status == BT_CALL_INCOMING) {
            // 如果有第二路来电，则接听该来电
            set_mic_mute(0);    // 确保麦克风不是静音状态
            bt_call_answer_incoming();    // 接听第二路来电
        } else if(call_status == BT_CALL_3WAY_CALL) {
            // 如果当前是三方通话状态，则切换两路通话
            set_mic_mute(0);    // 确保麦克风不是静音状态
            bt_call_swap();    // 切换两路通话
        } else if (xcfg_cb.bt_hfp_mute_switch_en) {
            // 如果是普通通话状态且启用了麦克风静音切换功能
            // 则切换麦克风静音状态
            set_mic_mute(2);    // 切换麦克风左声道静音状态
        }
        break;

    // 长按保持处理：音量调节
    case KH_PLAY_PWR_USER_DEF:    // 长按保持电源/多功能组合键
    case KH_PLAY_USER_DEF:    // 长按保持多功能键
        // 根据配置的长按功能获取对应的音量调节消息
        // 并将该消息传递给通用消息处理函数
        func_message(get_user_def_vol_msg(xcfg_cb.user_def_kl_sel));
        break;

    // 三击按键处理
    case KTH_PLAY_USER_DEF:    // 三击多功能键
    case KTH_PLAY_PWR_USER_DEF:    // 三击电源/多功能组合键
        // 执行配置工具中设置的三击功能
        user_def_key_msg(xcfg_cb.user_def_kt_sel);
        break;

    // 四击按键处理
    case KFO_PLAY_USER_DEF:    // 四击多功能键
    case KFO_PLAY_PWR_USER_DEF:    // 四击电源/多功能组合键
        // 执行配置工具中设置的四击功能
        user_def_key_msg(xcfg_cb.user_def_kfour_sel);
        break;

    // 五击按键处理
    case KFI_PLAY_USER_DEF:    // 五击多功能键
    case KFI_PLAY_PWR_USER_DEF:    // 五击电源/多功能组合键
        // 执行配置工具中设置的五击功能
        user_def_key_msg(xcfg_cb.user_def_kfive_sel);
        break;

    // A2DP音乐播放事件处理
    case EVT_A2DP_MUSIC_PLAY:
        // 音频淡入，避免声音突变
        dac_fade_in();
        break;

    // 1秒定时消息处理
    case MSG_SYS_1S:
        // 每秒向手机报告电池电量
        bt_send_msg(BT_MSG_HFP_REPORT_BAT);
        break;

    // 其他消息处理
    default:
        // 将消息传递给通用消息处理函数
        func_message(msg);
        break;
    }
}

/**
 * @brief 蓝牙通话状态下的按键消息处理函数
 *
 * 该函数负责分发蓝牙通话状态下的各种按键消息，是通话状态下按键处理的入口函数。
 * 为了减少flash缺页，将常见的空消息和1秒定时消息单独处理。
 *
 * 工作流程：
 * 1. 对于空消息(NO_MSG)和1秒定时消息(MSG_SYS_1S)，调用sfunc_bt_call_message_m处理
 * 2. 对于其他所有消息，调用sfunc_bt_call_message_do处理
 *
 * 按键复用机制：
 * - 该函数是通话状态下按键复用机制的入口
 * - 通过状态感知，同一个按键在不同状态下执行不同功能：
 *   * 在音乐播放状态：UDK_PLAY_PAUSE表示播放/暂停
 *   * 在来电状态：UDK_PLAY_PAUSE表示接听电话
 *   * 在通话状态：UDK_PLAY_PAUSE表示挂断电话
 *
 * @param msg 待处理的按键消息
 */
AT(.text.bfunc.bt)
void sfunc_bt_call_message(u16 msg)
{
    if (msg == NO_MSG || msg == MSG_SYS_1S) {       // 减少flash缺页
        // 处理空消息和1秒定时消息
        sfunc_bt_call_message_m(msg);
    } else {
        // 处理其他所有消息
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
