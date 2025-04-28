#ifndef _FUNC_AUX_H
#define _FUNC_AUX_H

///请勿修改func_aux_t结构体
typedef struct {
    u8  pause       : 1,
        start       : 1,
        delay_play  : 1;
    u8  aux2lpf;        //AUX直通PA或者直通LPF
    u8  direct_2_dac;   //模拟直通DAC, 用于处理tone播放
    u8  aux_tone_flag;
    u32 tick;
} func_aux_t;
extern func_aux_t f_aux;

void func_aux_pause_play(void);
void func_aux_message(u16 msg);
void func_aux(void);
void func_aux2dac_piano_start(u8 tone_type);
void func_aux2dac_piano_stop(u8 tone_type);
void func_aux_recover_play(u8 index);
bool aux_is_start(void);
void func_aux_stop(void);
void func_aux_start(void);

#endif // _FUNC_AUX_H
