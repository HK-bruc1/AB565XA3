#include "include.h"
#include "func.h"
#include "func_bt.h"

extern bt_voice_cfg_t bt_voice_cb;
void bt_voice_init(bt_voice_cfg_t *p);
void bt_voice_exit(void);
void bt_sco_eq_drc_init(u32 msbc_flag);
void bt_sco_eq_drc_exit(void);
void dac_fade_dvol_wait(void);
void bt_sco_magic_init(void);
u8 tws_call_switch_is_ready(void);

extern const u16 nr_wind_suppress_tbl[4];
extern const u8 nr_min_range_tbl[4][4];
extern const u8 aec_nlp_tbl[4];
extern const u8 low_fre_range_tbl[4];

#if FUNC_BT_EN

void sco_audio_init(void)
{
    memset(&bt_voice_cb, 0, sizeof(bt_voice_cfg_t));
    memset(&nr_noise_dyn_cb, 0, sizeof(nr_noise_dyn_cb_t));

#if BT_AEC_EN || BT_ALC_EN
    if (xcfg_cb.bt_aec_en) {
#if BT_AEC_EN
        bt_voice_cb.aec_en          = 1;
        bt_voice_cb.aec_echo_level  = BT_ECHO_LEVEL;
        bt_voice_cb.aec_far_offset  = BT_FAR_OFFSET;
        bt_voice_cb.ef2df_copy_en   = aec_nlp_tbl[xcfg_cb.bt_aec_nlp_sel] & 0x01;
        bt_voice_cb.nlp_monosig     = aec_nlp_tbl[xcfg_cb.bt_aec_nlp_sel] & 0x02;
        bt_voice_cb.nlp_bypass      = 0;
        bt_voice_cb.hnltmp_level    = 0;                    //范围：0~6, 值越大双讲越好，nlp回声抑制变差
#endif
    }
#if BT_ALC_EN
    else if (xcfg_cb.bt_alc_en) {
        bt_voice_cb.alc_en = 1;
        bt_voice_cb.alc_fade_in_delay   = BT_ALC_FADE_IN_DELAY;
        bt_voice_cb.alc_fade_in_step    = BT_ALC_FADE_IN_STEP;
        bt_voice_cb.alc_fade_out_delay  = BT_ALC_FADE_OUT_DELAY;
        bt_voice_cb.alc_fade_out_step   = BT_ALC_FADE_OUT_STEP;
        bt_voice_cb.alc_far_voice_thr   = BT_ALC_VOICE_THR;
        if (!bt_sco_is_msbc()) {
            bt_voice_cb.alc_fade_in_delay >>= 1;
        }
    }
#endif

#endif

#if BT_SCO_MAV_EN
    bt_sco_magic_init();
#endif

#if BT_SCO_NR_EN
    u8 min_range_sel = xcfg_cb.bt_sco_nr_min_range_sel & 0x3;
    bt_voice_cb.near_nr.enable          = xcfg_cb.bt_sco_nr_en;
    bt_voice_cb.near_nr.level           = xcfg_cb.bt_sco_nr_level & 0x1f;
    bt_voice_cb.near_nr.mode            = 0;
    bt_voice_cb.near_nr.trumpet_en      = xcfg_cb.bt_sco_nr_trumpet_en;
    bt_voice_cb.near_nr.min_range1      = nr_min_range_tbl[min_range_sel][0];
    bt_voice_cb.near_nr.min_range2      = nr_min_range_tbl[min_range_sel][1];
    bt_voice_cb.near_nr.min_range3      = nr_min_range_tbl[min_range_sel][2];
    bt_voice_cb.near_nr.min_range4      = nr_min_range_tbl[min_range_sel][3];
    bt_voice_cb.nr_wind.wind_en         = xcfg_cb.bt_sco_nr_wind_en;
    bt_voice_cb.nr_wind.detect_thr      = xcfg_cb.bt_sco_nr_wind_det_thr + 1;
    bt_voice_cb.nr_wind.nr_suppress     = nr_wind_suppress_tbl[xcfg_cb.bt_sco_nr_wind_level & 0x03];
    bt_voice_cb.nr_wind.enk1_thr        = 55000;
    bt_voice_cb.nr_wind.enk2_thr        = 1000000;
    bt_voice_cb.nr_wind.enk2_thr2       = 18000000;
    bt_voice_cb.gain2_smooth_en         = 0;        //DNN的gain2是否平滑处理

    nr_noise_dyn_cb.enable              = BT_NR_NOISE_DYN_EQ_EN;
    nr_noise_dyn_cb.dynamic_eq_thr      = 200000;   //大噪声阈值，指示切换MIC EQ
    nr_noise_dyn_cb.eq_valid_len        = EQ_VAILD_LEN;
    nr_noise_dyn_cb.eq_valid_end        = EQ_VAILD_END;
#endif

#if BT_SCO_AINS3_EN
    bt_voice_cb.ains3_nr.enable = xcfg_cb.bt_sco_ains3_en;
    bt_voice_cb.ains3_nr.level = xcfg_cb.bt_sco_nr_level & 0x1f;
    bt_voice_cb.ains3_nr.trumpet_en = xcfg_cb.bt_sco_nr_trumpet_en;
    bt_voice_cb.ains3_nr.prior_opt_idx = 6;
    bt_voice_cb.ains3_nr.ns_ps_rate = xcfg_cb.bt_ains3_ps_rate;
    bt_voice_cb.ains3_nr.low_fre_range_n2 = low_fre_range_tbl[xcfg_cb.bt_ains3_low_fre_range & 0x03];
#endif

#if BT_SCO_FAR_NR_EN
    bt_voice_cb.far_nr_en = xcfg_cb.bt_sco_far_nr_en;
    bt_voice_cb.peri_nr.level = xcfg_cb.bt_sco_far_nr_level & 0xf;
    bt_voice_cb.peri_nr.min_range1 = 30;
    bt_voice_cb.peri_nr.min_range2 = 50;
    bt_voice_cb.peri_nr.min_range3 = 80;
    bt_voice_cb.peri_nr.min_range4 = 120;
    bt_voice_cb.peri_nr.anksnr_en = 1;
    bt_voice_cb.peri_nr.nera_val = 0x7eb8;
#endif

    sco_set_incall_flag(INCALL_FLAG_SCO);
#if DAC_DNR_EN
    dac_dnr_set_sta(0);
#endif
    bt_voice_init(&bt_voice_cb);

    bt_sco_eq_drc_init(bt_sco_is_msbc());       //设置DAC、MIC EQ/DRC

    dac_aubuf_clr();
    audio_path_init(AUDIO_PATH_BTMIC);
    audio_path_start(AUDIO_PATH_BTMIC);
    dac_set_anl_offset(1);
    bsp_change_volume(bt_hfp_get_sys_vol(sys_cb.hfp_vol));
    dac_fade_in();
}

