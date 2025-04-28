#ifndef _BSP_SYS_H
#define _BSP_SYS_H

#define SW_RST_DC_IN            1
#define SW_QTEST_NORMAL         2
#define SW_QTEST_ONLY_PAIR      3
#define SW_RST_BT_MDM           4
#define SW_RST_FLAG             5                   //直接复位系统

#define VOL_MAX                 sys_cb.vol_max      //最大音量级数

///请勿修改sys_cb_t结构体
typedef struct {
    u8  vol;
    u8  vol_set;
    u8  hfp_vol;                //通话音量(范围: 0~15)
    u8  eq_mode;
    u8  lang_id;
    s8  gain_offset;            //用于动态修改音量表
    u8  hfp2sys_mul;            //系统音量与HFP音量倍数，手机HFP音量只有16级。
    u8  lpwr_warning_limit_cnt; //低电播报N次自动关机控制
    u8  lpwr_leave_vbat;        //离开低电的电压差
    u16 lpwr_warning_cnt;       //低电播放周期计数
    u16 lpwr_cnt;               //低电关机消抖计数
    u16 vol_max;
    u16 kh_vol_msg;
    u16 vbat;
    u32 sleep_time;
    u32 pwroff_time;
    u32 sleep_delay;
    u32 pwroff_delay;
    u32 sleep_wakeup_time;
    u32 ms_ticks;               //ms为单位
    u32 rand_seed;
    u8 sleep_counter;           //sleepmode counter计数
    u8 loc_house_state;         //bit0=is_open, bit1=key_state
    u8 rem_house_bat;
    u8 rem_bat;
    u8 rem_house_state;         //bit0=is_open, bit1=key_state
    u8 loc_house_bat;
    u8 loc_bat;
    u8 bt_tws_lr_mode;
    u8 tws_force_channel;       //TWS固定声道标志: 0->自动分配, 1->固定右声道, 2->固定左声道
    u8 rem_force_channel;
    u8 vusb_force_channel;      //VUSB固定声道
    u8 qtest_force_channel;
    u8 name_suffix_en;          //TWS名称是否增加后缀名了
    u8 tws_res_brk;
    u8 house_idx;
    u8 sw_rst_flag;             //软件复位标志
    u8 inbox_pwrdwn_flag;
	u8 outbox_pwron_flag;       //出仓开机标志
	u8 inbox_wken_flag;         //inbox wakeup
	u8 inbox_sta;               //是否在仓内
	s8 nr_sta;
    u8 nr_bits;
    u8 ear_menu_en;
	u8 rem_ear_sta;             //bit0=in_the_ear, bit1=in_the_house
    u8 loc_ear_sta;
    u8 popup_bat_hide;
    u8 popup_3bat_upd_en;
    u8 menu_det_dis;
    u8 inear_sta;
    u8 wko_pwrkey_en;
    u8 tkey_pwrdwn_en;
    u16 temp_tecnt;
    u32 te_tick;
    u8 lowpwr_dwn_flag;
    u8 discon_reason;           //0=单独关机, 1=同步关机
    u8 dac_sta;
volatile u8  dc_rst_flag;
volatile u8  cm_times;
volatile u8  loudspeaker_mute;  //功放MUTE标志
volatile u8  pwrkey_5s_check;   //是否进行开机长按5S的PWRKEY检测
volatile u8  pwrkey_5s_flag;    //长按5秒PWRKEY开机标志
volatile u8  charge_sta;        //0:充电关闭， 1：充电开启， 2：充满
volatile u8  charge_full_bled_flag; //charge充满蓝灯常亮标志
volatile u8  ch_bled_cnt;       //charge充满蓝灯亮时间计数
volatile u8  poweron_flag;      //pwrkey开机标志
volatile u8  pwrdwn_tone_en;    //POWER DOWN是否播放提示音
volatile u8  pwrdwn_hw_flag;    //模拟硬开关，关机标志
volatile u8  hall_2_pwrdwn_flag;
volatile u8  ntc_2_pwrdwn_flag;
volatile u8  breathe_led_sta;   //呼吸灯状态
volatile u8  qtest_flag;
volatile u8  incall_flag;
    u8 qtest_ack;
    u32 pwroff_tick;

    u8 warning_playing;
    u8 sleep_aupll_en;
    u8 ntc_adc_ch;
    u8 adkey_ch;
    u8 anc_start;
    u8 anc_user_mode;           //anc模式, 0:关闭, 1:ANC, 2:通透
    s8 anc_adjust_val;          //anc mic调整值
    u8 tp_adjust_val;           //tp anc mic调整值
	u32 anc_mic_gain;           //anc mic初始值
	u8 pdn_boat_flag;           //船运模式关机
	u8 sleep_dac_en;            //sleep休眠时是否保持DAC开启
	u8 unmute_rled_on;          //充电状态红灯亮, 功放解MUTE。
	u8 rled_mux_mute_en;        //红灯复用MUTE脚使能
	u8 chstop_pwrwn_en;         //软关机时关掉充电

    ///位变量只能在Main线程修改！！！
    u8 rtc_first_pwron  : 1,   //RTC是否第一次上电
       cm_factory       : 1,   //是否第一次FLASH上电
       cm_vol_change    : 1,   //音量级数是否需要更新到FLASH
       cm_inited        : 1,   //cm_buf是否已经初始化
       bthid_mode       : 1,   //独立HID拍照模式标志
       sleep_en         : 1,   //用于控制是否进入sleep
       clr_counter_en   : 1;   //连接中清自动关机计时

} sys_cb_t;
extern sys_cb_t sys_cb;

