#include "include.h"


#define BT_PROFILE          (PROF_A2DP*BT_A2DP_EN) | (PROF_HFP*BT_HFP_EN) | (PROF_SPP*BT_SPP_EN) | (PROF_HID*BT_HID_EN) | (PROF_MAP*BT_MAP_EN)
#define A2DP_FEATURE        (A2DP_AVRCP_VOL_CTRL*BT_A2DP_VOL_CTRL_EN) | (A2DP_RECON_CTRL*BT_A2DP_RECON_EN | (A2DP_AVDTP_DELAY_REPORT))
#define HFP_FEATURE         (HFP_BAT_REPORT*BT_HFP_BAT_REPORT_EN) | (HFP_INBAND_RING_TONE*BT_HFP_INBAND_RING_EN) | (HFP_CALL_PRIVATE*BT_HFP_CALL_PRIVATE_FORCE_EN) | (HFP_RING_NUMBER_EN*BT_HFP_RING_NUMBER_EN)



AT(.rodata.bt_cfg)
const bt_cfg_t bt_cfg_tbl = {
    .pwrup_connect_times = BT_POWER_UP_RECONNECT_TIMES,
    .sup_to_connect_times = BT_TIME_OUT_RECONNECT_TIMES,
    .nor_connect_times  = BT_NOR_RECONNECT_TIMES,
    .tws_connect_times  = BT_TWS_RECONNECT_TIMES,
    .tws_sup_to_connect_times = BT_TWS_TIME_OUT_RECONNECT_TIMES,
    .scan_timeout       = BT_DISCOVER_TIMEOUT,      //待实现
    .profile            = BT_PROFILE,
    .max_acl_link       = BT_2ACL_EN+1,
    .a2dp_feature       = A2DP_FEATURE,
    .hfp_feature        = HFP_FEATURE,
    .vx_set             = 2,    //0=hv3, 1=ev3, 2=2ev3

    .dual_mode          = BT_DUAL_MODE_EN,
    .tws_mode           = BT_TWS_EN,
    .simple_pair_mode   = BT_SIMPLE_PAIR_EN,
    .scan_ctrl          = BT_DISCOVER_CTRL_EN,
    .supervision_to_dft = 0x7d00,         //timeout 20s
    .new_conn_timeout_add = 8,

    .sco_plc_en         = BT_PLC_EN,
    .sco_msbc_en        = BT_HFP_MSBC_EN,

    .remote_name_req_en = 0,
    .sdp_client_avrcp_en = 1,
};

extern char bt_name_buf[32];
extern char redial_buf[2][32];
extern char bt_new_local_name[64];
extern bt_cfg_t bt_cfg;

extern volatile uint8_t a2dp_media_index;
extern volatile uint8_t hfp_index;
extern uint8_t cfg_bt_sw_ver_build;
extern uint8_t cfg_bt_sw_ver_build;
extern uint8_t cfg_bt_tws_limit_pair_time;    //设置TWS上电、出仓配对限制时间, 单位200ms, 0为不限制（L、R规定时间内上电、出仓才能配对）
extern uint8_t cfg_bt_tws_slot_inc;
extern uint8_t cfg_bt_a2dp_media_discard;
extern uint8_t cfg_bt_work_mode;
extern uint16_t cfg_bt_tws_feat;

bool btstack_is_invalid_addr(bd_addr_t addr);
void bt_new_name_init(void);
void btstack_set_tws_info_hook(uint8_t *param);
bool bt_get_stored_a2dp_volume(u8 *vol);
void btstack_send_tws_vol_ctrl_status(void);
void btstack_volume_change(void);
u8 bt_call_need_switch(void);
bool bt_get_stored_a2dp_volume(u8 *vol);
void timer3_capture_isr(void);
void btstack_volume_change(void);
void hfp_at_kick(void);
void bt_map_kick(void);
void btstack_tws_sync_info(void);

AT(.rodata.sup_to)
const u8 sup_to_con_times_tbl[4] = {40, 80, 160, 0xff};

//返回非0 地址有效按地址链接，返回0地址无效
uint8_t bt_get_connect_addr(uint8_t *bd_addr)
{
    u8 addr[6] = {0x41,0x42,0x77,0x88,0x99,0xaa};
    memcpy(bd_addr,addr,6);
    return 1;
}

WEAK bool bt_fcc_test_is_en(void)
{
    return BT_FCC_TEST_EN * xcfg_cb.bt_fcc_en;
}

u8 bt_hid_get_compatibility(void)
{
    //return 0;   //耳机图标（带电量显示），不影响键盘使用，使用V+拍照，影响部分Android拍照，体验较好
    //return 1;   //耳机图标（带电量显示），影响ios键盘使用，使用V+和ENTER拍照，拍照兼容性好
    //return 2;   //键盘图标（IOS不带电量显示），影响ios键盘使用，使用V+和ENTER拍照，拍照兼容性好
    return xcfg_cb.bt_hid_mode_sel;
}

//const char *bt_get_pin_code(void)
//{
//    return "0000";
//}

//此函数返回需要回拨的号码，例如“10086”
const char * hfp_get_outgoing_number(void) {
    return "10086";
}

