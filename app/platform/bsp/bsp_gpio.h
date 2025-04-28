#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H

enum {
    GPIOxSET    =   0,
    GPIOxCLR,
    GPIOx,
    GPIOxDIR,
    GPIOxDE,
    GPIOxFEN,
    GPIOxDRV,
    GPIOxPU,
    GPIOxPD,
    GPIOxPU200K,
    GPIOxPD200K,
    GPIOxPU300,
    GPIOxPD300,
};

enum {
    EDGE_FALLING = 0,
    EDGE_RISING,
};

enum {
    WKG_NONE = 0,
    WKG_PU,
    WKG_PD,
};

typedef struct {
    psfr_t sfr;             //GPIO SFR ADDR
    u8 num;
    u8 type;
} gpio_t;

extern const u8 adc_gpio_tbl[16];
extern const u8 configure_gpio_tbl[16];
void bsp_gpio_cfg_init(gpio_t *g, u8 io_num);
void wakeup_gpio_config(u8 io_num, u8 edge, u8 pupd_cfg); //任意GPIO的边沿唤醒配置。参数edge: 0下降沿, 1上升沿,  参数pupd  0:不开内部上拉, 1:开内部上拉, 2:开内部下拉
u8 get_adc_gpio_num(u8 adc_ch);
void adcch_io_pu10k_enable(u8 adc_ch);
uint gpiox_get_status(uint io_num);
void bsp_gpiode_reinit_save(void);
void bsp_gpiode_recover(void);
int gpiox_get_voltage_level(uint adc_num);
void gpiox_crbfpu2_cfg(u32 io_map, u32 pd_flag, u32 set_flag);
#endif // _BSP_GPIO_H