///sys_cb1_t结构体可修改
typedef struct {
    u8  a2dp_vol_delay_set;
    u8  va_start;
#if BT_TWS_CHARGE_DELAY_EN
	u8  dc_in_cnt;
	u32 charge_ticks;
#endif
#if CHARGE_TSEN_DETECT
    u16 adc_tsen_val;           //adc采集内部温度检测tsen值
    u8  trick_curr_bkp;
    u8  const_curr_bkp;
#endif
#if BT_A2DP_DEFAULT_VOLUME_EN
    u8  a2dp_default_volume;     //设置连上手机的默认音乐音量
    u32 a2dp_default_volume_ticks;     //超时清flag
#if BT_A2DP_IOS_DEFAULT_VOLUME_FIX
    u32 delay_send_vol_ticks;
#endif
#endif
    u8 tws_get_map_time;
    u8 dec_eq_enable;              //解密eq和dbb功能使能
#if USER_PWM_LED_EN
    u8 pwm_bled_on;
    u8 pwm_rled_on;
    u8 pwm_led_sta;
#endif
#if BT_HFP_CALL_EARPHONE_EN
    u8  call_switch;              //控制手机端接听后转移到耳机，后续切换不影响
    u8  call_need_switch;
    u32 call_switch_ticks;
#endif
#if CHARGE_NTC_DETECT
    u8  chg_ntc_stop;
#endif
#if BT_DISP_LOW_VBAT_EN
    u32 disp_low_vbat_ticks;
#endif
#if BT_TWS_WARNING_SLAVE_BYPASS_FIX
    u8  warning_is_playing;
#endif
#if CHARGE_DELAY_OPEN_LEAKAGE_EN
	u16 inbox_cnt_on;
#endif
} sys_cb1_t;
extern sys_cb1_t sys_cb1;

extern volatile u32 ticks_50ms;
extern uint lt_sdq_valid;

void bsp_sys_init(void);
void bsp_update_init(void);
void timer1_irq_init(void);
void bsp_sys_mute(void);
void bsp_sys_unmute(void);
void set_buck_mode(u8 buck_en);
void bsp_loudspeaker_mute(void);
void bsp_loudspeaker_unmute(void);
void linein_detect(void);
bool linein_micl_is_online(void);
void x26m_cap_tune(void);
void sw_reset_kick(u8 source);

void lt_uart_tx_auth_start(void);
void lt_uart_tx_auth_process(void);
void lt_var_init(void);
void lt_uart_wait_done(void);
void lt_charge_var_init(void);

void lt_charge_poweron(void);
void lt_charge_process(void);
void lt_charge_init(void);
void dongle_check_key(void);

#endif // _BSP_SYS_H