void bsp_bt_init_hook(void)
{
    memcpy(&bt_cfg, &bt_cfg_tbl, sizeof(bt_cfg_t));
	bt_cfg.sco_msbc_en = BT_HFP_MSBC_EN * xcfg_cb.bt_sco_msbc_en;


    bt_cfg.profile = (PROF_A2DP*BT_A2DP_EN*xcfg_cb.bt_a2dp_en) | (PROF_HFP*BT_HFP_EN*xcfg_cb.bt_sco_en)\
                    | (PROF_SPP*BT_SPP_EN*xcfg_cb.bt_spp_en) | (PROF_HID*BT_HID_EN*xcfg_cb.bt_hid_en) | (PROF_MAP*BT_MAP_EN);

    bt_cfg.a2dp_feature = (A2DP_AVRCP_VOL_CTRL*xcfg_cb.bt_a2dp_vol_ctrl_en) | A2DP_RESTORE_PLAYING | (A2DP_AVDTP_DELAY_REPORT*xcfg_cb.bt_a2dp_delay_report_en);

#if BT_RESOLVE_SIRI_STA_NOUPDATE_EN
    bt_cfg.hfp_feature = (HFP_BAT_REPORT*xcfg_cb.bt_hfp_bat_report_en) | (HFP_INBAND_RING_TONE*BT_HFP_INBAND_RING_EN)
                        | (HFP_CALL_PRIVATE*xcfg_cb.bt_hfp_private_en) | (HFP_RING_NUMBER_EN*xcfg_cb.bt_hfp_ring_number_en) | HFP_SIRI_CMD;
#else
    bt_cfg.hfp_feature = (HFP_BAT_REPORT*xcfg_cb.bt_hfp_bat_report_en) | (HFP_INBAND_RING_TONE*BT_HFP_INBAND_RING_EN)
                        | (HFP_CALL_PRIVATE*xcfg_cb.bt_hfp_private_en) | (HFP_RING_NUMBER_EN*xcfg_cb.bt_hfp_ring_number_en) ;
#endif

    if (xcfg_cb.bt_recon_play_dis) {
        bt_cfg.a2dp_feature &= ~A2DP_RESTORE_PLAYING;
    }
    if (xcfg_cb.bt_a2dp_avrcp_play_status) {
        bt_cfg.a2dp_feature |= A2DP_AVRCP_PLAY_STATUS;
    }
    cfg_bt_hid_feature = HID_CONN_IOS_ONLY * xcfg_cb.bt_hid_ios_only_en;
    cfg_bt_sw_ver_major = 14;   //bt6.0
    bt_cfg.tws_mode = xcfg_cb.bt_tws_en*BT_TWS_EN;
    if (xcfg_cb.bt_tws_pair_mode > 1) {
        bt_cfg.res[0] |= BT_FLAG0_TWS_SCAN;
    } else if (xcfg_cb.bt_tws_pair_mode == 1 && xcfg_cb.bt_tws_fast_con_en) {
        bt_cfg.res[0] |= BT_FLAG0_TWS_FAST_PAIR;
    }
    bt_cfg.dual_mode = 0;//BT_DUAL_MODE_EN * xcfg_cb.ble_en;
#if BT_2ACL_EN
    bt_cfg.max_acl_link = BT_2ACL_EN * xcfg_cb.bt_2acl_en + 1;
#endif
    cfg_bt_tws_feat = 0;
    if (bt_cfg.tws_mode) {
        if (xcfg_cb.bt_tws_public_addr) {
            cfg_bt_tws_feat |= BT_FEAT_NEGO_PUBLIC_ADDR;
        }
    }

#if BT_LINK_FILTER
    cfg_bt_link_filter = 1;
#endif

    cfg_bt_tws_limit_pair_time = 0;
    bt_cfg.tws_fast_con_en = xcfg_cb.bt_tws_fast_con_en;
    if (bt_cfg.tws_fast_con_en) {
        bt_cfg.tws_connect_times = BT_TWS_RECONNECT_TIMES * 5;                      //0.25s * n > 5s
        bt_cfg.tws_sup_to_connect_times = BT_TWS_TIME_OUT_RECONNECT_TIMES * 5;
        cfg_bt_tws_limit_pair_time = xcfg_cb.bt_tws_limit_pair_time;
    }
    bt_cfg.sup_to_connect_times = sup_to_con_times_tbl[xcfg_cb.bt_tout_reconn_times];
	if (BT_FCC_TEST_EN && xcfg_cb.bt_fcc_en) {
        cfg_bt_work_mode  = MODE_FCC_TEST;
	}

#if BT_QUICK_TEST_EN
    bt_cfg.bt_quick_test = BT_QUICK_TEST_EN*xcfg_cb.iphone_head_quick_test_en;
#endif // BT_QUICK_TEST_EN
    bt_cfg.new_conn_timeout_add = 8;
    cfg_bt_tws_slot_inc = 0;
    memset(bt_new_local_name, 0, sizeof(bt_new_local_name));
    sys_cb.ear_menu_en = 0;
    sys_cb.loc_ear_sta = 0x1;           //有入耳检测时，改为实际状态（0=戴入，1=取下）
    sys_cb.rem_ear_sta = 0x1;           //有入耳检测时，改为实际状态（0=戴入，1=取下）
    sys_cb.nr_bits = 0x7;               //bit0=NR disable, bit1=ANC, bit2=transparency
    sys_cb.nr_sta = 1;                  //NR disable


    ble_house_adv_init();

    memset(&f_bt, 0, sizeof(func_bt_t));
    f_bt.disp_status = 0xfe;
    f_bt.need_pairing = 1;  //开机若回连不成功，需要播报pairing
    f_bt.hid_menu_flag = 1;
	f_bt.tick_vbat = tick_get();
#if BT_DISP_LOW_VBAT_EN
    hfp_get_bat_level();                        //更新一下f_bt.loc_vbat
    f_bt.rem_vbat = 0xff;
#else
    f_bt.loc_vbat = hfp_get_bat_level();
#endif

#if FUNC_BTHID_EN
    if (is_bthid_mode()) {
        bt_cfg.max_nr_link_key = 1;
        bt_cfg.profile = PROF_HID;
        bt_cfg.max_acl_link = 1;
        bt_cfg.dual_mode = 0;
        f_bt.hid_menu_flag = 0;
    } else
#endif // FUNC_BTHID_EN
    {
        bt_cfg.max_nr_link_key = BT_MAX_NR_LINK_KEY;
    }
}

