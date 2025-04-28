/**********************************************************************
*
*   strong_symbol.c
*   定义库里面部分WEAK函数的Strong函数，动态关闭库代码
***********************************************************************/
#include "include.h"

#if (UART0_PRINTF_SEL == PRINTF_NONE)
void dhook_func_set(void) {}
uint sys_dhook_func(uint ihook_num, ...) {return 0;}
#endif

void va_enc_process(void) {}
void voice_assistant_sdadc_process(u8 *ptr,u32 samples,int ch_mode) {}
void latt_init(void) {}
void btstack_att_send(uint param) {}
uint8_t att_channel_create(u8 *addr) { return 0; }
u8 bt_va_get_sta(void) { return 0; }
u8 bt_va_is_connect(void) { return 0; }
void bt_va_start(u8 start) {}
void opus_kick_start(void) {}

#if !BT_SCO_MAV_EN
AT(.com_text.mav)
void mav_kick_start(void) {}
void magic_voice_process(void){}
void magic_voice_init(void *cfg){}
AT(.com_text.sco)
bool adc_sco_magic_process_hook(u16 *ptr, u32 samples) {return 0;}
#endif

#if !IODM_TEST_MODE
void btstack_fcc_test_func(void){}
#endif

#if !BT_FCC_TEST_EN
void btstack_fcc_init(void){}
void bt_fcc_test_ctrl_req(va_list va_param){}
void bt_fcc_test_ctrl_req_do(void* va_param){}
void bt_fcc_send_packet(va_list param){}
void h3c_init(u8 io_sel){}
bool bt_fcc_test_is_en(void) {return 0;};
#endif

#if !BT_ALG_DBB_EN
AT(.com_text.strong)
uint sbc_hwdec_kick_hook(void) {return 0;}
AT(.com_text.strong)
uint aac_audec_kick_hook(void) {return 0;}
AT(.com_text.strong)
void sbc_pcm_out_start(void) {}
AT(.text.codec)
void dbb_pcm_out_exit(void) {}
#endif

#if !MIC_MAXPOW_CALC_EN
u16 get_mic_maxpow(void){return 0;}
u16 dnr_buf_maxpow(void){return 0;}
#endif

#if BT_AEC_NLP_GAIN_NEW_EN
void aec_nlp_gain_proc_sw(void);
void aec_nlp_gain_process(void)
{
    aec_nlp_gain_proc_sw();
}
#endif

#if !BT_A2DP_AVRCP_PLAY_STATUS_FIX
void bt_a2dp_avrcp_play_status_patch(void) {}
#endif

#if !VUSB_TBOX_NEW_QTEST_EN
bool update_set_qcheck_code(void){return false;};
void qtest_set_check_code(u32 check_code){};
void qtest_show_user_info(u8* param){};
void qtest_custom_pair_id(u8* param){};
void qtest_generate_linkey(u8* random_key,u8 *linkey){};
void qtest_send_packet(u8* txbuf,u8 sub_cmd){};
void qtest_set_local_bt_info_do(u8* buf,u8* txbuf){};
void qtest_set_tx_param(void){};
void qtest_deal_uart_message(void){};
void qtest_set_tws_bt_info(u8* buf){};
void qtest_set_local_bt_info(u8 *buf){};
void qtest_beforehand_process(void){};
void qtest_set_tws_bt_info_do(u8 *buf){};
void qtest_process(void){};
void qtest_get_bt_link_info(u8* buf){};
u8 qtest_check_ear_feature(u8* tx_ptr){return 0;};
u32 qtest_check_tbox_feature(u8* rx_ptr){return 0;};
u32 qtest_get_osci_cap_addr(void) {return 0;}
u32 xcfg_get_flash_addr(void) {return 0;}
void param_bt_qpdn_boat_flag_write(u8 flag){};
void param_bt_qpdn_boat_flag_read(void){};
u8 param_bt_qpaird_id_read(void){return 0;};
void param_bt_qpaird_id_write(u8* buf){};
#endif

#if !VUSB_TBOX_NEW_QTEST_RECORD_TWS_INFO_EN
void pram_bt_set_tws_qtest_info(u8 *buf){};
u8 pram_bt_get_tws_qtest_info(u8 *buf){ return false; }
u8 bt_update_qtest_tws_info(void){ return false; }
#endif

#if !BT_MAP_EN
void map_client_init(void) {}
void goep_client_init(void) {}
void btstack_map(uint16_t param) {}
#endif

