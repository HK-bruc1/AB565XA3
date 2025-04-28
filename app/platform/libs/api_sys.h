#ifndef _API_SYS_H
#define _API_SYS_H

#define IRQ_TMR2_VECTOR                 5
#define IRQ_UART_VECTOR                 14
#define IRQ_HUART_VECTOR                15
#define IRQ_TMR3_VECTOR                 16
#define IRQ_PORT_VECTOR                 26

typedef void (*isr_t)(void);
typedef uint8_t bd_addr_t[6];

//按时钟频率升序
enum {
    SYS_RC3M,                //typical = 3.02MHz
    SYS_26M,
    SYS_52M,
    SYS_80M,
    SYS_120M,
};

enum {
    INDEX_VOICE,
    INDEX_DECODE,
    INDEX_STACK,
    INDEX_RES_PLAY,
    INDEX_RSVD0,
    INDEX_RSVD1,
    INDEX_RSVD2,
    INDEX_RSVD3,
    INDEX_MAX_NB = 8,
};

typedef struct _sys_t {
    u8  cnt_1us;             //delay 1us cnt
    u8  main_start;          //Main是否已启动
    u8  aupll_type;
    u8  aupll_band;
    u8  aupllhz_div;
    u8  clk_sel;             //clock select
    u8  clk_curr;            //curre sysclk
    u8  clk_set;             //main thread sysclk
    u8  clk_req[8];          //[0]=voice, [1]=music, [2]=stack
    u8  vddcore_sel;
    u8  e2p_xcfg_mem_en;    //e2p同时用于xcfg及memory记忆
    u8  tmr1ms_cnt;
    u8  resv;
    u32 rc2m_rtc_clk;
    u32 rc3m_clk;
    u8  tmr5ms_cnt0;
    u8  tmr5ms_cnt1;
    u8  tmr5ms_cnt2;
    u8  tmr5ms_cnt3;
    u32 uart0baud;
    u8  spi_baud;
    u8  spi_baud_w;
} sys_t;
extern sys_t sys;

typedef struct {
    u8  vbg;            //trim到0.6V的寄存器配置值      RI_BGTRIM,      RTCCON8[28:24]
    u8  vbat;           //trim到4.2V的寄存器配置值      RI_BGCH_TRIM,   RTCCON8[14 10]
    u16 chg_icoef;      //充电电流换算系数
    u8  dig_gain;       //rf digital gain
    u8  ft_trim_flag;
    u16 dac_digvol;
    u16 vbg_volt;       //vbg目标值, 默认1200. FT TRIM可以进行偏移调整
    u8  resv[2];
} sys_trim_t;
extern sys_trim_t sys_trim;
extern u32 vuart_baudrate;
extern uint8_t cfg_spiflash_speed_up_en;

isr_t register_isr(int vector, isr_t isr);

uint8_t sys_clk_get(void);
void sys_clk_set_do(u32 sys_clk);                       //上电启动未启用OS时调用
bool sys_clk_set(uint8_t sys_clk);                      //main线程调用，一般给客户调用
bool sys_clk_req(uint8_t index, uint8_t sys_clk);       //其它任务调用，一般在库内调用
bool sys_clk_free(uint8_t index);                       //跟sys_clk_req成对使用
bool sys_clk_free_all(void);

u32 sys_get_rc2m_clkhz(u32 type);
void dbg_clk_out(u32 type, u32 div);
void sys_uart0_baud_set(u32 baud);

bool sys_irq_init(int vector, int pr, isr_t isr);   //注册一个系统中断(pr=1注册高优先级中断时, 中断函数需要加FIQ), 详见timer.c例程

void watch_point_set(u32 addr);
void wdt_irq_init(void);
void uart_putchar(char ch);

void spi_init(void);
void my_spi_putc(u8 ch);
void bt_dbg_pin_init(void);
uint calc_crc(void *buf, uint len, uint seed);
#endif // _API_SYS_H