void bt_work_mode_init(void)
{
    if (xcfg_cb.bb_dut_test_en) {
        cfg_bt_work_mode = MODE_NORMAL_DUT;
    } else {
        cfg_bt_work_mode = MODE_NORMAL;
    }
}

//客户控制tws scan状态使能
#if BT_TWS_CUSTOMER_CONTROL_TWSSCAN_STA_EN
bool bt_tws_customer_control_tws_scan_sta_en(void)
{
    return true;
}
#endif

//tws一些同步信息，例如EQ, 语言等，param最大是12byte
bool bt_get_tws_info(uint8_t *param)
{
#if LANG_SELECT >= LANG_EN_ZH
    if(xcfg_cb.lang_id >= LANG_EN_ZH) {
        param[0] = sys_cb.lang_id & 0x0f;  //bit0~3
    }
#endif
#if EQ_MODE_EN
    param[0] |= (u8)sys_cb.eq_mode<<4;
#endif
    param[1] = sys_cb.hfp_vol;      //同步初始通话音量
    param[5] = sys_cb.tws_force_channel;
    param[6] = sys_cb.loc_ear_sta;
    param[8] = f_bt.loc_vbat;
    return true;
}

void bt_set_tws_info(uint8_t *param)
{
    uint8_t tmp = 0;

#if LANG_SELECT >= LANG_EN_ZH
    tmp = (param[0] & 0x0f);
    if(xcfg_cb.lang_id >= 2 && tmp < 2 && tmp != sys_cb.lang_id) {  //bit0~3
        sys_cb.lang_id = tmp;
        msg_enqueue(EVT_BT_SET_LANG_ID);
    }
#endif
#if EQ_MODE_EN
    tmp = (param[0] & 0xf0)>>4;
    if(tmp < 6 && tmp != sys_cb.eq_mode) {  //bit4~7
        sys_cb.eq_mode = tmp;
        msg_enqueue(EVT_BT_SET_EQ);
    }
#endif
    sys_cb.hfp_vol = param[1];      //同步初始通话音量

    sys_cb.rem_force_channel = param[5];
    if (sys_cb.rem_ear_sta != param[6]) {
        sys_cb.rem_ear_sta = param[6];
        msg_enqueue(EVT_BT_TWS_EAR_STA);
    }
    f_bt.rem_vbat = param[8];
}

#if BT_RF_POWER_BALANCE_EN
void btmdm_enable(void);
void btmdm_disable(void);
//使用空闲的IO,配置内部300欧上下拉,消耗电流来实现btrf tx,rx及空闲时电流不均衡问题
#define GPIOA_TEST_BIT  (BIT(2)|BIT(3)|BIT(4)|BIT(5)|BIT(6))

void rf_current_supply(void);
void rf_current_recover(void);
void bt_set_reconnect_txpwr(u8 gain);

static volatile bool bt_reconnect_flag;
static volatile u8 btrf_power_level = 0;

AT(.com_rodata.debug)
const char haha_debug[] = "**";
AT(.com_text.btrf_pwrbalance)
static void btrf_power_balance_level_set(u8 level)   //unit 3.3V/600 = 5.5mA
{
//    printf(haha_debug);
    GPIOAPU300 &= ~GPIOA_TEST_BIT;
    GPIOAPD300 &= ~GPIOA_TEST_BIT;
    if (0 == level) {
        return;
    }
    if(level >= 1){
        GPIOAPU300 |= BIT(3);
        GPIOAPD300 |= BIT(3);
    }
    if(level >= 2) {
        GPIOAPU300 |= BIT(4);
        GPIOAPD300 |= BIT(4);
    }
    if(level >= 3) {
        GPIOAPU300 |= BIT(5);
        GPIOAPD300 |= BIT(5);
    }
    if(level >= 4) {
        GPIOAPU300 |= BIT(6);
        GPIOAPD300 |= BIT(6);
    }
    if(level >= 5){
        GPIOAPU300 |= BIT(2);
        GPIOAPD300  |= BIT(2);
    }
}


AT(.com_text.btrf_pwrbalance)
void timer3_isr(void)
{
    if (TMR3CON & BIT(16)) {
        TMR3CPND = BIT(16);              //Clear Pending
        TMR3CON &=~ BIT(7);
        btrf_power_balance_level_set(btrf_power_level);

    }
    timer3_capture_isr();

}

AT(.com_text.btrf_pwrbalance)
static void btrf_power_balance_level_delay_set(u32 delay_us, u8 level)   //unit us
{
    btrf_power_level = level;
    TMR3CNT = 0;
    TMR3PR = delay_us;
    TMR3CPND = BIT(9);
    TMR3CON |=  BIT(7);
}

 void timer3_init(void)
{
//    printf("PICCON %x\n",PICCON);
    CLKGAT1 |= BIT(8);
    u8 div_sel = 2;
    u32 tim3pr_1s = 26000000/div_sel - 1;
    TMR3CON = 0;

	TMR3CNT = 0;
	TMR3PR  = tim3pr_1s;
	TMR3CON |= BIT(7);                          //Timer works in Timer Mode，Timer overflow interrupt enable
    TMR3CON |= BIT(0) ;                          //Timer enable
    PICCON |= BIT(2);

    PICPR|= BIT(IRQ_TMR3_VECTOR);
    PICEN |= BIT(IRQ_TMR3_VECTOR);
}

