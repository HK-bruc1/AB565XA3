#include "include.h"

#if ANC_EN

#define TRACE_EN                1

#if TRACE_EN
#define TRACE(...)              printf(__VA_ARGS__)
#else
#define TRACE(...)
#endif

typedef struct {
    u16 gain;
	u8  nos_band;
	u8  msc_band;
	u32 param_buf[8 * 5 + 2];
	u32 drc_buf[20];
	u8  drc_band;
	u8  drc_en;
	u8  rfu[1];
    u8  bypass       :1;
} anc_channel_t;

struct anc_cfg_t {
	anc_channel_t *ch;
	u8 spr;
	u8 rfu[1];
    u16 mic_type        :3; //mic输入类型
};

typedef struct {
    u32 *addr;
    u32 *len;
} anc_param;

anc_channel_t anc_ch AT(.anc_buf.ch);
struct anc_cfg_t anc_cfg AT(.anc.cfg);

#define ANC_FIX_BIT         23
#define EQ_GAIN_MAX         0x07eca9cd      //24dB
#define EQ_GAIN_MIN         0x00081385      //-24dB
#define EQ_GAIN_STEP        2               //0.2dB

static const anc_param anc_ff_eq_tbl[8] = {
    //降噪模式参数
    {&RES_BUF_ANC_L0_NOS_EQ,      &RES_LEN_ANC_L0_NOS_EQ},
    {&RES_BUF_ANC_L0_NOS_DRC,     &RES_LEN_ANC_L0_NOS_DRC},
    {&RES_BUF_ANC_R0_NOS_EQ,      &RES_LEN_ANC_R0_NOS_EQ},
    {&RES_BUF_ANC_R0_NOS_DRC,     &RES_LEN_ANC_R0_NOS_DRC},
    //通透模式参数
    {&RES_BUF_ANC_TP_L0_NOS_EQ,   &RES_LEN_ANC_TP_L0_NOS_EQ},
    {&RES_BUF_ANC_TP_L0_NOS_DRC,  &RES_LEN_ANC_TP_L0_NOS_DRC},
    {&RES_BUF_ANC_TP_R0_NOS_EQ,   &RES_LEN_ANC_TP_R0_NOS_EQ},
    {&RES_BUF_ANC_TP_R0_NOS_DRC,  &RES_LEN_ANC_TP_R0_NOS_DRC},
};

u32 bsp_anc_gain_adjust_do(s8 value)
{
    s32 gain, abs_gain, adjust_val = 0;
    gain = (s32)sys_cb.anc_mic_gain;                                //初始gain值
    if (value != 0) {
        adjust_val = (s32)anc_pow10_cal(value * EQ_GAIN_STEP);
        gain = ((s64)gain * adjust_val) >> ANC_FIX_BIT;
        abs_gain = gain > 0? gain : -gain;
        if (abs_gain > EQ_GAIN_MAX || abs_gain < EQ_GAIN_MIN) {
            return sys_cb.anc_mic_gain;
        }
    }
    return (u32)gain;
}

void bsp_anc_gain_adjust(s8 value)
{
    if (anc_cfg.ch == NULL) {
        return;
    }
    sys_cb.anc_adjust_val = value;
    anc_cfg.ch->param_buf[0] = bsp_anc_gain_adjust_do(value);
    anc_set_param();                //更新参数
}


bool bsp_anc_res_check_crc(u32 addr, u32 len)
{
    u8 *ptr = (u8 *)addr;
    u8 offset = 0;
    if (ptr[0] == 'E' && ptr[1] == 'q' && ptr[2] == 0x01) {
        offset = 4;
    }
    u8 band_cnt = ptr[3+offset];
    if (band_cnt > 8) {
        return false;
    }
    u32 res_crc = ptr[band_cnt*27+11+offset] << 8 | ptr[band_cnt*27+10+offset];
    u32 cal_crc = calc_crc(ptr, band_cnt*27+10+offset, 0xffff);
    if (res_crc == cal_crc) {
        return true;
    } else {
        return false;
    }
}

void bsp_anc_set_nos_param(u8 band_cnt, const u32 *param)
{
    anc_cfg.ch->nos_band = band_cnt;
    if (band_cnt == 0 || param == NULL) {
        return;
    }
    memcpy(anc_cfg.ch->param_buf, param, (band_cnt * 5 + 1) * 4);
}

void bsp_anc_set_msc_param(u8 band_cnt, const u32 *param)
{
    anc_cfg.ch->msc_band = band_cnt;
    if (band_cnt == 0 || param == NULL) {
        return;
    }
    anc_cfg.ch->param_buf[41] = param[0];
    memcpy(&anc_cfg.ch->param_buf[anc_cfg.ch->nos_band * 5 + 1], param + 1, band_cnt * 5 * 4);
}

