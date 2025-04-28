#include "include.h"

void bt_aec_process(u8 *ptr, u32 samples, int ch_mode);
void bt_sco_process(u8 *ptr, u32 samples, int ch_mode);
void bt_alc_process(u8 *ptr, u32 samples, int ch_mode);
void bt_adc_process(u8 *ptr, u32 samples, int ch_mode);
void voice_assistant_sdadc_process(u8 *ptr,u32 samples,int ch_mode);

void aux_sdadc_process(u8 *ptr,u32 samples,int ch_mode);
void speaker_sdadc_process(u8 *ptr,u32 samples,int ch_mode);
void bt_sco_mav_process(u8 *ptr, u32 samples);
void bt_adc_process_fix(u8 *ptr, u32 samples, int ch_mode);

#if FUNC_AUX_EN
#define aux_sdadc_callback      aux_sdadc_process
#else
#define aux_sdadc_callback      sdadc_dummy
#endif // FUNC_AUX_EN

#if FUNC_SPEAKER_EN
    #define speaker_sdadc_callback  speaker_sdadc_process
#else
    #define speaker_sdadc_callback  sdadc_dummy
#endif // FUNC_SPEAKER_EN

#if BT_AEC_EN
    #define bt_sdadc_callback    bt_aec_process
#elif BT_ALC_EN
    #define bt_sdadc_callback    bt_alc_process
#else
    #define bt_sdadc_callback    bt_sco_process
#endif //BT_AEC_EN

//MIC analog gain: 0~13(共14级), step 3DB (3db ~ +42db)
//adadc digital gain: 0~63, step 0.5 DB, 保存在gain的低6bit
const sdadc_cfg_t rec_cfg_tbl[] = {
/*   通道,              采样率,     增益,        通路控制,     保留,   样点数,   回调函数*/
    {AUX_CHANNEL_CFG,   SPR_44100,  (2 << 6),    ADC2DAC_EN,     0,      256,    aux_sdadc_callback},            /* AUX     */
    {MIC_CHANNEL_CFG,   SPR_8000,   (12 << 6),   ADC2DAC_EN,     0,      256,    bt_sdadc_callback},             /* BTMIC   */
    {MIC_CHANNEL_CFG,   SPR_8000,   (12 << 6),   ADC2DAC_EN,     0,      128,    sdadc_dummy},                   /* BTMIC test */
    {MIC_CHANNEL_CFG,   SPR_16000,  (10 << 6),   ADC2DAC_EN,     0,      160,    sdadc_dummy},
    {MIC_CHANNEL_CFG,   SPR_16000,  (12 << 6),   ADC2DAC_EN,     0,      256,    speaker_sdadc_callback},
};

void audio_path_init(u8 path_idx)
{
    sdadc_cfg_t cfg;
    memcpy(&cfg, &rec_cfg_tbl[path_idx], sizeof(sdadc_cfg_t));

#if FUNC_AUX_EN
    if (path_idx == AUDIO_PATH_AUX) {
        cfg.channel = get_aux_channel_cfg();
        cfg.gain = ((u16)xcfg_cb.aux_anl_gain << 6);
    }
#endif // FUNC_AUX_EN

    if (path_idx == AUDIO_PATH_BTMIC) {
        cfg.gain = ((u16)BT_ANL_GAIN << 6) | BT_DIG_GAIN;
#if BT_AEC_EN || BT_ALC_EN
        if (xcfg_cb.bt_aec_en) {
            cfg.callback = bt_aec_process;
        }
    #if BT_ALC_EN
        else if (xcfg_cb.bt_alc_en) {
            cfg.callback = bt_alc_process;
        }
    #endif // BT_ALC_EN
        else {
#if 1
            cfg.callback = bt_adc_process;  //不过AEC及ALC，可以过近端降噪
#else
            cfg.callback = bt_adc_process_fix;  //修正关通话算法后，无声的问题，不打开的原因是不想占用公共区
#endif
        }
#endif // BT_AEC_EN

        if (bt_sco_is_msbc() || cfg_bt_sco_dnn_en) {             //如果开了msbc或dnn，则采样率设为16k
            cfg.sample_rate = SPR_16000;
        }
    }
	if (path_idx == AUDIO_PATH_VA || path_idx == AUDIO_PATH_SPEAKER) {
        cfg.gain = ((u16)BT_ANL_GAIN << 6) | BT_DIG_GAIN;
    }
    sdadc_init(&cfg);

    if (path_idx == AUDIO_PATH_BTMIC) {
        if (!bt_sco_is_msbc() && cfg_bt_sco_dnn_en) {             //dnn: 走窄带通话时，ADC为16K，DAC为8K
            dac_spr_set(SPR_8000);
        }
    }
}

void audio_path_start(u8 path_idx)
{
    sdadc_cfg_t cfg;
    memcpy(&cfg, &rec_cfg_tbl[path_idx], sizeof(sdadc_cfg_t));
#if FUNC_AUX_EN
    if (path_idx == AUDIO_PATH_AUX) {
        cfg.channel = get_aux_channel_cfg();
    }
#endif // FUNC_AUX_EN
    sdadc_start(cfg.channel);
}

void audio_path_exit(u8 path_idx)
{
    sdadc_cfg_t cfg;
    memcpy(&cfg, &rec_cfg_tbl[path_idx], sizeof(sdadc_cfg_t));

#if FUNC_AUX_EN
    if (path_idx == AUDIO_PATH_AUX) {
        cfg.channel = get_aux_channel_cfg();
    }
#endif // FUNC_AUX_EN

    sdadc_exit(cfg.channel);
    if (!sys_cb.anc_start) {
        adpll_spr_set(DAC_OUT_SPR);
    }
}

AT(.rodata.aux)
const u8 cfg_aux_ch_tbl[4] = {
    0, CH_AUXL_PB3, CH_AUXL_PE7, CH_AUXL_PA7,
};

u8 get_aux_channel_cfg(void)
{
#if FUNC_AUX_EN
    return cfg_aux_ch_tbl[xcfg_cb.auxl_sel];    //工具配置AUX通路
#else
    return 0;
#endif // FUNC_AUX_EN
}

