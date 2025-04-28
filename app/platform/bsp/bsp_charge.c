#include "include.h"

#if CHARGE_EN

#define TRACE_EN                0

#if TRACE_EN
#define TRACE(...)              printf(__VA_ARGS__)
#else
#define TRACE(...)
#endif

#define DC_IN()                  ((RTCCON >> 20) & 0x01)            //VUSB Online state:    0->not online, 1->online

extern inbox_cb_t inbox_cb;
extern u32 user_timer_isr_en;

void charge_stop(u8 mode);
void charge_init(charge_cfg_t *p);
void bsp_188led_charge_full_disp(void);

//充满电亮蓝灯时间table表, 秒单位
AT(.rodata.charge)
const u8 chag_bled_timeout_tbl[8] = {0, 10, 20, 30, 60, 120, 180, 255};

//充电时间(单位30分钟)
AT(.rodata.charge)
const u8 chag_limited_tbl[8] = {0, 4, 5, 6, 7, 8, 10, 255};

//AT(.com_rodata.tkey)
//const char charge_on_str[] = "Charge on\n";
//AT(.com_rodata.tkey)
//const char charge_off_str[] = "Charge off: %d\n";

/*
///降低充电时主控耗电
AT(.text.bsp.charge) ALIGNED(512)
void bsp_charge_power_save(void)
{
    if (chbox_cb.pwrsave_en) {
        sys_clk_set(SYS_26M);                           //跑XOSC 26M
        adpll_disable();
        CLKGAT0 &= ~BIT(20);                            //disable bt 52m clock
        XO26MCON &= ~BIT(5);                            //DI_EN_CCKOUT_X2
        if (chbox_cb.pwrsave_en == 2) {                 //关晶振跑RC
            sys_clk_set(SYS_RC3M);
            sys.rc3m_clk = sys_get_rc2m_clkhz(0);
            vusb_uart_baud_set(0);                      //VUSB UART1使用系统时钟
            sys_uart0_baud_set(115200);                 //时钟频率太低, 使用低波特率
            RI_VIO_TC(0);
            GLOBAL_INT_DISABLE();
            TMR0CNT = 0;
            TMR0PR = sys.rc3m_clk / 1000 - 1;           //1ms
            TMR0CON &= ~BIT(2);                         //tmr0 select sysclk
            if (sys_cb.breathe_led_sta) {
                breathe_led_timer_start(0);
            }
            RSTCON0 &= ~BIT(7);                         //reset adda_clk48_a
            CLKGAT0 &= ~(BIT(12) | BIT(22));            //disable dac clk
            GLOBAL_INT_RESTORE();
            XO26MCON &= ~((0xf<<24) | BIT(10) | BIT(5));//X26 output, xosc, xosc52m disable
            PLL0CON &= ~(BIT(12) | BIT(0) | BIT(1));    //PLL0 & XOSC关电
        }
    }
}

AT(.text.bsp.charge) ALIGNED(512)
void bsp_charge_power_recover(void)
{
    if (chbox_cb.pwrsave_en) {
        if (chbox_cb.pwrsave_en == 2) {
            PLL0CON |= BIT(0) | BIT(12);                //enable xosc & pll电源,DI_EN_AVDD_SUPPLY
            XO26MCON |= BIT(10) | (0xf<<24);            //xosc en, output
            RI_VIO_TC(3);
            delay_ms(5);
            GLOBAL_INT_DISABLE();
            CLKCON1 = (CLKCON1 & ~0x03) | 2;            //dac clk select xosc26m_clk
            delay_us(2);
            CLKGAT0 |= (BIT(12) | BIT(22));             //dac clkgate enable
            RSTCON0 |= BIT(7);                          //release adda_clk48_a
            TMR0CNT = 0;
            TMR0PR = 1000 - 1;                          //1ms
            TMR0CON |= BIT(2);                          //tmr0 select tmr_inc
            if (sys_cb.breathe_led_sta) {
                breathe_led_timer_start(1);
            }
            GLOBAL_INT_RESTORE();
            sys_clk_set(SYS_26M);                       //跑XOSC 26M
            vusb_uart_baud_set(1);                      //VUSB UART1使用xosc_div_clk
            sys_uart0_baud_set(1500000);                //还原UART0波特率
        }
        XO26MCON |= BIT(5);                             //DI_EN_CCKOUT_X2
        CLKGAT0 |= BIT(20);                             //enable bt 52m clock
        adpll_init(DAC_OUT_SPR);
        dac_clk_source_sel(1);                          //dac clk select adpll_div(PLL0)
        sys_clk_set(SYS_CLK_SEL);
    }
}*/

