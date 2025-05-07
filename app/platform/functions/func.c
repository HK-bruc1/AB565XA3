#include "include.h"

extern func_cb_t func_cb;
void bsp_bt_tws_switch_for_charge(void);
void lowpower_vbat_process(void);
void bsp_smic_parse_cmd(void);
u8 bsp_bt_tws_charge_delay(void);
void func_qtest_process(void);
void ble_box_process(void);
void soft_timer_run(void);

/**
 * @brief 通用功能处理函数
 * 
 * 该函数负责处理一些通用的系统功能,主要包括:
 * 1. 喂狗操作,防止系统复位
 * 2. 电池电量检测和低电处理(如果启用)
 * 3. 快速测试功能处理(如果启用)
 * 4. 硬件按键关机处理:
 *    - 当PWRKEY配置为硬开关时,检测关机标志
 *    - 触发关机时启用提示音并切换到关机模式
 * 5. 霍尔传感器触发关机处理
 * 6. NTC温度保护关机处理(如果启用):
 *    - 当温度超过阈值时触发关机保护
 *    - 启用关机提示音并切换到关机模式
 */
AT(.text.func.process)
void func_process(void)
{
    WDT_CLR();                                     //清看门狗,防止系统复位
#if VBAT_DETECT_EN
    lowpower_vbat_process();                       //电池电量检测和低电处理
#endif // VBAT_DETECT_EN

#if VUSB_TBOX_QTEST_EN
    func_qtest_process();                          //快速测试功能处理
    qtest_other_usage_process();                   //其他测试功能处理
#endif

    //PWRKEY模拟硬开关关机处理
    if ((PWRKEY_2_HW_PWRON) && (sys_cb.pwrdwn_hw_flag)) {
        sys_cb.pwrdwn_tone_en = 1;                 //启用关机提示音
        func_cb.sta = FUNC_PWROFF;                 //切换到关机模式
        sys_cb.pwrdwn_hw_flag = 0;                 //清除关机标志
    }

    hall_trigger_to_pwrdwn();                      //霍尔传感器触发关机处理

#if USER_NTC
    if (sys_cb.ntc_2_pwrdwn_flag) {               //NTC温度保护关机处理
        sys_cb.pwrdwn_tone_en = 1;                 //启用关机提示音
        func_cb.sta = FUNC_PWROFF;                 //切换到关机模式
        sys_cb.ntc_2_pwrdwn_flag = 0;             //清除温度保护标志
    }
#endif // USER_NTC
}

void func_volume_up(void)
{
    if (func_cb.sta == FUNC_BT) {
        if (sys_cb.incall_flag) {
            bt_call_volume_change(KU_VOL_UP);
        } else {
#if BT_HID_VOL_SYNC_EN
            if (bsp_bt_hid_vol_set(HID_KEY_VOL_UP)) {
                bsp_set_volume(bsp_volume_inc(sys_cb.vol, sys_cb.hfp2sys_mul));
            } else
#endif
            {
                bsp_set_volume(bsp_volume_inc(sys_cb.vol, 1));
            }
#if BT_A2DP_STORE_VOL_EN
            bt_store_a2dp_volume(sys_cb.vol);
#endif
            bsp_bt_vol_change();
#if WARNING_MAX_VOLUME
            if (sys_cb.vol == VOL_MAX && !f_bt.ring_sta) {
                if (bt_tws_is_sync_voice()) {
                    delay_5ms(4);
                    tws_res_play(TWS_RES_MAX_VOL);
                } else {
                    sys_warning_play(T_WARNING_MAX_VOL, PIANO_MAX_VOL);
                }
            }
#endif
        }
    } else {
        bsp_set_volume(bsp_volume_inc(sys_cb.vol, 1));
        if (sys_cb.vol == VOL_MAX) {
            sys_warning_play(T_WARNING_MAX_VOL, PIANO_MAX_VOL);
        }
    }
    if (func_cb.set_vol_callback) {
        func_cb.set_vol_callback(1);
    }
}

