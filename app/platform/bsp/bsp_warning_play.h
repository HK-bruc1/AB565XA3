#ifndef _WARNING_PLAY_H
#define _WARNING_PLAY_H

typedef struct {
    u8 *buf;
    u32 len;
} sbcplay_cb_t;
extern sbcplay_cb_t sbcplay_cb;

typedef struct
{
    u8 res_dnr_sta;
    u8 res_bypass;
    u8 res_dac_sta;
}sbc_res_t;

extern sbc_res_t sbc_res;


bool warning_play(uint index, uint type);
bool sbc_res_play(uint index, u32 tws_sync);
int bt_ring_res_play(u8 num);
u32 sbcplay_set(uint index, uint type);
void wav_res_play_do(u32 addr, u32 len, bool sync);
bool sys_warning_play_do(uint index, uint type, u32 tws_sync);
u8 get_warning_volume(uint index);
extern void dac_src1_lr_fix(void);
extern void dac_src1_lr_fix_restore(void);
bool bt_ring_sbc_stop(u8 force);
void sbc_res_var_init(void);
void wav_res_play(u32 addr, u32 len);

#endif
