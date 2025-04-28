#include "include.h"
#include "bsp_iodm.h"

#if IODM_TEST_MODE

#define TRACE_EN                0

#if TRACE_EN
#define TRACE(...)              printf(__VA_ARGS__)
#else
#define TRACE(...)
#endif


struct{
    u8 precmd;//上次的命令
    u8 prelen;//上次读取到的位置
}iodm;

u8 iodm_ver[] = {1,2};

u8 fcc_param[10];
u32 bt_get_xosc_cap(void);
void bt_set_xosc_cap(u32 cap);
u8 param_bt_xosc_read(void);
u8 vhouse_cmd_ack(vh_packet_t *packet);

AT(.text.vhouse)
u8 iodm_spp_tx_ack(vh_packet_t *packet)
{

    u8 length = 5 + packet->length;
    u8* crc_ptr = (u8*)packet+length;
    *crc_ptr = vusb_crc8_maxim((u8*)packet, length);
#if TRACE_EN
    TRACE("spp_tx_ack\n");
    print_r(packet,length+4);
#endif
    bt_spp_tx((u8*)packet,length+1);
    return true;
}


u8 iodm_cmd_rsp(vh_packet_t *packet, u8 len, u8 result,u8 entrance)
{
    packet->header = 0xAA55;
    packet->distinguish = VHOUSE_DISTINGUISH;
    packet->length = len + 1;
    packet->buf[0] = result;
    if(entrance==0){
        vhouse_cmd_ack(packet);
    }else{
        iodm_spp_tx_ack(packet);
    }
    return 0;
}

void iodm_clear_all_btname(void)
{
    char buf = 0;
    param_bt_new_name_write(&buf,1);
}




void iodm_cmd_mic_lookback_exit(void)
{
    dac_fade_out();
    dac_fade_wait();                    //等待淡出完成
    audio_path_exit(AUDIO_PATH_MIC_TEST);
}

void iodm_cmd_mic_lookback_enter(void)
{
    iodm_cmd_mic_lookback_exit();
    audio_path_init(AUDIO_PATH_MIC_TEST);
    audio_path_start(AUDIO_PATH_MIC_TEST);
}
char iodm_btname[32] = {0};
u8 iodm_cmd_set_bt_name(vh_packet_t *packet)
{
    u8 len = iodm.prelen+packet->length;

    if(len>32){
        iodm.prelen = 0;
        return IODM_RESULT_FAIL;
    }
    memcpy(iodm_btname+iodm.prelen,packet->buf,packet->length);
    iodm.prelen = len;

    if(packet->length<VH_DATA_LEN){

        iodm_clear_all_btname();
        iodm_save_bt_new_name((char*)iodm_btname, iodm.prelen);
        memset(iodm_btname,0,32);
        len = 0;
        return IODM_RESULT_OK;
    }
    return IODM_RESULT_COMPLETE;
}

u8 iodm_cmd_get_bt_name(u8*tx_buf,u8* tx_len)
{
    u8 result = IODM_RESULT_COMPLETE;

    if(iodm.prelen == 0){
        memset(iodm_btname,0,32);
        if(!iodm_get_bt_name((u8*)iodm_btname)){
            memcpy(iodm_btname,bt_name_buf,32);
        }
    }

    u8 name_len = strlen(iodm_btname);
    *tx_len = VH_DATA_LEN-1;

    if(name_len<=iodm.prelen+(*tx_len)){
        *tx_len = name_len - iodm.prelen;
        result = IODM_RESULT_OK;
    }

    memcpy(tx_buf,(u8*)iodm_btname+iodm.prelen,*tx_len);

    if(result == IODM_RESULT_OK){
        iodm.prelen = 0;
    }else{
        iodm.prelen+=(*tx_len);
    }
    return result;
}

