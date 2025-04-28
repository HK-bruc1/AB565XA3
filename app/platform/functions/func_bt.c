#include "include.h"
#include "func.h"
#include "func_bt.h"
#include "func_bt_quick_test.h"

extern func_bt_t f_bt;

bool tws_res_is_busy(void);
void func_bt_status_update(void);
void func_bt_tws_set_channel(void);
u16 func_bt_chkclr_warning(u16 bits);
void func_bt_tws_bre_led_ctl(void);
void bt_ear_in(void);
void bt_ear_out(void);
void func_bt_inear_process(void);
bool bt_play_data_check(void);
void bt_play_data_init(void);
bool bt_tws_vbat_switch(void);
void ble_box_process(void);
void bsp_get_real_time_process(void);
void soft_timer_run(void);
void bt_tws_vbat_switch_m(void);
//snoop_rssi_threshold: 检测手机的信号强度 tws_rssi_threshold: 检测tws的信号强度 cnt:在50ms * cnt时间内一直低于设置的阈值则主从切换
bool bt_check_snoop_rssi(int8_t snoop_rssi_threshold,uint8_t tws_rssi_threshold,uint cnt);
#if FUNC_BT_EN

void func_bt_tws_status_update(u16 tws_warning)
{
    if(xcfg_cb.bt_tws_en) {
        func_bt_status_update();
        if(sys_cb.bt_tws_lr_mode != 0) {
            func_bt_tws_set_channel();
        }
#if LED_BREATHE_EN
        if ((tws_warning & BT_WARN_TWS_DISCON) && (sys_cb.breathe_led_sta >= T_BRE_TWS_CON)) {
            breathe_led_stop();
        }
#endif
    }
}

void func_bt_warning_discon(void)
{
#if BT_TWS_EN
        if(xcfg_cb.bt_tws_en) {
            func_bt_status_update();
            if(sys_cb.bt_tws_lr_mode != 0) {
                func_bt_tws_set_channel();
            }
        }
#endif
#if WARNING_BT_DISCONNECT
        motor_enable(MOTOR_DISCONN, T_MOTOR_DISCONN_SEL);
        if (bt_tws_is_sync_voice()) {
            tws_res_play(TWS_RES_DISCONNECT);
        } else {
            sys_warning_play(T_WARNING_BT_DISCONNECT, PIANO_BT_DISCONNECT);
        }
#endif // WARNING_BT_DISCONNECT

#if LED_BREATHE_EN
        if (sys_led.tws_con_bre_en) {
            if (bt_tws_is_connected()) {
                tws_res_play(TWS_BRE_LED_START);
            }
        }
#endif // LED_BREATHE_EN

#if WARNING_BT_WAIT_CONNECT
        sys_warning_play(T_WARNING_BT_WAIT4CONN, 0);
#endif // WARNING_BT_WAIT_CONNECT
        f_bt.autoplay = 0;
        sys_cb.ear_menu_en = 0;
#if BT_A2DP_VOL_CTRL_EN
        if (xcfg_cb.bt_vol_reset_for_discon_en && !bt_nor_is_connected()) {
            bsp_set_volume(SYS_INIT_VOLUME + 1);                 //恢复默认音量
            bsp_bt_vol_change();
        }
#endif
}

void func_bt_warning_conn(void)
{
#if BT_TWS_EN
        if(xcfg_cb.bt_tws_en) {
            func_bt_status_update();
        }
#endif
        motor_enable(MOTOR_CONN, T_MOTOR_CONN_SEL);
#if LED_BREATHE_EN
        if (sys_led.tws_con_bre_en) {
            tws_res_play(TWS_BRE_LED_STOP);
        }
#endif
        if (bt_tws_is_sync_voice()) {
            tws_res_play(TWS_RES_NOR_CONNECTED);
        } else {
            sys_warning_play(T_WARNING_BT_CONNECT, PIANO_BT_CONNECT);
        }
        f_bt.autoplay = 1;
}

