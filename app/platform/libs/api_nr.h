#ifndef _API_NR_H
#define _API_NR_H

typedef struct {
    u8 level;                                   // 降噪量
    u8 min_range1;                              // 噪声谱更新系数
    u8 min_range2;                              // 噪声谱更新系数
    u8 min_range3;                              // 噪声谱更新系数
    u8 min_range4;                              // 噪声谱更新系数
    u8 anksnr_en;                               // 是否使能ank snr
    u16 nera_val;                               // anksnr_en为0时有效，alp1固定值
    u32 block_cnt;
} peri_nr_cfg_t;

typedef struct {
    u8  wind_en;
    u8  detect_thr;
    u16 nr_suppress;
    u32 enk1_thr;
    u32 enk2_thr;
    u32 enk2_thr2;
} nr_wind_cfg_t;

typedef struct {
    u8 enable;
    u8 mode;
    u8 level;
    u8 trumpet_en;
    u8 min_range1;                              // 噪声谱更新系数
    u8 min_range2;                              // 噪声谱更新系数
    u8 min_range3;                              // 噪声谱更新系数
    u8 min_range4;                              // 噪声谱更新系数
} near_nr_cfg_t;

typedef struct {
    u8  enable;
    u8  level;
    u8  prior_opt_idx;
	u8  ns_ps_rate;
	u8  low_fre_range_n2;
	u8  trumpet_en;
} ains3_cb_t;

typedef struct {
    //AEC
    u8  aec_en;
    u8  aec_echo_level;
    u16 aec_far_offset;
    u8  ef2df_copy_en;
    u8  nlp_monosig;
    u8  nlp_bypass;

    //ALC
    u8 alc_en;
    u8 alc_fade_in_step;
    u8 alc_fade_out_step;
    u8 alc_fade_in_delay;
    u8 alc_fade_out_delay;
    s32 alc_far_voice_thr;

    //near nr
    near_nr_cfg_t near_nr;
    nr_wind_cfg_t nr_wind;
    ains3_cb_t ains3_nr;

    //far nr
    peri_nr_cfg_t peri_nr;
    u8 far_nr_en;

    //other
    u8 hnltmp_level;
    u8 gain2_smooth_en;
    u8 resv[1];
} bt_voice_cfg_t;
extern volatile bt_voice_cfg_t *bt_voice_cfg;
extern bt_voice_cfg_t bt_voice_cb;

#define EQ_VAILD_LEN                180
#define EQ_VAILD_END                210
//dnn动态检测大噪声切换EQ控制
typedef struct {
    u8  enable;
    u8  change_eq_idx;
    u8  cur_eq_idx;
    u8  status_eq;
	u16 eq_count;
	u16 no_eq_count;
	u16 eq_valid_len;
	u16 eq_valid_end;
    u32 noise_sum_sm;
	s32 dynamic_eq_thr;
} nr_noise_dyn_cb_t;
extern nr_noise_dyn_cb_t nr_noise_dyn_cb;

extern uint8_t cfg_bt_sco_dump;
extern uint8_t cfg_bt_sco_dnn_en;

#endif // _API_NR_H