void iodm_reveice_data_deal(vh_packet_t *packet,u8 entrance)
{
    u8 cmd_rsp_param_len = 0;
    u8 result = IODM_RESULT_OK;
    u8 *tx_buf = (u8*)packet->buf+1;
    if(iodm.precmd != packet->cmd){
        iodm.precmd = packet->cmd;
        iodm.prelen = 0;
    }

    switch(packet->cmd){

        case IODM_CMD_DEV_RST:
            result = IODM_RESULT_OK;
            iodm_cmd_rsp(packet, cmd_rsp_param_len, result,entrance);
            sw_reset_kick(SW_RST_FLAG);
            break;

        case IODM_CMD_SET_BT_ADDR:
            //复位生效
            TRACE("IODM CMD SET BT ADDR\n");
            bt_save_qr_addr(packet->buf);
            break;

        case IODM_CMD_GET_BT_ADDR:
            TRACE("IODM CMD GET BT ADDR\n");
            cmd_rsp_param_len = 6;
            if (!bt_get_qr_addr(tx_buf)) {
               bt_get_local_bd_addr(tx_buf);
            }
            break;

        case IODM_CMD_SET_BT_NAME:
            TRACE("IODM CMD SET BT NAME\n");
            result = iodm_cmd_set_bt_name(packet);
            break;

        case IODM_CMD_GET_BT_NAME:
            TRACE("IODM CMD GET BT NAME\n");
            result = iodm_cmd_get_bt_name(tx_buf,&cmd_rsp_param_len);
            break;

        case IODM_CMD_CBT_TEST_ENTER:
            TRACE("IODM CMD CBT TEST ENTER\n");
            if(func_cb.sta != FUNC_BT_DUT){
                func_cb.sta = FUNC_BT_DUT;
            }
            break;

        case IODM_CMD_CBT_TEST_EXIT:
            TRACE("IODM CMD CBT TEST EXIT\n");
            if (func_cb.sta != FUNC_BT){
                func_cb.sta = FUNC_BT;
                break;
            }
            break;

        case IODM_CMD_FCC_TEST_ENTER:
    //        printf("iodm IODM_FCC_TEST\n");
            result = IODM_RESULT_OK;
            if (func_cb.sta != FUNC_BT_FCC){
                func_cb.sta = FUNC_BT_FCC;
            }
            break;

        case IODM_CMD_FCC_SET_PARAM:
            if (func_cb.sta == FUNC_BT_FCC) {
                memcpy(fcc_param, packet->buf, sizeof(fcc_param));
                fcc_param[5] = 7;      //固定power
//                printf("fcc_param:");
//                print_r(fcc_param, 0x0a);
                bt_fcc_test_start();
                result = IODM_RESULT_OK;
            } else {
                result = IODM_RESULT_FAIL;
            }
            break;

        case IODM_CMD_FCC_TEST_EXIT:
    //        printf("iodm fcc exit\n");
            result = IODM_RESULT_OK;
            if (func_cb.sta != FUNC_BT){
                func_cb.sta = FUNC_BT;
                break;
            }
            break;

        case IODM_CMD_SET_XOSC_CAP:
            TRACE("IODM CMD SET XOSC CAP\n");
            u8 xtal = packet->buf[0];
            if (xtal < 63) {
                bt_set_xosc_cap(xtal);   //设置 频偏参数
            } else {
                result = IODM_RESULT_FAIL;
            }
            break;


        case IODM_CMD_GET_XOSC_CAP:
            TRACE("IODM CMD GET XOSC CAP\n");
            cmd_rsp_param_len = 1;
            u8 cap_param = param_bt_xosc_read();
            if (cap_param == 0xff) {
                tx_buf[0] = xcfg_cb.osc_cap;
            } else {
                cap_param &= ~0x80;
                tx_buf[0] = (cap_param & 0x7) << 3;
                tx_buf[0] |= (cap_param >> 3) & 0x07;
            }
            break;

        case IODM_CMD_GET_VER_INFO:
            TRACE("IODM CMD GET VER INFO\n");
            cmd_rsp_param_len = 4;
            memcpy(tx_buf, IODM_HARDWARE_VER, 2);
            memcpy(tx_buf+2, IODM_SOFTWARE_VER, 2);
            result = IODM_RESULT_OK;
            break;

        case IODM_CMD_GET_INFO:
            TRACE("IODM CMD GET INFO\n");
            cmd_rsp_param_len = 4;
            tx_buf[0] = 1;
            tx_buf[1] = 0;
            tx_buf[2] = 1;
            tx_buf[3] = VH_DATA_LEN;
            break;

        case IODM_CMD_BT_SET_SCAN:
            TRACE("IODM CMD BT SET_SCAN\n");
            if(bt_nor_is_connected()){
                bt_nor_disconnect();
                bt_nor_clr_link_info('I');
                bt_cfg.scan_ctrl = 1;
                bt_set_scan(true);
            }
            break;

        case IODM_CMD_MIC_LOOKBACK_ENTER:
            TRACE("IODM CMD MIC LOOKBACK ENTER\n");
            iodm_cmd_mic_lookback_enter();
            break;

        case IODM_CMD_MIC_LOOKBACK_EXIT:
            TRACE("IODM CMD MIC LOOKBACK EXIT\n");
            iodm_cmd_mic_lookback_exit();
            break;

        case IODM_CMD_PROTOCOL_VER:
            cmd_rsp_param_len = 2;
            memcpy(tx_buf,iodm_ver,2);
            break;

        case IODM_CMD_CLEAR_PAIR:
            bt_clr_all_link_info('I');
            break;

        default:
//            result = vhouse_custom_test(packet,&cmd_rsp_param_len);
            break;

    }

    if(result == IODM_RESULT_OK){
        vhouse_cb.ear_mute_flag = 0;
        bsp_change_volume(sys_cb.vol);
    }

    if(result != IODM_RESULT_NO_RSP){
        iodm_cmd_rsp(packet, cmd_rsp_param_len, result,entrance);
        TRACE("iodm_reveice_data_deal end\n");
    }


}