void bsp_anc_set_drc_param(u8 band_cnt, const u32 *param)
{
    if (band_cnt == 0 || param == NULL) {
        return;
    }
    anc_cfg.ch->drc_en = 1;
    anc_cfg.ch->drc_band = band_cnt;
    memcpy(&anc_cfg.ch->drc_buf[0], param, 4* 4);
    anc_cfg.ch->drc_buf[4] = 0;                             //in_level
    anc_cfg.ch->drc_buf[5] = 0;                             //out_gain
    anc_cfg.ch->drc_buf[6] = param[4];                      //tav
    anc_cfg.ch->drc_buf[7] = 0x00800000;                    //in_rms = 0
    memcpy(&anc_cfg.ch->drc_buf[8], &param[5], 3*4*band_cnt);
}

//EQ V2
int bsp_anc_set_nos_param_by_res(u32 addr, u32 len)
{
    if (len == 0) {
        return -1;
    }
    if (!bsp_anc_res_check_crc(addr, len)) {
        return -2;
    }
    u8 *ptr = (u8 *)addr;
    u8 band_cnt = ptr[7];
    u32 *param = (u32 *)&ptr[8];

    bsp_anc_set_nos_param(band_cnt, param);
    sys_cb.anc_mic_gain = anc_cfg.ch->param_buf[0];                                 //保存资源文件中的gain值
    if (xcfg_cb.anc_gain_en) {
        if (!anc_cb.tp_flag && sys_cb.anc_adjust_val != 0) {	                            //写入量产测试的Gain
            anc_cfg.ch->param_buf[0] = bsp_anc_gain_adjust_do(sys_cb.anc_adjust_val);
        } else if (anc_cb.tp_flag && sys_cb.tp_adjust_val != 0) {	                            //写入量产测试的Gain
            anc_cfg.ch->param_buf[0] = bsp_anc_gain_adjust_do(sys_cb.tp_adjust_val);
        }
    }
    return 0;
}

//EQ V2
int bsp_anc_set_msc_param_by_res(u32 addr, u32 len)
{
    if (len == 0) {
        return -1;
    }
    if (!bsp_anc_res_check_crc(addr, len)) {
        return -2;
    }
    u8 *ptr = (u8 *)addr;
    u8 band_cnt = ptr[7];
    u32 *param = (u32 *)&ptr[8];
    bsp_anc_set_msc_param(band_cnt, param);
    return 0;
}

//DRC V3
int bsp_anc_set_drc_param_by_res(u32 addr, u32 len)
{
    u8  drc_cnt;
    u32 res_crc, cal_crc;

    if (len == 0) {
        return -1;
    }
    u8 *ptr = (u8 *)addr;
    if (ptr[2] != 0x02 || ptr[7] > 4 || !ptr[7]) {          //DRC V3
        return -2;
    }
    drc_cnt = ptr[7];
    res_crc = ptr[drc_cnt * 17 + 41] << 8 | ptr[drc_cnt * 17 + 40];
    cal_crc = calc_crc(ptr, drc_cnt * 17 + 40, 0xffff);
    if (res_crc != cal_crc) {
        return -3;
    }
    bsp_anc_set_drc_param(drc_cnt, (const u32 *)&ptr[8]);
    return 0;
}

void bsp_anc_set_mic_gain(u8 anl, u8 gain)
{
    TRACE("anc set mic gain:%d %d\n", anl, gain);
    anc_set_mic_gain(anl, gain);
}

void bsp_anc_mic_mute(u8 mute)
{
    TRACE("anc mic mute:%x\n", mute);
    set_mic_mute(mute);
}

void bsp_anc_start(void)
{
    dac_fade_in();
    if (sys_cb.anc_start == 0) {
        sys_cb.anc_start = 1;
        sys_cb.sleep_aupll_en = 1;
        anc_start();
    }
}

void bsp_anc_stop(void)
{
    if (sys_cb.anc_start) {
        anc_stop();
        sys_cb.anc_start = 0;
        sys_cb.sleep_aupll_en = 0;
    }
}

//无线/uart调anc参数接口
void bsp_anc_dbg_eq_param(u8 packet, u8 band_cnt, u32 *eq_buf)
{
    if (packet > 2) {
        anc_set_tansparency_mode(1);
        packet -= 2;
    } else {
        anc_set_tansparency_mode(0);
    }

    if (packet == 1) {
        bsp_anc_set_nos_param(band_cnt, eq_buf);
    }
    anc_set_param();                //更新参数
}

void bsp_anc_set_param(u8 tp)
{
    const anc_param *anc_tbl;

    anc_tbl = &anc_ff_eq_tbl[4 * tp];
#if BT_TWS_EN
    if (xcfg_cb.bt_tws_en) {
        if (!(sys_cb.tws_force_channel - 1)) {                     //TWS下FF/FB模式右声道取R的曲线给MICL
            anc_tbl = &anc_ff_eq_tbl[4 * tp + 2];
        }
    }
#endif
    anc_set_tansparency_mode(tp);
    bsp_anc_set_nos_param_by_res(*anc_tbl[0].addr, *anc_tbl[0].len);
    bsp_anc_set_drc_param_by_res(*anc_tbl[1].addr, *anc_tbl[1].len);
    anc_set_param();            //更新参数
}

