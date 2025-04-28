#ifndef _API_SDADC_H
#define _API_SDADC_H

#define CHANNEL_L           0x0F
#define CHANNEL_R           0xF0

#define ADC2DAC_EN          0x01        //ADC-->DAC
#define ADC2SRC_EN          0x02        //ADC-->SRC

typedef void (*pcm_callback_t)(u8 *ptr, u32 samples, int ch_mode);

typedef struct {
    u8 channel;
    u8 sample_rate;
    u16 gain;                   //低5bit为sdadc digital gain, 其它bit为模拟gain控制
    u8 out_ctrl;
    u8 res;
    u16 samples;
    pcm_callback_t callback;
} sdadc_cfg_t;

enum {
    SPR_48000,
    SPR_44100,
    SPR_38000,
    SPR_32000,
    SPR_24000,
    SPR_22050,
    SPR_16000,
    SPR_12000,
    SPR_11025,
    SPR_8000,
    SPR_192000,
    SPR_176400,
    SPR_384000,
    SPR_352800,

};

void sdadc_set_digital_gain(u8 gain);
void set_mic_analog_gain(u8 level, u8 channel);         //level: 0~31, step 3DB
void sdadc_dummy(u8 *ptr, u32 samples, int ch_mode);
void sdadc_pcm_2_dac(u8 *ptr, u32 samples, int ch_mode);
void sdadc_var_init(void);

int sdadc_init(const sdadc_cfg_t *p_cfg);
int sdadc_start(u8 channel);
int sdadc_exit(u8 channel);

//AUX直通PA或LPF。 aux2lpf_en: 0 -> AUX2PA, 1 -> AUX2LPF
//AUX单声道直通，自动选择“单进单出”与“单进双出”配置。
int analog_aux2pa_start(u8 channel, u8 aux2lpf_en);
void analog_aux2pa_exit(u8 channel);

void mic_set_eq(u8 band_cnt, const u32 *eq_param);

void lefmic_zero_detect_do(u8 *ptr, u32 samples);

void anc_var_init(void);
void anc_init(void *cfg);
void anc_start(void);
void anc_stop(void);
void anc_exit(void);
void anc_set_param(void);
u8 anc_set_param_is_busy(void);         //ret: BIT(0):lch is busy; BIT(1):rch is busy
void anc_set_tansparency_mode(u8 en);   //使能/关闭通透模式，默认关闭,调用后需要更新参数才起作用
bool anc_is_tansparency_mode(void);     //当前是否处于通透模式
void anc_mic_mute(u8 mute);
void anc_set_mic_gain(u8 anl, u8 gain);
void set_mic_mute(u32 flag);

void sdadc_pdm_mic_init(u8 mode);
void sdadc_pdm_mic_exit(void);
void mic_bias_trim_w4_done_do(void);
#endif //_API_SDADC_H
