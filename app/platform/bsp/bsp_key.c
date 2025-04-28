#include "include.h"

#define VBG_VOLTAGE             sys_trim.vbg_volt

extern adc_cb_t adc_cb;
extern key_cb_t key_cb;

extern u8 pwrkey_num[6];
extern u8 adkey_num[8];
extern u8 iokey_num[4];
extern u8 iokey_gpio[4];
extern adkey_tbl_t adkey_tbuf[7];
//按键多击处理
typedef struct {
    u8  strikes;
    u8  counter;
    u16 val;
} key_multi_cb_t;
extern key_multi_cb_t key_multi_cb;

typedef struct {
    u16 vbat_max[2];
    u16 cnt;
    u16 max_cnt;
    u16 vbat_ret;
} vbat_filter_cb_t;
extern vbat_filter_cb_t vbat_filter_cb;

bool get_adc_val(void);
u16 get_vbat_val(void);
u8 bsp_tkey_scan(void);
bool check_key_return(u16 key_return);
void dkey_mask_bits_init(void);
bool mic_bias_trim_is_en(void);
u8 get_adkey(void);
u16 bsp_key_process(u16 key_val);
bool bsp_key_pwron_filter(u16 key_val);
u16 key_slide_press_process(u16 key_val);
void bsp_key_wko10s_reset(void);
bool is_poweron_first_bat_insert(u32 rtccon9);
u8 bsp_tkey_slide_press_scan(void);
void bsp_188led_disp_set_on(void);
void vbat_peek_filter(u32 *vbat);
u16 get_vbat_val_m(void);
u16 bsp_tkey_slide_process(u16 key_val);
u16 tkey_short_slide_process(u16 key_val);
u8 bsp_short_slide_tkey_scan(void);

AT(.com_rodata.multi.key)
const u16 key_multi_tbl[4] = {
    KEY_DOUBLE, KEY_THREE, KEY_FOUR, KEY_FIVE
};

AT(.com_rodata.key.table)
const key_filter_t key_filter_tbl = {
    .scan_cnt = KEY_SCAN_TIMES,
    .up_cnt   = KEY_UP_TIMES,
    .long_cnt = KEY_LONG_TIMES,
    .hold_cnt = KEY_LONG_HOLD_TIMES,
};
#if VBAT_DETECT_10BIT_EN
adc_cb1_t adc_cb1;
AT(.com_text.port.vbat)
u16 get_vbat_val2(void)
{
    static u16 vbat_bak = 0;
    u32 vbat = (u32)adc_cb1.vbat2 * VBG_VOLTAGE / adc_cb1.bg;
    if (xcfg_cb.vbat_peak_filter_en) {
        vbat_peek_filter(&vbat);
    }
    adc_cb1.vbat_total = adc_cb1.vbat_total - adc_cb1.vbat_val + vbat; //均值
    adc_cb1.vbat_val = adc_cb1.vbat_total>>5;
    if(adc_cb1.vbat_val > vbat_bak) {
        vbat = adc_cb1.vbat_val - vbat_bak;
    } else {
        vbat = vbat_bak - adc_cb1.vbat_val;
    }
    if (adc_cb1.vbat_val >= 30) {   //偏差大于一定值则更新
        vbat_bak = adc_cb1.vbat_val;
        //printf(bat_str, adc_cb.vbat_val/1000, adc_cb.vbat_val%1000);
    }

#if FPGA_EN
    if (vbat_bak < 2000) {
        return 4200;    //4.2v
    }
#endif // FPGA_EN

    return vbat_bak;
}
#endif
AT(.com_text.port.vbat)
u16 get_vbat_val(void)
{
#if LED_188LED_DISP_EN
    static u16 vbat_bak = 0;
    u32 vbat = (u32)adc_cb.vbat2 * VBG_VOLTAGE / adc_cb.bg;
    if (xcfg_cb.vbat_peak_filter_en) {
        vbat_peek_filter(&vbat);
    }
    adc_cb.vbat_total = adc_cb.vbat_total - adc_cb.vbat_val + vbat; //均值
    adc_cb.vbat_val = adc_cb.vbat_total>>5;

    if(adc_cb.vbat_val > vbat_bak) {
        vbat = adc_cb.vbat_val - vbat_bak;
    } else {
        vbat = vbat_bak - adc_cb.vbat_val;
    }
    if (vbat >= 10) {   //偏差大于一定值则更新
        vbat_bak = adc_cb.vbat_val;
        //printf(bat_str, adc_cb.vbat_val/1000, adc_cb.vbat_val%1000);
    }

#if FPGA_EN
    if (vbat_bak < 2000) {
        return 4200;    //4.2v
    }
#endif // FPGA_EN

    return vbat_bak;

#else
#if VBAT_DETECT_10BIT_EN
     return get_vbat_val2();
#else
    return get_vbat_val_m();
#endif
#endif
}