//AT(.com_rodata.tkey)
//const char charge_on_str[] = "Charge on\n";
//AT(.com_rodata.tkey)
//const char charge_off_str[] = "Charge off: %d\n";

#if CHARGE_DELAY_OPEN_LEAKAGE_EN
AT(.com_text.charge)
void charge_inbox_delay_leakage_check(void)
{
	if (get_qtest_mode() || CHARGE_DC_IN()) {
        sys_cb1.inbox_cnt_on = 0;
        return;
	}

	if (CHARGE_INBOX()) {
		sys_cb1.inbox_cnt_on++;
		if (sys_cb1.inbox_cnt_on >= CHARGE_DELAY_LEAKAGE_TICKS) {
            msg_enqueue(EVT_SET_LEAKAGE);
		} else if (sys_cb1.inbox_cnt_on >= CHARGE_DELAY_LEAKAGE_TICKS + 100) {
			msg_enqueue(EVT_CHARGE_INBOX);              //仓未起5v，入仓关机
			sys_cb1.inbox_cnt_on = 0;
		}
	} else {
		sys_cb1.inbox_cnt_on = 0;
	}
}
#endif

AT(.com_text.charge)
u8 bsp_charge_sta_hook(u8 sta)
{
    if (sta == 1) {
        //充电开启
        sys_cb.charge_sta = 1;
        sys_cb.charge_full_bled_flag = 0;
        if (sys_led.charge_led_2s_en) {
            sys_cb.charge_full_bled_flag = 2;                //借用下这个变量实现只亮2秒红灯
            sys_cb.ch_bled_cnt = 2;
        }
        charge_led_on();
//        printf(charge_on_str);
    } else {
        //充电关闭
//        printf(charge_off_str, sta);
        sys_cb.charge_sta = 0;
        sys_cb.charge_full_bled_flag = 0;
        charge_led_off(sta);
        if (sta == 2) {
            sys_cb.charge_sta = 2;
            //充电满亮蓝灯
            if (BLED_CHARGE_FULL > 1) {
                sys_cb.charge_full_bled_flag = 1;
                sys_cb.ch_bled_cnt = charge_cfg.bled_on_pr;
                charge_bled_on();
#if LED_188LED_DISP_EN
                bsp_188led_charge_full_disp();
#endif
            }
        }
    }

    return 1;                                               //返回0时时用回rom的代码
}

#if TRACE_EN
AT(.com_rodata.tkey)
const char charge_on_str[] = "Charge on\n";
AT(.com_rodata.tkey)
const char charge_off_str[] = "Charge off: %d\n";
#endif

