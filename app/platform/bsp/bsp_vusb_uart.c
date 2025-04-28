#include "include.h"

#if VUSB_SMART_VBAT_HOUSE_EN || VUSB_TBOX_QTEST_EN


#define HUART_CLK                 26000000
#define VUSB_HUART_BAUD           9600
#define VUSB_HUART_BAUD_VAL      (((HUART_CLK + (VUSB_HUART_BAUD / 2)) / VUSB_HUART_BAUD) - 1)


void eq_dbg_huart_init(void);
void eq_dbg_huart_init_m(void);
void bsp_vusb_uart_init_m (void);

void eq_dbg_huart_init(void)
{
    eq_dbg_huart_init_m();
    if ((xcfg_cb.eq_uart_sel == 0) && (xcfg_cb.qtest_en || xcfg_cb.chbox_smart_en)) {
        HSUT0BAUD = (VUSB_HUART_BAUD_VAL << 16) | VUSB_HUART_BAUD_VAL;
    }

}

AT(.text.vusb)
void bsp_vusb_uart_init(void)
{
    bsp_vusb_uart_init_m();
#if VUSB_HUART_DMA_EN
    xcfg_cb.eq_dgb_uart_en = 1;
    xcfg_cb.eq_uart_sel = 0;
    eq_dbg_huart_init();
#endif
}
#endif // VUSB_SMART_VBAT_HOUSE_EN