void func_volume_down(void)
{
    if (func_cb.sta == FUNC_BT) {
        if (sys_cb.incall_flag) {
            bt_call_volume_change(KU_VOL_DOWN);
        } else {
#if BT_HID_VOL_SYNC_EN
            if (bsp_bt_hid_vol_set(HID_KEY_VOL_DOWN)) {
                bsp_set_volume(bsp_volume_dec(sys_cb.vol, sys_cb.hfp2sys_mul));
            } else
#endif
            {
                bsp_set_volume(bsp_volume_dec(sys_cb.vol, 1));
            }
#if BT_A2DP_STORE_VOL_EN
            bt_store_a2dp_volume(sys_cb.vol);
#endif
            bsp_bt_vol_change();
#if WARNING_MIN_VOLUME
            if (sys_cb.vol == 0 && !f_bt.ring_sta) {
                minvol_tone_play();
            }
#endif
        }
    } else {
        bsp_set_volume(bsp_volume_dec(sys_cb.vol, 1));
        if (sys_cb.vol == 0) {
            minvol_tone_play();
        }
    }
    if (func_cb.set_vol_callback) {
        func_cb.set_vol_callback(0);
    }
}

//func common message process
AT(.text.bfunc.msg)
void func_message(u16 msg)
{
    switch (msg) {
#if WAV_KEY_VOICE_QUICK_EN
        case K_PLAY_PWR_USER_DEF:
            wav_res_play(RES_BUF_TEST_WAV, RES_LEN_TEST_WAV);
            sys_cb.tws_res_brk = 0;
            break;
#endif
        case KL_NEXT_VOL_UP:
        case KH_NEXT_VOL_UP:
        case KL_PREV_VOL_UP:
        case KH_PREV_VOL_UP:
        case KU_VOL_UP_NEXT:
        case KU_VOL_UP_PREV:
        case KU_VOL_UP:
        case KL_VOL_UP:
        case KH_VOL_UP:
        case KU_VOL_UP_DOWN:
            func_volume_up();
            break;

        case KL_PREV_VOL_DOWN:
        case KH_PREV_VOL_DOWN:
        case KL_NEXT_VOL_DOWN:
        case KH_NEXT_VOL_DOWN:
        case KU_VOL_DOWN_PREV:
        case KU_VOL_DOWN_NEXT:
        case KU_VOL_DOWN:
        case KL_VOL_DOWN:
        case KH_VOL_DOWN:
        case KL_VOL_UP_DOWN:
        case KH_VOL_UP_DOWN:
            func_volume_down();
            break;

        //耳机入仓关机
        case EVT_CHARGE_INBOX:
            sys_cb.pwrdwn_tone_en = 0;
            sys_cb.inbox_pwrdwn_flag = 1;
            bsp_charge_inbox_wakeup_enable();
            func_cb.sta = FUNC_PWROFF;
            break;

#if CHARGE_DELAY_OPEN_LEAKAGE_EN
		case EVT_SET_LEAKAGE:
			bsp_charge_leakage_set(1, 0);
			break;
#endif

        //长按PP/POWER软关机(通过PWROFF_PRESS_TIME控制长按时间)
        case KLH_POWER:
        case KLH_MODE_PWR:
        case KLH_PLAY_PWR_USER_DEF:
            if (!xcfg_cb.kl_pwrdwn_dis) {
                sys_cb.discon_reason = !xcfg_cb.bt_tswi_kpwr_en;       //按键关机是否主从切换
                sys_cb.pwrdwn_tone_en = 1;
                func_cb.sta = FUNC_PWROFF;
            }
            break;

#if IRRX_HW_EN
        case KU_IR_POWER:
            func_cb.sta = FUNC_SLEEPMODE;
            break;
#endif

        case KU_MODE:
        case KU_MODE_PWR:
#if FUNC_AUX_EN || FUNC_SPEAKER_EN || FUNC_MUSIC_EN
            func_cb.sta = FUNC_NULL;
#endif
            break;


#if EQ_MODE_EN
        case KU_EQ:
            sys_set_eq();
            break;
#endif // EQ_MODE_EN

//        case KU_MUTE:
//            if (sys_cb.mute) {
//                bsp_sys_unmute();
//            } else {
//                bsp_sys_mute();
//            }
//            break;

#if LINEIN_DETECT_EN
        case EVT_LINEIN_INSERT:
            if (device_is_online(DEV_LINEIN)) {
                if (xcfg_cb.linein_2_pwrdown_en) {
                    sys_cb.pwrdwn_tone_en = LINEIN_2_PWRDOWN_TONE_EN;
                    func_cb.sta = FUNC_PWROFF;
                } else {
                    func_cb.sta = FUNC_AUX;
                }

            }
            break;
#endif // LINEIN_DETECT_EN

        case EVT_HFP_SET_VOL:
            if(sys_cb.incall_flag & INCALL_FLAG_SCO){
                bsp_change_volume(bt_hfp_get_sys_vol(sys_cb.hfp_vol));
            }
            break;

        case EVT_TWS_SET_VOL:
        case EVT_A2DP_SET_VOL:
            if ((sys_cb.incall_flag & INCALL_FLAG_SCO) == 0) {
                printf("a2dp_vol: %d\n", sys_cb.vol);
#if BT_A2DP_STORE_VOL_EN
                bt_store_a2dp_volume(sys_cb.vol);
#endif
                bsp_change_volume(sys_cb.vol);
 //               gui_ocx_show_vol();
                param_sys_vol_write();
                sys_cb.cm_times = 0;
                sys_cb.cm_vol_change = 1;
            }
            break;
        case EVT_BT_SCAN_START:
            if (bt_get_status() < BT_STA_SCANNING) {
                bt_scan_enable();
            }
            break;

#if EQ_DBG_IN_UART || EQ_DBG_IN_SPP
        case EVT_ONLINE_SET_EQ:
            eq_parse_cmd();
            break;
#endif

#if ANC_EN
        case EVT_ONLINE_SET_ANC:
            bsp_anc_parse_cmd();
            break;
#endif

        case EVT_ONLINE_SET_AEC:
            bsp_tws_sync_info_eq_aec();
            break;

#if LANG_SELECT >= LANG_EN_ZH
        case EVT_BT_SET_LANG_ID:
            param_lang_write();
            break;
#endif

#if EQ_MODE_EN
        case EVT_BT_SET_EQ:
            music_set_eq_by_num(sys_cb.eq_mode);
            break;
#endif

#if SMIC_DBG_EN
        case EVT_ONLINE_SET_SMIC:
            bsp_smic_parse_cmd();
        break;
#endif

#if VUSB_TBOX_QTEST_EN
        case EVT_QTEST_PICKUP_PWROFF:
            func_cb.sta = FUNC_PWROFF;
            break;
#endif

        case EVT_BT_TWS_SWITCH:
            if (CHARGE_DC_IN()) {
#if BT_TWS_CHARGE_DELAY_EN
                if (!bsp_bt_tws_charge_delay()) {
                    sys_cb.dc_rst_flag = 1;
                    vusb_delay_rst_dis();
                    break;
                }
#endif
                if ((xcfg_cb.bt_enter_charge_rst_en) || (func_cb.sta != FUNC_BT) || (sys_cb.vbat < 3300)) {
                    vusb_delay_rst_dis();
                    func_bt_exit();
                    sw_reset_kick(SW_RST_DC_IN);                    //直接复位进入充电
                } else {
                    bsp_bt_tws_switch_for_charge();
                }
            } else {
                sys_cb.dc_rst_flag = 1;
                vusb_delay_rst_dis();
            }
            break;

#if BT_TWS_SLAVE_LOSTCONN_WARNING_EN
        case EVT_TWS_SLAVE_LOSTCONN:
            sys_warning_play(T_WARNING_BT_DISCONNECT, PIANO_BT_DISCONNECT);
            break;
#endif
    }

    //调节音量，3秒后写入flash
    if ((sys_cb.cm_vol_change) && (sys_cb.cm_times >= 3)) {
        sys_cb.cm_vol_change = 0;
        param_sync();
    }
}