////返回值 1：低电关机， 2：低电语音播报， 0：不动作
//int is_lowpower_vbat_warning(void)
//{
//    u16 lpwr_warning_vbat = (u16)LPWR_WARNING_VBAT*100 + 2900;
//
//    if (sys_cb.vbat <= ((u16)LPWR_OFF_VBAT*100+2800)) {
//        if (LPWR_OFF_VBAT) {
//            if (!sys_cb.lpwr_cnt) {
//                sys_cb.lpwr_cnt = 1;                            //低电关机计数
//            } else if (sys_cb.lpwr_cnt >= 10) {
//                return 1;                                       //VBAT低电关机
//            }
//        }
//        return 0;                                               //VBAT低电不关机
//    } else {
//        sys_cb.lpwr_cnt = 0;
//#if WARNING_LOW_BATTERY
//        if (sys_cb.vbat < lpwr_warning_vbat) {
//    #if RLED_LOWBAT_EN
//            if (sys_led.lowbat_led_en) {
//                if (!CHARGE_DC_IN()) {
//                    rled_lowbat();
//                } else {
//                    rled_lowbat_recover();
//                }
//            }
//    #endif
//            if (LPWR_WARNING_VBAT) {
//                return 2;                                       //低电压提示音播报
//            }
//        } else if (sys_cb.vbat > (lpwr_warning_vbat + (u16)sys_cb.lpwr_leave_vbat * 50)) {   //电池离开低电报警电压0.10V以上, 关闭低电的显示状态
//            sys_cb.lpwr_warning_limit_cnt = 0;
//    #if RLED_LOWBAT_EN
//            if (sys_led.lowbat_led_en) {
//                rled_lowbat_recover();
//            }
//    #endif
//        }
//#endif // WARNING_LOW_BATTERY
//        return 0;
//    }
//}

//充电过程中假关机处理
AT(.text.bsp.power)
u8 power_off_check(void)
{
    u32 pwron_press_nms;
    int pwrkey_pressed_flag, ticks = 0, up_cnt = 0;
    u8 restart_chk_en = 1;

    pwrkey_pressed_flag = 0;
    pwron_press_nms = PWRON_PRESS_TIME;
    if (pwron_press_nms == 0) {
        pwron_press_nms = 15;                                           //bootloader 80ms + 15ms, 最小开机时间在100ms左右
    }

    //要等PWRKEY开关释放后再次按下才能重新开机, 否则充电过程中5分钟关机, 低电关机等异常
    if ((PWRKEY_2_HW_PWRON) && (sys_cb.poweron_flag)) {
        restart_chk_en = 0;
        sys_cb.poweron_flag = 0;
    }

    while (1) {
        WDT_CLR();
        delay_ms(5);
        if ((bsp_key_pwr_scan() & K_PWR_MASK) == K_PWR) {
            up_cnt = 0;
            if (restart_chk_en) {
                if (!pwrkey_pressed_flag) {
                    ticks = tick_get();
                    pwrkey_pressed_flag = 1;
                    sys_cb.ms_ticks = tick_get();                                       //记录PWRKEY按键按下的时刻
                    sys_cb.pwrkey_5s_check = 1;
                }
                if (!sys_cb.poweron_flag) {
                    if (tick_check_expire(ticks, pwron_press_nms)) {                    //长按开机时间配置
                        sys_cb.poweron_flag = 1;
                        sys_cb.pwrdwn_hw_flag = 0;                                      //清PWRKEY硬开关的关机标志
                        sys_cb.hall_2_pwrdwn_flag = 0;                                  //清霍尔传感器的关机标志
                    }
                }
            }
        } else {
            if (up_cnt < 3) {
                up_cnt++;
            }
            if (up_cnt == 3) {
                up_cnt = 10;
                sys_cb.poweron_flag = 0;
                pwrkey_pressed_flag = 0;
                restart_chk_en = 1;
            }
        }

        if (sys_cb.poweron_flag) {
            if ((CHARGE_DC_NOT_PWRON) && CHARGE_DC_IN()) {
                continue;
            }
#if LINEIN_2_PWRDOWN_EN
            if ((xcfg_cb.linein_2_pwrdown_en) && (device_is_online(DEV_LINEIN))) {
                continue;
            }
#endif // LINEIN_2_PWRDOWN_EN

            //长按PP/POWER开机
//            gui_display(DISP_POWERON);
            led_power_up();
            dac_restart();
            bsp_change_volume(sys_cb.vol);
#if WARNING_POWER_ON
            sys_warning_play(T_WARNING_POWER_ON, PIANO_POWER_ON);
#endif // WARNING_POWER_ON
            func_cb.sta = FUNC_BT;
            return 0;
        } else {
            if (CHARGE_DC_IN()) {                                   //DC IN = 1
                continue;
            } else {
                return 1;                                           //VUSB拔出关机
            }
        }
    }
}


