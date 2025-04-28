#include "include.h"
#include "func.h"

void user_tmr_set_enable(bool tmr5ms, bool tmr1ms);
void bsp_set_stop_time(u16 stop_time);
u8 power_off_check(void);
uint32_t sleep_timer(void);         //休眠定时器，500ms进一次(返回值： 1->关机 2->退出sleep)
void sfunc_lowbat(void);
void sfunc_pwrdown(u8 vusb_wakeup_en);
void sfunc_power_save_enter(void);
void delay_15us(void);
bool led_bt_sleep_tws_sync(void);

extern u32 __sleep_comm_vma, __sleep_comm_lma, __sleep_comm_size;
extern u32 __pwrdwn_comm_vma, __pwrdwn_comm_lma, __pwrdwn_comm_size;

#define RI_VRTC(bit)                         RTCCON4 = (RTCCON4 & ~(3<<24)) | ((bit)<<24)

//休眠定时器，500ms进一次(返回值： 1->关机 2->退出sleep)
AT(.text.sleep_com.timer)
uint32_t sleep_timer(void)
{
    uint32_t ret = 0;

    sys_cb.sleep_counter++;
#if CHARGE_EN
    //定时检测charge状态
    if (xcfg_cb.charge_en) {
        charge_detect();
        if ((sys_cb.sleep_counter % 2) == 0) {      //1秒调用一次
            bsp_charge_bled_ctrl();
        }
        if (!port_2led_is_sleep_en()) {             //1个IO推两个灯, 充电状态需要退出休眠, 否则充电灯指示不正常
            ret = 2;
        }
    }
#endif // CHARGE_EN

#if VBAT_DETECT_EN || USER_NTC || CHARGE_TSEN_DETECT
    //定时检测电池电量, 低电需要唤醒
    if (sys_cb.sleep_counter == 10) {
        sys_cb.sleep_counter = 0;
        bsp_bcnt_temp_calibration();
        bsp_saradc_init(adc_cb.channel);
        if ((LPWR_WARNING_VBAT) && (sys_cb.vbat < ((u16)LPWR_WARNING_VBAT*100 + 2850))) {
            //低电需要唤醒sniff mode
            ret = 2;
        }
#if USER_NTC
        if ((xcfg_cb.sys_ntc_en) && (adc_cb.ntc_val <= xcfg_cb.ntc_thd_val)) {
            ret = 2;
        }
#endif
#if CHARGE_TSEN_DETECT
        charge_tsen_check(1);
#endif
        saradc_exit(adc_cb.channel);
    }
#endif // VBAT_DETECT_EN

#if BT_TWS_SLEEP_LED_SYNC_EN
    if (led_bt_sleep_tws_sync()) {
        sys_cb.sleep_wakeup_time = 50000 / 312;
    } else {
        sys_cb.sleep_wakeup_time = -1L;
    }
#else
    sys_cb.sleep_wakeup_time = led_bt_sleep(xcfg_cb.led_bt_sleep_sel);
#endif
    if ((sys_cb.pwroff_delay != -1L) && (sys_cb.sleep_wakeup_time == -1L)) {
        if(sys_cb.pwroff_delay > 5) {
            sys_cb.pwroff_delay -= 5;
        } else {
            sys_cb.pwroff_delay = 0;
            return 1;
        }
    }

    if ((PWRKEY_2_HW_PWRON) && (!IS_PWRKEY_PRESS())){
        ret = 1;
    }

    return ret;
}

AT(.text.lowpwr.sleep)
static void sfunc_sleep_dac_off(u8 off)
{
#if ANC_EN
    if (sys_cb.anc_start) {
        return;
    }
#endif
    if (off) {
#if ANC_EN
        bsp_anc_stop();
#endif
#if TINY_TRANSPARENCY_EN
        bsp_ttp_stop();
#endif
        dac_power_off();                    //dac power down
        if(!SLEEP_DAC_OFF_EN) {
            dac_power_off_pulldown_daclr();
        }
    } else {
        if(!SLEEP_DAC_OFF_EN) {
            dac_power_off_restore_daclr();
        }
        dac_restart();
#if ANC_EN || TINY_TRANSPARENCY_EN
        bsp_anc_set_mode(sys_cb.anc_user_mode);
#endif
    }
}

