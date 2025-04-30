#include "include.h"

///检查结构体Size，以下结构体被修改会编译报错。防止这些结构体被修改!!!
SIZE_OF_TYPE_EQUAL_TO(inbox_cb_t, 8)
SIZE_OF_TYPE_EQUAL_TO(charge_cfg_t, 16)
SIZE_OF_TYPE_EQUAL_TO(adc_cb_t, 0x1c)
SIZE_OF_TYPE_EQUAL_TO(key_cb_t, 0x1c)
SIZE_OF_TYPE_EQUAL_TO(led_cfg_t, 4)
SIZE_OF_TYPE_EQUAL_TO(led_cb_t, 10)
SIZE_OF_TYPE_EQUAL_TO(piano_cb_t, 0x18)
SIZE_OF_TYPE_EQUAL_TO(vh_packet_t, 0x16)
SIZE_OF_TYPE_EQUAL_TO(vhouse_cb_t, 0x30)
SIZE_OF_TYPE_EQUAL_TO(vuart_cb_t, 0x64)
SIZE_OF_TYPE_EQUAL_TO(sys_cb_t, 0x84)
SIZE_OF_TYPE_EQUAL_TO(tk_cb_t, 0x34)
SIZE_OF_TYPE_EQUAL_TO(te_cali_cb_t, 8)
SIZE_OF_TYPE_EQUAL_TO(spp_tkcb_t, 0xc8)
SIZE_OF_TYPE_EQUAL_TO(func_bt_t, 0x1c)
SIZE_OF_TYPE_EQUAL_TO(bt_cfg_t, 0x18)
SIZE_OF_TYPE_EQUAL_TO(bt_voice_cfg_t, 0x40)
SIZE_OF_TYPE_EQUAL_TO(eq_dbg_cb_t, 8)
OFFSET_OF_TYPE_EQUAL_TO(eq_dbg_cb_t, rx_type, 6)
SIZE_OF_TYPE_EQUAL_TO(gpio_t, 8)
SIZE_OF_TYPE_EQUAL_TO(f_bt_ring_cb, 0x30)
SIZE_OF_TYPE_EQUAL_TO(sbc_dec_t, 0x114)
OFFSET_OF_TYPE_EQUAL_TO(sbc_dec_t, sbc, 4)
SIZE_OF_TYPE_EQUAL_TO(sdadc_cfg_t, 12)
SIZE_OF_TYPE_EQUAL_TO(sys_trim_t, 0x0c)
SIZE_OF_TYPE_EQUAL_TO(tk_pnd_cb_t, 12)
SIZE_OF_TYPE_EQUAL_TO(sys_t, 0x28)
OFFSET_OF_TYPE_EQUAL_TO(sys_t, tmr5ms_cnt0, 0x1c)


AT(.text.startup.init)
void platform_user_init(void)
{
#if UART0_PRINTF_SEL != PRINTF_NONE
    wdt_irq_init();                         //watchdog中断使能
#endif
    memset(&xcfg_cb, 0, sizeof(xcfg_cb));
    u16 len = sizeof(xcfg_cb);
    if(len > sizeof(xcfg_cb_t)){
        printf("xcfg info len error: %d %d\n", sizeof(xcfg_cb_t), len);
        WDT_RST();
    }
    xcfg_init(&xcfg_cb, sizeof(xcfg_cb));
    xcfg_cb.flash_en = 1;
    xcfg_cb.config_en = 1;
}

//正常启动Main函数
int main(void)
{
    printf("Hello AB565XA3: %x\n", LVDCON);

    //系统初始化函数，是整个耳机SDK的核心初始化入口
    bsp_sys_init();

    //功能运行函数，是整个耳机SDK的主循环入口
    func_run();
    return 0;
}