void func_bt_warning_tws_con(u16 tws_warning)
{
    if (sys_cb.bt_tws_lr_mode != 0) {
        func_bt_tws_set_channel();
    }
#if LED_BREATHE_EN
            if (sys_led.tws_con_bre_en) {
                if ((tws_warning & BT_WARN_TWS_MCON) && (!bt_nor_is_connected())) {
                    tws_res_play(TWS_BRE_LED_START);
                }
            }
#endif
    if ((sys_cb.bt_tws_lr_mode != 0) && (!xcfg_cb.bt_tws_lr_voice_sel)) {
        u8 left_channel = func_bt_tws_get_channel();
        u8 left_index, right_index;
        left_index = T_WARNING_LEFT_CH;
        right_index = T_WARNING_RIGHT_CH;
        if (left_channel) {
            sys_warning_play(left_index, PIANO_TWS_CH);
            delay_5ms(230);
        } else {
            bt_audio_bypass();
            delay_5ms(190);
            sys_warning_play(right_index, PIANO_TWS_CH);
            bt_audio_enable();
        }
    } else {
        if (tws_warning & BT_WARN_TWS_MCON) {
            f_bt.w4_tws_warning = 1;
            tws_res_play(TWS_RES_CONNECTED);
        }
    }
}

AT(.text.bfunc.warning)
void func_bt_warning_hid_menu(void)
{
    if (func_bt_chkclr_warning(BT_WARN_HID_CON)) {
#if WARNING_BT_HID_MENU
        sys_warning_play(T_WARNING_CAMERA_ON, PIANO_CAMERA_ON);
#endif
    }
    if (func_bt_chkclr_warning(BT_WARN_HID_DISCON)) {
#if WARNING_BT_HID_MENU
        sys_warning_play(T_WARNING_CAMERA_OFF, PIANO_CAMERA_OFF);
#endif
    }

#if BT_HID_DISCON_DEFAULT_EN
    if (f_bt.hid_discon_flag) {
        if (bt_hid_is_ready_to_discon()) {
            f_bt.hid_discon_flag = 0;
            bt_hid_disconnect();
        }
    }
#endif // BT_HID_DISCON_DEFAULT_EN
}

AT(.text.bfunc.bt)
static void func_bt_warning(void)
{
    u16 tws_warning;

    while(tws_res_is_busy()) {
        tws_res_proc();
    }

#if BT_TWS_EN
    tws_warning = func_bt_chkclr_warning(BT_WARN_TWS_DISCON | BT_WARN_TWS_CON);
    if(tws_warning) {
        func_bt_tws_status_update(tws_warning);
    }
#endif

    if(func_bt_chkclr_warning(BT_WARN_DISCON)) {
        func_bt_warning_discon();
    }

	if(func_bt_chkclr_warning(BT_WARN_CON)) {
        func_bt_warning_conn();
    }

#if BT_TWS_EN
    if(xcfg_cb.bt_tws_en) {
        u16 tws_warning = func_bt_chkclr_warning(BT_WARN_TWS_SCON | BT_WARN_TWS_MCON );
        if (tws_warning != 0) {
            func_bt_warning_tws_con(tws_warning);
        }
    }
#endif

    if(func_bt_chkclr_warning(BT_WARN_PAIRING)) {
        if (bt_tws_is_sync_voice()) {
            tws_res_play(TWS_RES_PAIRING);                          //左右耳同步播放pairing提示音
        } else {
            if (f_bt.w4_tws_warning) {                              //先等TWS Connected同步提示音播完
                f_bt.warning_status |= BT_WARN_PAIRING;
            } else {
                sys_warning_play(T_WARNING_PAIRING, PIANO_PAIR);
            }
        }
    }

#if BT_HID_MENU_EN
    //按键手动断开HID Profile的提示音
    if (xcfg_cb.bt_hid_menu_en) {
        func_bt_warning_hid_menu();
    }
#endif // BT_HID_MENU_EN
}

void func_bt_dac_ctrl(void)
{
    if (xcfg_cb.dac_off_for_bt_disconn) {
        if (bt_nor_is_connected()) {
            if (sys_cb.dac_sta == 0) {
                func_bt_set_dac(1);
            }
        } else {
            if (sys_cb.dac_sta == 1) {
                func_bt_set_dac(0);
            }
        }
    }
}