#if USER_LED_SLEEP_EN
void sys_sleep_cb(u8 lpclk_type)
{
    u32 clkgat0 = CLKGAT0;
    u32 clkcon0 = CLKCON0;
    u32 clkcon2 = CLKCON2;
    u32 clkcon1 = CLKCON1;
    u32 xo26mcon = XO26MCON;

    RI_VIO_TC(0);                                       //减小VDDIO尾电流, 减小功耗
    CLKCON2 = (CLKCON2&0x00ffffff) | (25<<24);          //div26, x26m_div_clk=1m
    CLKCON0 |= BIT(2) | BIT(3);                         //select x26m_div_clk as sysclk
    asm("nop");asm("nop");
    if (!sys_cb.sleep_aupll_en) {
        RSTCON0 &= ~BIT(7);                             //reset adda_clk48_a
        CLKGAT0 &= ~(BIT(12) | BIT(22));                //disable dac_clk, sdadcl_clk gate
        CLKCON1 &= ~0x3;                                //disable dac clk
    }
    asm("nop");asm("nop");
//    CLKGAT0 &= ~BIT(20);                                //disable bt 52m clock
    XO26MCON &= ~BIT(5);                                //DI_EN_CCKOUT_X2
    asm("nop");asm("nop");

//    LPMCON |= BIT(2);                                   //system sniff
    asm("nop");asm("nop");
    asm("nop");asm("nop");

    //wakeup now
    XO26MCON = xo26mcon;
    delay_15us();
    if (!sys_cb.sleep_aupll_en) {
        CLKCON1 = clkcon1;                              //recover dac clk
        delay_15us();
        CLKGAT0 |= (BIT(12) | BIT(22));                 //enable dac_clk, sdadcl_clk gate
        delay_15us();
        RSTCON0 |= BIT(7);                              //release adda_clk48_a
    }
    asm("nop"); asm("nop");
    CLKGAT0 = clkgat0;                                  //enable bt 52m clock
    CLKCON2 = clkcon2;
    CLKCON0 = clkcon0;
    RI_VIO_TC(3);
}
#endif // USER_LED_SLEEP_EN

AT(.text.sleep_com.process)
u32 sfunc_sleep_process(void)
{
    uint32_t status, ret = 0;
    u32 wkupedg;

    bsp_gpiode_reinit_save();                       //io analog input
    loudspeaker_gpiode_set();                       //sleep维持mute脚状态
    atmosphere_led_gpiode_set();                    //sleep维持氛围灯状态
#if BT_EARIN_DETECT_EN
    earin_sniff_gpiode_set();                       //sleep维持入耳检测IO状态
#endif
//    if(xcfg_cb.bb_rxtx_sync_dbg_en) {
//        GPIOADE |= BIT(7) | BIT(6);
//        GPIOBDE |= BIT(4);
//    }
#if USER_NTC
    if (xcfg_cb.sys_ntc_en) {
        adc_cb.channel |= BIT(NTC_ADCCH);
        ntc_gpio_inner_pu10k_init();                //sleep维持ntc检测
    }
#endif
#if CHARGE_TSEN_DETECT
    adc_cb.channel |= BIT(ADCCH_VTSEN);             //sleep维持tsen检测
#endif
#if CHARGE_EN
    if (CHARGE_DC_IN()) {                           //保持充电灯的IO状态
        if (sys_cb.charge_sta == 1) {
            charge_led_on();
        } else if (sys_cb.charge_full_bled_flag == 1) {
            charge_bled_on();
        }
    }
    bsp_set_stop_time(3600);
#endif // CHARGE_EN

    WKUPEDG = 0;
    WKUPCON = 0;
    PORTINTEDG = 0;
    PORTINTEN = 0;

    sys_cb.sleep_wakeup_time = -1L;
    sys_cb.sleep_counter = 0;
    sleep_wakeup_config();

    while(bt_is_sleep()) {
        WDT_CLR();
        status = bt_sleep_proc();
        if(status == 1) {
            ret = sleep_timer();
            if(ret) {
                if (ret == 1) {
                    sys_cb.pwrdwn_tone_en = 1;
                    func_cb.sta = FUNC_PWROFF;
                }
                break;
            }
        }
//        printf("WKUPEDG:%x\n", WKUPEDG);
        if (WKUPEDG & (0xff << 16)) {
            break;
        }
#if BT_TWS_AUTO_SWITCH
        if((sys_cb.dc_rst_flag != 0) && CHARGE_DC_IN()) {
            break;
        }
#endif // BT_TWS_AUTO_SWITCH
        if (bsp_chbox_type_is_maintain_volt() && CHARGE_INBOX()) {
            break;
        }
#if USER_TKEY
        if (tkey_is_pressed()) {
            break;
        }
#endif
#if BT_EARIN_DETECT_EN
        if (earin_is_wakeup() || touchear_is_wakeup()) {
            break;
        }
#endif
    }
    wkupedg = WKUPEDG;
    WKUPCPND = 0xff << 16;
    WKUPCON = 0;
    WDT_CLR();
    bsp_gpiode_recover();
    return wkupedg;
}