AT(.text.func)
void func_clear(void)
{
    param_sync();
    reset_sleep_delay();
    dis_auto_pwroff();
#if FUNC_AUX_EN
    func_cb.set_vol_callback = NULL;
#endif
    AMPLIFIER_SEL_D();
    WDT_CLR();
}

AT(.text.func)
void func_exit(void)
{
    u8 func_num;
    u8 funcs_total = get_funcs_total();

    for (func_num = 0; func_num != funcs_total; func_num++) {
        if (func_cb.last == func_sort_table[func_num]) {
            break;
        }
    }
    func_num++;                                     //切换到下一个任务
    if (func_num >= funcs_total) {
        func_num = 0;
    }
    func_cb.sta = func_sort_table[func_num];        //新的任务
}

AT(.text.func)
void func_run(void)
{
    /**
     * @brief 功能运行函数，是整个耳机SDK的主循环入口
     *
     * 该函数是一个无限循环，根据func_cb.sta的值来决定执行哪个功能模式
     * 默认情况下，系统会进入蓝牙模式(FUNC_BT)
     * 各功能模式通过条件编译宏在config.h中配置是否启用
     */
    printf("%s\n", __func__);

    func_bt_chk_off();                                      // 蓝牙关闭检查
    while (1) {                                             // 无限循环，永不退出
        func_clear();                                       // 清除功能状态，同步参数，重置睡眠延时等

        // 根据func_cb.sta的值切换到不同的功能模式
        // 进入模式的两个要素：在config.h中进行了宏定义使能，第二个是func_cb.sta的值要是对应的功能才行。
        switch (func_cb.sta) {
#if FUNC_BT_EN 
        //蓝牙宏启用的前提下,默认为1，启用蓝牙功能
        //func_cb.sta的值要是FUNC_BT（枚举值），才会进入蓝牙模式
        //这是状态机轮询模型，蓝牙模式一直保持进入
        case FUNC_BT:
            func_bt();                                      // 进入蓝牙模式
            break;
#endif

#if BT_DUT_TEST_EN                                          // 蓝牙DUT测试模式
        case FUNC_BT_DUT:
            func_bt_dut();                                  // 进入蓝牙DUT测试模式
            break;
#endif

#if FUNC_BTHID_EN                                           // 默认为0，不启用蓝牙HID功能（自拍器模式）
        case FUNC_BTHID:
            func_bthid();                                   // 进入蓝牙HID模式
            break;
#endif // FUNC_BTHID_EN

#if FUNC_AUX_EN                                             // 默认为0，不启用AUX功能
        case FUNC_AUX:
            func_aux();                                     // 进入AUX模式（线路输入模式）
            break;
#endif // FUNC_AUX_EN

#if FUNC_SPEAKER_EN                                         // 默认为0，不启用Speaker模式
        case FUNC_SPEAKER:
            func_speaker();                                 // 进入Speaker模式
            break;
#endif // FUNC_SPEAKER_EN

        case FUNC_PWROFF:                                   // 关机模式，不受条件编译控制，始终可用
            func_pwroff(sys_cb.pwrdwn_tone_en);             // 进入关机模式，参数控制是否播放关机提示音
            break;

#if BT_FCC_TEST_EN                                          // 蓝牙FCC测试模式
        case FUNC_BT_FCC:
            func_bt_fcc();                                  // 进入蓝牙FCC测试模式
            break;
#endif // BT_FCC_TEST_EN

        default:                                            // 未知模式或未启用的模式
            func_exit();                                    // 切换到下一个可用的功能模式
            break;
        }
    }
}