void bsp_anc_parse_cmd_process(u8 *rx_buf)
{
//    printf("anc rx:");
//    print_r(rx_buf, 16);

    u8 *ptr = rx_buf;
    u8 channel = ptr[5];
    u8 bit_ctl = ptr[6];
    s8 adjust0 = (s8)ptr[8];
    bool eq_tool = (bool)(bit_ctl & BIT(4));//判断是EQ工具还是量产工具
    bool tp_flag = (bool)(bit_ctl & BIT(5));//判断是anc模式测试，还是通透模式测试


    if(!tp_flag){
        if (!sys_cb.anc_start|| sys_cb.anc_user_mode != 1) {
            bsp_anc_set_mode(1);
        }
    }else{
        if (!sys_cb.anc_start|| sys_cb.anc_user_mode != 2) {
            bsp_anc_set_mode(2);
        }
    }

    eq_dbg_cb.eq_spp_cb.rx_size = 16;

    do {
        u8 ch = channel - 1;
#if BT_TWS_EN
        if (xcfg_cb.bt_tws_en) {
            if (ch == 2 || (ch == func_bt_tws_get_channel())) {   //声道不相等，转发
                if(bt_tws_is_connected()) {
                    if (ch == 2) {                              //把这个声道去掉，重新算下CRC
                        ptr[5] = (func_bt_tws_get_channel() == 1) ? 2 : 1;
                        u16 crc = calc_crc(ptr, 14, 0xffff);
                        ptr[14] = crc;
                        ptr[15] = crc >> 8;
                    }
                    bt_tws_sync(TWS_SYNC_INFO_EQ);
                }
                if (ch != 2) {                                  //如果收到单声道的命令，转发后跳出
                    break;
                }
            }
        }
#endif


        bool ch_mute = !((bit_ctl & BIT(0)) && (bit_ctl & BIT(2)) == 0);
        s8  old_ajust_val = sys_cb.anc_adjust_val;

        bsp_anc_mic_mute(ch_mute);
        if (!eq_tool) {
            bsp_anc_gain_adjust(adjust0);
        }

        if (bit_ctl & BIT(1)) {
             if (!tp_flag) {
                param_write((u8*)&sys_cb.anc_adjust_val, PARAM_ANC_MIC_VAL, 1);
             } else {
                sys_cb.tp_adjust_val = sys_cb.anc_adjust_val;
                sys_cb.anc_adjust_val = old_ajust_val;
                param_write((u8*)&sys_cb.tp_adjust_val, PARAM_ANC_TP_MIC_VAL, 1);
            }
        }

        //调整again，研发调试用
        if (bit_ctl & BIT(4)) {
            bsp_anc_set_mic_gain(1, ptr[10]);
        }

        if (bit_ctl & BIT(1)) {
            param_sync();
        }
        TRACE("## anc dbg mute:%d  eq_tool:%d anl_vol:%d save:%d\n", ch_mute, eq_tool, ptr[10], (bool)(bit_ctl & BIT(1)));
    } while (0);
}

//mode: 0[off], 1[anc], 2[tansparency]
void bsp_anc_set_mode(u8 mode)
{
    if (!xcfg_cb.anc_en) {
        return;
    }
    sys_cb.anc_user_mode = mode;
    if (mode == 0) {
        bsp_anc_stop();
    } else {
        bsp_anc_set_param(mode - 1);
        bsp_anc_start();
    }
    TRACE("anc user mode:%d\n", mode);
}

void bsp_anc_init(void)
{
    if (!xcfg_cb.anc_en) {
        TRACE("'xcfg_cb.anc_en' define 0\n");
        return;
    }
    memset(&anc_cfg, 0, sizeof(anc_cfg));
    memset(&anc_ch, 0, sizeof(anc_ch));
    anc_cfg.ch = &anc_ch;
    anc_cfg.spr = SPR_384000;
    anc_cfg.ch->drc_en = 0;
    anc_cfg.ch->gain = ((u16)BT_ANL_GAIN << 6) | 0;
    anc_cfg.ch->bypass = 0;
    param_read((u8*)&sys_cb.anc_adjust_val, PARAM_ANC_MIC_VAL, 1);
    param_read((u8*)&sys_cb.tp_adjust_val,  PARAM_ANC_TP_MIC_VAL, 1);
    anc_init(&anc_cfg);
    bsp_anc_set_param(0);

    TRACE("anc init\n");
    TRACE("anc mic type:%d\n", anc_cfg.mic_type);
    TRACE("anc mic gain:%x\n", anc_cfg.ch->gain);
    TRACE("anc adjust mic gain: %d, %d\n", sys_cb.anc_adjust_val, sys_cb.tp_adjust_val);
}

void bsp_anc_exit(void)
{
    anc_exit();
    sys_cb.anc_start = 0;
}
#endif //ANC_EN
