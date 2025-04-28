#include "include.h"

void sco_audio_set_param(u8 *ptr, u8 size);
void bsp_aec_ack_param(void);
void bsp_aec_ack(void);

extern u8 eq_rx_buf[EQ_BUFFER_LEN];

void bt_tws_set_spp_eq_info(uint8_t *packet, uint16_t size)
{
    memcpy(eq_rx_buf, packet, size);
    if (memcmp(eq_rx_buf, "ANC", 3) == 0) {
#if ANC_EN
        msg_enqueue(EVT_ONLINE_SET_ANC);
#endif
    }else{
        msg_enqueue(EVT_ONLINE_SET_EQ);
    }
}

uint bt_app_cmd_process_hook(u8 *ptr, u16 size)
{
    uint ret = 0;

#if IODM_TEST_MODE
    if((ptr[0]==0x55)&&(ptr[1]==0xAA)&&(ptr[2]==0xff)){
        memcpy(eq_rx_buf, ptr, size);
        msg_enqueue(EVT_BT_SPP_IODM);
        ret = 1;
    }
#endif

#if SMIC_DBG_EN
    if (memcmp(ptr, "SMIC", 4) == 0) {
        memcpy(eq_rx_buf, ptr, size);
        msg_enqueue(EVT_ONLINE_SET_SMIC);
        ret = 1;
    }
#endif

#if ANC_EN
    if (memcmp(ptr, "ANC", 3) == 0) {
        memcpy(eq_rx_buf, ptr, size);
        msg_enqueue(EVT_ONLINE_SET_ANC);
        ret = 1;
    }
#endif
    return ret;
}