static void btrf_power_balance_init(void)
{
    printf("%s\n",__func__);
    timer3_init();
    GPIOADE |= GPIOA_TEST_BIT;
    GPIOAFEN &= ~GPIOA_TEST_BIT;
    GPIOADIR |= GPIOA_TEST_BIT;
    btmdm_enable();
}

static void btrf_power_balance_exit(void)
{
    printf("%s\n",__func__);
    TMR3CON = 0;
    GPIOAPU300 &= ~GPIOA_TEST_BIT;
    GPIOAPD300 &= ~GPIOA_TEST_BIT;
    GPIOADE &=~GPIOA_TEST_BIT;
    btmdm_disable();
}

AT(.com_text.btrf_pwrbalance)
static void btrf_power_balance_isr(u32 rf_sta)
{
    TMR3CON |= BIT(7);
    if(rf_sta & BIT(8)) {			//tx on
        btrf_power_balance_level_delay_set(750,0);  //TX开始后,TX延时一段时间才真正耗电,这里延时一段时间后才降低功耗
    } else if(rf_sta & BIT(9)){	    //tx down
        btrf_power_balance_level_delay_set(120,5);  //TX结束,增加耗电
    } else if(rf_sta & BIT(10)){	//rx on
        btrf_power_balance_level_set(2);
    } else if(rf_sta & BIT(11)){	//rx down
        btrf_power_balance_level_set(5);           //RX结束,增加耗电
    }
}


WEAK void bt_power_balance_reconnect(u8 start)
{
    bt_reconnect_flag = start;
    if (start) {
        bt_set_reconnect_txpwr(55);
        rf_current_supply();
        btrf_power_balance_init();
    } else {
        btrf_power_balance_exit();
        rf_current_recover();
		bt_set_reconnect_txpwr(xcfg_cb.rf_dig_gain0);
    }
}

AT(.com_text.isr.txrx)
void bb_rf_ext_ctl_cb(u32 rf_sta)
{
    if (bt_reconnect_flag) {
        btrf_power_balance_isr(rf_sta);
    }

}
#endif


void bt_emit_notice_init_finish(void)
{
    u8 opcode = 0;
    u8 scan_status = 0x03;

    //是否使能vusb delay reset?
    if (xcfg_cb.charge_dc_reset) {
        if (xcfg_cb.bt_tswi_en || (!(RTCCON & BIT(6)))) {
            RTCCON &= ~BIT(6);
            vusb_delay_rst_en(8);                               //soft kick vusb延时8秒复位使能
            sys_cb.dc_rst_flag = 1;
        }
    }

#if BT_TWS_EN
    if(xcfg_cb.bt_tws_en && (bt_cfg.res[0] & BT_FLAG0_TWS_SCAN)) {
        bt_tws_set_scan(0x3);                                   //手动配对，默认只需要按一只耳机。如果需要左右耳都双击才配对，此处改为2，双击按键时改成3
    }
#endif

    if (cfg_bt_work_mode == MODE_BQB_RF_BREDR || get_qtest_mode()) {
        opcode = 1;                                             //测试模式，不回连，打开可被发现可被连接
#if BT_PWRKEY_5S_DISCOVER_EN
    } else if(!bsp_bt_w4_connect()) {
        opcode = 1;                                             //长按5S开机，不回连，打开可被发现可被连接
#endif
    } else {
        if (bt_cfg.res[0] & BT_FLAG0_TWS_FAST_PAIR) {           //tws不回连, 直接进入自动配对
            opcode = 2;
        }
        if (bt_nor_get_link_info(NULL)) {
            scan_status = 0x02;                                 //有回连信息，不开可被发现
        }
    }
    bt_start_work(opcode, scan_status);
}

void bt_emit_notice_disconnect(u8 *param)
{
#if VUSB_TBOX_QTEST_EN
    if(get_qtest_mode()) {
        if(qtest_feature_is_btdisconnet_pwroff()){
            msg_enqueue(EVT_QTEST_PICKUP_PWROFF);
        }else if(!qtest_feature_is_pickup_pwroff()) {
            sw_reset_kick(SW_RST_FLAG);
        }
    }
#endif
    f_bt.tws_status &= ~0x01;
    f_bt.warning_status |= BT_WARN_DISCON;
    bsp_bt_redial_reset(param[0] & 0x01);
    f_bt.popup_delay_dis = 0;
    sys_cb.popup_bat_hide = 0;
    sys_cb.menu_det_dis = 0;
#if BT_A2DP_DEFAULT_VOLUME_EN
    if (!bt_is_ios_device()) {
        sys_cb1.a2dp_default_volume = 0;            //断开清flag
    }
#endif
    delay_5ms(5);
}

void bt_emit_notice_connected(u8 *param)
{
    f_bt.warning_status |= BT_WARN_CON;
    f_bt.tws_status |= 0x01;
    bsp_bt_redial_reset(param[0] & 0x01);
#if BT_PWRKEY_5S_DISCOVER_EN
    bsp_bt_pwrkey5s_clr();
#endif // BT_PWRKEY_5S_DISCOVER_EN
#if BT_A2DP_STORE_VOL_EN
    u8 volume;
    if (bt_get_stored_a2dp_volume(&volume)) {
        if (volume != sys_cb.vol) {
            bsp_set_volume(volume);
            btstack_volume_change();
        }
    }
#endif
    delay_5ms(5);
}

