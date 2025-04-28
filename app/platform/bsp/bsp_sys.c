#include "include.h"

sys_cb1_t sys_cb1;
extern sys_cb_t sys_cb;

void user_tmr_set_enable(bool tmr5ms, bool tmr1ms);
void voltage_trim_value_init(void);
void mic_bias_var_init(void);
void bsp_xcfg_enable_default(void);
void bsp_get_xcfg_bt_name(void);
void tws_lr_xcfg_sel(void);
bool rtc_init(void);
void x26m_cap_tune(void);
void eeprom_init(void);
void e2p_var_init(void);
void bsp_bt_name_suffix(void);
void sw_reset_flag_get(void);
void pwrkey_2_hw_pwroff_detect(void);
void pwrkey_5s_on_check(void);
void mic_bias_trim(void);
void lt_charge_poweron(void);
void lt_charge_init(void);
void lt_charge_tx_kick(void);
void lt_charge_var_init(void);
void bsp_var_init_m(void);
void bsp_188led_gpio_init(void);
void bsp_188led_var_init(void);
void charge_box_ble_init(void);
void ble_charge_box_addr_set(void);
void led_pwm_scan(void);
void charge_inbox_delay_leakage_check(void);

extern const u16 sleep_time_tbl[22];
AT(.text.dongle)
const u8 tbl_secret_key[16] = {0x45, 0xDB, 0x98, 0x3A, 0xE2, 0x59, 0x7A, 0x35, 0x17, 0x66, 0xDD, 0x7D, 0x9F, 0x2F, 0xD2, 0x5D};

//timer tick interrupt(1ms)
AT(.com_text.timer)
void usr_tmr1ms_isr(void)
{
}

//timer tick interrupt(5ms)
AT(.com_text.timer)
void usr_tmr5ms_isr_hook(void)
{
    dac_fade_process();
    bsp_key_scan();
#if VUSB_SMART_VBAT_HOUSE_EN
    if (xcfg_cb.chbox_smart_en) {
        bsp_vhouse_cmd_ack();
    }
#endif
    pwrkey_2_hw_pwroff_detect();
    hall_sensor_detect();

#if USER_NTC
    sys_ntc_check();
#endif // USER_NTC

#if LINEIN_DETECT_EN
    linein_detect();
#endif // LINEIN_DETECT_EN

#if EARPHONE_DETECT_EN
    earphone_detect();
#endif // EARPHONE_DETECT_EN

#if BT_EARIN_DETECT_EN
    earin_detect();
#endif

#if BT_LIGHTNINIG_EN
    lt_uart_tx_auth_process();
#endif

#if CHARGE_TSEN_DETECT
    charge_tsen_check(20);
#endif

#if CHARGE_DELAY_OPEN_LEAKAGE_EN
    charge_inbox_delay_leakage_check();
#endif

    mic_bias_trim();

    //SYNC 50ms timer process
    if (bt_sync_tick()) {
        led_scan();
        led_sync();
    }
}

void uart0_mapping_sel(void)
{
    //等待uart0发送完成
    if(UART0CON & BIT(0)) {
        while (!(UART0CON & BIT(8)));
    }

    GPIOBDE  &= ~BIT(3);
    GPIOBPU  &= ~BIT(3);
    GPIOBDIR |= BIT(3);
    FUNCMCON0 = (0xf << 12) | (0xf << 8);           //clear uart0 mapping

#if (UART0_PRINTF_SEL == PRINTF_PA7)
    GPIOADE  |= BIT(7);
    GPIOAPU  |= BIT(7);
    GPIOADIR |= BIT(7);
    GPIOAFEN |= BIT(7);
    FUNCMCON0 = URX0MAP_TX | UTX0MAP_PA7;           //RX0 Map To TX0, TX0 Map to G1
#elif (UART0_PRINTF_SEL == PRINTF_PB3)
    GPIOBDE  |= BIT(3);
    GPIOBPU  |= BIT(3);
    GPIOBDIR |= BIT(3);
    GPIOBFEN |= BIT(3);
    FUNCMCON0 = URX0MAP_TX | UTX0MAP_PB3;           //RX0 Map To TX0, TX0 Map to G3
#elif (UART0_PRINTF_SEL == PRINTF_PE7)
    GPIOEDE  |= BIT(7);
    GPIOEPU  |= BIT(7);
    GPIOEDIR |= BIT(7);
    GPIOEFEN |= BIT(7);
    FUNCMCON0 = URX0MAP_TX | UTX0MAP_PE7;           //RX0 Map To TX0, TX0 Map to G4
#elif (UART0_PRINTF_SEL == PRINTF_PF2)
    GPIOFDE  |= BIT(2);
    GPIOFPU  |= BIT(2);
    GPIOFDIR |= BIT(2);
    GPIOFFEN |= BIT(2);
    FUNCMCON0 = URX0MAP_TX | UTX0MAP_PF2;           //RX0 Map To TX0, TX0 Map to G4
#elif (UART0_PRINTF_SEL == PRINTF_PE0)
    GPIOEDE  |= BIT(0);
    GPIOEPU  |= BIT(0);
    GPIOEDIR |= BIT(0);
    GPIOEFEN |= BIT(0);
    FUNCMCON0 = URX0MAP_TX | UTX0MAP_PE0;           //RX0 Map To TX0, TX0 Map to G4
#endif
}

