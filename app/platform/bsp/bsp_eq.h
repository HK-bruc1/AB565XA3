#ifndef _BSP_EQ_H
#define _BSP_EQ_H

#define little_endian_read_16(buf, ofs)         *(uint16_t *)((uint8_t *)buf + (ofs))
#define little_endian_read_32(buf, ofs)         *(uint32_t *)((uint8_t *)buf + (ofs))
#define EQ_BUFFER_LEN                           (260+10)

#define MUSIC_EQ_TBL_LEN                        6

typedef struct {
    u32 *addr;
    u32 *len;
} eq_param;

typedef struct  {
    u8 remain   :   1;      //spp拼包标志
    u16 remian_ptr;         //拼包长度
    u16 rx_size;            //拼包长度
} eq_spp_cb_t;

typedef struct {
    eq_spp_cb_t eq_spp_cb;
    u8 rx_type;
    u8 anc_packet;
} eq_dbg_cb_t;

extern eq_dbg_cb_t eq_dbg_cb;
extern u8 eq_tx_buf[12];
extern u8 eq_rx_buf[EQ_BUFFER_LEN];
extern const char tbl_anc_header[3];

void sys_set_eq(void);
void eq_parse_cmd(void);
void eq_dbg_init(void);
void eq_check_res(void);
const u8 *music_dbb_eq_coef_enc_param_get(u8 *table_max_level, u8 *max_level, u8* dac_spr,u32 *len);

void music_set_eq(u8 band_cnt, const u32 *eq_param);
void eq_coef_cal(int index, int *coef, int db_index, int mode);
void mic_set_eq(u8 band_cnt, const u32 *eq_param);
void bsp_anc_parse_cmd(void);
void eq_dbg_huart_init_m(void);

void music_eq_fade_out(void);
void music_eq_set_param(u32 ch, u32 band_cnt, const u32 *tbl);
void music_drc_set_param(u32 ch, u32 band_cnt, const u32 *tbl);
void mic_eq_set_param(u8 band_cnt, const u32 *tbl);
void mic_drc_set_param(u32 band_cnt, const u32 *tbl);
void music_set_eq(u8 band_cnt, const u32 *eq_param);
void music_set_drc(u8 band_cnt, const u32 *drc_param);
void mic_set_eq(u8 band_cnt, const u32 *eq_param);
void mic_set_drc(u8 band_cnt, const u32 *drc_param);

bool music_set_eq_by_res(u32 addr, u32 len);
bool mic_set_eq_by_res(u32 addr, u32 len);
bool music_set_drc_by_res(u32 addr, u32 len);
bool mic_set_drc_by_res(u32 addr, u32 len);
void music_eq_off(void);
void music_drc_off(void);
void mic_eq_off(void);
void mic_drc_off(void);

void music_set_eq_by_num(u8 num);
void mic_eq_init(void);
#endif