void func_bt_disp_status_do(void)
{
    if(!bt_is_connected()) {
        en_auto_pwroff();
        sys_cb.sleep_en = BT_PAIR_SLEEP_EN;
    } else {
        dis_auto_pwroff();
        sys_cb.sleep_en = 1;
    }

    switch (f_bt.disp_status) {
    case BT_STA_CONNECTING:
        led_bt_reconnect();
        break;
    case BT_STA_INITING:
    case BT_STA_IDLE:
        led_bt_idle();
#if WARNING_BT_PAIR
        if(f_bt.need_pairing && f_bt.disp_status == BT_STA_IDLE) {
            f_bt.need_pairing = 0;
            if(xcfg_cb.warning_bt_pair && xcfg_cb.bt_tws_en) {
                f_bt.warning_status |= BT_WARN_PAIRING;
            }
        }
#endif
        break;
    case BT_STA_SCANNING:
        led_bt_scan();
        break;

    case BT_STA_DISCONNECTING:
        led_bt_connected();
        break;

    case BT_STA_CONNECTED:
        led_bt_connected();
        break;
    case BT_STA_INCOMING:
        led_bt_ring();
        break;
    case BT_STA_PLAYING:
        led_bt_play();
        break;
    case BT_STA_OUTGOING:
    case BT_STA_INCALL:
        led_bt_call();
        break;
    }

    if(f_bt.disp_status >= BT_STA_CONNECTED) {
        f_bt.need_pairing = 1;
    }
}

AT(.text.bfunc.bt)
static void func_bt_disp_status(void)
{
    uint status = bt_get_disp_status();

    if(f_bt.disp_status != status) {
        f_bt.disp_status = status;
        func_bt_disp_status_do();
    }
    func_bt_dac_ctrl();
    func_bt_tws_bre_led_ctl();
}

AT(.text.bfunc.bt)
void func_bt_status(void)
{
    while(1) {
        func_bt_disp_status();

        {
            func_bt_warning();
        }

        if(f_bt.disp_status != 0xff) {
            break;
        }
    }
}

#if BT_EARIN_DETECT_EN

//入耳检测处理
//AT(.text.bfunc.bt)
//void func_bt_inear_process(void)
//{
//    if (xcfg_cb.earin_gpio_sel) {
//        if (touchear_is_online()) {
//            if ((!sys_cb.loc_ear_sta) && tick_check_expire(sys_cb.te_tick, 5000)) {
//                bsp_bcnt_temp_calibration();
//            }
//        }
//        if (touchear_is_online() || device_is_online(DEV_EARIN)) {
//            if (sys_cb.loc_ear_sta) {
//                bsp_bcnt_temp_calibration_start();
//                bt_ear_in();
//            }
//            //BT_EARIN_USAGE_PP_EN = 1, 等待手机连接后自动播放
//            if ((sys_cb.inear_sta & 0x3) == 0x01 && bt_nor_is_connected()) {
//                sys_cb.inear_sta |= 0x02;
//                if (f_bt.disp_status < BT_STA_INCOMING) {
//                    bt_music_play();
//                }
//            }
//        } else {
//            if (!sys_cb.loc_ear_sta) {
//                bsp_bcnt_temp_calibration_stop();
//                bt_ear_out();
//            }
//            sys_cb.inear_sta = 0;
//        }
//    }
//}
#endif // BT_EARIN_DETECT_EN

//#if BT_2ACL_AUTO_SWITCH
//struct {
//    uint32_t check_tick;
//    uint16_t play_timer_cnt;
//    uint16_t clear_timer_cnt;
//    uint16_t protect_timer_cnt;
//} bt_silence;
//
//bool bt_play_data_check(void)
//{
//    bool ret = false;
//    if (tick_check_expire(bt_silence.check_tick, 10)) {
//        bt_silence.check_tick = tick_get();
//    } else {
//        return ret;
//    }
//
//    if (bt_silence.protect_timer_cnt) {
//        bt_silence.protect_timer_cnt--;
//        return ret;
//    }
//    //消抖
//    if (!bt_is_silence()) {
//        bt_silence.clear_timer_cnt = 0;
//        bt_silence.play_timer_cnt++;
//        if (bt_silence.play_timer_cnt > 100) {
//            ret = true;
//            bt_silence.play_timer_cnt = 0;
//            bt_silence.protect_timer_cnt = 200;
//        }
//    } else {
//        bt_silence.clear_timer_cnt++;
//        if (bt_silence.clear_timer_cnt > 100) {
//            bt_silence.play_timer_cnt = 0;
//        }
//    }
//    return ret;
//}
//
//
//void bt_play_data_init(void)
//{
//    memset(&bt_silence, 0, sizeof(bt_silence));
//}
//
//void bt_play_switch_device()
//{
//    bt_silence.play_timer_cnt = 0;
//    bt_silence.clear_timer_cnt = 0;
//    bt_silence.protect_timer_cnt = 400;
//    bt_music_play_switch();
//}
//#endif //BT_2ACL_AUTO_SWITCH