void bt_emit_notice_tws_connected(u8 *param)
{
    u8 reason;
#if VUSB_SMART_VBAT_HOUSE_EN
    if (xcfg_cb.chbox_smart_en) {
        bsp_vhouse_update_sta();
    } else
#endif
    {
        ble_bat_tws_sync();
    }
    reason = param[0];
    f_bt.tws_status |= reason & 0xc0;
    if (reason & 0x08) {
        f_bt.warning_status |= BT_WARN_TWS_CON;     //无连接提示音
    } else if(reason & 0x80) {
        f_bt.warning_status |= BT_WARN_TWS_SCON;    //TWS连接提示音
    } else if(reason & 0x40) {
        f_bt.warning_status |= BT_WARN_TWS_MCON;    //TWS连接提示音
    }
    if (bt_nor_is_connected()) {
        f_bt.tws_status |= 0x01;                    //已连接手机
        btstack_send_tws_vol_ctrl_status();
    }

    if ((f_bt.tws_status & 0xc1) == 0x41) {         //主耳且已连接手机
        bt_msg_tws_ring_sync();
    }
    if (atmosphere_led_is_on()) {                   //同步到对方
        btstack_tws_atmos_led();
    }
#if BT_DISP_LOW_VBAT_EN
    sys_cb1.disp_low_vbat_ticks = 0;
    if (bt_tws_is_slave()) {
        btstack_tws_sync_info();                    //主耳也要同步一下
    }
#endif
}

