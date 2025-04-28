#ifndef __BSP_ANC_H
#define __BSP_ANC_H

typedef struct {
    u16 init            :1;
    u16 start           :1;
    u16 tp_flag         :1; //通透模式使能标志
    u8  resv[6];            //保留6byte
} anc_cb_t;
extern anc_cb_t anc_cb;

u32 anc_pow10_cal(int index);
void bsp_anc_dbg_eq_param(u8 packet, u8 band_cnt, u32 *eq_buf);
void bsp_anc_start(void);
void bsp_anc_stop(void);
void bsp_anc_set_mode(u8 mode);
void bsp_anc_init(void);
void bsp_anc_exit(void);
void bsp_anc_parse_cmd_process(u8 *rx_buf);
#endif