///根据tws电量进行主从切换
#if BT_TWS_VBAT_AUTO_SWITCH
bool bt_tws_vbat_switch(void)
{
    if (tick_check_expire(f_bt.tick_vbat, 10000)) {
        f_bt.tick_vbat = tick_get();
    } else {
        return false;
    }
    if (bt_tws_is_connected()) {
#if BT_DISP_LOW_VBAT_EN
        hfp_get_bat_level();                        //更新一下f_bt.loc_vbat
#else
        f_bt.loc_vbat = hfp_get_bat_level();
#endif
        if (!bt_tws_is_slave()) {
            int8_t rssi[4];
#if BT_DISP_LOW_VBAT_EN
            if (f_bt.rem_vbat == 0xff) {
                return false;
            }
#endif
            if (f_bt.rem_vbat - f_bt.loc_vbat > 1 && bt_tws_get_link_rssi(rssi, true)) {
                //rssi[0]:主耳tws, rssi[1]:从耳tws, rssi[2]:主耳与手机, rssi[3]：从耳与手机
                // 1、主从RSSI都低于-70允许切换
                // 2、从耳电量高并且从耳信号也好，允许切换
                if((rssi[0] > -78) && ((rssi[2] > -70 && rssi[3] > -70) || (rssi[3] - rssi[2] > 3))){
                    bt_tws_switch();
                    return true;
                }
            }
        } else {
            bt_send_msg(BT_MSG_TWS_SYNC_INFO);          //报告副耳电量给主耳
        }
    }
    return false;
}
#endif // BT_TWS_VBAT_AUTO_SWITCH

#if BT_A2DP_DEFAULT_VOLUME_EN
void func_bt_default_volume_process(void)
{
#if BT_A2DP_IOS_DEFAULT_VOLUME_FIX
    if (bt_is_ios_device()) {
        if (sys_cb1.delay_send_vol_ticks && tick_check_expire(sys_cb1.delay_send_vol_ticks, 2000)) {
            sys_cb1.delay_send_vol_ticks = 0;
            msg_enqueue(EVT_A2DP_DEFAULT_VOLUME);
        }
        return;
    }
#endif
    if (sys_cb1.a2dp_default_volume_ticks) {
        u32 ticks = 2000;                                    //这里修改连上笔记本的修改速度

        if (sys_cb1.a2dp_default_volume == 3) {
            ticks = 500;                                     //这里修改连上IOS播放时的修改速度
        }
        if (tick_check_expire(sys_cb1.a2dp_default_volume_ticks, ticks)) {
            sys_cb1.a2dp_default_volume_ticks = 0;
            if (bt_is_ios_device() && (sys_cb1.a2dp_default_volume != 3)) {
                return;
            }
            sys_cb1.a2dp_default_volume = 0;                 //笔记本超时2s未发absolute volume，清flag并设置默认音量
            msg_enqueue(EVT_A2DP_DEFAULT_VOLUME);
        }
    }
}
#endif