AT(.text.lowpwr.sleep)
void sfunc_sleep(void)
{
    uint32_t sysclk;
    u32 wkupedg;
    u16 adc_ch;

#if VBAT_DETECT_EN
    if (is_lowpower_vbat_warning() || sys_cb.breathe_led_sta) {  //低电或呼吸灯不进sniff mode
        return;
    }
#endif // VBAT_DETECT_EN
#if LOUDSPEAKER_MUTE_MUX_RLED_EN
    if (sys_cb1.unmute_rled_on) {
        return;
    }
#endif

    printf("sleep\n");
#if !USER_LED_SLEEP_EN
    bt_enter_sleep();

    led_off();
    rled_off();
#endif

#if DAC_DNR_EN
    u8 sta = dac_dnr_get_sta();
    dac_dnr_set_sta(0);
#endif
    sfunc_sleep_dac_off(1);
    saradc_exit(adc_cb.channel);                //close saradc及相关通路模拟

#if !USER_LED_SLEEP_EN
    user_tmr_set_enable(0, 0);
#endif
    sysclk = sys_clk_get();
    sys_clk_set(SYS_26M);
    bsp_bcnt_temp_calibration_start();

    if (!sys_cb.sleep_aupll_en) {
        adpll_disable();
    }
    adc_ch = adc_cb.channel;
    adc_cb.channel = BIT(ADCCH_VBAT) | BIT(ADCCH_BGOP);
    memcpy(&__sleep_comm_vma, &__sleep_comm_lma, (u32)&__sleep_comm_size);    //Load sleep comm

    wkupedg = sfunc_sleep_process();                //不能切Flash

    if (!sys_cb.sleep_aupll_en) {
        adpll_init(DAC_OUT_SPR);                    //enable adpll
    }
    dac_clk_source_sel(1);                          //dac clk select adpll_div(PLL0)
    bsp_bcnt_temp_calibration_stop();
    adc_cb.channel = adc_ch;
    bsp_saradc_init(adc_cb.channel);
    if (wkupedg & BIT(16 + 5)) {                    //pwrkey wakeup, 加快第一次短按检测
        u16 key_val = get_pwrkey();
        if (key_val != NO_KEY) {
            bsp_key_process(key_val);
            key_cb.cnt = key_cb.filter.scan_cnt - 2;
        }
    }
#if CHARGE_EN
    bsp_set_stop_time(18000);
#endif // CHARGE_EN
    sys_clk_set(sysclk);
    user_tmr_set_enable(1, 1);

    sfunc_sleep_dac_off(0);

    bsp_change_volume(sys_cb.vol);
#if DAC_DNR_EN
    dac_dnr_set_sta(sta);
#endif
    bt_exit_sleep();

#if BT_DISP_LOW_VBAT_EN
    sys_cb1.disp_low_vbat_ticks = 0;
#endif

    printf("wakeup\n");
}

AT(.text.bfunc.sleep)
bool sleep_process(is_sleep_func is_sleep)
{
    if ((*is_sleep)()) {
        if ((!sys_cb.sleep_en) || (!port_2led_is_sleep_en())) {
            reset_sleep_delay();
            return false;
        }
        if(sys_cb.sleep_delay == 0) {
            sfunc_sleep();
            reset_sleep_delay();
            return true;
        }
    } else {
        reset_sleep_delay();
    }
    return false;
}

extern const u8 chg_timeout_tbl[4];

///解决充满关机后按充电仓按键不能唤醒进入充电的问题（5V掉电但有维持电压的仓类型）
AT(.text.pwrdwn.vusb)
static u8 sfunc_pwrdown_w4_vusb_offline(void)
{
    u32 off_cnt = 0;
    u32 timeout = (u32)chg_timeout_tbl[xcfg_cb.ch_w4_off_sel] * 200;

    if ((xcfg_cb.ch_box_type_sel != 1) && timeout) {
        if (xcfg_cb.led_charge_fully_xcfg == 1) {
            charge_bled_on();
        }
        while (timeout--) {
            WDT_CLR();
            delay(1700);                        //5ms
            if (!CHARGE_DC_IN()) {
                off_cnt++;
            } else {
                off_cnt = 0;
            }
            if (off_cnt > 5) {
                return 1;                       //VUSB已掉线，打开VUSB唤醒
            }
        }
    }
    return 0;
}

