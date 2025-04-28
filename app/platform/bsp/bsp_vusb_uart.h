#ifndef _BSP_VUSB_UART_H
#define _BSP_VUSB_UART_H

#define VUSB_LEFT_CHANNEL             0x11
#define VUSB_RIGHT_CHANNEL            0x22
#define VUSB_NOTFIX_LCHANEL           0x33
#define VUSB_NOTFIX_RCHANEL           0x44

enum {
    NO_DISTRIBUTION = 0,
    RIGHT_CHANNEL,
    LEFT_CHANNEL,
};

typedef struct {
    volatile u8 w_cnt;
    volatile u8 r_cnt;
    u8 resv0;
    u8 resv1;
    u8 buf[96];
} vuart_cb_t;
extern vuart_cb_t vuart_cb;

extern const u8 vusb_crc8_tbl[256];
void bsp_vusb_uart_init(void);
u8 bsp_vusb_uart_get(u8 *ch);
u8 vusb_get_tws_channel(void);
u8 vusb_crc8_maxim(u8 *buf, u8 length);
void vusb_uart_var_reset(void);
bool bsp_vusb_channel_check(u8 channel, u8 type);
u8 vusb_get_tws_role(void);
void bsp_qtest_init(void);
void vusb_uart_isr(void);
#endif // _BSP_VUSB_UART_H
