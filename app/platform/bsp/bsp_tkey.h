#ifndef _BSP_TKEY_H
#define _BSP_TKEY_H

#define TKC_MAX_BITS               2
#define TKC_MAX_SIZE              (1 << TKC_MAX_BITS)

typedef struct {
    u32 anov_thres;
    u8  sta;
    u8  stable_cnt;
    u16 cnt;
} te_cali_cb_t;
extern te_cali_cb_t te_cali_cb;

///请勿修改spp_tkcb_t结构体
typedef struct {
    u16 tk_buf[16];
    u16 te_buf[16];
    char spp_buf[128];
    u8  tk_idx;
    u8  ch;
volatile u8 tk_print_sta;
    u32 ticks;
} spp_tkcb_t;
extern spp_tkcb_t spp_tkcb;

extern u16 tkcnt_tmr[4];

void bsp_tkey_init(void);
bool bsp_tkey_wakeup_en(void);
u8 tkey_get_key(void);
u8 tkey_is_inear(void);     //入耳检测，0：无效，1：入耳，2：出耳
u8 tkey_wakeup_status(void);
void tkey_inear_disable(void);
void bsp_tkey_spp_tx(void);
void bsp_tkey_str_spp_tx(char *str);
u8 touchear_is_online(void);
bool touchear_is_wakeup(void);
bool tkey_is_pressed(void);
void bsp_bcnt_temp_calibration_start(void);
void bsp_bcnt_temp_calibration(void);
void bsp_bcnt_temp_calibration_stop(void);
void tkey_press_timeout_clr(void);
void tkey_press_timeout_process(void);

#if USER_TKEY
void bsp_charge_bcnt_calibration(u32 min_avg_cnt);
void bsp_charge_tebcnt_calibration(void);
void bsp_charge_tebcnt_calibration_stop(void);
#else
#define bsp_charge_bcnt_calibration(x)
#define bsp_charge_tebcnt_calibration()
#define bsp_charge_tebcnt_calibration_stop()
#endif // USER_TKEY

#endif