void sco_audio_exit(void)
{
    dac_fade_out();
    dac_fade_wait();
    dac_aubuf_clr();
    audio_path_exit(AUDIO_PATH_BTMIC);
    bt_voice_exit();
    bt_sco_eq_drc_exit();
    dac_set_anl_offset(0);
#if DAC_DNR_EN
    dac_dnr_set_sta(xcfg_cb.dac_dnr_en);
#endif
    sco_clr_incall_flag(INCALL_FLAG_SCO);
    bsp_change_volume(sys_cb.vol);
}

//大噪声时使用不同的EQ, 硬件DNN下才起作用。
#if BT_NR_NOISE_DYN_EQ_EN
void bt_nr_noise_dynamic_process(void)
{
    u32 mic_eq_addr, mic_eq_len;
    nr_noise_dyn_cb_t *st = &nr_noise_dyn_cb;

    if (st->enable) {
        if (st->cur_eq_idx != st->change_eq_idx) {
            st->cur_eq_idx = st->change_eq_idx;
            if (st->cur_eq_idx) {
                if (bt_sco_is_msbc()) {
                    mic_eq_addr  = RES_BUF_EQ_BT_MIC_16K_NS_EQ;
                    mic_eq_len   = RES_LEN_EQ_BT_MIC_16K_NS_EQ;
                } else {
                    mic_eq_addr  = RES_BUF_EQ_BT_MIC_8K_NS_EQ;
                    mic_eq_len   = RES_LEN_EQ_BT_MIC_8K_NS_EQ;
                }
            } else {
                if (bt_sco_is_msbc()) {
                    mic_eq_addr  = RES_BUF_EQ_BT_MIC_16K_EQ;
                    mic_eq_len   = RES_LEN_EQ_BT_MIC_16K_EQ;
                } else {
                    mic_eq_addr  = RES_BUF_EQ_BT_MIC_8K_EQ;
                    mic_eq_len   = RES_LEN_EQ_BT_MIC_8K_EQ;
                }
            }
            mic_set_eq_by_res(mic_eq_addr, mic_eq_len);
        }
    }
}
#endif