AT(.text.bfunc.bt)
void func_bt_sub_process(void)
{
    func_bt_status();
#if USER_TKEY
    bsp_tkey_spp_tx();
#endif
#if BT_EARIN_DETECT_EN
    func_bt_inear_process();
#endif
#if VUSB_SMART_VBAT_HOUSE_EN
    bsp_smart_vhouse_process(0);
#endif
    ble_popup_delay_process();
#if BT_2ACL_AUTO_SWITCH
    if (xcfg_cb.bt_2acl_auto_switch) {
        if (bt_play_data_check()) {
            bt_music_play_switch();
        }
    }
#endif
#if BT_TWS_VBAT_AUTO_SWITCH
    if (xcfg_cb.bt_tswi_vbat_en && f_bt.disp_status != BT_STA_INCOMING) {
#if BT_DISP_LOW_VBAT_EN
        bt_tws_vbat_switch();
#else
        bt_tws_vbat_switch_m();
#endif
    }
#endif
#if BT_A2DP_DEFAULT_VOLUME_EN
    func_bt_default_volume_process();
#endif
#if BT_TWS_STANDBY_SUPPORT_SWITCH_EN
    if(bt_check_snoop_rssi(-65,-65,30)){
       printf("Standby_AUDIO SWITCH\n");
       bt_tws_switch();
    }
#endif
#if BT_DISP_LOW_VBAT_EN
    if(tick_check_expire(sys_cb1.disp_low_vbat_ticks, 10000)) {
        sys_cb1.disp_low_vbat_ticks = tick_get();
        hfp_get_bat_level();                        //更新一下f_bt.loc_vbat
        if (bt_tws_is_connected()) {
            bt_send_msg(BT_MSG_TWS_SYNC_INFO);
        }
    }
#endif
}

AT(.text.bfunc.bt)
static void func_bt_process(void)
{
    /**
     * @brief 蓝牙模式的主要处理函数，负责蓝牙相关的周期性任务
     *
     * 该函数在蓝牙模式的主循环中被不断调用，处理以下任务：
     * 1. 通用功能处理（喂狗、电量检测等）
     * 2. 蓝牙特有功能处理（状态更新、TWS连接等）
     * 3. 来电/通话状态处理
     * 4. 自动关机和睡眠处理
     */

    // 1. 通用功能处理（喂狗、电量检测等）
    func_process();

    // 2. 蓝牙特有功能处理（状态更新、TWS连接等）
    func_bt_sub_process();

#if BT_TWS_AUTO_SWITCH
    // TWS自动切换功能：当满足切换条件时，执行主从切换
    if ((xcfg_cb.bt_tswi_msc_en) && tws_switch_is_need()) {
        bt_tws_switch();
    }
#endif

#if BT_MAP_EN
    // 蓝牙MAP功能：获取实时信息（如日历、消息等）
    bsp_get_real_time_process();
#endif

#if BT_QUICK_TEST_EN
    // 蓝牙快速测试功能
    if(f_bt.quick_test_flag){
        func_bt_quick_test();
    }
#endif // BT_QUICK_TEST_EN

    // 3. 来电/通话状态处理
    if(f_bt.disp_status == BT_STA_INCOMING) {
        // 来电状态：进入来电子状态机
        sfunc_bt_ring();
        reset_sleep_delay();      // 重置睡眠延时
        reset_pwroff_delay();     // 重置关机延时
        f_bt.siri_kl_flag = 0;    // 清除Siri长按标志
        f_bt.user_kl_flag = 0;    // 清除用户长按标志
    } else if(f_bt.disp_status >= BT_STA_OUTGOING) {
        // 通话状态：进入通话子状态机
        sfunc_bt_call();
        reset_sleep_delay();      // 重置睡眠延时
        reset_pwroff_delay();     // 重置关机延时
        f_bt.siri_kl_flag = 0;    // 清除Siri长按标志
        f_bt.user_kl_flag = 0;    // 清除用户长按标志
    }

    // 4. 自动关机处理
    if(sys_cb.pwroff_delay == 0) {
        // 如果关机延时到期且蓝牙未连接，则进入关机模式
        if (f_bt.disp_status < BT_STA_CONNECTED) {
            sys_cb.discon_reason = 1;             // 连接超时关主从切换,同步关机
            sys_cb.pwrdwn_tone_en = 1;            // 启用关机提示音
            func_cb.sta = FUNC_PWROFF;            // 切换到关机模式
            return;
        }
    }

    // 5. 睡眠处理
    if(sleep_process(bt_is_sleep)) {
        f_bt.disp_status = 0xff;                  // 设置特殊状态值，触发状态更新
    }
}

