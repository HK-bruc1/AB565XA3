//该头文件由软件自动生成，请勿随意修改！
#ifndef _XCFG_H
#define _XCFG_H

#define XCFG_EN             1

typedef struct __attribute__((packed)) _xcfg_cb_t {
    u32 config_en                        : 1;   //启用配置
    u32 lang_id                          : 2;   //语音语言选择: 英文: 0, 中文: 1, 中英文(出厂默认英文): 2, 中英文(出厂默认中文): 3
    u32 soft_power_on_off                : 1;   //软开关机功能
    u32 powkey_10s_reset                 : 1;   //POWKEY 10s复位系统
    u32 pwron_frist_bat_en               : 1;   //第一次上电直接开机
    u32 sys_clk                          : 1;   //系统时钟52MHz
    u32 vol32_en                         : 1;   //系统32级音量
    u32 spk_mute_en                      : 1;   //功放MUTE功能
    u32 high_mute                        : 1;   //高MUTE
    u32 mute_io_sel                      : 3;   //功放MUTE的IO选择: None: 0, PA6: 1, PA7: 2, PB3: 3, PB4: 4, PE0: 5, PE7: 6, PF2: 7
    u32 loudspeaker_unmute_delay         : 4;   //功放MUTE延时(单位50ms)
    u32 sys_sleep_time                   : 5;   //自动休眠时间: 不休眠: 0, 10秒钟后: 1, 20秒钟后: 2, 30秒钟后: 3, 40秒钟后: 4, 50秒钟后: 5, 1分钟后: 6, 2分钟后: 7, 3分钟后: 8, 4分钟后: 9, 5分钟后: 10, 6分钟后: 11, 7分钟后: 12, 8分钟后: 13, 9分钟后: 14, 10分钟后: 15, 12分钟后: 16, 15分钟后: 17, 20分钟后: 18, 25分钟后: 19, 30分钟后: 20, 45分钟后: 21, 1小时后: 22, 关闭SNIFF: 23
    u32 sys_off_time                     : 5;   //自动关机时间: 不关机: 0, 10秒钟后: 1, 20秒钟后: 2, 30秒钟后: 3, 40秒钟后: 4, 50秒钟后: 5, 1分钟后: 6, 2分钟后: 7, 3分钟后: 8, 4分钟后: 9, 5分钟后: 10, 6分钟后: 11, 7分钟后: 12, 8分钟后: 13, 9分钟后: 14, 10分钟后: 15, 12分钟后: 16, 15分钟后: 17, 20分钟后: 18, 25分钟后: 19, 30分钟后: 20, 45分钟后: 21, 1小时后: 22
    u32 lpwr_warning_vbat                : 3;   //低电语音提示电压: 不低电提示: 0, 3.0V: 1, 3.1V: 2, 3.2V: 3, 3.3V: 4, 3.4V: 5, 3.5V: 6, 3.6V: 7
    u32 lpwr_off_vbat                    : 3;   //低电关机电压: 不关机: 0, 2.9V: 1, 3.0V: 2, 3.1V: 3, 3.2V: 4, 3.3V: 5, 3.4V: 6, 3.5V: 7
    u32 lpwr_warning_period              : 4;   //低电语音播报周期: 5秒: 0, 10秒: 1, 15秒: 2, 20秒: 3, 25秒: 4, 30秒: 5, 45秒: 6, 1分钟: 7, 2分钟: 8, 3分钟: 9, 4分钟: 10, 5分钟: 11, 6分钟: 12, 7分钟: 13, 8分钟: 14, 10分钟: 15
    u32 lwrpwr_counter_limit             : 3;   //低电播报几次后关机: None: 0, 1次: 1, 2次: 2, 3次: 3, 5次: 4, 8次: 5, 10次: 6, 15次: 7
    u32 sys_init_vol                     : 5;   //开机默认音量
    u32 sys_recover_initvol              : 5;   //恢复默认音量阈值
    u32 warning_volume                   : 5;   //提示音播放音量
    u32 tone_sysvol_en                   : 1;   //提示音跟随系统音量
    u32 pwron_press_time                 : 3;   //软开机长按时间选择: 0.1秒: 0, 0.5秒: 1, 1秒: 2, 1.5秒: 3, 2秒: 4, 2.5秒: 5, 3秒: 6, 3.5秒: 7
    u32 pwroff_press_time                : 3;   //软关机长按时间选择: 1.5秒: 0, 2秒: 1, 2.5秒: 2, 3秒: 3, 3.5秒: 4, 4秒: 5, 4.5秒: 6, 5秒: 7
    u32 func_aux_en                      : 1;   //AUX功能
    u32 lt_sa_en                         : 1;   //SYS_RESV0
    u32 lt_chg_en                        : 1;   //SYS_RESV1
    u32 tone_config_en                   : 1;   //配置提示音
    u32 eq_en                            : 1;   //是否使能EQ
    u32 eq_dgb_spp_en                    : 1;   //EQ调试（蓝牙串口）
    u32 eq_dgb_uart_en                   : 1;   //EQ调试（UART）
    u32 eq_uart_sel                      : 2;   //EQ调试串口选择: VUSB: 0, PA7: 1, PB3: 2, PB4: 3
    u32 pwrkey_2_hw_pwron                : 1;   //PWRKEY模拟硬开关
    u32 motor_io_sel                     : 3;   //马达控制IO选择: None: 0, PA6: 1, PA7: 2, PB3: 3, PB4: 4, PE0: 5, PE7: 6, PF2: 7
    u32 motor_time_cfg_en                : 1;   //马达振动时间配置
    u32 atmos_led_io_sel                 : 3;   //氛围灯控制IO选择: None: 0, PA6: 1, PA7: 2, PB3: 3, PB4: 4, PE0: 5, PE7: 6, PF2: 7
    u32 atmos_led_default_on             : 1;   //氛围灯上电默认亮
    u32 uart0_printf_en                  : 1;   //打开PB3调试信息
    u32 e2p_mem_en                       : 1;   //eeprom记忆
    u32 flash_en                         : 1;   //Flash方案
    u32 sys_ntc_en                       : 1;   //打开NTC功能
    u32 ntc_user_inner_pu                : 1;   //NTC使用内部10K上拉
    u32 ntc_adc_ch                       : 3;   //NTC的ADC通路选择: ADCCH_PA6: 1, ADCCH_PA7: 2, ADCCH_PB3: 3, ADCCH_PB4: 4, ADCCH_PE7: 5, ADCCH_PF2: 6
    u32 ntc_thd_val                      : 8;   //NTC关机的ADC阈值
    u32 vbat_peak_filter_en              : 1;   //电池电量检测PEAK滤波
    u32 buck_mode_en                     : 1;   //BUCK MODE
    u32 vddio_capless_en                 : 1;   //VDDIO省电容
    u32 vddbt_capless_en                 : 1;   //VDDBT省电容
    u32 vbat_capless_en                  : 1;   //VBAT省电容
    u32 vddio_sel                        : 2;   //VDDIO电压: 3.0V: 0, 3.1V: 1, 3.2V: 2, 3.3V: 3
    u32 vddcore_sel                      : 3;   //VDDCORE电压: 0.925V: 0, 0.95V: 1, 0.975V: 2, 1.0V: 3, 1.025: 4, 1.05V: 5, 1.075V: 6, 1.10V: 7
    u32 vddbt_sel                        : 4;   //VDDBT电压: 1.2V: 7, 1.25V: 8, 1.3V: 9, 1.35V: 10, 1.4V: 11, 1.45V: 12, 1.5V: 13, 1.55V: 14, 1.6V: 15
    u32 lvd_sel                          : 2;   //LVD电压: 1.8V: 0, 2.0V: 1, 2.2V: 2, 2.4V: 3
    u32 buck_climit_dis                  : 1;   //BUCK限流关闭
    u32 buck_freq_sel                    : 3;   //BUCK频率
    u32 buck_nmos_sel                    : 2;   //BUCK NMOS驱动
    u32 buck_pmos_sel                    : 1;   //BUCK PMOS驱动
    u32 pll0_anl_trim_band_en            : 1;   //PLL模拟二分找BAND
    u32 dac_sel                          : 3;   //DAC声道选择: VCMBUF单声道: 0, VCMBUF双声道: 1, 差分单声道: 2, 差分双声道: 3, 单端单声道: 4, 单端双声道: 5
    u32 dac_ldoh_sel                     : 2;   //VDDPA电压: 1.25V: 0, 1.3V: 1, 1.35V: 2, 1.4V: 3
    u32 vddpa_bypass_en                  : 1;   //VDDPA BYPASS
    u32 vddmic_always_on                 : 1;   //VDDMIC电源使能
    u32 dac_pa_dcoc_dis                  : 1;   //DAC DIG DCOC关闭
    u32 dac_out_spr                      : 1;   //DAC输出采样率48K
    u32 dac_dnr_en                       : 1;   //DAC动态降噪
    u32 dac_channel_exchange_en          : 1;   //DAC左右声道互换
    u32 dacl_phase_inv_en                : 1;   //DAC左声道相位取反
    u32 dacr_phase_inv_en                : 1;   //DAC右声道相位取反
    u32 dac_off_for_bt_disconn           : 1;   //蓝牙未连接时关闭DAC
    u32 dac_max_gain                     : 3;   //DAC最大音量: 0DB: 0, -1DB: 1, -2DB: 2, -3DB: 3, -4DB: 4, -5DB: 5, -6DB: 6, -7DB: 7
    u32 bt_call_max_gain                 : 3;   //通话最大音量: 0DB: 0, -1DB: 1, -2DB: 2, -3DB: 3, -4DB: 4, -5DB: 5, -6DB: 6, -7DB: 7
    u32 dac_cic_gain                     : 5;   //DAC后级数字增益选择
    u32 piano_tone_vol                   : 4;   //PianoTone音量控制
    u32 charge_en                        : 1;   //充电使能
    u32 charge_dc_reset                  : 1;   //插入DC复位系统
    u32 charge_dc_not_pwron              : 1;   //插入DC禁止软开机
    u32 charge_limited_time              : 3;   //充电时间限制: 不限制: 0, 2小时: 1, 2.5小时: 2, 3小时: 3, 3.5小时: 4, 4小时: 5, 5小时: 6
    u32 charge_stop_volt                 : 3;   //充电截止电压: 4.08V: 0, 4.10V: 1, 4.13V: 2, 4.16V: 3, 4.2V: 4, 4.23V: 5, 4.26V: 6, 4.29V: 7
    u32 charge_stop_curr                 : 4;   //充电截止电流: 2.5mA: 1, 5mA: 2, 7.5mA: 3, 10mA: 4, 12.5mA: 5, 15mA: 6, 17.5mA: 7, 20mA: 8, 22.5mA: 9, 25mA: 10, 27.5mA: 11, 30mA: 12
    u32 charge_constant_curr             : 5;   //恒流充电电流: 5mA: 0, 10mA: 1, 15mA: 2, 20mA: 3, 25mA: 4, 30mA: 5, 35mA: 6, 40mA: 7, 45mA: 8, 50mA: 9, 55mA: 10, 60mA: 11, 65mA: 12, 70mA: 13, 75mA: 14, 80mA: 15, 90mA: 16, 100mA: 17, 110mA: 18, 120mA: 19, 130mA: 20, 140mA: 21, 150mA: 22, 160mA: 23, 170mA: 24, 180mA: 25, 190mA: 26, 200mA: 27
    u32 charge_trickle_curr              : 3;   //涓流充电电流: 5mA: 0, 10mA: 1, 15mA: 2, 20mA: 3, 25mA: 4, 30mA: 5, 35mA: 6, 40mA: 7
    u32 ch_stop_curr_thd                 : 2;   //充满时截止电流占比阈值: 15: 0, 30: 1, 45: 2, 60: 3
    u32 charge_lowpwr_en                 : 1;   //充电时进入低功耗模式
    u32 ch_full_auto_pwrdwn_en           : 1;   //充满电自动关机
    u32 bt_enter_charge_rst_en           : 1;   //耳机进入充电前先复位
    u32 chbox_en                         : 1;   //充电仓功能
    u32 ch_box_type_sel                  : 2;   //充电仓类型选择: 兼容5V短暂掉0V后维持电压: 0, 5V不掉电的充电仓: 1, 5V掉电但有维持电压: 2, 5V完全掉电无维持电压: 3
    u32 ch_inbox_sel                     : 1;   //仓允许最低维持电压: 1.7V: 0, 1.1V: 1
    u32 ch_leakage_sel                   : 3;   //入仓VUSB漏电配置: None: 0, 漏电电流90uA: 1, 漏电电流180uA: 2, 漏电电流270uA: 4, 漏电电流360uA: 5, 漏电电流450uA: 6, 漏电电流540uA: 7
    u32 ch_out_leakage_sel               : 2;   //出仓VUSB漏电配置: None: 0, 漏电电流90uA: 1, 漏电电流180uA: 2, 漏电电流270uA: 3
    u32 ch_out_auto_pwron_en             : 1;   //从充电仓拿出自动开机
    u32 bt_outbox_voice_pwron_en         : 1;   //耳机出仓播放开机提示音
    u32 chbox_out_delay                  : 4;   //短暂掉0V的仓稳定检测时间
    u32 chg_inbox_pwrdwn_en              : 1;   //入仓耳机关机
    u32 ch_w4_off_sel                    : 2;   //充满关机前等仓休眠的时间: None: 0, 40秒: 1, 1分钟: 2, 2分钟: 3
    u32 chbox_smart_en                   : 1;   //昇生微智能充电仓
    u32 bt_name_config_en                : 1;   //蓝牙名称配置
    u32 bt_addr_config_en                : 1;   //蓝牙地址配置
    u32 bt_name_suffix_en                : 1;   //蓝牙名称加编号后缀
    u32 bt_local_name_sel                : 5;   //预设蓝牙名称选择: Bluetooth music: 0, Bluetooth Music: 1, S6: 2, S530: 3, M165: 4, LT: 5, LT-AP: 6, I7-TWS: 7, I9-TWS: 8, TWS-i7: 9, I8-TWS: 10, i7s: 11, i9s: 12, i11: 13, I7: 14, X18: 15, I8X: 16, X8S: 17, i7: 18, I8: 19, I9: 20, i12: 21, i13: 22, i7S-TWS: 23, TWS: 24, Pro 4: 25, Pro 5: 26, A7S: 27, E7S: 28, W100: 29
    u32 bt_pwrkey_nsec_discover          : 3;   //长按几秒开机进配对状态: 不支持: 0, 2秒: 1, 3秒: 2, 4秒: 3, 5秒: 4, 6秒: 5, 7秒: 6, 8秒: 7
    u32 bt_pwrkey_nsec_clr_pair_en       : 1;   //长按N秒进配对后清配对信息
    u32 bt_init_fast_dis                 : 1;   //关闭蓝牙提前初始化
    u32 bt_pair_sleep_en                 : 1;   //未连接允许进休眠模式
    u32 bt_always_reconn                 : 1;   //是否支持一直回连手机
    u32 bt_tout_reconn_times             : 2;   //远距离断开后超时回连时间: 1分钟: 0, 2分钟: 1, 4分钟: 2, 一直回连: 3
    u32 warning_bt_pair                  : 1;   //配对提示音
    u32 bt_2acl_en                       : 1;   //连接两部手机功能
    u32 bt_2acl_auto_switch              : 1;   //点击播放自动切换手机
    u32 bt_a2dp_en                       : 1;   //音乐播放功能
    u32 bt_a2dp_delay_report_en          : 1;   //DELAY REPORT功能
    u32 bt_aac_audio_en                  : 1;   //AAC蓝牙音乐
    u32 bt_aac_bit_rate                  : 2;   //蓝牙AAC码率: 128kbit/s: 0, 165kbit/s: 1, 192kbit/s: 2, 不限制: 3
    u32 a2dp_normal_mode_latency         : 3;   //蓝牙音乐正常模式延时: 150ms: 1, 180ms: 2, 210ms: 3, 240ms: 4, 270ms: 5, 300ms: 0, 330ms: 6, 360ms: 7
    u32 a2dp_game_mode_latency           : 2;   //蓝牙音乐游戏模式延时: 80ms: 1, 100ms: 0, 120ms: 2, 140ms: 3
    u32 bt_sco_en                        : 1;   //通话功能
    u32 bt_sco_msbc_en                   : 1;   //mSBC高清通话
    u32 bt_hfp_private_en                : 1;   //强制私密接听功能
    u32 bt_hfp_private_switch_en         : 1;   //手机与耳机通话切换
    u32 bt_hfp_mute_switch_en            : 1;   //通话静音开启与关闭
    u32 bt_hfp_zero_vol_en               : 1;   //通话最小级音量时静音
    u32 bt_hfp_ring_number_en            : 1;   //来电报号功能
    u32 bt_hfp_rings_before_number       : 2;   //报号前先响几次铃声: 直接报号: 0, 先响1次: 1, 先响2次: 2, 先响3次: 3
    u32 bt_ring_ticks_sel                : 3;   //来电响铃间隔: 0.5秒: 0, 1.0秒: 1, 1.5秒: 2, 2.0秒: 3, 2.5秒: 4, 3.0秒: 5, 3.5秒: 6, 4.0秒: 7
    u32 bt_hfp_inband_ring_en            : 1;   //来电播放手机铃声
    u32 bt_hfp_bat_report_en             : 1;   //电量显示功能
    u32 bt_bat_non_linear_en             : 1;   //电量非线性变化
    u32 bt_vbat_max_volt                 : 3;   //电量计算的满电电压: 4.08V: 0, 4.10V: 1, 4.12V: 2, 4.14V: 3, 4.16V: 4, 4.18V: 5, 4.20V: 6, 4.22V: 7
    u32 bt_a2dp_vol_ctrl_en              : 1;   //手机音量同步功能
    u32 bt_hid_vol_ctrl_en               : 1;   //蓝牙HID方式音量同步
    u32 bt_hid_ios_only_en               : 1;   //HID音量同步不支持安卓
    u32 bt_vol_reset_for_discon_en       : 1;   //断开手机连接后恢复默认音量
    u32 bt_vol_ctrl_hmos_en              : 1;   //支持鸿蒙系统的音量同步
    u32 bt_autoplay_en                   : 1;   //蓝牙连接自动播放功能
    u32 bt_recon_play_dis                : 1;   //关闭断线回连恢复播放功能
    u32 bt_a2dp_avrcp_play_status        : 1;   //加快播放暂停响应
    u32 bt_spp_en                        : 1;   //SPP串口功能
    u32 bt_sco_dbg_en                    : 1;   //SPP调试通话参数
    u32 bt_tws_dbg_en                    : 1;   //SPP分析信号质量
    u32 bt_hid_en                        : 1;   //HID拍照功能
    u32 bt_hid_menu_en                   : 1;   //按键HID连接/断开功能
    u32 bt_hid_discon_default_en         : 1;   //HID默认不连接
    u32 bt_hid_mode_sel                  : 2;   //HID兼容模式: 耳机电量图标，兼容软键盘: 0, 耳机电量图标，兼容拍照: 1, 键盘图标，兼容拍照: 2
    u32 bt_hid_voldown_sel               : 2;   //双击VOL-功能选择: None: 0, 发送VOL+消息拍照: 1, 发送VOL-消息拍照: 2, HID服务连接/断开: 3
    u32 bt_hid_volup_sel                 : 2;   //双击VOL+功能选择: None: 0, 发送VOL+消息拍照: 1, 发送VOL-消息拍照: 2, HID服务连接/断开: 3
    u32 iphone_head_quick_test_en        : 1;   //蓝牙快速测试
    u32 bt_tws_en                        : 1;   //TWS功能
    u32 bt_tws_fast_con_en               : 1;   //TWS快速连接
    u32 bt_tws_limit_pair_time           : 4;   //TWS快配时间限制
    u32 bt_tws_public_addr               : 1;   //TWS配对生成新地址
    u32 bt_tws_single_pair               : 1;   //TWS单次配对功能
    u32 bt_tws_pair_mode                 : 3;   //TWS配对方式选择: 自动配对: 0, 快速连接(不回连): 1, 双击USER_DEF键配对: 2, 单击USER_DEF配对: 3, 长按USER_DEF配对: 4, 双击MODE/POWER键配对: 5
    u32 bt_tws_lr_mode                   : 3;   //TWS声道分配选择: 不分配，主副均双声道输出: 0, 自动分配，主右声道副左声道: 1, 自动分配，主左声道副右声道: 2, GPIOx接地为左声道: 3, 配置选择为左声道: 4, 配置选择为右声道: 5
    u32 tws_sel_left_gpio_sel            : 3;   //TWS声道GPIOx: None: 0, PA7: 2, PB3: 3, PB4: 4, PE0: 5, PE7: 6, PF2: 7
    u32 bt_tws_name_suffix_en            : 1;   //TWS蓝牙名加L或R后缀
    u32 bt_tws_lr_voice_sel              : 1;   //TWS连接成功提示音: 异步播声道提示音: 0, 同步播连接成功提示音: 1
    u32 bt_tws_sbc_bitpool               : 2;   //TWS SBC码率选择: 32: 2, 39: 0, 45: 1, 53: 3
    u32 bt_tswi_en                       : 1;   //TWS主从切换功能
    u32 bt_tswi_kpwr_en                  : 1;   //TWS按键关机支持主从切换
    u32 bt_tswi_msc_en                   : 1;   //TWS音乐模式支持主从切换
    u32 bt_tswi_sco_en                   : 1;   //TWS通话模式支持主从切换
    u32 bt_tswi_lowpwr_en                : 1;   //TWS低电关机支持主从切换
    u32 bt_tswi_vbat_en                  : 1;   //TWS耳机电量支持主从切换
    u32 bt_tws_voice_sync_en             : 1;   //TWS双边语音(提示音)
    u32 bt_tws_ext_nane_en               : 1;   //TWS配对匹配扩展字
    u32 bt_tws_ext_name                  : 16;  //TWS配对扩展字
    u32 qtest_en                         : 1;   //VUSB快测快配功能
    u32 qtest_dc5v_reset_en              : 1;   //VUSB快测连接后打开5V复位
    u32 qtest_ch_wr_otp_en               : 1;   //VUSB快配时固定声道
    u32 bt_resvm                         : 8;   //BT_RESVM
    u32 bt_resvp                         : 19;  //BT_RESVP
    u32 bb_dut_test_en                   : 1;   //正常连接模式使能DUT测试
    u32 bb_dut_test_k5s_en               : 1;   //五击按键进DUT模式
    u32 bb_dut_test_addr_fixed_en        : 1;   //DUT测试模式固定蓝牙地址
    u32 bb_dut_test_edr3m_en             : 1;   //DUT测试模式使能EDR 3M包
    u32 bb_bt_cry_fast_en                : 1;   //兼容CRY测试仪连接
    u32 bb_tx_crc_nak_dis                : 1;   //关闭TXNAK功能
    u32 bb_ext_wakeup_dis                : 1;   //关闭sleep外部唤醒功能
    u32 bt_ver_major_sel                 : 1;   //蓝牙版本号: 5.3: 0, 5.4: 1
    u32 bb_bt_t2r_delay                  : 4;   //T2R_DELAY
    u32 bb_bt_tx_pwrup                   : 8;   //TXPWRUP_DELAY
    u32 bb_bt_rx_pwrup                   : 8;   //RXPWRUP_DELAY
    u32 bb_nb_sync_err                   : 4;   //SYNC容错
    u32 bt_rf_dec_txpwr                  : 2;   //降低未连接时TX功率: 不降低: 0, 降低3dbm: 1, 降低6dbm: 2, 降低9dbm: 3
    u32 rf_param_sco_en                  : 1;   //通话RF参数
    u32 rf_mix_gain_sco                  : 3;   //通话MIX_GAIN0
    u32 rf_dig_gain_sco                  : 6;   //通话DIG_GAIN0
    u32 rf_pa_gain0                      : 3;   //PA_GAIN0
    u32 rf_mix_gain0                     : 3;   //MIX_GAIN0
    u32 rf_dig_gain0                     : 6;   //DIG_GAIN0
    u32 rf_captune_vl                    : 4;   //CAPTUNE_VL
    u32 rf_captune_vh                    : 4;   //CAPTUNE_VH
    u32 di_pa_vcasn2                     : 3;   //PA_VCASN2
    u32 di_pa_vcasn                      : 3;   //PA_VCASN
    u32 di_pa_nbias                      : 4;   //PA_NBIAS: 0: 0, 2: 1, 4: 2, 6: 3, 8: 4, 10: 5, 12: 6, 14: 7, 16: 8, 18: 9, 20: 10, 22: 11, 24: 12, 26: 13, 28: 14, 30: 15
    u32 di_pa_cc_tx                      : 4;   //PA_CC_TX
    u32 di_ldo_vddpa_vsel                : 3;   //LDO_VDDPA_VSEL
    u32 di_ldo_vddvco_vsel               : 3;   //LDO_VDDVCO_VSEL
    u32 di_ldo_vddloop_vsel              : 3;   //LDO_VDDLOOP_VSEL
    u32 di_sx_cp_current                 : 3;   //SX_CP_CURRENT: 0: 0, 2: 1, 4: 2, 6: 3, 8: 4, 10: 5, 12: 6, 14: 7
    u32 di_sx_cp_offset                  : 4;   //SX_CP_OFFSET: 0: 0, 2: 1, 4: 2, 6: 3, 8: 4, 10: 5, 12: 6, 14: 7, 16: 8, 18: 9, 20: 10, 22: 11, 24: 12, 26: 13, 28: 14, 30: 15
    u32 di_sx_vco_bias                   : 3;   //SX_VCO_BIAS: 0: 0, 2: 1, 4: 2, 6: 3, 8: 4, 10: 5, 12: 6, 14: 7
    u32 di_lna_bias_hg                   : 3;   //LNA_BIAS_HG: 1: 0, 3: 1, 5: 2, 7: 3, 9: 4, 11: 5, 13: 6, 15: 7
    u32 di_bq2_dcoc_bias                 : 1;   //BQ2_DCOC_BIAS: 1: 0, 0: 1
    u32 di_mix_gate_bias                 : 3;   //MIX_GATE_BIAS
    u32 le_gsg_dphi_den                  : 4;   //LE_GSG_DPHI_DEN
    u32 bt_gsg_dphi_den                  : 4;   //BT_GSG_DPHI_DEN
    u32 le_gsg_dphi_nom                  : 7;   //LE_GSG_DPHI_NOM
    u32 bt_gsg_dphi_nom                  : 7;   //BT_GSG_DPHI_NOM
    u32 agc_gain_tbl_sel                 : 2;   //AGC_GAIN表选择: 标准表: 0, 增强抗干扰表: 1, 自动切换: 2, 重烧回标准表: 3
    u32 agc_target_d                     : 3;   //AGC_TARGET: 22: 0, 24: 1, 26: 2, 28: 3, 30: 4, 32: 5, 34: 6, 36: 7
    u32 agc_high_gt                      : 2;   //AGC_HIGH_GT: 4: 0, 5: 1, 6: 2, 7: 3
    u32 agc_low_gt                       : 2;   //AGC_LOW_GT: 3: 0, 4: 1, 5: 2, 6: 3
    u32 ramp_down_step                   : 4;   //RAMPING_DOWN
    u32 ramp_up_step                     : 4;   //RAMPING_UP
    u32 mic_rmdc_sel                     : 2;   //MIC去直流Fliter配置: CutOff 30Hz: 0, CutOff 60Hz: 1, CutOff 120Hz: 2, CutOff 240Hz: 3
    u32 mic_bias_method                  : 2;   //MIC偏置电路配置: None: 0, MIC省电阻电容: 1, MIC只省电阻: 2
    u32 mic_bias_resistor                : 3;   //MIC内部偏置电阻大小
    u32 mic_eq_en                        : 1;   //打开MIC EQ
    u32 mic_eq_gain                      : 4;   //MIC EQ衰减: 0DB: 0, -1DB: 1, -2DB: 2, -3DB: 3, -4DB: 4, -5DB: 5, -6DB: 6, -7DB: 7, -8DB: 8, -9DB: 9, -10DB: 10, -11DB: 11, -12DB: 12, -13DB: 13, -14DB: 14, -15DB: 15
    u32 mic_eq_gain_1                    : 4;   //MIC EQ增益(200Hz): 0DB: 0, 1DB: 1, 2DB: 2, 3DB: 3, 4DB: 4, 5DB: 5, 6DB: 6, 7DB: 7, -1DB: 8, -2DB: 9, -3DB: 10, -4DB: 11, -5DB: 12, -6DB: 13, -7DB: 14, -8DB: 15
    u32 mic_eq_gain_2                    : 4;   //MIC EQ增益(600Hz): 0DB: 0, 1DB: 1, 2DB: 2, 3DB: 3, 4DB: 4, 5DB: 5, 6DB: 6, 7DB: 7, -1DB: 8, -2DB: 9, -3DB: 10, -4DB: 11, -5DB: 12, -6DB: 13, -7DB: 14, -8DB: 15
    u32 mic_eq_gain_3                    : 4;   //MIC EQ增益(1000Hz): 0DB: 0, 1DB: 1, 2DB: 2, 3DB: 3, 4DB: 4, 5DB: 5, 6DB: 6, 7DB: 7, -1DB: 8, -2DB: 9, -3DB: 10, -4DB: 11, -5DB: 12, -6DB: 13, -7DB: 14, -8DB: 15
    u32 mic_eq_gain_4                    : 4;   //MIC EQ增益(1600Hz): 0DB: 0, 1DB: 1, 2DB: 2, 3DB: 3, 4DB: 4, 5DB: 5, 6DB: 6, 7DB: 7, -1DB: 8, -2DB: 9, -3DB: 10, -4DB: 11, -5DB: 12, -6DB: 13, -7DB: 14, -8DB: 15
    u32 mic_eq_gain_5                    : 4;   //MIC EQ增益(2500Hz): 0DB: 0, 1DB: 1, 2DB: 2, 3DB: 3, 4DB: 4, 5DB: 5, 6DB: 6, 7DB: 7, -1DB: 8, -2DB: 9, -3DB: 10, -4DB: 11, -5DB: 12, -6DB: 13, -7DB: 14, -8DB: 15
    u32 mic_eq_gain_6                    : 4;   //MIC EQ增益(5000Hz): 0DB: 0, 1DB: 1, 2DB: 2, 3DB: 3, 4DB: 4, 5DB: 5, 6DB: 6, 7DB: 7, -1DB: 8, -2DB: 9, -3DB: 10, -4DB: 11, -5DB: 12, -6DB: 13, -7DB: 14, -8DB: 15
    u32 mic_eq_lowp                      : 2;   //MIC EQ 低通: 关闭低通: 0, 3000(3800)Hz: 1, 3400(4500)Hz: 2, 3800(7500)Hz: 3
    u32 mic_eq_highp                     : 2;   //MIC EQ 高通: 关闭高通: 0, 100Hz: 1, 150Hz: 2, 250Hz: 3
    u32 bt_anl_gain                      : 4;   //MIC模拟增益: 2 (3DB): 2, 3 (6DB): 3, 4 (9DB): 4, 5 (12DB): 5, 6 (15DB): 6, 7 (18DB): 7, 8 (21DB): 8, 9 (24DB): 9, 10 (27DB): 10, 11 (30DB): 11, 12 (33DB): 12, 13 (36DB): 13, 14 (39DB): 14, 15 (42DB): 15
    u32 bt_dig_gain                      : 6;   //通话MIC数字增益
    u32 mic_post_gain                    : 3;   //通话MIC后置数字增益
    u32 bt_sco_nr_en                     : 1;   //DNN近端降噪
    u32 bt_sco_ains3_en                  : 1;   //AINS3近端降噪
    u32 bt_sco_nr_level                  : 5;   //近端降噪级别
    u32 bt_sco_nr_trumpet_en             : 1;   //近端降噪去喇叭声
    u32 bt_ains3_ps_rate                 : 4;   //AINS3非平稳噪声抑制能力
    u32 bt_ains3_low_fre_range           : 2;   //AINS3低频的噪声谱: 7: 0, 1: 1, 3: 2, 10: 3
    u32 bt_sco_nr_min_range_sel          : 2;   //DNN噪声谱选择: 噪声谱0: 0, 噪声谱1: 1, 噪声谱2: 2, 噪声谱3: 3
    u32 bt_sco_nr_wind_en                : 1;   //DNN降噪去风声
    u32 bt_sco_nr_wind_det_thr           : 2;   //风噪检测阈值
    u32 bt_sco_nr_wind_level             : 2;   //风噪降噪强度: 3000: 0, 4800: 1, 6500: 2, 8000: 3
    u32 bt_sco_far_nr_en                 : 1;   //远端降噪功能
    u32 bt_sco_far_nr_level              : 4;   //远端降噪级别
    u32 bt_alc_en                        : 1;   //ALC功能
    u32 bt_aec_en                        : 1;   //AEC功能
    u32 bt_aec_nlp_sel                   : 2;   //AEC的NLP模式选择: nlp模式1 (推荐): 0, nlp模式0 (最强): 1, nlp模式2 (最弱): 2, nlp模式1 (重写项): 3
    u32 bt_echo_level                    : 3;   //AEC回声消除级别
    u32 bt_far_offset                    : 8;   //AEC远端补偿值
    u32 bt_sco_dump_cfg                  : 2;   //通话DUMP数据功能: 关闭: 0, 近端降噪输入及输出: 1, AEC输入及输出: 2, 远端降噪输入及输出: 3
    u32 eq_gain                          : 4;   //EQ衰减: 0DB: 0, -1DB: 1, -2DB: 2, -3DB: 3, -4DB: 4, -5DB: 5, -6DB: 6, -7DB: 7, -8DB: 8, -9DB: 9, -10DB: 10, -11DB: 11, -12DB: 12, -13DB: 13, -14DB: 14, -15DB: 15
    u32 eq_gain_1                        : 4;   //EQ增益1（31Hz）: 0DB: 0, 1DB: 1, 2DB: 2, 3DB: 3, 4DB: 4, 5DB: 5, 6DB: 6, 7DB: 7, -1DB: 8, -2DB: 9, -3DB: 10, -4DB: 11, -5DB: 12, -6DB: 13, -7DB: 14, -8DB: 15
    u32 eq_gain_2                        : 4;   //EQ增益2（62Hz）: 0DB: 0, 1DB: 1, 2DB: 2, 3DB: 3, 4DB: 4, 5DB: 5, 6DB: 6, 7DB: 7, -1DB: 8, -2DB: 9, -3DB: 10, -4DB: 11, -5DB: 12, -6DB: 13, -7DB: 14, -8DB: 15
    u32 eq_gain_3                        : 4;   //EQ增益3（125Hz）: 0DB: 0, 1DB: 1, 2DB: 2, 3DB: 3, 4DB: 4, 5DB: 5, 6DB: 6, 7DB: 7, -1DB: 8, -2DB: 9, -3DB: 10, -4DB: 11, -5DB: 12, -6DB: 13, -7DB: 14, -8DB: 15
    u32 eq_gain_4                        : 4;   //EQ增益4（250Hz）: 0DB: 0, 1DB: 1, 2DB: 2, 3DB: 3, 4DB: 4, 5DB: 5, 6DB: 6, 7DB: 7, -1DB: 8, -2DB: 9, -3DB: 10, -4DB: 11, -5DB: 12, -6DB: 13, -7DB: 14, -8DB: 15
    u32 eq_gain_5                        : 4;   //EQ增益5（500Hz）: 0DB: 0, 1DB: 1, 2DB: 2, 3DB: 3, 4DB: 4, 5DB: 5, 6DB: 6, 7DB: 7, -1DB: 8, -2DB: 9, -3DB: 10, -4DB: 11, -5DB: 12, -6DB: 13, -7DB: 14, -8DB: 15
    u32 eq_gain_6                        : 4;   //EQ增益6（1000Hz）: 0DB: 0, 1DB: 1, 2DB: 2, 3DB: 3, 4DB: 4, 5DB: 5, 6DB: 6, 7DB: 7, -1DB: 8, -2DB: 9, -3DB: 10, -4DB: 11, -5DB: 12, -6DB: 13, -7DB: 14, -8DB: 15
    u32 eq_gain_7                        : 4;   //EQ增益7（2000Hz）: 0DB: 0, 1DB: 1, 2DB: 2, 3DB: 3, 4DB: 4, 5DB: 5, 6DB: 6, 7DB: 7, -1DB: 8, -2DB: 9, -3DB: 10, -4DB: 11, -5DB: 12, -6DB: 13, -7DB: 14, -8DB: 15
    u32 eq_gain_8                        : 4;   //EQ增益8（4000Hz）: 0DB: 0, 1DB: 1, 2DB: 2, 3DB: 3, 4DB: 4, 5DB: 5, 6DB: 6, 7DB: 7, -1DB: 8, -2DB: 9, -3DB: 10, -4DB: 11, -5DB: 12, -6DB: 13, -7DB: 14, -8DB: 15
    u32 eq_gain_9                        : 4;   //EQ增益9（8000Hz）: 0DB: 0, 1DB: 1, 2DB: 2, 3DB: 3, 4DB: 4, 5DB: 5, 6DB: 6, 7DB: 7, -1DB: 8, -2DB: 9, -3DB: 10, -4DB: 11, -5DB: 12, -6DB: 13, -7DB: 14, -8DB: 15
    u32 eq_gain_10                       : 4;   //EQ增益10（16000Hz）: 0DB: 0, 1DB: 1, 2DB: 2, 3DB: 3, 4DB: 4, 5DB: 5, 6DB: 6, 7DB: 7, -1DB: 8, -2DB: 9, -3DB: 10, -4DB: 11, -5DB: 12, -6DB: 13, -7DB: 14, -8DB: 15
    u32 v_power_sel                      : 2;   //开机/关机提示音: 无提示音: 0, TONE音: 1, 语音: 2, 和弦音: 3
    u32 v_connect_sel                    : 2;   //蓝牙连接/断开提示音: 无提示音: 0, TONE音: 1, 语音: 2, 和弦音: 3
    u32 v_pairing_sel                    : 2;   //进入配对提示音: 无提示音: 0, TONE音: 1, 语音: 2
    u32 v_channel_sel                    : 2;   //左/右声道提示音: 无提示音: 0, TONE音: 1, 语音: 2
    u32 v_lang_switch_sel                : 2;   //语言切换提示音: 无提示音: 0, TONE音: 1, 语音: 2
    u32 v_lowbattery_sel                 : 2;   //电池低电提示音: 无提示音: 0, TONE音: 1, 和弦音: 3
    u32 v_game_mode_sel                  : 1;   //游戏/音乐模式提示音: TONE音: 0, 语音: 1
    u32 v_redialing_sel                  : 1;   //回拔/拒接/挂断提示音: 无提示音: 0, TONE音: 1
    u32 v_maxvol_sel                     : 1;   //最大音量提示音: 无提示音: 0, TONE音: 1
    u32 v_minvol_sel                     : 1;   //最小音量提示音: 无提示音: 0, TONE音: 1
    u32 v_ring_sel                       : 1;   //来电响铃提示音: 无提示音: 0, TONE音: 1
    u32 v_cameraon_sel                   : 1;   //开启/关闭拍照提示音: 无提示音: 0, TONE音: 1
    u32 v_take_photo_sel                 : 1;   //拍照提示音: 无提示音: 0, TONE音: 1
    u32 v_btmode_sel                     : 1;   //蓝牙模式提示音: 无提示音: 0, TONE音: 1
    u32 v_auxmode_sel                    : 1;   //AUX模式提示音: 无提示音: 0, TONE音: 1
    u32 tone_lightning_en                : 1;   //各状态Tone音相同
    u32 tone_maxvol_sel                  : 2;   //最大音量Tone音选择: Tone音0: 0, Tone音1: 1, Tone音2: 2, Tone音3: 3
    u32 tone_minvol_sel                  : 2;   //最小音量Tone音选择: Tone音0: 0, Tone音1: 1, Tone音2: 2, Tone音3: 3
    u32 user_tkey_debug_en               : 1;   //内置触摸SPP调试
    u32 user_tkey                        : 1;   //内置触摸按键功能
    u32 user_tkey_soft_pwr_en            : 1;   //内置触摸软开关机
    u32 user_tkey_wakeup_dis             : 1;   //触摸按键仅需按键关机
    u32 user_tkey_alarm_dis              : 1;   //内置触摸定时唤醒关闭
    u32 tk_avg_exp_en                    : 1;   //触摸平均值差值算法
    u32 tkey_slide_gpio_sel              : 2;   //内置触摸滑动功能GPIO: None: 0, PA7: 1, PB3: 2, PB4: 3
    u32 tkey_slide_interval_min_sel      : 2;   //内置触摸滑动功能按下最小间隔选择: 15ms: 0, 20ms: 1, 25ms: 2, 30ms: 3
    u32 tkey_slide_msg_sel               : 1;   //内置触摸滑动功能定义: 上滑音量加: 0, 上滑音量减: 1
    u32 tkey_cdpr                        : 4;   //触摸按键CDPR参数
    u32 tkey_fil_high                    : 3;   //触摸按键FILTER参数
    u32 tk_charge_current                : 5;   //触摸充电电流
    u32 tkpthresh                        : 4;   //TKPTHRESH参数
    u32 tkrthresh                        : 4;   //TKRTHRESH参数
    u32 user_tkey_inear                  : 1;   //内置触摸入耳检测
    u32 te_charge_current                : 5;   //入耳充电电流
    u32 te_cdpr                          : 4;   //入耳CDPR参数
    u32 tepthresh                        : 4;   //TEPTHRESH参数
    u32 terthresh                        : 4;   //TERTHRESH参数
    u32 earin_gpio_sel                   : 3;   //入耳检测: None: 0, PA6: 1, PA7(支持内置触摸): 2, PB3(支持内置触摸): 3, PB4(支持内置触摸): 4, PE0: 5, PE7: 6, PF2: 7
    u32 bt_earin_usage_pp_en             : 1;   //PP控制的通用入耳功能
    u32 user_pwrkey_en                   : 1;   //PWRKEY功能
    u32 user_iokey_en                    : 1;   //IOKEY功能
    u32 user_adkey_en                    : 1;   //ADKEY功能
    u32 user_adkey_pu10k_en              : 1;   //ADKEY使用内部10K上拉
    u32 adkey_ch                         : 3;   //选择ADKEY的Channel: ADCCH_PA6: 1, ADCCH_PA7: 2, ADCCH_PB3: 3, ADCCH_PB4: 4, ADCCH_PE7: 5, ADCCH_PF2: 6
    u32 kl_time_sel                      : 2;   //按键长按时间选择: 0.8秒: 0, 1.4秒: 1, 2秒: 2, 2.6秒: 3
    u32 double_key_time                  : 3;   //多击响应时间选择: 200ms: 0, 300ms: 1, 400ms: 2, 500ms: 3, 600ms: 4, 700ms: 5, 800ms: 6, 900ms: 7
    u32 user_def_ks_sel                  : 5;   //USER_DEF键单按: 无: 0, 回拨: 1, SIRI: 2, PREV: 3, NEXT: 4, VOL+: 5, VOL-: 6, SIRI/回拨: 7, HID拍照: 8, IOS HOME: 9, 中英文切换: 10, 氛围灯开/关: 11, PLAY/PAUSE: 12, NR切换: 13, POPUP开/关: 14, Game切换: 15, MODE: 16
    u32 user_def_kl_sel                  : 5;   //USER_DEF键长按: 无: 0, 回拨: 1, SIRI: 2, PREV: 3, NEXT: 4, VOL+: 5, VOL-: 6, SIRI/回拨: 7, HID拍照: 8, IOS HOME: 9, 中英文切换: 10, 氛围灯开/关: 11, PLAY/PAUSE: 12, NR切换: 13, POPUP开/关: 14, Game切换: 15, MODE: 16
    u32 user_def_kd_sel                  : 5;   //USER_DEF键双击: 无: 0, 回拨: 1, SIRI: 2, PREV: 3, NEXT: 4, VOL+: 5, VOL-: 6, SIRI/回拨: 7, HID拍照: 8, IOS HOME: 9, 中英文切换: 10, 氛围灯开/关: 11, PLAY/PAUSE: 12, NR切换: 13, POPUP开/关: 14, Game切换: 15, MODE: 16
    u32 user_def_kt_sel                  : 5;   //USER_DEF键三击: 无: 0, 回拨: 1, SIRI: 2, PREV: 3, NEXT: 4, VOL+: 5, VOL-: 6, SIRI/回拨: 7, HID拍照: 8, IOS HOME: 9, 中英文切换: 10, 氛围灯开/关: 11, PLAY/PAUSE: 12, NR切换: 13, POPUP开/关: 14, Game切换: 15, MODE: 16
    u32 user_def_kfour_sel               : 5;   //USER_DEF键四击: 无: 0, 回拨: 1, SIRI: 2, PREV: 3, NEXT: 4, VOL+: 5, VOL-: 6, SIRI/回拨: 7, HID拍照: 8, IOS HOME: 9, 中英文切换: 10, 氛围灯开/关: 11, PLAY/PAUSE: 12, NR切换: 13, POPUP开/关: 14, Game切换: 15, MODE: 16
    u32 user_def_kfive_sel               : 5;   //USER_DEF键五击: 无: 0, 回拨: 1, SIRI: 2, PREV: 3, NEXT: 4, VOL+: 5, VOL-: 6, SIRI/回拨: 7, HID拍照: 8, IOS HOME: 9, 中英文切换: 10, 氛围灯开/关: 11, PLAY/PAUSE: 12, NR切换: 13, POPUP开/关: 14, Game切换: 15, MODE: 16
    u32 user_def_kl_tone_en              : 1;   //USER_DEF键长按按键音
    u32 user_def_kd_tone_en              : 1;   //USER_DEF键双击按键音
    u32 user_def_kt_tone_en              : 1;   //USER_DEF键三击按键音
    u32 user_def_kfour_tone_en           : 1;   //USER_DEF键四击按键音
    u32 user_def_kd_lang_en              : 1;   //USER_DEF键双击切语言
    u32 user_def_kfour_clr_pair_en       : 1;   //USER_DEF键四击清配对
    u32 user_def_lr_en                   : 1;   //USER_DEF键区分左右耳
    u32 user_def_lr_default_r            : 1;   //单耳默认为右耳功能
    u32 user_def_kl_reject_en            : 1;   //USER_DEF键长按来电拒接
    u32 user_def_kd_reject_en            : 1;   //USER_DEF键双击来电拒接
    u32 user_def_kl_call_pwrdwn_en       : 1;   //USER_DEF键允许通话关机
    u32 kl_pwrdwn_dis                    : 1;   //关闭按键长按关机功能
    u32 pwrkey_config_en                 : 1;   //PWRKEY按键定制
    u32 pwrkey_num0                      : 5;   //PWRKEY按键1功能: None: 0, P/P: 1, PWR: 2, NEXT/VOL+: 3, PREV/VOL-: 4, NEXT/VOL-: 5, PREV/VOL+: 6, VOL+/NEXT: 7, VOL-/PREV: 8, VOL-/NEXT: 9, VOL+/PREV: 10, VOL+: 11, VOL-: 12, NEXT: 13, PREV: 14, MODE: 15, HSF: 16, MODE/PWR: 17, SIRI: 18, HOME: 19, PP/PWR/USER_DEF: 20, PP/USER_DEF: 21, VOL+/VOL-: 22
    u32 pwrkey_num1                      : 5;   //PWRKEY按键2功能: None: 0, P/P: 1, PWR: 2, NEXT/VOL+: 3, PREV/VOL-: 4, NEXT/VOL-: 5, PREV/VOL+: 6, VOL+/NEXT: 7, VOL-/PREV: 8, VOL-/NEXT: 9, VOL+/PREV: 10, VOL+: 11, VOL-: 12, NEXT: 13, PREV: 14, MODE: 15, HSF: 16, MODE/PWR: 17, SIRI: 18, HOME: 19, PP/PWR/USER_DEF: 20, PP/USER_DEF: 21, VOL+/VOL-: 22
    u32 pwrkey_num2                      : 5;   //PWRKEY按键3功能: None: 0, P/P: 1, PWR: 2, NEXT/VOL+: 3, PREV/VOL-: 4, NEXT/VOL-: 5, PREV/VOL+: 6, VOL+/NEXT: 7, VOL-/PREV: 8, VOL-/NEXT: 9, VOL+/PREV: 10, VOL+: 11, VOL-: 12, NEXT: 13, PREV: 14, MODE: 15, HSF: 16, MODE/PWR: 17, SIRI: 18, HOME: 19, PP/PWR/USER_DEF: 20, PP/USER_DEF: 21, VOL+/VOL-: 22
    u32 pwrkey_num3                      : 5;   //PWRKEY按键4功能: None: 0, P/P: 1, PWR: 2, NEXT/VOL+: 3, PREV/VOL-: 4, NEXT/VOL-: 5, PREV/VOL+: 6, VOL+/NEXT: 7, VOL-/PREV: 8, VOL-/NEXT: 9, VOL+/PREV: 10, VOL+: 11, VOL-: 12, NEXT: 13, PREV: 14, MODE: 15, HSF: 16, MODE/PWR: 17, SIRI: 18, HOME: 19, PP/PWR/USER_DEF: 20, PP/USER_DEF: 21, VOL+/VOL-: 22
    u32 pwrkey_num4                      : 5;   //PWRKEY按键5功能: None: 0, P/P: 1, PWR: 2, NEXT/VOL+: 3, PREV/VOL-: 4, NEXT/VOL-: 5, PREV/VOL+: 6, VOL+/NEXT: 7, VOL-/PREV: 8, VOL-/NEXT: 9, VOL+/PREV: 10, VOL+: 11, VOL-: 12, NEXT: 13, PREV: 14, MODE: 15, HSF: 16, MODE/PWR: 17, SIRI: 18, HOME: 19, PP/PWR/USER_DEF: 20, PP/USER_DEF: 21, VOL+/VOL-: 22
    u32 iokey_io0                        : 3;   //IOKEY按键1选择IO: None: 0, PA6: 1, PA7: 2, PB3: 3, PB4: 4, PE0: 5, PE7: 6, PF2: 7
    u32 iokey_io1                        : 3;   //IOKEY按键2选择IO: None: 0, PA6: 1, PA7: 2, PB3: 3, PB4: 4, PE0: 5, PE7: 6, PF2: 7
    u32 iokey_num0                       : 5;   //IOKEY按键1功能: None: 0, P/P: 1, PWR: 2, NEXT/VOL+: 3, PREV/VOL-: 4, NEXT/VOL-: 5, PREV/VOL+: 6, VOL+/NEXT: 7, VOL-/PREV: 8, VOL-/NEXT: 9, VOL+/PREV: 10, VOL+: 11, VOL-: 12, NEXT: 13, PREV: 14, MODE: 15, HSF: 16, MODE/PWR: 17, SIRI: 18, HOME: 19, PP/PWR/USER_DEF: 20, PP/USER_DEF: 21, VOL+/VOL-: 22
    u32 iokey_num1                       : 5;   //IOKEY按键2功能: None: 0, P/P: 1, PWR: 2, NEXT/VOL+: 3, PREV/VOL-: 4, NEXT/VOL-: 5, PREV/VOL+: 6, VOL+/NEXT: 7, VOL-/PREV: 8, VOL-/NEXT: 9, VOL+/PREV: 10, VOL+: 11, VOL-: 12, NEXT: 13, PREV: 14, MODE: 15, HSF: 16, MODE/PWR: 17, SIRI: 18, HOME: 19, PP/PWR/USER_DEF: 20, PP/USER_DEF: 21, VOL+/VOL-: 22
    u32 iokey_num2                       : 5;   //两IO中间的按键功能: None: 0, P/P: 1, PWR: 2, NEXT/VOL+: 3, PREV/VOL-: 4, NEXT/VOL-: 5, PREV/VOL+: 6, VOL+/NEXT: 7, VOL-/PREV: 8, VOL-/NEXT: 9, VOL+/PREV: 10, VOL+: 11, VOL-: 12, NEXT: 13, PREV: 14, MODE: 15, HSF: 16, MODE/PWR: 17, SIRI: 18, HOME: 19, PP/PWR/USER_DEF: 20, PP/USER_DEF: 21, VOL+/VOL-: 22
    u32 adkey_config_en                  : 1;   //ADKEY按键定制
    u32 adkey_adcv_config_en             : 1;   //ADKEY的ADC值自定义
    u32 adkey_num0                       : 5;   //ADKEY按键1功能: None: 0, P/P: 1, PWR: 2, NEXT/VOL+: 3, PREV/VOL-: 4, NEXT/VOL-: 5, PREV/VOL+: 6, VOL+/NEXT: 7, VOL-/PREV: 8, VOL-/NEXT: 9, VOL+/PREV: 10, VOL+: 11, VOL-: 12, NEXT: 13, PREV: 14, MODE: 15, HSF: 16, MODE/PWR: 17, SIRI: 18, HOME: 19, PP/PWR/USER_DEF: 20, PP/USER_DEF: 21, VOL+/VOL-: 22
    u32 adkey_num1                       : 5;   //ADKEY按键2功能: None: 0, P/P: 1, PWR: 2, NEXT/VOL+: 3, PREV/VOL-: 4, NEXT/VOL-: 5, PREV/VOL+: 6, VOL+/NEXT: 7, VOL-/PREV: 8, VOL-/NEXT: 9, VOL+/PREV: 10, VOL+: 11, VOL-: 12, NEXT: 13, PREV: 14, MODE: 15, HSF: 16, MODE/PWR: 17, SIRI: 18, HOME: 19, PP/PWR/USER_DEF: 20, PP/USER_DEF: 21, VOL+/VOL-: 22
    u32 adkey_num2                       : 5;   //ADKEY按键3功能: None: 0, P/P: 1, PWR: 2, NEXT/VOL+: 3, PREV/VOL-: 4, NEXT/VOL-: 5, PREV/VOL+: 6, VOL+/NEXT: 7, VOL-/PREV: 8, VOL-/NEXT: 9, VOL+/PREV: 10, VOL+: 11, VOL-: 12, NEXT: 13, PREV: 14, MODE: 15, HSF: 16, MODE/PWR: 17, SIRI: 18, HOME: 19, PP/PWR/USER_DEF: 20, PP/USER_DEF: 21, VOL+/VOL-: 22
    u32 adkey_num3                       : 5;   //ADKEY按键4功能: None: 0, P/P: 1, PWR: 2, NEXT/VOL+: 3, PREV/VOL-: 4, NEXT/VOL-: 5, PREV/VOL+: 6, VOL+/NEXT: 7, VOL-/PREV: 8, VOL-/NEXT: 9, VOL+/PREV: 10, VOL+: 11, VOL-: 12, NEXT: 13, PREV: 14, MODE: 15, HSF: 16, MODE/PWR: 17, SIRI: 18, HOME: 19, PP/PWR/USER_DEF: 20, PP/USER_DEF: 21, VOL+/VOL-: 22
    u32 adkey_num4                       : 5;   //ADKEY按键5功能: None: 0, P/P: 1, PWR: 2, NEXT/VOL+: 3, PREV/VOL-: 4, NEXT/VOL-: 5, PREV/VOL+: 6, VOL+/NEXT: 7, VOL-/PREV: 8, VOL-/NEXT: 9, VOL+/PREV: 10, VOL+: 11, VOL-: 12, NEXT: 13, PREV: 14, MODE: 15, HSF: 16, MODE/PWR: 17, SIRI: 18, HOME: 19, PP/PWR/USER_DEF: 20, PP/USER_DEF: 21, VOL+/VOL-: 22
    u32 adkey_num5                       : 5;   //ADKEY按键6功能: None: 0, P/P: 1, PWR: 2, NEXT/VOL+: 3, PREV/VOL-: 4, NEXT/VOL-: 5, PREV/VOL+: 6, VOL+/NEXT: 7, VOL-/PREV: 8, VOL-/NEXT: 9, VOL+/PREV: 10, VOL+: 11, VOL-: 12, NEXT: 13, PREV: 14, MODE: 15, HSF: 16, MODE/PWR: 17, SIRI: 18, HOME: 19, PP/PWR/USER_DEF: 20, PP/USER_DEF: 21, VOL+/VOL-: 22
    u32 adcv_num0                        : 8;   //ADKEY按键1的ADC值
    u32 adcv_num1                        : 8;   //ADKEY按键2的ADC值
    u32 adcv_num2                        : 8;   //ADKEY按键3的ADC值
    u32 linein_2_pwrdown_en              : 1;   //插入LINEIN关机
    u32 linein_2_pwrdown_tone_en         : 1;   //插入LINEIN关机提示音
    u32 mode_2_aux_en                    : 1;   //模式切换进AUX模式
    u32 aux_anl_gain                     : 2;   //AUX模拟增益: +3DB: 0, 0DB: 1, -6DB: 2, -12DB: 3
    u32 auxl_sel                         : 2;   //AUXL通路选择: 无AUXL输入: 0, AUXL_PB3: 1, AUXL_PE7: 2, AUXL_PA7: 3
    u32 linein_det_io                    : 3;   //选择LINEIN检测IO: None: 0, PA7: 1, PB3: 2, PB4: 3, PE0: 4, PE7: 5, PF2: 6, 复用PWRKEY检测: 7
    u32 motor_pwron_sel                  : 2;   //开机振动时间: 无振动: 0, 500ms: 1, 1秒: 2, 2秒: 3
    u32 motor_pwroff_sel                 : 2;   //关机振动时间: 无振动: 0, 500ms: 1, 1秒: 2, 2秒: 3
    u32 motor_conn_sel                   : 2;   //蓝牙连接振动时间: 无振动: 0, 500ms: 1, 1秒: 2, 2秒: 3
    u32 motor_disconn_sel                : 2;   //蓝牙断开振动时间: 无振动: 0, 500ms: 1, 1秒: 2, 2秒: 3
    u32 motor_ring_sel                   : 2;   //响铃振动时间: 无振动: 0, 500ms: 1, 1秒: 2, 2秒: 3
    u32 led_bled_en                      : 1;   //系统指示灯(蓝灯)
    u32 led_rled_en                      : 1;   //电源状态灯(红灯)
    u32 port_2led_resless_en             : 1;   //省电阻1个IO推两个灯
    u32 bled_io_sel                      : 3;   //蓝灯IO选择: None: 0, PA7: 2, PB3: 3, PB4: 4, PE0: 5, PE7: 6, PF2: 7, MICIN: 1
    u32 rled_io_sel                      : 3;   //红灯IO选择: None: 0, PA7: 2, PB3: 3, PB4: 4, PE0: 5, PE7: 6, PF2: 7, DACL: 1
    u32 led_sta_config_en                : 1;   //自定义LED闪灯状态
    u32 led_on_bre_only_en               : 1;   //呼吸灯仅亮灯过程缓变
    u32 led_charge_xcfg                  : 4;   //充电状态闪灯: 红蓝灯全灭: 0, 红蓝灯全亮: 1, 红灯亮: 2, 红灯只亮2秒: 3, 蓝灯亮: 4, 蓝灯只亮2秒: 5, 红蓝同时呼吸灯模式: 11, 红蓝交替呼吸灯模式: 12, 蓝灯呼吸灯模式: 13, 红灯呼吸灯模式: 14, 无LED灯显示效果: 15
    u32 led_charge_fully_xcfg            : 3;   //充满电状态蓝灯亮: 不亮蓝灯: 0, 充满进低功耗后亮蓝灯: 1, 亮20秒: 2, 亮30秒: 3, 1分钟: 4, 2分钟: 5, 3分钟: 6, 常亮: 7
    u32 led_aux_play_sel                 : 2;   //AUX播放闪灯选择: 常灭: 0, 亮50ms灭5秒: 1, 亮1秒灭1秒: 2, 常亮: 3
    u32 led_aux_pause_sel                : 2;   //AUX暂停闪灯选择: 常灭: 0, 亮50ms灭5秒: 1, 亮1秒灭1秒: 2, 常亮: 3
    u32 led_bt_sleep_sel                 : 2;   //蓝牙休眠闪灯选择: 常灭: 0, 亮50ms灭5秒: 1, 亮1秒灭1秒: 2, 常亮: 3
    u32 led_pwron_xcfg                   : 5;   //开机状态: 红蓝灯全灭: 0, 红蓝灯全亮: 1, 蓝灯亮: 2, 蓝灯慢闪(1秒周期): 3, 蓝灯快闪(300ms周期): 4, 蓝灯5秒闪烁1下: 5, 蓝灯5秒连闪2下: 6, 蓝灯5秒连闪3下: 7, 蓝灯闪烁1下(500ms): 8, 蓝灯闪烁1下(1秒): 9, 蓝灯闪烁2下: 10, 蓝灯闪烁3下: 11, 红灯亮: 12, 红灯慢闪(1秒周期): 13, 红灯快闪(300ms周期): 14, 红灯5秒闪烁1下: 15, 红灯5秒连闪2下: 16, 红灯5秒连闪3下: 17, 红灯闪烁1下(500ms): 18, 红灯闪烁1下(1秒): 19, 红灯闪烁2下: 20, 红灯闪烁3下: 21, 红蓝交替闪(快闪): 22, 红蓝交替闪(慢闪): 23, 红蓝灯同时闪烁1下: 24, 红蓝灯同时闪烁2下: 25, 红蓝灯同时闪烁3下: 26, 无LED灯显示效果: 31
    u32 led_pwroff_xcfg                  : 5;   //关机状态: 红蓝灯全灭: 0, 红蓝灯全亮: 1, 蓝灯亮: 2, 蓝灯慢闪(1秒周期): 3, 蓝灯快闪(300ms周期): 4, 蓝灯5秒闪烁1下: 5, 蓝灯5秒连闪2下: 6, 蓝灯5秒连闪3下: 7, 蓝灯闪烁1下(500ms): 8, 蓝灯闪烁1下(1秒): 9, 蓝灯闪烁2下: 10, 蓝灯闪烁3下: 11, 红灯亮: 12, 红灯慢闪(1秒周期): 13, 红灯快闪(300ms周期): 14, 红灯5秒闪烁1下: 15, 红灯5秒连闪2下: 16, 红灯5秒连闪3下: 17, 红灯闪烁1下(500ms): 18, 红灯闪烁1下(1秒): 19, 红灯闪烁2下: 20, 红灯闪烁3下: 21, 红蓝交替闪(快闪): 22, 红蓝交替闪(慢闪): 23, 红蓝灯同时闪烁1下: 24, 红蓝灯同时闪烁2下: 25, 红蓝灯同时闪烁3下: 26, 无LED灯显示效果: 31
    u32 led_reconn_xcfg                  : 5;   //回连状态: 红蓝灯全灭: 0, 红蓝灯全亮: 1, 蓝灯亮: 2, 蓝灯慢闪(1秒周期): 3, 蓝灯快闪(300ms周期): 4, 蓝灯5秒闪烁1下: 5, 蓝灯5秒连闪2下: 6, 蓝灯5秒连闪3下: 7, 蓝灯闪烁1下(500ms): 8, 蓝灯闪烁1下(1秒): 9, 蓝灯闪烁2下: 10, 蓝灯闪烁3下: 11, 红灯亮: 12, 红灯慢闪(1秒周期): 13, 红灯快闪(300ms周期): 14, 红灯5秒闪烁1下: 15, 红灯5秒连闪2下: 16, 红灯5秒连闪3下: 17, 红灯闪烁1下(500ms): 18, 红灯闪烁1下(1秒): 19, 红灯闪烁2下: 20, 红灯闪烁3下: 21, 红蓝交替闪(快闪): 22, 红蓝交替闪(慢闪): 23, 红蓝灯同时闪烁1下: 24, 红蓝灯同时闪烁2下: 25, 红蓝灯同时闪烁3下: 26, 无LED灯显示效果: 31
    u32 led_pair_xcfg                    : 5;   //配对状态: 红蓝灯全灭: 0, 红蓝灯全亮: 1, 蓝灯亮: 2, 蓝灯慢闪(1秒周期): 3, 蓝灯快闪(300ms周期): 4, 蓝灯5秒闪烁1下: 5, 蓝灯5秒连闪2下: 6, 蓝灯5秒连闪3下: 7, 蓝灯闪烁1下(500ms): 8, 蓝灯闪烁1下(1秒): 9, 蓝灯闪烁2下: 10, 蓝灯闪烁3下: 11, 红灯亮: 12, 红灯慢闪(1秒周期): 13, 红灯快闪(300ms周期): 14, 红灯5秒闪烁1下: 15, 红灯5秒连闪2下: 16, 红灯5秒连闪3下: 17, 红灯闪烁1下(500ms): 18, 红灯闪烁1下(1秒): 19, 红灯闪烁2下: 20, 红灯闪烁3下: 21, 红蓝交替闪(快闪): 22, 红蓝交替闪(慢闪): 23, 红蓝灯同时闪烁1下: 24, 红蓝灯同时闪烁2下: 25, 红蓝灯同时闪烁3下: 26, 无LED灯显示效果: 31
    u32 led_conn_xcfg                    : 5;   //连接状态: 红蓝灯全灭: 0, 红蓝灯全亮: 1, 蓝灯亮: 2, 蓝灯慢闪(1秒周期): 3, 蓝灯快闪(300ms周期): 4, 蓝灯5秒闪烁1下: 5, 蓝灯5秒连闪2下: 6, 蓝灯5秒连闪3下: 7, 蓝灯闪烁1下(500ms): 8, 蓝灯闪烁1下(1秒): 9, 蓝灯闪烁2下: 10, 蓝灯闪烁3下: 11, 红灯亮: 12, 红灯慢闪(1秒周期): 13, 红灯快闪(300ms周期): 14, 红灯5秒闪烁1下: 15, 红灯5秒连闪2下: 16, 红灯5秒连闪3下: 17, 红灯闪烁1下(500ms): 18, 红灯闪烁1下(1秒): 19, 红灯闪烁2下: 20, 红灯闪烁3下: 21, 红蓝交替闪(快闪): 22, 红蓝交替闪(慢闪): 23, 红蓝灯同时闪烁1下: 24, 红蓝灯同时闪烁2下: 25, 红蓝灯同时闪烁3下: 26, 红蓝同时呼吸灯模式: 27, 红蓝交替呼吸灯模式: 28, 蓝灯呼吸灯模式: 29, 红灯呼吸灯模式: 30, 无LED灯显示效果: 31
    u32 led_play_xcfg                    : 5;   //播放状态: 红蓝灯全灭: 0, 红蓝灯全亮: 1, 蓝灯亮: 2, 蓝灯慢闪(1秒周期): 3, 蓝灯快闪(300ms周期): 4, 蓝灯5秒闪烁1下: 5, 蓝灯5秒连闪2下: 6, 蓝灯5秒连闪3下: 7, 蓝灯闪烁1下(500ms): 8, 蓝灯闪烁1下(1秒): 9, 蓝灯闪烁2下: 10, 蓝灯闪烁3下: 11, 红灯亮: 12, 红灯慢闪(1秒周期): 13, 红灯快闪(300ms周期): 14, 红灯5秒闪烁1下: 15, 红灯5秒连闪2下: 16, 红灯5秒连闪3下: 17, 红灯闪烁1下(500ms): 18, 红灯闪烁1下(1秒): 19, 红灯闪烁2下: 20, 红灯闪烁3下: 21, 红蓝交替闪(快闪): 22, 红蓝交替闪(慢闪): 23, 红蓝灯同时闪烁1下: 24, 红蓝灯同时闪烁2下: 25, 红蓝灯同时闪烁3下: 26, 红蓝同时呼吸灯模式: 27, 红蓝交替呼吸灯模式: 28, 蓝灯呼吸灯模式: 29, 红灯呼吸灯模式: 30, 无LED灯显示效果: 31
    u32 led_lowbat_xcfg                  : 5;   //低电状态: 保持灯状态: 0, 红蓝灯全亮: 1, 蓝灯亮: 2, 蓝灯慢闪(1秒周期): 3, 蓝灯快闪(300ms周期): 4, 蓝灯5秒闪烁1下: 5, 蓝灯5秒连闪2下: 6, 蓝灯5秒连闪3下: 7, 蓝灯闪烁1下(500ms): 8, 蓝灯闪烁1下(1秒): 9, 蓝灯闪烁2下: 10, 蓝灯闪烁3下: 11, 红灯亮: 12, 红灯慢闪(1秒周期): 13, 红灯快闪(300ms周期): 14, 红灯5秒闪烁1下: 15, 红灯5秒连闪2下: 16, 红灯5秒连闪3下: 17, 红灯闪烁1下(500ms): 18, 红灯闪烁1下(1秒): 19, 红灯闪烁2下: 20, 红灯闪烁3下: 21, 红蓝交替闪(快闪): 22, 红蓝交替闪(慢闪): 23, 红蓝灯同时闪烁1下: 24, 红蓝灯同时闪烁2下: 25, 红蓝灯同时闪烁3下: 26, 无LED灯显示效果: 31
    struct __attribute__((packed)) {
        u8 redpat;
        u8 bluepat;
        u8 unit;
        u8 cycle;
    } led_poweron;                              //开机闪灯控制
    struct __attribute__((packed)) {
        u8 redpat;
        u8 bluepat;
        u8 unit;
        u8 cycle;
    } led_poweroff;                             //关机闪灯控制
    struct __attribute__((packed)) {
        u8 redpat;
        u8 bluepat;
        u8 unit;
        u8 cycle;
    } led_reconnect;                            //回连状态闪灯控制
    struct __attribute__((packed)) {
        u8 redpat;
        u8 bluepat;
        u8 unit;
        u8 cycle;
    } led_pairing;                              //配对状态闪灯控制
    struct __attribute__((packed)) {
        u8 redpat;
        u8 bluepat;
        u8 unit;
        u8 cycle;
    } led_connected;                            //已连接闪灯控制
    struct __attribute__((packed)) {
        u8 redpat;
        u8 bluepat;
        u8 unit;
        u8 cycle;
    } led_playing;                              //播放闪灯控制
    struct __attribute__((packed)) {
        u8 redpat;
        u8 bluepat;
        u8 unit;
        u8 cycle;
    } led_ring;                                 //来电闪灯控制
    struct __attribute__((packed)) {
        u8 redpat;
        u8 bluepat;
        u8 unit;
        u8 cycle;
    } led_calling;                              //通话闪灯控制
    struct __attribute__((packed)) {
        u8 redpat;
        u8 bluepat;
        u8 unit;
        u8 cycle;
    } led_lowbat;                               //低电闪灯控制
    u8 bt_addr[6];                              //蓝牙地址
    u8 xm_keep_start[0];                        //For Keep Area Start
    u8 osc_cap;                                 //产测OSCI电容
    u8 xm_keep_end[0];                          //For Keep Area End
    char bt_name[32];                           //蓝牙名称 (32字符)
    u32 user_cap                         : 1;   //不使用产测电容值
    u32 uosc_cap                         : 6;   //自定义OSCI电容
    u32 uosc_both_cap                    : 1;   //自定义OSC基础电容: 0PF: 0, 6PF: 1
    u32 anc_en                           : 1;   //ANC功能
    u32 anc_gain_en                      : 1;   //ANC MIC Gain产测配置
    u32 bt_fcc_en                        : 1;   //FCC测试功能
    u32 bt_fcc_io                        : 2;   //FCC通信IO: PB3: 0, PA7: 1, PF2: 2, VUSB: 3
} xcfg_cb_t;

extern xcfg_cb_t xcfg_cb;
#endif