static void sfunc_bt_call_process(void)
{
    func_process();
    func_bt_sub_process();
#if (BT_TWS_AUTO_SWITCH && BT_TWS_TSCO_AUTO_SWITCH)
    if ((xcfg_cb.bt_tswi_sco_en) && tws_switch_is_need() && tws_call_switch_is_ready()) {
        bt_tws_switch();
    }
#endif
#if BT_NR_NOISE_DYN_EQ_EN
    if (nr_noise_dyn_cb->enable) {
        bt_nr_noise_dynamic_process();
    }
#endif
#if BT_HFP_CALL_EARPHONE_EN
    if (sys_cb1.call_switch_ticks && tick_check_expire(sys_cb1.call_switch_ticks, 2000)) {      //防抖
        if (sco_is_connected()) {
            sys_cb1.call_switch = 0;
            sys_cb1.call_switch_ticks = 0;
        }
    }

    if (sys_cb1.call_need_switch) {
        sys_cb1.call_need_switch = 0;
        bt_call_private_switch();
//        printf("call private switch\n");
    }
#endif
}

void sfunc_bt_call_enter(void)
{
#if BT_HFP_CALL_EARPHONE_EN
    sys_cb1.call_switch = 1;
    sys_cb1.call_switch_ticks = tick_get();
#endif
    sco_set_incall_flag(INCALL_FLAG_CALL);
    if(sys_cb.incall_flag == INCALL_FLAG_FADE) {
        bsp_change_volume(bt_hfp_get_sys_vol(sys_cb.hfp_vol));
        dac_fade_in();
    }
}

void sfunc_bt_call_exit(void)
{
	printf("%s\n", __func__);
    bool vol_change = sco_clr_incall_flag(INCALL_FLAG_CALL);

#if BT_HFP_CALL_EARPHONE_EN
    sys_cb1.call_switch = 0;
    sys_cb1.call_switch_ticks = 0;
#endif

    if(vol_change) {
        u32 tick = tick_get();
        while (!tick_check_expire(tick, 800)) {      //等待sco退出
            delay_5ms(2);
            if (!sys_cb.incall_flag) {
                break;
            }
        }
        bsp_change_volume(sys_cb.vol);
    }
}

AT(.text.func.bt)
void sfunc_bt_call(void)
{
    printf("%s\n", __func__);
    sfunc_bt_call_enter();
    while ((f_bt.disp_status >= BT_STA_OUTGOING) && (func_cb.sta == FUNC_BT)) {
        sfunc_bt_call_process();
        sfunc_bt_call_message(msg_dequeue());
//        func_bt_display();
    }
    sfunc_bt_call_exit();
}
#else

void sco_audio_init(void){}
void sco_audio_exit(void){}

#endif //FUNC_BT_EN
