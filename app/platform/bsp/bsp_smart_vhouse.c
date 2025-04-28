#include "include.h"

#if VUSB_SMART_VBAT_HOUSE_EN

#define TRACE_EN                0

#if TRACE_EN
#define TRACE(...)              printf(__VA_ARGS__)
#else
#define TRACE(...)
#endif

#define VHOUSE_FUNC_NO_MASKROM_EN       1   //是否添加不使用maskrom的自定义函数

extern vhouse_cb_t vhouse_cb;
extern vh_packet_t vh_packet;

extern u8 *huart_buffer;
vhouse_cb1_t vhouse_cb1;
vh_packet_t vh_packet1 ;

uint ble_get_bat_level(void);
void bt_get_local_bd_addr(u8 *addr);
void bt_tws_put_link_info_addr(uint8_t *bd_addr, u8 flag);
void bt_tws_put_link_info_feature(uint8_t *bd_addr, uint8_t feature);
void bsp_vhouse_vbat_ack(vh_packet_t *packet);
u8 vhouse_cmd_ack(vh_packet_t *packet);
void bsp_vhouse_open_windows(void);
u32 bsp_vhouse_packet_parse(vh_packet_t *p, u8 data);
void bsp_vhouse_analysis_packet_for_charge(vh_packet_t *packet);
void bsp_vhouse_analysis_packet(vh_packet_t *packet);
void bsp_vhouse_pair_ack(vh_packet_t *packet);
void bsp_vhouse_get_tws_btaddr_ack(vh_packet_t *packet);
void bsp_vhouse_popup_ctrl(vh_packet_t *packet);
void bsp_vhouse_update_bat_adv(void);
int btstack_get_link_key(uint8_t *bd_addr, uint8_t *link_key, uint8_t *type, bool ext);
int eq_huart_putcs(uint8_t *packet, uint16_t len);

#if VHOUSE_FUNC_NO_MASKROM_EN

AT(.text.vhouse)
static void bsp_vhouse_update_bat_value(vh_packet_t *packet)
{
    bool update_flag = false;
    if ((packet->cmd == VHOUSE_CMD_OPEN_WINDOW) || (packet->cmd == VHOUSE_CMD_GET_VBAT) || (packet->cmd == VHOUSE_CMD_CLOSE_WIN_GET_VBAT)) {
        if(sys_cb.loc_house_bat != packet->buf[1]) {
            sys_cb.loc_house_bat = packet->buf[1];
            sys_cb.rem_house_bat = packet->buf[1];
            update_flag = true;
        }
        if (packet->cmd == VHOUSE_CMD_CLOSE_WIN_GET_VBAT) {
            return;
        }
        if(packet->buf[2] & BIT(7)) {
            if(sys_cb.rem_bat != packet->buf[2]) {              //对方在仓内，且电量不为0时，用仓互传
                if((packet->buf[2]&0x7f) == 0) {
                    sys_cb.rem_bat |= BIT(7);
                } else {
                    sys_cb.rem_bat = packet->buf[2];
                    vhouse_cb.rem_bat_ok = true;
                }
                update_flag = true;
            }
        } else {
            if(vhouse_cb.rem_bat_ok) {
                update_flag = true;
            }
            vhouse_cb.rem_bat_ok = false;                       //对方出仓后，电量通过TWS互传
        }

        if(!vhouse_cb.inbox_sta) {
            vhouse_cb.inbox_sta = true;
            update_flag = true;
        }
    }
    vhouse_cb.update_ear_flag = update_flag;
}

//分析电量接收包并回应相关电量数据
AT(.text.vhouse)
void bsp_vhouse_vbat_ack(vh_packet_t *packet)
{
    u8 channel = packet->buf[0];
    if (!bsp_vusb_channel_check(channel, 0)) {
//        vhouse_send_message_err();
        return;
    }
    bsp_vhouse_update_bat_value(packet);

    //发送电量响应包
    packet->header = 0xAA55;
    packet->distinguish = VHOUSE_DISTINGUISH;
    packet->length = 0x03;
    packet->buf[0] = channel;
    packet->buf[2] = sys_cb.loc_bat;
    vhouse_cmd_ack(packet);
}

#endif // VHOUSE_FUNC_NO_MASKROM_EN