AT(.text.bsp.power)
static void power_on_check_do(void)
{
    u8 chbox_sta = 1, chbox_out2pwr_en = 0;
    int pwrkey_pressed_flag = 0;
    u32 pwron_press_nms;
    u32 rtccon9 = RTCCON9;                                              //wakeup pending
    u32 lvdcon = LVDCON;
    u8 key_val;

    RTCCON9 = 0xfff;                                                    //Clr pending

#if CHARGE_PULL_OUT_FILTER_EN
    //因为没有时钟，所以RTCCON10 BIT(3)置上之后，清的操作就没有释放，导致DC_IN无法正确识别
    //由于5V复位功能打开，如果打开时钟后，去置RTCCON10 BIT(3)，就会导致在5V插着的状态下，重新识别一次5V插入导致复位，所以不能清除该BIT(3)
    RTCCON0 |= BIT(0);
    RTCCON10 = 0xff7;                                                   //Clr pending
#else
    RTCCON10 = 0xfff;                                                   //Clr pending
#endif

    RTCCON1 &= ~BIT(6);                                                 //wko pin low level wakeup
    CRSTPND = 0x1ff0000;                                                //clear reset pending
    LVDCON &= ~(0x1f << 8);                                             //clear software reset

    if (lvdcon & BIT(19)) {
        if (IS_PWRKEY_PRESS()) {
            sys_cb.poweron_flag = 1;
        }
        return;                                                         //长按PWRKEY 10S复位后直接开机。
    }

    if ((sys_cb.sw_rst_flag == SW_RST_FLAG) ||
#if VUSB_TBOX_QTEST_EN
         get_qtest_mode()												//下面是charge低功耗的初始化，所以qtest需要返回
#endif
    ){
        return;
    }

    if ((rtccon9 & BIT(4)) && (xcfg_cb.ch_out_auto_pwron_en)) {         //charge inbox wakeup直接开机
        if (bsp_charge_outbox_stable_check()) {
            sys_cb.outbox_pwron_flag = 1;
            //printf("inbox wakeup\n");
            return;
        }
    }

#if USER_PWRKEY
    int up_cnt = 0, ticks = 0;
    if (IS_PWRKEY_PRESS() || IS_TKEY_PRESS()) {                          //PWRKEY是否按下
        pwrkey_pressed_flag = 1;
        ticks = sys_cb.ms_ticks;
    }
#endif // USER_PWRKEY
    pwron_press_nms = PWRON_PRESS_TIME;
    if (pwron_press_nms == 0) {
        pwron_press_nms = 15;                                           //bootloader 80ms + 15ms, 最小开机时间在100ms左右
    }

    //是否充电复位决定是否要拿起开机
    if ((lvdcon & BIT(17)) || (rtccon9 & BIT(3)) || (sys_cb.sw_rst_flag == SW_RST_DC_IN) || (sys_cb.inbox_wken_flag)) {
        chbox_out2pwr_en = xcfg_cb.ch_out_auto_pwron_en;
        CRSTPND = BIT(17);
    }
    bsp_charge_box_enter(chbox_out2pwr_en);

    //第一次上电是否直接开机
    if (is_poweron_first_bat_insert(rtccon9) || (sys_cb.sw_rst_flag == SW_RST_BT_MDM)
#if VUSB_TBOX_QTEST_EN
        || get_qtest_mode()
#endif
        ) {
        bsp_charge_box_exit();
        return;
    }

    while (1) {
        WDT_CLR();
        delay_ms(5);
        key_val = key_scan();
        bsp_key_process(key_val);
        if ((bsp_key_pwr_scan() & K_PWR_MASK) == K_PWR) {
            up_cnt = 0;
            if (!pwrkey_pressed_flag) {
                ticks = tick_get();
                sys_cb.ms_ticks = ticks;                                //记录PWRKEY按键按下的时刻
                pwrkey_pressed_flag = 1;
            }
            if (!sys_cb.poweron_flag) {
                if (tick_check_expire(ticks, pwron_press_nms)) {        //长按开机时间配置
                    sys_cb.poweron_flag = 1;
                }
            }
        } else {
            if (up_cnt < 3) {
                up_cnt++;
            }
            if (up_cnt == 3) {
                up_cnt = 10;
                pwrkey_pressed_flag = 0;
                sys_cb.poweron_flag = 0;
            }
        }

#if LINEIN_2_PWRDOWN_EN
        linein_detect();
#endif // LINEIN_2_PWRDOWN_EN

#if VBAT_DETECT_EN
        sys_cb.vbat = get_vbat_val();
#endif // VBAT_DETECT_EN

#if VUSB_TBOX_QTEST_EN
        if(sys_cb.qtest_flag){
          if(bsp_qtest_other_usage()){
              break;
          }
        }
#endif

#if CHARGE_EN
        if (xcfg_cb.charge_en) {
            chbox_sta = bsp_charge_box_process();
            if (chbox_sta == 2) {
                break;                  //充电仓拿起开机
            }
        }
#endif // CHARGE_EN

        if (sys_cb.poweron_flag) {
#if VBAT_DETECT_EN
            if (sys_cb.vbat <= 2950) {  //电压小于2.95v不开机
                continue;
            }
#endif
            if ((CHARGE_DC_NOT_PWRON) && CHARGE_DC_IN()) {
                continue;
            }
#if LINEIN_2_PWRDOWN_EN
            if ((xcfg_cb.linein_2_pwrdown_en) && (device_is_online(DEV_LINEIN))) {
                RTCCON10 = BIT(10);    //clear pwrkey10s pending and counter
                continue;
            }
#endif // LINEIN_2_PWRDOWN_EN
            break;
        } else {
            //PWKKEY已松开, 不在充电仓或未充电直接进行关机
            if ((!pwrkey_pressed_flag) && (chbox_sta)) {
                if ((!SOFT_POWER_ON_OFF) || ((!sys_cb.wko_pwrkey_en) && (!sys_cb.tkey_pwrdwn_en))) {
                    break;                          //没有按键软开关机功能，不在充电状态直接开机
                }
                saradc_exit(adc_cb.channel);
                sfunc_pwrdown(1);
            }
        }
    }
    bsp_charge_box_exit();
}