void bsp_charge_init(void)
{
    charge_cfg_t *p = &charge_cfg;
    memset(p, 0, sizeof(charge_cfg_t));

    if (xcfg_cb.charge_en) {
        p->trick_curr_en = CHARGE_TRICK_EN;
        p->const_curr = CHARGE_CONSTANT_CURR & 0x1f;
        if (p->const_curr > 15) {
            p->const_curr = (p->const_curr - 15) * 2 + 15;
        }
        p->trick_curr = CHARGE_TRICKLE_CURR & 0x07;
        p->stop_curr = CHARGE_STOP_CURR & 0x0f;
        p->stop_volt = 0;
        p->dcin_reset = CHARGE_DC_RESET & 0x01;
        p->volt_follow = CHARGE_VOLT_FOLLOW;
        p->stop_time = 18000;               //默认30分钟
        p->bled_on_pr = chag_bled_timeout_tbl[BLED_CHARGE_FULL];
        p->limited_time = UINT_MAX;
        p->stop_curr_thd = (xcfg_cb.ch_stop_curr_thd + 1) * 15;
        if (xcfg_cb.charge_limited_time > 6) {
            xcfg_cb.charge_limited_time = 0;
        }
        if (xcfg_cb.charge_limited_time) {
            p->limited_time = (u32)chag_limited_tbl[xcfg_cb.charge_limited_time] * 18000;
        }
        cfg_vio_chg_en = !CHARGE_PULL_OUT_FILTER_EN;
//        if (RTCCON3 & BIT(12)) {
//            sys_cb.inbox_wken_flag = 1;
//        }
//        RTCCON3 &= ~BIT(12);                                //RTCCON3[12], INBOX Wakeup disable
        if (!xcfg_cb.ch_inbox_sel) {
            RTCCON8 |= BIT(4);                              //inbox voltage selection.  1'b1,  1.7v;   1'b0, 1.1v
        } else {
            RTCCON8 &= ~BIT(4);
        }
        RTCCON11 &= ~BIT(6);                                //VUSB pull out filter disable

        bsp_charge_leakage_set(1, 0);                       //开机先打开leakage
        charge_init(p);
	}
#if VUSB_SMART_VBAT_HOUSE_EN
    bsp_smart_vhouse_init();
#endif
    if (xcfg_cb.charge_dc_reset && !(RTCCON & BIT(6))) {
        vusb_delay_rst_en(8);                               //soft kick vusb延时8秒复位使能
        sys_cb.dc_rst_flag = 1;
    }
#if CHARGE_TSEN_DETECT
    sys_cb1.trick_curr_bkp = p->trick_curr;
    sys_cb1.const_curr_bkp = p->const_curr;
#endif
}

///5ms调用一次
static u32 bsp_charge_box_process_do(void)
{
    u8 chbox_sta, vhouse_sta = 0;
    chbox_cb.counter++;
    if ((user_timer_isr_en & BIT(1)) == 0) {
        if ((chbox_cb.counter % 20) == 0) {                             //100ms
            charge_detect();
        }
        if ((chbox_cb.counter % 180) == 0) {                            //1s
            bsp_charge_bled_ctrl();
        }
#if USER_NTC
        sys_ntc_check();
#endif // USER_NTC

#if CHARGE_TSEN_DETECT
        charge_tsen_check(20);
#endif
    }

#if VUSB_TBOX_QTEST_EN
    if(get_qtest_mode()){
        return 2;
    }
#endif

#if VUSB_SMART_VBAT_HOUSE_EN
    vhouse_sta = bsp_smart_vhouse_process(1);
#endif
    if ((bsp_charge_box_sta() == 2) || (vhouse_sta == 2) || (sys_cb.ntc_2_pwrdwn_flag)) { //是否充满或NTC关机
        u8 vusb_wakeup_en = 1;
        if ((!sys_cb.ntc_2_pwrdwn_flag) || CHARGE_INBOX()) {
            bsp_charge_inbox_wakeup_enable();                           //INBOX Wakeup enable
            vusb_wakeup_en = 0;
        }
        saradc_exit(adc_cb.channel);
        bsp_charge_tebcnt_calibration_stop();
        sfunc_pwrdown(vusb_wakeup_en);
    }

    if (!CHARGE_DC_IN()) {                                              //DC IN online?
        if (chbox_cb.dc_in) {                                           //VUSB拔出?
            chbox_cb.dc_in = 0;
            bsp_charge_leakage_set(0, 0);                               //无VUSB电容的直接关漏电，防止唤醒先充满的耳机。
            if (xcfg_cb.ch_out_leakage_sel) {
                bsp_charge_leakage_set(0x81, xcfg_cb.ch_out_leakage_sel);
            }
        }
        chbox_sta = bsp_charge_inbox_check() & 0x3;                     //检查inbox online状态
        if ((chbox_sta == 0) && tick_check_expire(chbox_cb.ticks, 3000)) { //兼容IP5303的仓维持电压不稳且处于检测临界态问题
            chbox_sta = (!xcfg_cb.ch_inbox_sel) ? 2 : 1;                 //inbox 1.1v临界状态认为拿起，1.7v临界状态认为inbox在线。
        }
#if VUSB_SMART_VBAT_HOUSE_EN
        if (vhouse_sta) {                                               //智能充电仓已开盖, 需要退出充电流程
            bsp_charge_leakage_set(1, 0);
            if (chbox_sta == 1) {                                       //dc_in = 0, 且inbox = 0
                sys_cb.outbox_pwron_flag = 1;
            }
            return 2;
        }
#endif
        if (chbox_sta == 1) {                                           //dc_in = 0, 且inbox = 0
            bsp_charge_leakage_set(1, 0);
            if (chbox_cb.out2pwr_en) {                                  //拿起开机
                sys_cb.outbox_pwron_flag = 1;
                return 2;
            }
            return 1;
        } else if (chbox_sta == 2) {                                    //dc_in = 0, inbox = 1, 仓内维持电压状态，需要关机
#if CHARGE_DELAY_OPEN_LEAKAGE_EN
            return 2;
#endif
            bsp_charge_inbox_wakeup_enable();                           //INBOX Wakeup enable
            saradc_exit(adc_cb.channel);
            bsp_charge_tebcnt_calibration_stop();
            sfunc_pwrdown(3);
        }
    } else {
        chbox_cb.dc_in = 1;
        chbox_cb.out2pwr_en = xcfg_cb.ch_out_auto_pwron_en;
        inbox_cb.off_cnt = 0;
        inbox_cb.on_cnt = 0;
        bsp_charge_leakage_set(1, 0);
        chbox_cb.ticks = tick_get();
        if ((chbox_cb.counter % 1000) == 0) {                           //5秒
            bsp_charge_bcnt_calibration((xcfg_cb.chbox_smart_en) ? 100 : 500);
        } else if (chbox_cb.counter == 400) {
            bsp_charge_bcnt_calibration((xcfg_cb.chbox_smart_en) ? 100 : 150);
        }
    }
    return 0;
}