//开user timer前初始化的内容
void bsp_var_init(void)
{
    memset(&sys_cb1, 0, sizeof(sys_cb1));
    memset(&sys_cb, 0, sizeof(sys_cb));
    sys_cb.loudspeaker_mute = 1;
    sys_cb.ms_ticks = tick_get();                               //用于5S开机判断
    sys_cb.pwrkey_5s_check = 1;
    sys_cb.vol_max = (xcfg_cb.vol32_en) ? 32 : 16;
    sys_cb.hfp2sys_mul = (sys_cb.vol_max + 2) / 16;
    if (SYS_INIT_VOLUME > (sys_cb.vol_max - 1)) {
        SYS_INIT_VOLUME = sys_cb.vol_max - 1;
    }
    if (SYS_RECOVER_INITVOL > (sys_cb.vol_max - 1)) {
        SYS_RECOVER_INITVOL = sys_cb.vol_max - 1;
    }
    if (WARNING_VOLUME > (sys_cb.vol_max - 1)) {
        WARNING_VOLUME = sys_cb.vol_max - 1;
    }
    sys_cb.lpwr_leave_vbat = 2;
    sys_cb.sleep_time = -1L;
    sys_cb.pwroff_time = -1L;
    if ((xcfg_cb.sys_sleep_time > 0) && (xcfg_cb.sys_sleep_time < 23)) {
        sys_cb.sleep_time = sleep_time_tbl[xcfg_cb.sys_sleep_time - 1];
    }
    if ((xcfg_cb.sys_off_time > 0) && (xcfg_cb.sys_off_time < 23)) {
        sys_cb.pwroff_time = sleep_time_tbl[xcfg_cb.sys_off_time - 1];
    }
    sys_cb.sleep_delay = -1L;
    sys_cb.pwroff_delay = -1L;
    sys_cb.sleep_en = 0;
    bt_clkn_rst = 1;
    sys_cb.adkey_ch = cfg_adcch_tbl[xcfg_cb.adkey_ch];
    sys_cb.ntc_adc_ch = cfg_adcch_tbl[xcfg_cb.ntc_adc_ch];

    sys_cb.loc_bat = 100 | BIT(7);                              //初始化耳机在仓电量默认100%
    sys_cb.loc_house_bat = 100;                                 //上电电池仓电量默认100%
    sys_cb.loc_house_state = BIT(0);                            //上电电池仓盖默认打开
    sys_cb.rem_bat = sys_cb.loc_bat;
    sys_cb.rem_house_bat = sys_cb.loc_house_bat;
    sys_cb.rem_house_state = sys_cb.loc_house_state;
    cfg_spiflash_speed_up_en = SPIFLASH_SPEED_UP_EN;

    memset(&f_aux, 0, sizeof(func_aux_t));
    memset(&f_bt, 0, sizeof(func_bt_t));

    plugin_var_init();
    key_var_init();
    msg_queue_init();

    device_init();

    sdadc_var_init();

    dac_cb_init();

    piano_var_init();

    mic_bias_var_init();

    e2p_var_init();

    charge_box_var_init();
    anc_var_init();

    lt_var_init();
    lt_charge_var_init();

#if LED_188LED_DISP_EN
    if(CHARGE_DC_IN()) {           //开机进充电提前初始化
        bsp_188led_gpio_init();
        bsp_188led_var_init();
    }
#endif
}

AT(.text.bsp.sys.init)
static void bsp_io_init(void)
{
    if (lt_sdq_valid) {
        GPIOAPU = BIT(7);
        GPIOADE = BIT(7);
        GPIOBDE = BIT(4);
    } else {
        GPIOAPU = 0;
        GPIOADE = 0;
        GPIOBDE = 0;
    }
    GPIOEDE = 0;
    GPIOFDE = 0;
    GPIOGDE = 0x3F; //MCP FLASH

#if (UART0_PRINTF_SEL == PRINTF_PA7)
    GPIOADE = BIT(7);
#elif (UART0_PRINTF_SEL == PRINTF_PB3)
    GPIOBDE = BIT(3);
#elif (UART0_PRINTF_SEL == PRINTF_PF2)
    GPIOFDE = BIT(2);
#elif (UART0_PRINTF_SEL == PRINTF_PE7)
    GPIOEDE = BIT(7);
#elif (UART0_PRINTF_SEL == PRINTF_PE0)
    GPIOEDE = BIT(0);
#endif
}

