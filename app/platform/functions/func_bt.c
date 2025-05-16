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
            //这是TWS配对成功时的提示音播放（双耳同步播放）
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

/**
 * @brief 关于配置工具都没有的LED灯效配置项，他们从哪里读取灯效配置？？？
 * 
 */
void func_bt_disp_status_do(void)
{
    // 根据蓝牙连接状态更新电源管理设置
    if(!bt_is_connected()) {
        en_auto_pwroff();  // 未连接时启用自动关机
        sys_cb.sleep_en = BT_PAIR_SLEEP_EN;  // 设置睡眠模式
    } else {
        dis_auto_pwroff();  // 已连接时禁用自动关机
        sys_cb.sleep_en = 1;  // 保持常亮模式
    }

    // 根据蓝牙状态触发光效
    switch (f_bt.disp_status) {
    case BT_STA_CONNECTING:
        led_bt_reconnect();  // 连接过程中显示重连灯效，此配置在配置工具中配置
        break;
    case BT_STA_INITING:
    case BT_STA_IDLE:
        //主耳TWS组队成功后进入到BT_STA_IDLE
        //副耳TWS组队成功后进入到BT_STA_CONNECTED
        led_bt_idle();
#if WARNING_BT_PAIR
        // 当需要配对且处于空闲状态时（仅在TWS组队模式下触发）
        if(f_bt.need_pairing && f_bt.disp_status == BT_STA_IDLE) {
            f_bt.need_pairing = 0;
            // 当启用蓝牙配对提醒且TWS功能开启时
            if(xcfg_cb.warning_bt_pair && xcfg_cb.bt_tws_en) {
                f_bt.warning_status |= BT_WARN_PAIRING;  // 触发配对提醒
            }
        }
#endif
        break;
    case BT_STA_SCANNING:
        //首次上电左右耳机首先显示开机灯效，然后进入 case BT_STA_SCANNING,TWS 未组队和蓝牙未配对灯效
        led_bt_scan();  // 扫描状态显示扫描灯效
        break;

    case BT_STA_DISCONNECTING:
        //为什么连接状态与未连接状态都是一个灯效？
        //这个应该时TWS未组队，蓝牙未配对
        led_bt_connected();
        break;

    case BT_STA_CONNECTED:
        //TWS组队成功，副耳的状态
        //蓝牙连接成功后，主耳也会进入这个状态
        led_bt_connected();  // 已连接状态灯效
        break;
    case BT_STA_INCOMING:
        led_bt_ring();  // 来电状态显示来电灯效
        break;
    case BT_STA_PLAYING:
        led_bt_play();  // 播放状态显示动态灯效
        break;
    case BT_STA_OUTGOING:
    case BT_STA_INCALL:
        led_bt_call();  // 拨出/通话状态显示通话灯效
        break;
    }

    // 当状态为已连接及以上时，标记需要配对（用于断开后快速回连）
    if(f_bt.disp_status >= BT_STA_CONNECTED) {
        f_bt.need_pairing = 1;
    }
}

AT(.text.bfunc.bt)
static void func_bt_disp_status(void)
{
    /**
     * @brief 蓝牙状态获取和LED显示控制函数
     *
     * 该函数负责获取当前蓝牙状态，并根据状态变化控制LED显示、DAC状态和TWS呼吸灯效果。
     * 主要功能：
     * 1. 获取当前蓝牙状态并检测是否有变化
     * 2. 如果状态有变化，更新状态并执行相应的LED显示控制
     * 3. 根据蓝牙连接状态控制DAC开关
     * 4. 控制TWS呼吸灯效果
     */

    // 1. 获取当前蓝牙状态
    uint status = bt_get_disp_status(); // 调用底层函数获取当前蓝牙状态

    // 2. 检测状态是否有变化，如有变化则更新LED显示
    if(f_bt.disp_status != status) {    // 如果状态有变化
        f_bt.disp_status = status;      // 更新状态变量
        func_bt_disp_status_do();       // 执行状态变化后的操作（更新LED显示）
                                        // 不同状态对应不同的LED显示模式：
                                        // - BT_STA_CONNECTING: led_bt_reconnect()
                                        // - BT_STA_IDLE: led_bt_idle()
                                        // - BT_STA_SCANNING: led_bt_scan()
                                        // - BT_STA_CONNECTED: led_bt_connected()
                                        // - BT_STA_PLAYING: led_bt_play()
                                        // - BT_STA_INCOMING: led_bt_ring()
                                        // - BT_STA_OUTGOING/BT_STA_INCALL: led_bt_call()
    }

    // 3. 根据蓝牙连接状态控制DAC开关
    func_bt_dac_ctrl();                 // 根据蓝牙连接状态控制DAC开关
                                        // 如果配置了蓝牙断开时关闭DAC(dac_off_for_bt_disconn)：
                                        // - 已连接时打开DAC
                                        // - 未连接时关闭DAC，节省功耗

    // 4. 控制TWS呼吸灯效果
    func_bt_tws_bre_led_ctl();          // 控制TWS呼吸灯效果
                                        // 根据TWS连接状态和配置控制呼吸灯：
                                        // - TWS已连接但未连接手机：可能启用T_BRE_TWS_CON模式呼吸灯
                                        // - TWS已连接且正在播放音乐/通话：可能启用T_BRE_TWS_PLAY模式呼吸灯
}