AT(.text.pwrdwn.lowbat)
void sfunc_lowbat_do(void)
{
    sfunc_power_save_enter();
    WDT_DIS();
    LPMCON |= BIT(0);                               //Sleep mode enable
    asm("nop");asm("nop");asm("nop");
    while(1);
}

//硬开关方案，低电时，进入省电状态
void sfunc_lowbat(void)
{
    memcpy(&__pwrdwn_comm_vma, &__pwrdwn_comm_lma, (u32)&__pwrdwn_comm_size);    //Load pwrdwn comm
    sfunc_lowbat_do();
}

//软开关方案，POWER键/低电时，进入关机状态
AT(.text.pwrdwn)
void sfunc_pwrdown_do(u8 vusb_wakeup_en)
{
    printf("%s:%d\n", __func__, vusb_wakeup_en);

    u32 tkey_wakeup_en = sys_cb.tkey_pwrdwn_en;
    u32 wakeup_bit = 0;

    WDT_CLR();
    vusb_delay_rst_dis();
    if (sys_cb.vbat < 3050) {
        tkey_wakeup_en = 0;                         //低电关机, 关掉触模唤醒
    }

    RTCCON4 &= ~BIT(15);                            //RI_VIO_PD(0)
    RTCCON8 &= ~BIT(15);                            //RI_EN_VUSBDIV = 0
    RTCCON1 &= ~(BIT(5) | BIT(7));                  //BIT(7): VRTC voltage for ADC, BIT(5):WK pin analog
    RTCCON9 = 0xfff;                                //Clr pending
    if (!vusb_wakeup_en && xcfg_cb.ch_box_type_sel == 3) {  //5V完全掉电的仓
        RTCCON3 &= ~BIT(12);                        //RTCCON3[12], INBOX Wakeup disable
    }
    if (sys_cb.pdn_boat_flag) {                     //船运模式关机, 只保留5V唤醒。
        RTCCON3 &= ~BIT(12);                        //RTCCON3[12], INBOX Wakeup disable
        tkey_wakeup_en = 0;                         //disable tkey wakeup
    }
    if (!(RTCCON3 & BIT(12))) {                     //非仓内关机, 开下漏电
        bsp_charge_leakage_set(1, 0);
    } else {
        if (bsp_chbox_type_is_maintain_volt()) {    //有维持电压的充电仓关闭leakage漏电
            bsp_charge_leakage_set(0, 0);
        }
    }
    if ((RTCCON3 & BIT(12)) || sys_cb.lowpwr_dwn_flag || xcfg_cb.user_tkey_wakeup_dis) { //仓内关机或低电关机？
        tkey_wakeup_en = 0;                         //disable tkey wakeup
    }
    if ((tkey_wakeup_en) && (!xcfg_cb.user_tkey_alarm_dis)) {
        RTCCON0 |= BIT(21);                         //BGUV reset TKSWRSTN
        RTCCON8 |= BIT(29);                         //RI_EN_BGUVT = 1, rtc bgp undervoltage detect enable
    }
    sfunc_power_save_enter();
    if (!vusb_wakeup_en || sys_cb.chstop_pwrwn_en) {
        RTCCON8 = (RTCCON8 & ~BIT(6)) | BIT(1);     //disable charger function
    }
    if (!vusb_wakeup_en) {                          //充满关机
        vusb_wakeup_en = sfunc_pwrdown_w4_vusb_offline();
    }
    RTCCON11 = (RTCCON11 & ~0x03) | BIT(2);         //WK PIN filter select 8ms
    uint rtccon3 = RTCCON3 & ~BIT(11);
#if CHARGE_EN
    if (((xcfg_cb.charge_en) && (vusb_wakeup_en))||get_qtest_mode()) {
        rtccon3 |= BIT(11);                         //VUSB wakeup enable
#if CHARGE_PULL_OUT_FILTER_EN
        RTCCON &= ~BIT(6);                          //VUSB insert reset system disable
        asm("nop"); asm("nop");
        RTCCON3 |= BIT(11);                         //VUSB wakeup enable
        RTCCON11 |= BIT(6);                         //VUSB pull out filter VUSB拔出滤波
        delay_us(100);
        RTCCON10 = BIT(3);
#endif
    }
#endif
    RTCCON3 = rtccon3 & ~(BIT(10) | BIT(14));       //关WK PIN，再打开，以清除Pending
    wakeup_bit = RTCCON3 & (0xff << 8);             //备份wakeup
    if ((TKCON1 & BIT(13)) && tkey_wakeup_en) {     //TK is enable?
        pmu_vdd11_select_vrtc11();
        RTCCON3 |= BIT(14);                         //Touch key long press wakeup
        wakeup_bit |= BIT(14);
        RTCCON0 &= ~BIT(4);                         //TKITF_EN disable
    } else {
        RTCCON0 |= BIT(4);
        INPCPND = BIT(0);
        RTCCON0 &= ~BIT(5);                         //touch key reset
        RTCCON0 &= ~BIT(0);                         //RING2M_RTC Disable
        RTCCON0 &= ~BIT(4);                         //TKITF_EN disable
    }

    uint rtccon1 = RTCCON1 & ~0x1f;
    rtccon3 = RTCCON3 & ~0x17;                      //Disable VDDCORE, buck
    rtccon3 |= BIT(6);                              //Core power down enable, VDDCORE short disable
#if USER_PWRKEY
    if (sys_cb.wko_pwrkey_en) {
        if (!sys_cb.pdn_boat_flag) {
            wakeup_bit |= BIT(10);
            rtccon3 |= BIT(10);                     //WK pin wake up enable
        }
        rtccon1 |= BIT(0) | BIT(2) | BIT(4);        //WK 90K pull up enable
        rtccon3 &= ~BIT(5);                         //RI_EN_PG_2CORE = 0, vddcore-vdd11 short disable
    }
#endif
    WDT_DIS();
    RTCCON |= BIT(5);                               //PowerDown Reset，如果有Pending，则马上Reset
    RTCCON1 = rtccon1;
    RTCCON3 = rtccon3 | wakeup_bit;
    LPMCON |= BIT(0);
    LPMCON |= BIT(1);                               //idle mode
    asm("nop");asm("nop");asm("nop");
    while (1);
}