void bt_rf_update_ft_trim_value(void)
{
    if (sys_trim.ft_trim_flag) {
        s32 offset = 55 - (s32)xcfg_cb.rf_dig_gain0;
        xcfg_cb.rf_dig_gain0 = sys_trim.dig_gain - offset - 2;
        xcfg_cb.rf_dig_gain_sco = sys_trim.dig_gain - offset - 2;
    }
}

void bsp_xcfg_init(void)
{
    voltage_trim_value_init();                              //初始化CP Trim值
    bsp_xcfg_enable_default();                              //启动配置后,部分模块关闭时需要设置默认值
    update_set_qcheck_code();
    if(xcfg_cb.anc_en){
        xcfg_cb.dac_out_spr = 1;                            //anc使能后固定dac输出采样率48K
    }
    bt_rf_update_ft_trim_value();
    if (!xcfg_cb.lt_sa_en) {
        lt_sdq_valid = 0;
        SDQCON = 0;
    } else {
        if (!xcfg_cb.lt_chg_en) {
            SDQCON = 0x01;
        }
    }
    //更新蓝牙名称
    bsp_get_xcfg_bt_name();
    bsp_bt_name_suffix();
}


AT(.text.bsp.sys.init)
void bsp_sys_init(void)
{
    /// config
    bsp_xcfg_init();

    // io init
    bsp_io_init();

    // var init
    bsp_var_init();

    // power init
    pmu_init(BUCK_MODE_EN*xcfg_cb.buck_mode_en);

    // clock init
    adpll_init(DAC_OUT_SPR);                                //要放在pmu_init之后
    sys_clk_set(SYS_CLK_SEL);

    // peripheral init
    rtc_init();
#if SYS_PARAM_EEPROM
    eeprom_init();
#endif // SYS_PARAM_EEPROM
    param_init(sys_cb.rtc_first_pwron);

#if IODM_TEST_MODE
    iodm_init();
    iodm_reflash_bt_name();
#endif

    sw_reset_flag_get();
    tws_lr_xcfg_sel();                                      //TWS声道初始化, 放在param_init之后
    plugin_popup_auto_config();                             //popup初始化，放在param_init之后
    x26m_cap_tune();                                        //26M晶振电容配置

    clkgate_configure();

#if CHARGE_EN
    if (xcfg_cb.charge_en) {
        bsp_charge_init();
    }
#endif // CHARGE_EN

#if SYS_SUPPORT_DONGLE_EN
    dongle_check_key();
#endif

#if LINEIN_2_PWRDOWN_EN
    if (sys_cb.sleep_dac_en) {
        dac_power_off_pulldown_daclr();
    }
#endif

    led_init();
    key_init();

#if LED_188LED_DISP_EN
    bsp_188led_gpio_init();
    bsp_188led_var_init();
#endif

#if PWM_RGB_EN
    pwm_init();
#endif // PWM_RGB_EN
    plugin_init();

    /// enable user timer for display & dac
    user_tmr_set_enable(1, 1);

    led_power_up();
    motor_enable(MOTOR_PWRON, T_MOTOR_PWRON_SEL);         //开机马达振动

#if BT_LIGHTNINIG_EN
    if (xcfg_cb.lt_sa_en) {
        lt_uart_tx_auth_start();
        if (xcfg_cb.lt_chg_en) {
            lt_charge_init();
        }
    }
#endif

    bt_init();
    bt_work_mode_init();
    if (!xcfg_cb.bt_init_fast_dis) {                    //是否蓝牙提前初始化，可以加速回连
        func_bt_init();
    }
    dac_init();
#if VUSB_TBOX_QTEST_EN
    qtest_create_env();
#endif

#if ANC_EN
    bsp_anc_init();
	bsp_anc_set_mode(1);
#endif

    bsp_change_volume(sys_cb.vol);

#if LINEIN_2_PWRDOWN_EN
    if (xcfg_cb.linein_2_pwrdown_en) {
        delay_5ms(60);
        if (device_is_online(DEV_LINEIN)) {
            sys_cb.pwrdwn_tone_en = LINEIN_2_PWRDOWN_TONE_EN;
            func_cb.sta = FUNC_PWROFF;
            return;
        }
    }
#endif // LINEIN_2_PWRDOWN_EN

#if WARNING_POWER_ON
    if ((xcfg_cb.bt_outbox_voice_pwron_en) || (!sys_cb.outbox_pwron_flag)) {
        mic_bias_trim_w4_done_do();
        sys_warning_play(T_WARNING_POWER_ON, PIANO_POWER_ON);
    }
    sys_cb.outbox_pwron_flag = 0;
#endif // WARNING_POWER_ON

#if FUNC_AUX_EN
    if ((xcfg_cb.func_aux_en) && (!xcfg_cb.linein_2_pwrdown_en)) {
        if (device_is_online(DEV_LINEIN)) {
            func_cb.sta = FUNC_AUX;
            return;
        }
    }
#endif // FUNC_AUX_EN

    func_cb.sta = FUNC_BT;

#if EQ_DBG_IN_UART || EQ_DBG_IN_SPP
    eq_dbg_init();
#endif // EQ_DBG_IN_UART

}


