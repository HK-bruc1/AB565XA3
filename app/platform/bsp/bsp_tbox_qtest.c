#include "include.h"

#define TRACE_EN                0

#if TRACE_EN
#define TRACE(...)              printf(__VA_ARGS__)
#else
#define TRACE(...)
#endif


#define DC_IN()                  ((RTCCON >> 20) & 0x01)            //VUSB Online state:    0->not online, 1->online
#define QTEST_OTHER_USAGE_VERSION        1

#define QTEST_MODE_DUT              '3'
#define QTEST_MODE_CLR_INFO         '4'
#define QTEST_MODE_PWROFF           '5'
#define QTEST_MODE_PWROFF_LP        '6'             //船运模式
#define QTEST_MODE_VERSION_INFO     0xff

typedef struct {
    u32 inbox_tick;
    u32 vusb_release_tick;
    u8  charge_inbox_cnt;
    u8  ear_sta;
    u8  status;
    u8  dc_rst;
} bsp_qtest_cb_t;
extern bsp_qtest_cb_t bsp_qtest_cb;

//void bsp_qtest_var_init(void)
//{
//    memset(&bsp_qtest_cb, 0, sizeof(bsp_qtest_cb));
//    bsp_qtest_cb.charge_inbox_cnt = 150;
//}
//
//void qtest_other_usage_status_check(u8 *status)
//{
//    //在主循环中再进行状态判断
//    if(!func_cb.sta){
//        return;
//    }
//    if (*status == QTEST_MODE_DUT) {
//        func_cb.sta = FUNC_BT_DUT;
//    } else if(*status == QTEST_MODE_PWROFF || *status == QTEST_MODE_PWROFF_LP) {
//        func_cb.sta = FUNC_PWROFF;
//    }
//    *status = 0;
//}
//
//u8 qtest_other_usage_process_do(void)
//{
//    u32 buf_len = 0;
//    u8 tx_len = 5;
//	u8 status = 0;
//    u8 *tx_buf = qtest_other_usage_alloc_txbuf(&buf_len);
//
//    sys_cb.qtest_ack = sys_cb.qtest_flag;
//    bsp_qtest_cb.dc_rst = 1;              //不让耳机进入充电
//    sys_cb.dc_rst_flag = 0;
//    vusb_delay_rst_dis();
//    bsp_charge_leakage_set(1, 1);           //如果漏电流打开，则设置为90uA漏电流
//    sys_cb.qtest_flag = 0;
//    if (sys_cb.qtest_ack == QTEST_MODE_DUT) {
//        status = sys_cb.qtest_ack;
//    } else if (sys_cb.qtest_ack == QTEST_MODE_CLR_INFO) {
//        bt_clr_all_link_info('b');
//        func_cb.sta = FUNC_NULL;
//    } else if (sys_cb.qtest_ack == QTEST_MODE_PWROFF || sys_cb.qtest_ack == QTEST_MODE_PWROFF_LP) {
//        if (sys_cb.qtest_ack == QTEST_MODE_PWROFF_LP) {
//            sys_cb.pdn_boat_flag = 1;
//        }
//        sys_cb.pwrdwn_tone_en = 1;
//        status = sys_cb.qtest_ack;
//    } else if (sys_cb.qtest_ack == QTEST_MODE_VERSION_INFO){
//        tx_buf[tx_len] = QTEST_OTHER_USAGE_VERSION;
//        tx_len += 1;
//    }
//    //1byte crc
//    if(tx_len + 1 > buf_len) {
//        tx_len = 0;
//        TRACE("TX_LEN_ERR\n");
//    }
//
//    tx_buf[0] = 'S';
//    tx_buf[1] = 'I';
//    tx_buf[2] = 'G';
//    tx_buf[3] = sys_cb.qtest_ack;
//    tx_buf[4] = tx_len - 5;
//    tx_buf[tx_len] = vusb_crc8_maxim(tx_buf,tx_len);
//    send_msg2tbox(tx_buf, tx_len + 1);
//    sys_cb.qtest_ack = 0;
//    return status;
//}
//
//AT(.text.bfunc.bt)
//bool qtest_other_usage_process(void)
//{
//    bool res = false;
//
//    if (!xcfg_cb.qtest_en) {
//        return false;
//    }
//
//    //检查任务是否已经执行
//    if (bsp_qtest_cb.status){
//        qtest_other_usage_status_check(&bsp_qtest_cb.status);
//    }
//
//    //如果超过2S没接收到仓发过来的数据，则退出循环
//    if (!tick_check_expire(bsp_qtest_cb.vusb_release_tick, 2000)) {
//        res = true;
//    } else if (bsp_qtest_cb.dc_rst) {           //超过2S,将dc_rst_flag恢复成原来的值
//        bsp_qtest_cb.dc_rst = 0;
//        vusb_delay_rst_en(8);
//        sys_cb.dc_rst_flag = 1;
//    }
//
//    if (sys_cb.qtest_ack != sys_cb.qtest_flag) {
//        bsp_qtest_cb.status = qtest_other_usage_process_do();
//        bsp_qtest_cb.vusb_release_tick = tick_get();
//        res = true;
//    }
//    return res;
//}

#if VUSB_TBOX_NEW_QTEST_EN

AT(.com_text.qtest)
bool qtest_is_new_mode(void)
{
    return VUSB_TBOX_NEW_QTEST_EN;
}

//尽快结束战斗
AT(.text.qtest)
void qtest_tbox_set_custom_param(u8 param)
{
//    printf("param %d\n",param);
}

//传给测试盒数据用于显示，测试盒需要打开“显示耳机传入信息”的功能，可自定义显示内容
//第0byte为：字符串数据
//第1byte为：数值
AT(.text.qtest)
void qtest_show_user_info(u8* param)
{
    param[0] = 'V';
    param[1] = ble_get_bat_level();
}

//pair_mode选择ID配对时，用该函数获取ID
AT(.text.qtest)
uint32_t bt_tws_get_pair_id(void)
{
    return xcfg_cb.bt_tws_ext_nane_en? xcfg_cb.bt_tws_ext_name : 0;

}

AT(.text.qtest)
void qtest_custom_pair_id(u8* param)
{
    uint pair_id = bt_tws_get_pair_id();
    memcpy(param,(u8*)&pair_id,4) ;
}

AT(.com_text.qtest)
u8 qtest_get_charge_pwrsave_sta(void) {
    return chbox_cb.pwrsave_en;
}
#if BT_TWS_FIX_LR_SAME_CH_MAC_EN
u8 vusb_check_tws_master_addr(u8* bt_addr)
{
    u8 ms_role = vusb_get_tws_role();
    //如果左耳为主耳
    if(BT_TWS_SET_MAC_CH == 2 && bt_tws_set_lr_same_ch_mac_is_en()){
        ms_role ^= 1;
    }
    if (!ms_role) {
       bt_get_local_bd_addr(bt_addr);
    }
    print_r(bt_addr,6);
    return ms_role;
}
#endif
#endif