u32 bsp_charge_box_process(void)
{
    u32 charge_sta = bsp_charge_box_process_do();
    bsp_charge_tebcnt_calibration();
    return charge_sta;
}

void bsp_charge_box_enter(u32 out_auto_pwron)
{
    memset(&chbox_cb, 0, sizeof(chbox_cb));
    chbox_cb.out2pwr_en = out_auto_pwron;
#if (UART0_PRINTF_SEL != PRINTF_NONE)
    xcfg_cb.charge_lowpwr_en = 0;
#endif
    chbox_cb.pwrsave_en = xcfg_cb.charge_lowpwr_en + 1;
    bsp_charge_inbox_chk_init(20, 120);                                 //inbox online检测长点，等待VUSB电容放电
    if (!xcfg_cb.ch_box_type_sel) {                                     //短暂掉0V的仓
        inbox_cb.off_delay = 100+(u16)xcfg_cb.chbox_out_delay*20;       //inbox offline check 500ms + n*100ms
    }
	chbox_cb.ticks = tick_get();
	bsp_bcnt_temp_calibration_start();
	bsp_charge_power_save();
}

void bsp_charge_box_reinit(void)
{
    sys_cb.inbox_wken_flag = 0;
    bsp_charge_inbox_chk_init(10, 20);
    chbox_cb.leakage_sta = 0;
    if (bsp_chbox_type_is_maintain_volt()) {
        chbox_cb.inbox_rtt_check = 1;
    }
}

void bsp_charge_box_exit(void)
{
    bsp_charge_power_recover();
#if VUSB_TBOX_NEW_QTEST_EN
    chbox_cb.pwrsave_en = 0;
#endif
    bsp_charge_box_reinit();
    bsp_charge_tebcnt_calibration_stop();
}

#if CHARGE_NTC_DETECT
AT(.com_text.charge)
u8 charge_ntc_get_sta(void)
{
    if (sys_cb1.chg_ntc_stop) {
        return 1;                                   // 1: 关闭充电灯，2：开启充电灯
    }
    return 0;
}

//AT(.com_rodata.text)
//const char charge_ntc_check_str[] = "charge enable\n";
//
//AT(.com_rodata.text)
//const char charge_ntc_check_str1[] = "charge disable\n";