AT(.text.bsp.power)
void power_on_check(void)
{
    bsp_key_wko10s_reset();
    power_on_check_do();
    bsp_charge_box_reinit();
    sys_cb.sw_rst_flag = 0;
}

AT(.text.bsp.key.init)
void key_var_init(void)
{
    memset(&key_cb, 0, sizeof(key_cb));
    dkey_mask_bits_init();
}
void bsp_saradc_init(u16 adc_ch)
{
    bsp_saradc_init_m(adc_ch);
#if VBAT_DETECT_10BIT_EN
    while(!get_adc_val());                  //获取一次初值，再kick一次
    adc_cb1.vbat_val = (uint)adc_cb1.vbat2 * VBG_VOLTAGE / adc_cb1.bg;
    adc_cb1.vbat_total = adc_cb1.vbat_val << 5;
    sys_cb.vbat = get_vbat_val();
#endif // VBAT_DETECT_EN
}

AT(.text.bsp.key.init)
void key_init(void)
{
    int adc_ch = 0;
    key_var_init();

    memcpy(&key_cb.filter, &key_filter_tbl, sizeof(key_filter_t));
    if (xcfg_cb.kl_time_sel) {
        key_cb.filter.long_cnt = KEY_LONG_TIMES + (u32)xcfg_cb.kl_time_sel*120;  //配置长按时间
        key_cb.filter.hold_cnt = key_cb.filter.long_cnt + KEY_HOLD_TIMES;
    }

    key_cb.delay = ((u8)xcfg_cb.double_key_time + 2) * 20 + 1;
    key_cb.times = 5;                                                           //按键最多检测几击？
    key_cb.tbl   = key_multi_tbl;
    memset(&key_multi_cb, 0, sizeof(key_multi_cb));



    memset(iokey_gpio, 0, sizeof(iokey_gpio));
    if (xcfg_cb.user_iokey_en) {
        iokey_gpio[0] = configure_gpio_tbl[xcfg_cb.iokey_io0];
        iokey_gpio[1] = configure_gpio_tbl[xcfg_cb.iokey_io1];

        iokey_num[0]  = xcfg_cb.iokey_num0;
        iokey_num[1]  = xcfg_cb.iokey_num1;
        iokey_num[2]  = xcfg_cb.iokey_num2;
        io_key_init();
    }


    if (xcfg_cb.user_adkey_en) {
        memcpy(adkey_tbuf, adkey_table, sizeof(adkey_tbl_t)*7);
        if (xcfg_cb.user_adkey_pu10k_en) {
            adcch_io_pu10k_enable(ADKEY_CH);
        }
        adc_ch |= BIT(ADKEY_CH);
        if (xcfg_cb.adkey_config_en) {
            adkey_tbuf[0].usage_id = get_key_configure_val(xcfg_cb.adkey_num0);
            adkey_tbuf[1].usage_id = get_key_configure_val(xcfg_cb.adkey_num1);
            adkey_tbuf[2].usage_id = get_key_configure_val(xcfg_cb.adkey_num2);
            adkey_tbuf[3].usage_id = get_key_configure_val(xcfg_cb.adkey_num3);
            adkey_tbuf[4].usage_id = get_key_configure_val(xcfg_cb.adkey_num4);
            adkey_tbuf[5].usage_id = get_key_configure_val(xcfg_cb.adkey_num5);
        }
        if (xcfg_cb.adkey_adcv_config_en) {
            adkey_tbuf[0].adc_val = xcfg_cb.adcv_num0;
            adkey_tbuf[1].adc_val = xcfg_cb.adcv_num1;
            adkey_tbuf[2].adc_val = xcfg_cb.adcv_num2;
            ///配置ADC VALUE时，只能做3个ADC按键
            adkey_tbuf[3].adc_val = 0xff;
            adkey_tbuf[3].usage_id = NO_KEY;
        }
    }

#if USER_ADKEY2
    if (xcfg_cb.user_adkey2_en) {
        adc_ch |= BIT(ADKEY2_CH);
    }
#endif // USER_ADKEY2

#if USER_NTC
    if (xcfg_cb.sys_ntc_en) {
        adc_ch |= BIT(NTC_ADCCH);
        ntc_gpio_inner_pu10k_init();
    }
#endif // USER_NTC

    RTCCON11 |= BIT(4);                                         //WKOPRT = 1
    uint rtccon1 = RTCCON1 & ~0x1f;                             //wko tkey需要关掉WKO上拉
    if (sys_cb.wko_pwrkey_en) {
        rtccon1 |= BIT(0) | BIT(2) | BIT(4);                    //WK 90K pull up enable
        adc_ch |= (BIT(ADCCH_VRTC) | BIT(ADCCH_WKO));
        key_cb.pwr10_val = pwrkey_table[0].usage_id;            //支持PWRKEY 10S强制复位的按键
        if (xcfg_cb.pwrkey_config_en) {
            key_cb.pwr10_val = get_key_configure_val(xcfg_cb.pwrkey_num0);
            pwrkey_num[0] = xcfg_cb.pwrkey_num0;
            pwrkey_num[1] = xcfg_cb.pwrkey_num1;
            pwrkey_num[2] = xcfg_cb.pwrkey_num2;
            pwrkey_num[3] = xcfg_cb.pwrkey_num3;
            pwrkey_num[4] = xcfg_cb.pwrkey_num4;
            pwrkey_num[5] = NO_KEY;
        }
    }
    RTCCON1 = rtccon1;

#if CHARGE_EN || VBAT_DETECT_EN
	adc_ch |= BIT(ADCCH_BGOP);
#if VBAT_DETECT_EN
    adc_ch |=  BIT(VBAT2_ADCCH);
#endif // VBAT_DETECT_EN
#endif

    if (mic_bias_trim_is_en()) {
        adc_ch |= BIT(ADCCH_MIC);
    }

#if CHARGE_TSEN_DETECT
    PWRCON2 |= BIT(7) | BIT(6) | BIT(5);                        //DI_RES_X2 normal mode, DI_OE_TSEN enable, DI_EN_TSEN enable
    adc_ch |= BIT(ADCCH_VTSEN);
#endif

    bsp_saradc_init(adc_ch);
    bsp_tkey_init();
#if VUSB_TBOX_QTEST_EN
    bsp_qtest_init();
#endif
    power_on_check();
}

