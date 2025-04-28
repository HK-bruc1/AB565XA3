#ifndef __BSP_AUDIO_H__
#define __BSP_AUDIO_H__

#define AUDIO_PATH_AUX             0
#define AUDIO_PATH_BTMIC           1
#define AUDIO_PATH_MIC_TEST        2
#define AUDIO_PATH_VA              3
#define AUDIO_PATH_SPEAKER         4

#define NR_CHCHE_LEN        256

typedef struct{
    u16 wptr;
    u16 rptr;
    u16 optr;
    u16 len;
    u16 olen;
    s16 buf[NR_CHCHE_LEN];
} sco_cache_t;

void audio_path_init(u8 path_idx);
void audio_path_exit(u8 path_idx);
void audio_path_start(u8 path_idx);
u8 get_aux_channel_cfg(void);

#endif //__BSP_AUDIO_H__