void bt_emit_notice(uint evt, u32 param)
{
    switch(evt) {
    case BT_NOTICE_INIT_FINISH:
        bt_emit_notice_init_finish();
        break;

    case BT_NOTICE_DISCONNECT:
        bt_emit_notice_disconnect((u8 *)param);
        break;

    case BT_NOTICE_CONNECTED:
        bt_emit_notice_connected((u8 *)param);
#if BT_RF_POWER_BALANCE_EN
        bt_power_balance_reconnect(0);
#endif
        break;

    case BT_NOTICE_TWS_DISCONNECT:
        f_bt.tws_status &= ~0xc0;
        f_bt.warning_status |= BT_WARN_TWS_DISCON;
#if BT_DISP_LOW_VBAT_EN
        f_bt.rem_vbat = 0xff;
#endif
        break;

    case BT_NOTICE_TWS_CONNECTED:
#if BT_RF_POWER_BALANCE_EN
        if(bt_tws_is_slave()){
            bt_power_balance_reconnect(0);
        }
#endif
        bt_emit_notice_tws_connected((u8 *)param);
        break;

    case BT_NOTICE_TWS_RES_PLAY:
        if (*(u8 *)param <= TWS_RES_RING && !f_bt.ring_sta) {
            break;
        }
        if (*(u8 *)param < TWS_RES_MAX) {
            sys_cb.tws_res_brk = 1;    //打断当前提示音
        }
        tws_res_add(*(u8 *)param, *(u32 *)(param+1));
        break;

//    case BT_NOTICE_LOSTCONNECT:
//        msg_enqueue(EVT_BT_LOSTCONNECT);
//        break;
//
//    case BT_NOTICE_RECON_FINISH:
//        if (param == 0) {
//            msg_enqueue(EVT_BT_LOSTCONN_END);
//        }
//        break;

//    case BT_NOTICE_DISCONNECT_STATUS:
//        if (((u8 *)param)[0] == 0x08) {
//            //超距断连
//        }
//        break;

    case BT_NOTICE_SCO_KILL:
#if BT_HFP_CALL_EARPHONE_EN
        if (bt_call_need_switch()) {
            sys_cb1.call_need_switch = 1;
        }
#endif
        break;

    case BT_NOTICE_INCOMING:
    case BT_NOTICE_RING:
    case BT_NOTICE_OUTGOING:
    case BT_NOTICE_CALL:
        break;

    case BT_NOTICE_SET_SPK_GAIN:
        if(param != sys_cb.hfp_vol) {
            sys_cb.hfp_vol = param;
            msg_enqueue(EVT_HFP_SET_VOL);
        }
        break;

    case BT_NOTICE_MUSIC_PLAY:
#if (BT_A2DP_DEFAULT_VOLUME_EN && !BT_A2DP_IOS_DEFAULT_VOLUME_FIX)
        if (bt_is_ios_device()) {
            if (sys_cb1.a2dp_default_volume == 2) {
                sys_cb1.a2dp_default_volume = 3;
                sys_cb1.a2dp_default_volume_ticks = tick_get();
            }
        }
#endif
        msg_enqueue(EVT_A2DP_MUSIC_PLAY);
        break;

    case BT_NOTICE_MUSIC_STOP:
        msg_enqueue(EVT_A2DP_MUSIC_STOP);
        break;

    case BT_NOTICE_MUSIC_CHANGE_VOL:
        if(param == 0) {
            msg_enqueue(KU_VOL_DOWN);
        } else {
            msg_enqueue(KU_VOL_UP);
        }
        break;

#if BT_HID_EN
    case BT_NOTICE_TWS_HID_SHUTTER:
        if (param == HID_KEY_VOL_UP) {
            msg_enqueue(EVT_TWS_HID_VOLUP);
        } else if (param == HID_KEY_VOL_DOWN) {
            msg_enqueue(EVT_TWS_HID_VOLDOWN);
        }
        break;
#endif // BT_HID_EN

    case BT_NOTICE_TWS_INFO:
        bt_set_tws_info((u8 *)param);
        break;

        //TWS自定义按键,通过bt_tws_user_key函数发送
    case BT_NOTICE_TWS_USER_KEY:
        if (param == TWS_USER_KEY_RING_STOP) {
            f_bt.ring_stop = 1;
        } else if (param == TWS_USER_KEY_ATMOS) {
            atmosphere_led_sta_set(1);
        }
        break;

    case BT_NOTICE_SET_QTEST_CH:
        if (param && !sys_cb.qtest_force_channel) { //tws检查下qtest固定的声道信息(不能保存到RTCRAM, 否则会传染)
            sys_cb.qtest_force_channel = param;
            sys_cb.tws_force_channel = param;
        }
        break;

    case BT_NOTICE_TWS_SET_VOL:
        param = (param+1) * VOL_MAX / 128;
        sys_cb.vol = param;
        if (sys_cb.vol > VOL_MAX) {
            sys_cb.vol = VOL_MAX;
        }
        msg_enqueue(EVT_TWS_SET_VOL);
        break;

    case BT_NOTICE_MUSIC_SET_VOL:
#if BT_A2DP_DEFAULT_VOLUME_EN
        if (sys_cb1.a2dp_default_volume == 2) {
            if (!bt_is_ios_device()) {
                sys_cb1.a2dp_default_volume = 0;
                sys_cb1.a2dp_default_volume_ticks = 0;
                msg_enqueue(EVT_A2DP_DEFAULT_VOLUME);
                break;
            }
        }
#endif
        param = (param+1) * VOL_MAX / 128;
        if(param != sys_cb.vol) {
            sys_cb.vol = param;
            if (sys_cb.vol > VOL_MAX) {
                sys_cb.vol = VOL_MAX;
            }
            msg_enqueue(EVT_A2DP_SET_VOL);
        }
        break;

#if BT_HID_MENU_EN
    case BT_NOTICE_HID_CONN_EVT:
        bt_emit_notice_hid_con(param);
        break;
#endif // BT_HID_MENU_EN

    case BT_NOTICE_FIRST_CONNECT:
#if BT_A2DP_DEFAULT_VOLUME_EN
        sys_cb1.a2dp_default_volume = 1;
#endif
#if BT_A2DP_IOS_DEFAULT_VOLUME_FIX
        sys_cb1.delay_send_vol_ticks = tick_get();
#endif
        break;

    case BT_NOTICE_A2DP_VOL_CTRL:
#if BT_A2DP_DEFAULT_VOLUME_EN
        if (sys_cb1.a2dp_default_volume == 1) {
            sys_cb1.a2dp_default_volume = 2;
            sys_cb1.a2dp_default_volume_ticks = tick_get();
        }
#endif
        break;

    case BT_NOTICE_TWS_GET_MAP_TIME:
        sys_cb1.tws_get_map_time = 1;
        break;

        case BT_NOTICE_CONNECT_START:
#if BT_RF_POWER_BALANCE_EN
        printf("BT_NOTICE_CONNECT_START\n");
        bt_power_balance_reconnect(1);
#endif
        break;

    case BT_NOTICE_CONNECT_FAIL:

#if BT_RF_POWER_BALANCE_EN
        printf("BT_NOTICE_CONNECT_FAIL\n");
        bt_power_balance_reconnect(0);
#endif
        break;

    case BT_NOTICE_TWS_CONNECT_START:
#if BT_RF_POWER_BALANCE_EN
		 bt_power_balance_reconnect(1);
         printf("BT_NOTICE_TWS_CONNECT_START\n");
#endif
        break;

    case BT_NOTICE_TWS_CONNECT_FAIL:
#if BT_RF_POWER_BALANCE_EN
	 	bt_power_balance_reconnect(0);
        printf("BT_NOTICE_TWS_CONNECT_FAIL\n");
#endif
        break;

    }
}

//TWS断连状态
void bt_notice_tws_disconnect_status(u32 param)
{
#if BT_TWS_SLAVE_LOSTCONN_WARNING_EN
    if (((u8 *)param)[0] == 0x08) {                 //TWS超距断连
        if (bt_tws_is_slave() && bt_nor_is_connected()) {
            msg_enqueue(EVT_TWS_SLAVE_LOSTCONN);
        }
    }
#endif
}

//调用后可动态更新蓝牙名字
void updata_bt_name(const char *bt_name)
{
    lm_set_name_buf(bt_name, 32);
    bt_send_msg(BT_MSG_RESET_BT_NAME);
}

const char *bt_get_local_name(void)
{
#if FUNC_BTHID_EN
    if (is_bthid_mode()) {
        return bt_name_buf;         //可修改独立HID模式的蓝牙名称
    }
#endif
    return bt_name_buf;
}


#if BT_TWS_FIX_LR_SAME_CH_MAC_EN
bool bt_tws_set_lr_same_ch_mac_is_en(void)
{
    return BT_TWS_FIX_LR_SAME_CH_MAC_EN && (xcfg_cb.bt_tws_public_addr == 0);
}

bool bt_tws_is_mac_channel(void)
{
    if(sys_cb.tws_force_channel == BT_TWS_SET_MAC_CH && (bt_tws_set_lr_same_ch_mac_is_en())){
        return true;
    }
    return false;
}
#endif