#if (USER_MULTI_PRESS_EN && USER_TKEY_SLIDE)
AT(.com_text.bsp.key)
void key_multi_press_clr(void)
{
    key_multi_cb_t *s = &key_multi_cb;

    s->counter = 0;
    s->strikes = 0;
    s->val = NO_KEY;
}
#endif

#if USER_TKEY_SHORT_SLIDE
AT(.com_text.bsp.key)
u16 bsp_key_slide_process(u16 key_val)
{
    u16 key_return = NO_KEY;

    key_return = key_process(key_val);

    key_return = tkey_short_slide_process(key_return);

#if USER_MULTI_PRESS_EN
    if (key_cb.times > 1) {
        key_return = key_multi_press_process(key_return);
    }
#endif
    return key_return;
}
#endif

u8 bsp_key_pwr_scan(void)
{
    u8 key_val = NO_KEY;
#if USER_TKEY
    key_val = bsp_tkey_scan();
#endif

#if USER_PWRKEY
    if (key_val == NO_KEY) {
        key_val = get_pwrkey();
    }
#endif // USER_PWRKEY
    return key_val;
}

AT(.com_text.port.key)
void get_adc_val_hook(void)
{
#if CHARGE_TSEN_DETECT
    sys_cb1.adc_tsen_val = adc_cb.sfr[ADCCH_VTSEN] * 600 / adc_cb.sfr[ADCCH_BGOP];
#endif
#if VBAT_DETECT_10BIT_EN
	adc_cb1.bg = (u16)adc_cb.sfr[ADCCH_BGOP];
    adc_cb1.vbat2 = (u16)adc_cb.sfr[VBAT2_ADCCH];
#endif // VBAT_DETECT_EN
}