#if (EQ_CHECK_RES_SEL == 0) || (EQ_CHECK_RES_SEL == 2)
u8 eq_dec_param_buf[512] AT(.eq_dec_buf) ;
#endif

#if (EQ_CHECK_RES_SEL == 0) || (EQ_CHECK_RES_SEL == 1)
AT(.text.dbb)
const u8 *music_dbb_eq_coef_enc_param_get(u8 *table_max_level, u8 *max_level, u8* dac_spr,u32 *len){return NULL;}
void dbb_dec_param(void){};
#endif

#if !EQ_CHECK_RES_SEL
AT(.com_text.eq)
bool eq_dec_en(void){return false;}
AT(.com_text.eq)
bool eq_dec_param(u8 *param, u8 *ptr, u32 len){ return true;}
void eq_check_res(void){}
#endif


#if !(USER_TKEY_MULTI_EN || USER_TKEY_SLIDE || USER_TKEY_SHORT_SLIDE)
AT(.com_text.tkey.isr)
void tkey_multi_timeout_calibration(tk_multi_cb_t *tkm, u16 tkcnt) {}
AT(.com_text.tkey.isr)
void tkey_multi_press_check(tk_multi_cb_t *tkm, u16 tkcnt) {}
AT(.com_text.tkey.isr)
void tkey_bcnt_range_exception_fix(tk_multi_cb_t *tkm, u16 tkcnt, u8 tk_flag) {}
#endif

#if !USER_TKEY_SLIDE
AT(.com_text.tkey.isr)
void tkey_slide_up_down_check(tk_slide_cb_t *ts, tk_slide_cb_t *tp) {}
#endif

#if !BT_TWS_RECONN_NOR_FIX
u8 btstack_tws_link_ctrl_timer_is_active(void) {return 0;}
#endif

#if !BT_A2DP_STORE_VOL_EN
void btstack_store_a2dp_volume(u8 vol) {}
bool bt_get_stored_a2dp_volume(u8 *vol) {return false;}
#endif

#if !BT_TWS_CUSTOMER_CONTROL_TWSSCAN_STA_EN
void btstack_tws_set_scan_m(uint bt_param);
void btstack_tws_set_scan(uint bt_param){
    btstack_tws_set_scan_m(bt_param);
}
#endif

#if !BT_RF_POWER_BALANCE_EN
void rf_current_supply(void){};
void rf_current_recover(void){};
void btmdm_disable(void){};
void btmdm_enable(void){};
void bt_power_balance_reconnect(u8 start) {};
#endif

#if !CHARGE_NTC_DETECT
void charge_ntc_stop(u8 mode) {}
void charge_detect_hook(void) {}
void charge_detect_patch(void) {}
#endif

#if !BT_TWS_STANDBY_SUPPORT_SWITCH_EN
bool bt_check_snoop_rssi(int8_t snoop_rssi_threshold,uint8_t tws_rssi_threshold,uint cnt){ return false;}
#endif

WEAK void bt_map_data_callback(uint8_t *packet)
{
}

#if !BT_A2DP_AAC_BITRATE_CONFIG_EN
void a2dp_set_aac_max_bitrate(void) {}
#endif

#if !BT_TWS_SET_RECONN_NOR_EN
u8 btstack_tws_remote_send_nor_reconnect(void) {return 0;}
void tws_send_set_reconnect_nor(u8 connect_retry) {}
u8 tws_remote_reconnect_nor(void) {return 0;}
void tws_remote_set_reconnect(u8 connect_retry) {}
#endif

#if !BT_LOW_LATENCY_QUICK_FIX
u8 bt_low_latency_is_busy(void) {return 0;}
void bt_low_latency_set_busy(void) {}
void bt_low_latency_clr_busy(void) {}
#endif

#if !BT_A2DP_MUSIC_XIAOMI_FIX_EN
u8 avdtp_set_configuration_rsp_hook(void * signal_con) {return 0;}
#endif

#if !VUSB_HUART_DMA_EN
void vusb_huart_set_patch(void){}
void qtest_dma_send_msg2tbox(u8 *buf,u16 len){}
AT(.com_text.qtest)
void bsp_tbox_qest_packet_dma_recv(u8 *rx_buf){}
AT(.com_text.vhouse)
void bsp_vhouse_packet_dma_recv(u8 *buf){}
AT(.com_text.timer)
void bsp_vhouse_cmd_ack(void)
{
    bsp_vhouse_cmd_ack_m();
}
#endif

#if !DAC_MUTE_ISSUE_FIX
void dac_digvol_fade_patch(void){}
#endif //!DAC_MUTE_ISSUE_FIX