void func_bt_init(void)
{
    if (!f_bt.bt_is_inited) {
        msg_queue_clear();
        bt_ring_var_init();
        memset(&f_bt, 0, sizeof(func_bt_t));
        tws_res_init();
        bsp_bt_init();
        f_bt.bt_is_inited = 1;
        ble_popup_ctrl(1);
    }
}

void func_bt_chk_off(void)
{
    if ((func_cb.sta != FUNC_BT) && (f_bt.bt_is_inited)) {
#if BT_PWRKEY_5S_DISCOVER_EN
        bsp_bt_pwrkey5s_clr();
#endif
        bt_disconnect();
        bt_off();
        f_bt.bt_is_inited = 0;
    }
}

void func_bt_enter(void)
{
    led_bt_init();
    func_bt_init();
    en_auto_pwroff();

#if WARNING_FUNC_BT
    sys_warning_play(T_WARNING_BT_MODE, PIANO_BT_MODE);
#endif

#if WARNING_BT_WAIT_CONNECT
    sys_warning_play(T_WARNING_BT_WAIT4CONN, 0);
#endif

    if (f_bt.disp_status < BT_STA_CONNECTED || f_bt.disp_status == 0xfe) {
        func_bt_set_dac(0);
    }
    bsp_bt_redial_init();
    bt_audio_enable();
#if DAC_DNR_EN
    dac_dnr_set_sta(xcfg_cb.dac_dnr_en);
#endif

#if BT_PWRKEY_5S_DISCOVER_EN
    if(bsp_bt_pwrkey5s_check()) {
        f_bt.need_pairing = 0;  //已经播报了
        func_bt_disp_status();  //先更新显示，再播报
        if (xcfg_cb.bt_pwrkey_nsec_clr_pair_en) {
            bt_clr_all_link_info('5');
        }
#if WARNING_BT_PAIR
        sys_warning_play(T_WARNING_PAIRING, PIANO_PAIR);
#endif // WARNING_BT_PAIR
    } else {
        func_bt_disp_status();  //先更新显示，再播报
#if WARNING_BT_PAIR
        if (xcfg_cb.warning_bt_pair && !xcfg_cb.bt_tws_en) {
            sys_warning_play(T_WARNING_PAIRING, PIANO_PAIR);
        }
#endif // WARNING_BT_PAIR
    }
#endif

#if BT_2ACL_AUTO_SWITCH
    if (xcfg_cb.bt_2acl_auto_switch) {
        bt_play_data_init();
    }
#endif
}

void func_bt_exit(void)
{
    bt_ring_tone_stop(1);
#if DAC_DNR_EN
    dac_dnr_set_sta(0);
#endif
    bsp_change_volume(sys_cb.vol);
#if BT_PWRKEY_5S_DISCOVER_EN
    bsp_bt_pwrkey5s_clr();
#endif
//    func_bt_exit_display();
    tws_res_reset();
    dis_auto_pwroff();
    ble_popup_ctrl(0);
    bt_disconnect();

    dac_fade_out();
    bt_audio_bypass();
    bt_off();
#if LED_BREATHE_EN
    if (sys_cb.breathe_led_sta >= T_BRE_TWS_CON) {
        breathe_led_stop();
    }
#endif
    if (get_music_dec_sta() != MUSIC_STOP) {
        music_control(MUSIC_MSG_STOP);
    }
    func_bt_set_dac(1);
    if (sys_cb.pwroff_tick) {
        bt_exit_wait_ticks(sys_cb.pwroff_tick);
        sys_cb.pwroff_tick = 0;
    }
    f_bt.ring_sta = 0;
    f_bt.ring_stop = 0;
    f_bt.disp_status = BT_STA_OFF;
    f_bt.bt_is_inited = 0;
    func_cb.last = FUNC_BT;
}

AT(.text.bfunc.bt)
void func_bt(void)
{
    printf("%s\n", __func__);

    func_bt_enter();    // 进入蓝牙模式，初始化资源

    while (func_cb.sta == FUNC_BT) {    // 只要状态没变，就一直在这个循环中
        func_bt_process();  // 处理蓝牙相关的周期性任务
        func_bt_message(msg_dequeue());  // 处理蓝牙相关的消息
    }

    func_bt_exit(); // 退出蓝牙模式，清理资源
}