AT(.com_text.charge)
void charge_ntc_check(void)
{
    static int chg_en_cnt = 0;
    static int chg_dis_cnt = 0;

    if (xcfg_cb.sys_ntc_en && xcfg_cb.charge_en) {
        if (adc_cb.ntc_val <= CHARGE_NTC_DETECT_VAL) {
            if (chg_dis_cnt < 20) {
                chg_dis_cnt++;
            } else if (chg_dis_cnt == 20) {
                chg_dis_cnt = 30;
                RTCCON8 = (RTCCON8 & ~BIT(6)) | BIT(1);     //disable charger function
                sys_cb1.chg_ntc_stop = 1;
//                printf(charge_ntc_check_str1);
            }
            chg_en_cnt = 0;
        } else {
            if (chg_en_cnt < 20) {
                chg_en_cnt++;
            } else if (chg_en_cnt == 20) {
                chg_en_cnt = 30;
                RTCCON8 = (RTCCON8 & ~BIT(1)) | BIT(6);     //enable charger function
                charge_stop(0);                             //enter charge
                sys_cb1.chg_ntc_stop = 0;
//                printf(charge_ntc_check_str);
            }
            chg_dis_cnt = 0;
        }
    }
}
#endif

#if CHARGE_TSEN_DETECT
//AT(.com_rodata.text)
//const char charge_tsen_check_str[] = "---> charge enable\n";
//
//AT(.com_rodata.text)
//const char charge_tsen_check_str1[] = "---> charge disable\n";
//
//AT(.com_rodata.text)
//const char charge_tsen_check_str2[] = "---> charge slow\n";

AT(.com_text.charge)
void charge_tsen_check(u8 change_cnt)
{
    static int chg_tsen_en_cnt = 0;
    static int chg_tsen_dis_cnt = 0;
    static int chg_tsen_slow_cnt = 0;

    if (xcfg_cb.charge_en) {
        if (sys_cb1.adc_tsen_val >= CHARGE_TSEN_STOP_VAL) {
            if (chg_tsen_dis_cnt < change_cnt) {
                chg_tsen_dis_cnt++;
            } else if (chg_tsen_dis_cnt == change_cnt) {
                chg_tsen_dis_cnt = change_cnt + 10;
                RTCCON8 = (RTCCON8 & ~BIT(6)) | BIT(1);         //disable charger function
//                printf(charge_tsen_check_str1);
            }
            chg_tsen_en_cnt = 0;
            chg_tsen_slow_cnt = 0;
        } else if (sys_cb1.adc_tsen_val < CHARGE_TSEN_SLOW_VAL) {
            if (chg_tsen_slow_cnt < change_cnt) {
                chg_tsen_slow_cnt++;
            } else if (chg_tsen_slow_cnt == change_cnt) {
                chg_tsen_slow_cnt = change_cnt + 10;
                charge_cfg.trick_curr = CHARGE_TSEN_SLOW_CURR;
                charge_cfg.const_curr = CHARGE_TSEN_SLOW_CURR;
                RTCCON8 = (RTCCON8 & ~BIT(1)) | BIT(6);         //enable charger function
                charge_stop(0);
//                printf(charge_tsen_check_str2);
            }
            chg_tsen_en_cnt = 0;
            chg_tsen_dis_cnt = 0;
        } else {
            if (chg_tsen_en_cnt < change_cnt) {
                chg_tsen_en_cnt++;
            } else if (chg_tsen_en_cnt == change_cnt) {
                chg_tsen_en_cnt = change_cnt + 10;
                charge_cfg.trick_curr = sys_cb1.trick_curr_bkp;
                charge_cfg.const_curr = sys_cb1.const_curr_bkp;
                RTCCON8 = (RTCCON8 & ~BIT(1)) | BIT(6);         //enable charger function
                charge_stop(0);
//                printf(charge_tsen_check_str);
            }
            chg_tsen_slow_cnt = 0;
            chg_tsen_dis_cnt = 0;
        }
    }
}
#endif
#endif // CHARGE_EN
