#ifndef _API_VUSB_UART_H
#define _API_VUSB_UART_H

typedef void (*isr_t)(void);

void uart1_putchar(char ch);            //vusb uart发送1byte
void vusb_uart_register_isr(isr_t isr);
bool vusb_uart_getchar(u8 *ch);         //读取1byte，成功返回true
void vusb_uart_init(u32 baudrate);      //vusb uart初始化，使用的UART1。输入波特率
void vusb_uart_dis(void);               //关闭vusb gpio及uart1功能
bool vusb_uart_baud_set(u32 type);

#define vusb_uart_putchar(ch)           uart1_putchar(ch)
#endif // _API_VUSB_UART_H
