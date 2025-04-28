#ifndef _API_DAC_H_
#define _API_DAC_H_

void dac_cb_init(void);
void dac_set_volume(u8 vol);
void dac_set_dvol(u16 vol);
void dac_fade_process(void);
void dac_fade_out(void);
void dac_fade_in(void);
void dac_fade_wait(void);
void dac_analog_fade_in(void);
void dac_set_analog_vol(u8 vol);
void dac_digital_enable(void);
void dac_digital_disable(void);
void dac_spr_set(uint spr);
void dac_obuf_init(void);
void dac_src1_init(void);
void dac_power_on(void);
void dac_restart(void);
void dac_power_off(void);
void dac_aubuf_clr(void);
void dac_mono_init(bool mix_en, bool lr_sel);
void adpll_init(u8 out_spr);
void adpll_disable(void);
void adpll_spr_set(u8 out48k_flag);
void obuf_put_one_sample(u32 left, u32 right);
void obuf_put_samples(void *buf, uint len);
void dac_unmute_set_delay(u16 delay);
u16 dac_pcm_pow_calc(void);
void dac_channel_enable(void);
void dac_channel_disable(void);
void dac_channel_exchange(void);
bool dac_sclk_is_enable(void);
void dac_msc_vol_en(void);
u16 dac_get_dvol(void);
void dac_clk_source_sel(u32 val);

//按键音单频TONE
void key_voice_play(u32 freq, u32 play_ms, u32 gain);   //freq: tone频率,单位Hz,  play_ms:持续时间， gain：keytone vol select (0DB, -6DB, 12DB, -24DB)
#endif