void bt_get_local_bd_addr(u8 *addr)
{
    if(qtest_is_loading_local_addr()){
        memcpy(addr, xcfg_cb.bt_addr, 6);
        return;
    }
#if BT_TWS_FIX_LR_SAME_CH_MAC_EN
    if(sys_cb.tws_force_channel == BT_TWS_SET_MAC_CH && (bt_tws_set_lr_same_ch_mac_is_en())){
        memcpy(addr, xcfg_cb.bt_addr, 6);
#if IODM_TEST_MODE
        if(bt_get_qr_addr(addr)){
            return;
        }
#endif
        return;
    }
#endif

    if (!bt_get_master_addr(addr)) {
#if IODM_TEST_MODE
        if(bt_get_qr_addr(addr)){
            return;
        }
#endif
        memcpy(addr, xcfg_cb.bt_addr, 6);
    }

#if FUNC_BTHID_EN
    if (is_bthid_mode()) {
        addr[5] ^= BIT(0);
    }
#endif
}


void bt_get_channel_mac_addr(u8 *addr)
{
#if BT_TWS_FIX_LR_SAME_CH_MAC_EN
    if(sys_cb.tws_force_channel ==  BT_TWS_SET_MAC_CH) {
        bt_get_local_bd_addr(addr);
    }
    return ;
#endif
}
void bt_get_link_info(void *buf, u16 addr, u16 size)
{
//    printf("bt_read: %04x,%04x, %08lx\n", addr, size, BT_CM_PAGE(addr));
#if FUNC_BTHID_EN
    if (is_bthid_mode()) {
        param_read(buf, size*4, size); //only 1
    } else
#endif
    {
        param_read(buf, PAGE1(addr), size);
    }
//    print_r(buf, size);
}

void bt_put_link_info(void *buf, u16 addr, u16 size)
{
    if(qtest_is_loading_local_addr()){
        return;
    }
//    printf("bt_write: %04x,%04x, %08lx\n", addr, size, BT_CM_PAGE(addr));
//    print_r(buf, size);
#if FUNC_BTHID_EN
    if (is_bthid_mode()) {
        param_write(buf, size*4, size); //only 1
    } else
#endif
    {
        param_write(buf, PAGE1(addr), size);
    }
}

#if BT_TWS_CHARGE_DELAY_EN
u8 bsp_bt_tws_charge_delay(void)
{
    sys_cb1.charge_ticks = tick_get();
    dac_fade_out();
    dac_power_off();
    while(!tick_check_expire(sys_cb1.charge_ticks, 3000)) {
        WDT_CLR();
        delay_ms(5);
        if (!CHARGE_DC_IN()) {
            sys_cb1.dc_in_cnt++;
        } else {
            sys_cb1.dc_in_cnt = 0;
        }
        if (sys_cb1.dc_in_cnt == 10) {
            sys_cb1.dc_in_cnt = 0;
            dac_restart();
            dac_fade_in();
            return 0;
        }
    }
    return 1;
}
#endif

void ble_box_tws_switch(u8 en);
//耳机进仓充电
void bsp_bt_tws_switch_for_charge(void)
{
    printf("bsp_bt_tws_switch_for_charge\n");
    bt_clkn_rst = 0;
    atmosphere_led_sta_set(0);
	led_off();
	if (!sys_cb.charge_sta) {
        sys_cb.charge_sta = 1;
        charge_led_on();                                //先亮charge灯状态
	}

#if VUSB_TBOX_QTEST_EN
    //防止快测蓝牙还未断开就入仓,会一直反复进出充电函数
    if(bt_is_testmode() && get_qtest_mode()){
        sw_reset_kick(SW_RST_DC_IN);
    }
#endif

    func_bt_exit();
#if ANC_EN
    u8 anc_user_mode = sys_cb.anc_user_mode;
    bsp_anc_set_mode(0);
#endif
	dac_power_off();
	bsp_charge_box_enter(1);
	sys_cb.dc_rst_flag = 0;
	vusb_delay_rst_dis();
    while(1) {
        WDT_CLR();
        delay_ms(5);
        if (bsp_charge_box_process()) {
            break;
        }
    }
    bsp_charge_box_exit();
    sys_cb.charge_sta = 0;
    printf("dc out\n");
    charge_led_off(0);
    led_bt_init();
    func_bt_init();
    reset_sleep_delay();
    en_auto_pwroff();
    bt_clkn_rst = 1;
    dac_restart();

#if ANC_EN
    bsp_anc_set_mode(anc_user_mode);
    dac_fade_in();
#endif

#if DAC_DNR_EN
    dac_dnr_set_sta(xcfg_cb.dac_dnr_en);
#endif
    if (xcfg_cb.bt_outbox_voice_pwron_en) {
        sys_warning_play(T_WARNING_POWER_ON, PIANO_POWER_ON);
    }
    bt_audio_enable();
//    printf("bt connect\n");
}