AT(.text.vhouse)
void bsp_vhouse_analysis_packet(vh_packet_t *packet)
{
    switch (packet->cmd) {
        case VHOUSE_CMD_GET_VBAT:
//            TRACE("VHOUSE_CMD_GET_VBAT\n");
            bsp_vhouse_vbat_ack(packet);
            break;

        case VHOUSE_CMD_PAIR:
            TRACE("VHOUSE_CMD_PAIR\n");
            bsp_vhouse_pair_ack(packet);
            break;

        case VHOUSE_CMD_GET_TWS_BTADDR:
            TRACE("VHOUSE_CMD_GET_TWS_BTADDR\n");
            bsp_vhouse_get_tws_btaddr_ack(packet);
            break;

        case VHOUSE_CMD_CLEAR_PAIR:
            TRACE("VHOUSE_CMD_CLEAR_PAIR\n");
            bt_clr_all_link_info('v');                      //删除所有配对信息
            break;

        case VHOUSE_CMD_PWROFF:
            TRACE("VHOUSE_CMD_PWROFF\n");
            if (bsp_vusb_channel_check(packet->buf[0], 0)) {
                vhouse_cb.status = 2;                           //充满电
                func_cb.sta = FUNC_PWROFF;
            }
            break;

        case VHOUSE_CMD_CLOSE_WINDOW:
            TRACE("VHOUSE_CMD_CLOSE_WINDOW\n");
            vhouse_cb.open_win_flag = 0;
            vhouse_cb.status = 0;                           //关盖, 充电
            bsp_vhouse_vbat_ack(packet);
            bsp_charge_bcnt_calibration(10);
            break;

        case VHOUSE_CMD_OPEN_WINDOW:
            TRACE("VHOUSE_CMD_OPEN_WINDOW\n");
            bsp_vhouse_open_windows();
            bsp_vhouse_vbat_ack(packet);
            break;

        case VHOUSE_CMD_SYS_RST:
            sw_reset_kick(SW_RST_FLAG);
            break;

        default:
#if IODM_TEST_MODE
            iodm_reveice_data_deal(packet,0);
#endif
        break;
    }
}

AT(.text.vhouse)
void bsp_vhouse_analysis_packet_for_charge(vh_packet_t *packet)
{
    switch (packet->cmd) {
        //仓关盖后获取电量
        case VHOUSE_CMD_CLOSE_WIN_GET_VBAT:
            TRACE("VHOUSE_CMD_CLOSE_WIN_GET_VBAT\n");
            bsp_vhouse_vbat_ack(packet);
            vhouse_cb.open_win_flag = 0;
            vhouse_cb.status = 0;                           //关盖, 充电
            break;

        //仓开盖后获取电量
        case VHOUSE_CMD_OPEN_WINDOW:
            TRACE("VHOUSE_CMD_OPEN_WINDOW\n");
        case VHOUSE_CMD_GET_VBAT:
            TRACE("VHOUSE_CMD_GET_VBAT\n");
            bsp_vhouse_vbat_ack(packet);
            vhouse_cb.status = 1;                           //开窗，停止充电
            break;

        case VHOUSE_CMD_PWROFF:
            if (bsp_vusb_channel_check(packet->buf[0], 0)) {
                TRACE("VHOUSE_CMD_PWROFF\n");
                vhouse_cb.status = 2;                           //充满电
            }
            break;

        default:
            break;
    }
}

static u32 bsp_smart_vhouse_process_do(u32 charge_sta)
{
#if VUSB_HUART_DMA_EN
    //因为HUART DMA只在SRAM0 SRAM1有效，所以需要重新申请一个buf用于HUART TX DMA
    vh_packet_t *packet = &vh_packet1;
#else
    vh_packet_t *packet = &vhouse_cb.packet;
#endif

    if (vhouse_cb.open_win_flag && tick_check_expire(vhouse_cb.win_ticks, 180)) {
        bsp_vhouse_open_windows();                          //防止开盖命令被堵而丢失的问题
    }
    if (tick_check_expire(vhouse_cb.loc_ticks, 500)) {      //没必要一直检测，容易跳来跳去
        u8 loc_bat = ble_get_bat_level();
        if (charge_sta && (loc_bat == 100)) {               //修正耳机会充不满电的问题
            loc_bat = 99;
        }
        if((sys_cb.loc_bat & 0x7f) != loc_bat) {
            sys_cb.loc_bat = (sys_cb.loc_bat & 0x80) | loc_bat;
            vhouse_cb.update_ear_flag = true;
        }
        vhouse_cb.loc_ticks = tick_get();
    }
#if !VUSB_HUART_DMA_EN
    u8  ch;
    u32 parse_done;
    while (bsp_vusb_uart_get(&ch)) {
        WDT_CLR();
        parse_done = bsp_vhouse_packet_parse(packet, ch);
        if ((parse_done) && (packet->checksum == vusb_crc8_maxim((u8 *)packet, 5 + packet->length))) {
            TRACE("cmd [%d], %d\n", packet->cmd, charge_sta);
            if (charge_sta) {
                bsp_vhouse_analysis_packet_for_charge(packet);
                vhouse_cb.ear_mute_flag = 0;
            } else {
                bsp_change_volume(0);                       //入仓后耳机需要静音
                vhouse_cb.ear_mute_flag = 1;
                reset_sleep_delay();                        //耳机在仓内不进休眠, 需要接收电量心跳包数据
                bsp_vhouse_analysis_packet(packet);
                bsp_vhouse_update_bat_adv();
            }
        }
    }
#else
    if(vhouse_cb1.rx_flag){
        if(huart_buffer[4] < VH_DATA_LEN){
            memcpy(packet,huart_buffer,huart_buffer[4]+5);
	        packet->checksum = huart_buffer[packet->length+5];
	        if(packet->checksum == vusb_crc8_maxim((u8 *)packet, 5 + packet->length)){
	            TRACE("cmd [%d], %d\n", packet->cmd, charge_sta);
	            if (charge_sta) {
	                bsp_vhouse_analysis_packet_for_charge(packet);
                    vhouse_cb.ear_mute_flag = 0;
	            } else {
                    bsp_change_volume(0);                       //入仓后耳机需要静音
                    vhouse_cb.ear_mute_flag = 1;
                    reset_sleep_delay();                        //耳机在仓内不进休眠, 需要接收电量心跳包数据
	                bsp_vhouse_analysis_packet(packet);
	                bsp_vhouse_update_bat_adv();
	            }
	        }
		}
        vhouse_cb1.rx_flag=0;
    }
#endif

    if (charge_sta == 0) {
        bsp_vhouse_update_bat_adv();
        if (!vhouse_cb.inbox_sta) {                         //耳机出仓后还原音量
            if (vhouse_cb.ear_mute_flag) {
                vhouse_cb.ear_mute_flag = 0;
                bsp_change_volume(sys_cb.vol);
            }
        }
    }
    return vhouse_cb.status;
}