AT(.rodata.bt.cbt)
const u8 led_bt_dut_tbl[] = {
    0xff, 0x00, 0x02, 0x00,
    0xff, 0x00, 0x02, 0x00,
    0xff, 0x00, 0x02, 0x00,
};

#if BT_DUT_TEST_EN
//DUT测试模式, 红灯常亮
AT(.text.func.bt)
void func_bt_dut(void)
{
    u8 addr[6];
    u8 led[12];
    u8 bt_addr_config_en;
    u8 warning_bt_pair;
    u8 bt_tws_pair_mode;
    u8 bt_tws_en;

    if (f_bt.bt_is_inited) {
        bt_disconnect();
        bt_off();
        f_bt.bt_is_inited = 0;
    }
    memcpy(addr, xcfg_cb.bt_addr, 6);
    memcpy(led, &xcfg_cb.led_reconnect, 12);
    bt_addr_config_en = xcfg_cb.bt_addr_config_en;
    warning_bt_pair = xcfg_cb.warning_bt_pair;
    bt_tws_pair_mode = xcfg_cb.bt_tws_pair_mode;
    bt_tws_en = xcfg_cb.bt_tws_en;

    //printf("%s\n", __func__);
    sys_cb.pwroff_delay = sys_cb.pwroff_time = -1;                          //关闭未连接自动关机
    sys_cb.sleep_time = -1;                                                 //不进siff mode
    sys_cb.sleep_en = 0;
    memcpy(&xcfg_cb.led_reconnect, led_bt_dut_tbl, sizeof(led_bt_dut_tbl));    //红灯常亮
    if (xcfg_cb.bb_dut_test_addr_fixed_en) {
        xcfg_cb.bt_addr_config_en = 1;
        memset(xcfg_cb.bt_addr, 0x66, 6);                                   //固定蓝牙地址
    }
    cfg_bt_work_mode  = MODE_BQB_RF_BREDR;                                  //使能DUT模式
    xcfg_cb.warning_bt_pair = 0;                                            //关闭配对提示音
    xcfg_cb.bt_tws_pair_mode = 2;                                           //手动配对
    xcfg_cb.bt_tws_en = 0;

    func_bt_enter();
    while (func_cb.sta == FUNC_BT_DUT) {
        func_bt_process();
        func_bt_message(msg_dequeue());
    }
    func_bt_exit();

    memcpy(xcfg_cb.bt_addr, addr, 6);
    memcpy(&xcfg_cb.led_reconnect, led, 12);
    xcfg_cb.bt_addr_config_en = bt_addr_config_en;
    xcfg_cb.warning_bt_pair = warning_bt_pair;
    xcfg_cb.bt_tws_pair_mode = bt_tws_pair_mode;
    xcfg_cb.bt_tws_en = bt_tws_en;
}
#endif // BT_DUT_TEST_EN

#if BT_FCC_TEST_EN
//FCC 测试模式
AT(.text.func.bt)
void func_bt_fcc(void)
{
    printf("%s\n", __func__);
    sys_cb.pwroff_delay = sys_cb.pwroff_time = -1;                          //关闭未连接自动关机
    sys_cb.sleep_time = -1;                                                 //不进siff mode
    sys_cb.sleep_en = 0;
    memcpy(&xcfg_cb.led_reconnect, led_bt_dut_tbl, sizeof(led_bt_dut_tbl));    //红灯常亮
//    if (xcfg_cb.bb_cbt_test_addr_fixed_en) {
//        xcfg_cb.bt_addr_config_en = 1;
//        memset(xcfg_cb.bt_addr, 0x68, 6);                                   //固定蓝牙地址
//    }

    u8 fcc_en = xcfg_cb.bt_fcc_en;

    xcfg_cb.bt_fcc_en = 1;
    cfg_bt_work_mode = MODE_FCC_TEST;
    fcc_init_hook();
    func_bt_enter();
    while (func_cb.sta == FUNC_BT_FCC) {
        func_bt_process();
        func_bt_message(msg_dequeue());
    }

    fcc_exit_hook();
    xcfg_cb.bt_fcc_en = fcc_en;

    func_bt_exit();
    bt_work_mode_init();
}
#endif // BT_FCC_TEST_EN

#endif //FUNC_BT_EN