uint8_t *bt_rf_get_fcc_param(void)
{
    return fcc_param;
}

void bt_save_qr_addr(u8 *addr)
{
    cm_write8(PARAM_QR_ADDR_VALID, 1);
    cm_write(addr, PARAM_QR_ADDR, 6);
    cm_sync();
}

bool bt_get_qr_addr(u8 *addr)
{
    if (cm_read8(PARAM_QR_ADDR_VALID) == 1) {
        cm_read(addr, PARAM_QR_ADDR, 6);
        return true;
    }
    return false;
}


bool iodm_get_bt_name(u8 *buf)
{
    u8 len = cm_read8(PARAM_BT_NAME_LEN);
    if(len > 32 || len == 0){
        return false;
    }
    memset(buf,0x00,32);  //clear
    cm_read(buf, PARAM_BT_NAME, len);
    return true;

}

void iodm_init(void)
{
    memset(&iodm,0,sizeof(iodm));
}

bool iodm_reflash_bt_name(void)
{
    if(iodm_get_bt_name((u8*)bt_name_buf)){
         return true;
    }
    return false;

}


bool iodm_save_bt_new_name(char *name,u8 len)
{
    if(len>32){
        return false;
    }
    cm_write8(PARAM_BT_NAME_LEN, len);
    cm_write(name, PARAM_BT_NAME, len);
    cm_sync();
    return true;
}



extern u8 eq_rx_buf[EQ_BUFFER_LEN];

void bt_spp_iodm_event(void)
{

#if TRACE_EN
    TRACE("bt_spp_iodm_event\n");
    print_r(eq_rx_buf,eq_rx_buf[4]+1);
#endif
    vh_packet_t *packet =&vhouse_cb.packet;
    u8 len = eq_rx_buf[4];
    if(len>VH_DATA_LEN){
        return;
    }
    len = len+5;
    packet->checksum = eq_rx_buf[len];
    memcpy((u8*)packet,eq_rx_buf,len);
    if(packet->checksum == vusb_crc8_maxim((u8 *)packet, len)){
        iodm_reveice_data_deal(packet,1);
    }
}
#endif