AT(.com_text.bat)
void hfp_get_bat_level_hook(uint *level)
{
#if BT_DISP_LOW_VBAT_EN
    f_bt.loc_vbat = *level;
    if(bt_tws_is_connected() && (bt_tws_is_slave() == 0)){
        if((*level > f_bt.rem_vbat) && (f_bt.rem_vbat != 0xff)){
            *level = f_bt.rem_vbat;
        }
    }
#endif
//    printf("*level %d vbat %d role %d\n",*level,sys_cb.vbat,bt_tws_is_slave());
    return;

}
//播放降噪状态提示音
void bsp_bt_msg_nr_sta_change(void)
{
    if(bt_tws_is_slave()) {
        return;
    }

#if ANC_EN
//    switch(sys_cb.nr_sta) {
//    case 1:
//        tws_res_play(TWS_RES_NR_DISABLE);                 //同步播放语言提示音
//        break;
//    case 2:
//        tws_res_play(TWS_RES_ANC);                        //同步播放语言提示音
//        break;
//    case 3:
//        tws_res_play(TWS_RES_TRANSPARENCY);               //同步播放语言提示音
//        break;
//    }

    bsp_anc_set_mode(sys_cb.nr_sta - 1);
#endif // ANC_EN
}

uint8_t bt_get_loc_nr_sta(void)
{
    return sys_cb.nr_sta;
}

void bt_va_callback(u16 operate)
{
}

u8 bt_sco_magic_voice_en(void)
{
    return BT_SCO_MAV_EN;
}

#if (BT_TWS_AUTO_SWITCH && BT_TWS_TSCO_AUTO_SWITCH)
u8 tws_call_switch_is_ready(void)
{
    return (bt_get_tws_remote_status() >= BT_STA_OUTGOING);
}
#endif

#if BT_A2DP_DEFAULT_VOLUME_EN
u8 bt_a2dp_default_volume_is_enable(void)
{
    return 1;
}
#endif

#if BT_TWS_SLAVE_CONN_NOR_FIX
u8 bt_tws_slave_conn_nor_fix_enable(void)
{
    return 1;
}
#endif

//void tws_send_custom_data(u8* buf, u8 len)    自定义数据发送接口，最大14bytes
void tws_set_custom_data(u8* buf)
{
//    u8 len = buf[0];
}

#if BT_HFP_CALL_EARPHONE_EN
u8 bt_call_need_switch(void)
{
    return sys_cb1.call_switch;
//    return 1;                         // 1: 强制使用耳机端接听, 手机无法切换
}
#endif

u8 bt_play_pause_siri_ctrl_dis(void)
{
    return BT_PLAY_PAUSE_SIRI_CTRL_DIS_EN;
}

void bt_store_a2dp_volume(u8 vol)
{
    bt_send_msg(BT_MSG_STORE_A2DP_VOLUME | (vol << 8));
}

//设置TWS搜索从机的时间
uint8_t bt_tws_get_search_slave_times(void)
{
    return 3;
}

//设置TWS快速连接(不回连)的出仓配对时间
u8 bt_get_tws_fast_pair_inq_status(void)
{
    return 4;
}

#if BT_TWS_TIMEOUT_DELAY_TIME_CTRL_EN
u32 bt_tws_timeout_delay_times(void)
{
    return 1000;                        //增加TWS超距断线单次回连时间1000ms
}
#endif

u8 bt_hid_shutter_oppo_is_enable(void)
{
    return BT_HID_SHUTTER_OPPO_EN;
}

#if BT_TWS_WARNING_SLAVE_BYPASS_FIX
void bt_warning_set_playing(u8 flag)
{
    sys_cb1.warning_is_playing = flag;
}

u8 bt_warning_is_playing(void)
{
    return sys_cb1.warning_is_playing;
}
#endif

#if BT_A2DP_AAC_BITRATE_CONFIG_EN
u32 bt_get_a2dp_aac_max_bitrate(void)
{
    return 256000;                              //256kbit/s
}
#endif

#if BT_A2DP_MUSIC_XIAOMI_FIX_EN
u8 bt_a2dp_music_xiaomi_fix_enable(void)
{
    return 1;
}
#endif

//解决和AB5656A2配对，通话声音不同步的问题
//u8 bt_sco_far_delay_enable(void)
//{
//    return 1;
//}

//解决VT-S20点读笔音频连接问题
//u8 bt_talking_pen_fix_enable(void)
//{
//    return 1;
//}

#if BT_TWS_SET_RECONN_NOR_EN
u8 bt_tws_set_reconn_nor_enable(void)
{
    return 1;
}
#endif

#if BT_TWS_CONN_NOR_STOP_RECON_FIX_EN
u8 bt_tws_conn_nor_stop_recon_fix_enable(void)
{
    return 1;
}
#endif

#if BT_SUP_TIME_OUT_RECON_FIX_EN
u8 bt_sup_time_out_recon_fix_enable(void)
{
    return 1;
}
#endif

#if BT_HID_EN
bool bsp_bt_hid_photo(u16 keycode)
{
    if ((xcfg_cb.bt_hid_en) && (bt_hid_is_connected())) {
        bt_hid_key(HID_KEY_ENTER);              //enter key, android 4.0以上
        delay_5ms(10);
        bt_hid_consumer(keycode);               //consumer key vol_up or vol_down, ios

        sys_warning_play(T_WARNING_TAKE_PHOTO, PIANO_TAKE_PHOTO);
        return true;
    }
    return false;
}
#endif // BT_HID_EN

#if BT_MAP_EN
#if BT_MAP_EN
bool bt_time_get_flag = false;
void bsp_get_real_time_process(void)
{
    static u32 map_ticks = 1;
    if (tick_check_expire(map_ticks, 10000)) {
        map_ticks = tick_get();
        if(bt_nor_is_connected()) {
            if (!bt_tws_is_slave() && !bt_time_get_flag) {
                printf("---map timer---\r\n");
                #if BT_MAP_EN
                    hfp_at_kick();
                    bt_map_kick();
                #endif
            }
        } else {
            bt_time_get_flag = false;
        }
    }
}
#endif
#endif