AT(.com_text.bsp.key)
u8 key_scan(void)
{
    u8 key_val = NO_KEY;

    if (!get_adc_val()) {
        return NO_KEY;
    }

#if USER_TKEY
#if USER_TKEY_SHORT_SLIDE
    key_val = bsp_short_slide_tkey_scan();
#else
    key_val = bsp_tkey_scan();
#endif
#endif

#if USER_ADKEY
    if (key_val == NO_KEY) {
        key_val = get_adkey();
    }
#endif // USER_ADKEY

#if USER_ADKEY2
    if (key_val == NO_KEY) {
        key_val = get_adkey2();
    }
#endif // USER_ADKEY2

#if USER_PWRKEY
    if ((key_val == NO_KEY) && (!PWRKEY_2_HW_PWRON)) {
        key_val = get_pwrkey();
    }
#endif // USER_PWRKEY

#if USER_IOKEY
    if (key_val == NO_KEY) {
        key_val = get_iokey();
    }
#endif // USER_IOKEY

#if (IRRX_SW_EN || IRRX_HW_EN)
    if (key_val == NO_KEY) {
        key_val = get_irkey();
    }
#endif // (IRRX_SW_EN || IRRX_HW_EN)
    return key_val;
}

//AT(.com_rodata.key)
//const char key_msg_str[] = "key msg: %04x\n";

AT(.com_text.bsp.key)
u8 bsp_key_scan(void)
{
    u8 key_val;
    u16 key = NO_KEY;

    key_val = key_scan();
#if VBAT_DETECT_EN
    sys_cb.vbat = get_vbat_val();
#endif // VBAT_DETECT_EN

#if USER_TKEY_SHORT_SLIDE
    key = bsp_key_slide_process(key_val);
#else
    key = bsp_key_process(key_val);
#endif

#if USER_TKEY_SLIDE
    key = bsp_tkey_slide_process(key);
#endif
    if ((key != NO_KEY) && (!bsp_key_pwron_filter(key))) {
        //防止enqueue多次HOLD消息
        if ((key & KEY_TYPE_MASK) == KEY_LONG) {
            sys_cb.kh_vol_msg = (key & 0xff) | KEY_HOLD;
        } else if ((key & KEY_TYPE_MASK) == KEY_LONG_UP) {
            msg_queue_detach(sys_cb.kh_vol_msg, 0);
            sys_cb.kh_vol_msg = NO_KEY;
        } else if (sys_cb.kh_vol_msg == key) {
            msg_queue_detach(key, 0);
        }
#if WAV_KEY_VOICE_QUICK_EN
        if (key == K_PLAY_PWR_USER_DEF) {
            sys_cb.tws_res_brk = 1;
        }
#endif
#if LED_188LED_DISP_EN
        if (key == K_PLAY_PWR_USER_DEF) {
            bsp_188led_disp_set_on();
        }
#endif
//        printf(key_msg_str, key);
        msg_enqueue(key);
    }
    return key_val;
}