AT(.text.bfunc.bt)
void func_bt_status(void)
{
    /**
     * @brief 蓝牙状态更新和提示音处理函数
     *
     * 该函数负责更新蓝牙状态并根据状态变化控制LED显示和播放提示音。
     * 它在一个循环中执行，直到状态更新完成（f_bt.disp_status != 0xff）。
     *
     * 主要功能：
     * 1. 调用func_bt_disp_status()获取并更新蓝牙状态，同时根据状态控制LED显示
     * 2. 调用func_bt_warning()处理各种提示音播放（如连接/断开/配对等）
     * 3. 当睡眠唤醒时，f_bt.disp_status会被设为0xff，此时需要重新获取状态
     */
    while(1) {
        // 1. 获取并更新蓝牙状态，同时根据状态控制LED显示
        // 调用bt_get_disp_status()获取当前蓝牙状态，可能的值包括：
        // BT_STA_OFF(0)：蓝牙已关闭
        // BT_STA_INITING(1)：蓝牙初始化中
        // BT_STA_IDLE(2)：蓝牙空闲（未连接）
        // BT_STA_SCANNING(3)：正在扫描
        // BT_STA_DISCONNECTING(4)：正在断开连接
        // BT_STA_CONNECTING(5)：正在连接
        // BT_STA_CONNECTED(6)：已连接
        // BT_STA_PLAYING(7)：正在播放音乐
        // BT_STA_INCOMING(8)：来电振铃
        // BT_STA_OUTGOING(9)：正在拨号
        // BT_STA_INCALL(10)：通话中
        func_bt_disp_status();

        // 2. 处理各种提示音播放（如连接/断开/配对等）
        {
            func_bt_warning();
        }

        // 3. 检查状态是否已更新完成
        // 当从睡眠唤醒时，f_bt.disp_status会被设为0xff，需要重新获取状态
        // 一旦状态更新完成（不等于0xff），就退出循环
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
    /**
     * @brief 蓝牙模式的子处理函数，负责处理蓝牙模式下的各种周期性任务
     *
     * 该函数在蓝牙主循环中被调用，处理以下任务：
     * 1. 蓝牙状态更新和显示
     * 2. 各种蓝牙特性的周期性处理（如TWS、BLE弹窗等）
     * 3. 电量监测和同步
     * 4. 信号强度检测和主从切换
     */

    // 1. 更新蓝牙状态和LED显示
    func_bt_status();                                // 更新蓝牙状态并处理LED显示

#if USER_TKEY
    // 2. 触摸按键SPP数据发送（如果启用）
    bsp_tkey_spp_tx();                               // 通过SPP发送触摸按键数据
#endif

#if BT_EARIN_DETECT_EN
    // 3. 入耳检测处理（如果启用）
    func_bt_inear_process();                         // 处理入耳检测功能，可能会触发自动开关机或播放/暂停
#endif

#if VUSB_SMART_VBAT_HOUSE_EN
    // 4. 智能充电仓处理（如果启用）
    bsp_smart_vhouse_process(0);                     // 处理智能充电仓相关功能，如电量显示、开盖检测等
#endif

    // 5. BLE弹窗功能处理
    ble_popup_delay_process();                       // 处理BLE弹窗延时，用于iOS设备连接时显示电量弹窗

#if BT_2ACL_AUTO_SWITCH
    // 6. 双ACL自动切换功能（如果启用）
    if (xcfg_cb.bt_2acl_auto_switch) {               // 如果启用了双ACL自动切换功能
        if (bt_play_data_check()) {                  // 检查是否需要切换播放设备
            bt_music_play_switch();                  // 执行音乐播放设备切换
        }
    }
#endif

#if BT_TWS_VBAT_AUTO_SWITCH
    // 7. TWS根据电量自动主从切换功能（如果启用）
    if (xcfg_cb.bt_tswi_vbat_en && f_bt.disp_status != BT_STA_INCOMING) {  // 如果启用了电量切换且不在来电状态
#if BT_DISP_LOW_VBAT_EN
        bt_tws_vbat_switch();                        // 根据电量执行TWS主从切换（显示低电量版本）
#else
        bt_tws_vbat_switch_m();                      // 根据电量执行TWS主从切换（标准版本）
#endif
    }
#endif

#if BT_A2DP_DEFAULT_VOLUME_EN
    // 8. A2DP默认音量处理（如果启用）
    func_bt_default_volume_process();                // 处理A2DP默认音量设置，确保连接后音量正确
#endif

#if BT_TWS_STANDBY_SUPPORT_SWITCH_EN
    // 9. 根据信号强度自动主从切换功能（如果启用）
    if(bt_check_snoop_rssi(-65,-65,30)){            // 检查手机和TWS信号强度，如果低于阈值
       printf("Standby_AUDIO SWITCH\n");
       bt_tws_switch();                              // 执行TWS主从切换
    }
#endif

#if BT_DISP_LOW_VBAT_EN
    // 10. 低电量显示和同步功能（如果启用）
    if(tick_check_expire(sys_cb1.disp_low_vbat_ticks, 10000)) {  // 每10秒检查一次
        sys_cb1.disp_low_vbat_ticks = tick_get();    // 更新计时器
        hfp_get_bat_level();                         // 更新本地电量值(f_bt.loc_vbat)
        if (bt_tws_is_connected()) {                 // 如果TWS已连接
            bt_send_msg(BT_MSG_TWS_SYNC_INFO);       // 发送消息同步TWS信息（包括电量）
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
    //比如一个耳机入仓了
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
    //不开启的话，就是正常处理：来电/通话状态处理，自动关机处理
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
            func_cb.sta = FUNC_PWROFF;            // 切换到关机模式，不受模式宏控制
            return;
        }
    }

    // 5. 睡眠处理
    if(sleep_process(bt_is_sleep)) {
        /**
         * @brief 设置特殊状态值0xff，触发状态更新机制
         *
         * 这里的0xff是一个特殊的状态标志值，不是实际的蓝牙状态。
         * 当系统从睡眠状态唤醒时，需要重新获取当前的蓝牙状态并更新显示。
         *
         * 工作原理：
         * 1. 设置f_bt.disp_status = 0xff作为一个特殊标记
         * 2. 在func_bt_status()函数中，会检测到这个特殊值
         * 3. func_bt_status()会进入一个循环，不断调用func_bt_disp_status()直到状态更新完成
         * 4. func_bt_disp_status()会调用bt_get_disp_status()获取实际的蓝牙状态
         * 5. 当检测到状态变化时，会调用func_bt_disp_status_do()更新LED显示
         *
         * LED状态更新机制：
         * - 在func_bt_disp_status_do()中，根据不同的蓝牙状态调用不同的LED显示函数：
         *   * BT_STA_IDLE: led_bt_idle() - 蓝牙空闲状态LED显示
         *   * BT_STA_CONNECTED: led_bt_connected() - 蓝牙已连接状态LED显示
         *   * BT_STA_PLAYING: led_bt_play() - 音乐播放状态LED显示
         *   * BT_STA_INCOMING: led_bt_ring() - 来电状态LED显示
         *   * BT_STA_INCALL: led_bt_call() - 通话状态LED显示
         *
         * 因此，设置f_bt.disp_status = 0xff会触发整个状态更新链，最终导致LED状态根据当前蓝牙状态更新。
         */
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

/**
 * @brief 在进入蓝牙模式主循环前，做蓝牙相关的初始化和资源准备。它的主要作用是：
 * 初始化LED、蓝牙协议栈、自动关机、提示音、DAC、重拨、音频、降噪、配对等相关功能，为后续蓝牙主循环做好准备。
 * 1. 初始化蓝牙LED显示（如蓝牙模式下的灯效）
 * 2. 初始化蓝牙相关变量和协议栈（如清消息队列、变量清零、协议栈初始化等）
 * 3. 使能自动关机功能（如长时间未连接自动关机）
 * 4. 播放进入蓝牙模式的提示音（如"蓝牙模式"）
 * 5. 播放"等待连接"提示音
 * 6. 如果当前蓝牙未连接或处于特殊状态，则关闭DAC（节省功耗）   
 * 7. 初始化蓝牙重拨功能（如来电重拨相关变量）
 * 8. 使能蓝牙音频功能（如打开音频通路）
 * 9. 设置DAC降噪功能（如果配置了降噪）
 * 10. 检查是否长按电源键进入配对模式
 * 11. 如果支持双ACL自动切换，初始化相关变量
 */
void func_bt_enter(void)
{
    // 1. 初始化蓝牙LED显示（如蓝牙模式下的灯效）,不可见
    led_bt_init();

    // 2. 初始化蓝牙相关变量和协议栈（如清消息队列、变量清零、协议栈初始化等）
    func_bt_init();

    // 3. 使能自动关机功能（如长时间未连接自动关机），不可见
    en_auto_pwroff();

#if WARNING_FUNC_BT
    // 4. 播放进入蓝牙模式的提示音（如"蓝牙模式"）
    //通过 sys_warning_play(提示音ID, 音色ID) 指定。
    //音色ID:type: 0->无提示音, 1->piano, 2->语音
    sys_warning_play(T_WARNING_BT_MODE, PIANO_BT_MODE);
#endif

#if WARNING_BT_WAIT_CONNECT
    // 5. 播放"等待连接"提示音
    sys_warning_play(T_WARNING_BT_WAIT4CONN, 0);
#endif

    // 6. 如果当前蓝牙未连接或处于特殊状态，则关闭DAC（节省功耗）
    if (f_bt.disp_status < BT_STA_CONNECTED || f_bt.disp_status == 0xfe) {
        func_bt_set_dac(0);
    }

    // 7. 初始化蓝牙重拨功能（如来电重拨相关变量）
    bsp_bt_redial_init();

    // 8. 使能蓝牙音频功能（如打开音频通路）
    bt_audio_enable();

#if DAC_DNR_EN
    // 9. 设置DAC降噪功能（如果配置了降噪）
    dac_dnr_set_sta(xcfg_cb.dac_dnr_en);
#endif

#if BT_PWRKEY_5S_DISCOVER_EN
    // 10. 检查是否长按电源键进入配对模式
    if(bsp_bt_pwrkey5s_check()) {
        f_bt.need_pairing = 0;  // 已经播报过配对，不再重复
        func_bt_disp_status();  // 先更新LED等显示
        if (xcfg_cb.bt_pwrkey_nsec_clr_pair_en) {
            bt_clr_all_link_info('5'); // 清除配对信息
        }
#if WARNING_BT_PAIR
        // 播放配对提示音
        sys_warning_play(T_WARNING_PAIRING, PIANO_PAIR);
#endif // WARNING_BT_PAIR
    } else {
        func_bt_disp_status();  // 先更新LED等显示
#if WARNING_BT_PAIR
        // 如果配置了配对提示音且未启用TWS，则播放配对提示音
        if (xcfg_cb.warning_bt_pair && !xcfg_cb.bt_tws_en) {
            sys_warning_play(T_WARNING_PAIRING, PIANO_PAIR);
        }
#endif // WARNING_BT_PAIR
    }
#endif

#if BT_2ACL_AUTO_SWITCH
    // 11. 如果支持双ACL自动切换，初始化相关变量
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
        func_bt_process();  // 处理蓝牙相关的周期性任务，来电状态和通话状态会自己处理。让来电和通话状态有完全独立的处理逻辑，不受主循环的干扰，确保这些重要功能的稳定性和响应速度。
        func_bt_message(msg_dequeue());  // 处理蓝牙相关的消息事件
    }

    func_bt_exit(); // 退出蓝牙模式，清理资源
}

//DUT模式的灯效设置
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