AT(.com_text.vhouse)
u32 bsp_smart_vhouse_process(u32 charge_sta)
{
#if VUSB_TBOX_QTEST_EN
    if (get_qtest_mode()){
        return 0;
    }
#endif
    if (!xcfg_cb.chbox_smart_en) {
        return 0;
    }
    return bsp_smart_vhouse_process_do(charge_sta);
}

AT(.text.vhouse)
void bsp_smart_vhouse_init(void)
{
    memset(&vh_packet, 0, sizeof(vh_packet));
    memset(&vhouse_cb, 0, sizeof(vhouse_cb));
    if (xcfg_cb.chbox_smart_en) {
        vhouse_cb.xcfg_ch = vusb_get_tws_channel();
        bsp_vusb_uart_init();
    }
}
#else
AT(.com_text.vhouse)
u32 bsp_smart_vhouse_process(u32 charge_sta) {return 0;}
void bsp_vhouse_analysis_packet_for_charge(void *packet) {}
void bsp_vhouse_analysis_packet(void *packet) {}
#endif

#if VUSB_HUART_DMA_EN
//uart中断解析并匹配命令包
AT(.com_rodata.vhouse_tbl)
const char vhouse_header_str[] = {0x55,0xAA,VHOUSE_DISTINGUISH};

bool vusb_is_support_huart_en(void)
{
    return VUSB_HUART_DMA_EN;
}

//接收心跳包后需要5~10ms回应1字节
AT(.com_text.timer)
void bsp_vhouse_cmd_ack(void)
{
    if (vhouse_cb.need_ack == 1) {
        if (tick_check_expire(vhouse_cb.ticks, 6)) {
            //因HUART只能在SRAM0~1使用，所以需要把数据存放再SRAM0-1再发送
            vhouse_cb1.ack_dat = vhouse_cb.ack_dat;
            eq_huart_putcs((u8*)&vhouse_cb1.ack_dat,1);
            vhouse_cb.need_ack = 2;
        }
    }
}


AT(.text.vhouse)
void vhouse_dma_send_data(u8 *buf,u8 len)
{
    //超过5ms没有接收到VUSB程序才发送数据
    while (!tick_check_expire(vhouse_cb.ticks, 5)) {
         WDT_CLR();
    }
    vhouse_cb.need_ack = 0;
    if(tick_check_expire(vhouse_cb.ticks, 90)){
        return;
    }

    eq_huart_putcs(buf,len);
#if TRACE_EN
	printf(">> ");
	print_r(buf,len);
#endif
}

AT(.text.vhouse)
u8 vhouse_cmd_dma_ack(vh_packet_t *packet)
{
    u8 *buf = (u8*)packet;
    u8 length = 5 + packet->length;
    buf[length] = vusb_crc8_maxim(buf, length);
    vhouse_dma_send_data(buf, length + 1);
    return true;
}


AT(.com_text.bsp.uart.vusb)
void bsp_vhouse_packet_dma_recv(u8 *buf)
{
    if(get_qtest_mode()){
        return ;
    }

    if(!memcmp(buf,vhouse_header_str,3)) {
        u8 len = buf[4];
        u8 crc_pos = len+5;
        if(huart_buffer[crc_pos] != vusb_crc8_maxim(buf, crc_pos)){
            return;
        }
        if(len < VH_DATA_LEN) {
            vhouse_cb.ticks = tick_get();
	        if ((vhouse_cb.xcfg_ch) && ((buf[5] == VUSB_LEFT_CHANNEL) || ( buf[5] == VUSB_RIGHT_CHANNEL))) {
	            if (vhouse_cb.xcfg_ch != buf[5]) {   //配置已固定声道，需要判断正确声道再回ACK数据
	                return;
	            }
	        }
            if (buf[3] == VHOUSE_CMD_OPEN_WINDOW) {
                vhouse_cb.win_ticks = vhouse_cb.ticks;
                vhouse_cb.open_win_flag = 1;
            }
            vhouse_cb.need_ack = 1;
            vhouse_cb1.rx_flag = 1;
        }
    }
}
#endif