void sfunc_pwrdown(u8 vusb_wakeup_en)
{
    memcpy(&__pwrdwn_comm_vma, &__pwrdwn_comm_lma, (u32)&__pwrdwn_comm_size);    //Load pwrdwn comm
    sfunc_pwrdown_do(vusb_wakeup_en);
}

AT(.text.lowpwr.pwroff)
void func_pwroff(int pwroff_tone_en)
{
    printf("%s\n", __func__);

#if VUSB_SMART_VBAT_HOUSE_EN
    if (vhouse_cb.inbox_sta) {
        bsp_charge_inbox_wakeup_enable();
    }
#endif
    vusb_delay_rst_dis();
    led_power_down();
    motor_enable(MOTOR_PWROFF, T_MOTOR_PWROFF_SEL);
#if WARNING_POWER_OFF
    if ((pwroff_tone_en) && (SOFT_POWER_ON_OFF)) {
        sys_warning_play(T_WARNING_POWER_OFF, PIANO_POWER_OFF);
    }
#endif // WARNING_POWER_OFF
    dac_power_off();                                            //dac power down
    motor_disable();
    if (SOFT_POWER_ON_OFF) {
        if (!PWRKEY_2_HW_PWRON) {
            u8  dcin_cnt = 0;
        #if USER_TKEY
            tkey_press_timeout_clr();
        #endif
            while (IS_PWRKEY_PRESS() || IS_TKEY_PRESS()) {      //等待PWRKWY松开
                if (CHARGE_DC_IN()) {
                    dcin_cnt++;
                    if (dcin_cnt > 3) {
                        sw_reset_kick(SW_RST_DC_IN);            //直接复位进入充电
                    }
                } else {
                    dcin_cnt = 0;
                }
        #if USER_TKEY
                if (sys_cb.tkey_pwrdwn_en) {
                    tkey_press_timeout_process();
                }
        #endif
                delay_5ms(2);
                WDT_CLR();
            }
        }

        led_power_down_w4_done();
        if (CHARGE_DC_IN() && (xcfg_cb.chbox_en == 0)) {
            if (!power_off_check()) {
                return;                                         //pwrkey按下，再次开机
            }
        }
        saradc_exit(adc_cb.channel);                            //close saradc及相关通路模拟
        if ((PWRKEY_2_HW_PWRON) && (sys_cb.poweron_flag)) {
            RTCCON1 |= BIT(6);                                  //WK PIN High level wakeup
        }
        sfunc_pwrdown(2);
    } else {
        saradc_exit(adc_cb.channel);                            //close saradc及相关通路模拟
        led_power_down_w4_done();
        sfunc_lowbat();                                         //低电关机进入Sleep Mode
    }
}

