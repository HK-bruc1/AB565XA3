#ifndef _API_TKEY_H
#define _API_TKEY_H

typedef struct {
    u16 cdpr;
    u8  type;                           //0: disable channel, 1: touch key channel, 2: touch ear channel
    u8  ctrim;                          //cur channel ctrim select
    u8  itrim;                          //cur channel itrim select
} tkey_ch_t;

typedef struct {
    const tkey_ch_t *key[4];
    union {
        struct {
            u32 fil_low     :  4;       //touch key state low state filter length, fil_low + 1
            u32 fil_high    :  4;       //touch key state high state filter length, fil_high + 1
            u32 fil_except  :  8;       //touch key state exception filter length, fil_except + 1
            u32 fil_val     :  4;       //touch key base_cnt valid filter length, fil_val + 1
            u32 to_except   :  12;      //touch key state high timeout length
        };
        u32 reg_tktmr;
    };
    union {
        struct {
            u32 tkpthd      :   12;     //touch key press threshold
            u32 resv0       :   4;
            u32 tkrthd      :   12;     //touch key release threshold
            u32 pto_except  :   4;      //touch key press time out length. (pto_except << 8)
        };
        u32 reg_tkpthd;
    };
    union {
        struct {
            u32 tksthd      :   12;     //touch key smaller threshold
            u32 resv1       :   4;
            u32 tklthd      :   12;     //touch key larger threshold
            u32 resv2       :   4;
        };
        u32 reg_tkethd;
    };

    union {
        struct {
            u32 tkvthd      :   16;     //touch key variance threshold
            u32 val         :   16;     //touch key variance
        };
        u32 reg_tkvari;
    };

    union {
        struct {
            u32 tkarthd     :   12;     //touch key average range threshold
            u32 tkaethd     :   4;      //touch key average equal threshold
            u32 tkvfil      :   8;      //touch key variance filter count
            u32 tkbadd      :   8;      //touch key base counter adder value
        };
        u32 reg_tkvarithd;
    };

    union {
        struct {
            u32 press_vari_en   :   1;  //press variance enable bit
            u32 rels_vari_en    :   1;  //release variance enable bit
            u32 press_fil_sel   :   1;  //press variance after filter
            u32 rels_fil_sel    :   1;  //release variance after filter
            u32 press_vari_thd  :   12; //press variance threshold
            u32 rels_vari_thd   :   12; //release variance threshold
            u32 to_bcnt_thd     :   4;  //time out base counter threshold
        };
        u32 reg_tkcon2;
    };


    //in ear
    union {
        struct {
            u32 ear_fil_low     :  4;
            u32 ear_fil_high    :  4;
            u32 ear_fil_except  :  8;
            u32 ear_fil_val     :  4;
            u32 tkpwup          :  6;
        };
        u32 reg_tetmr;
    };
    union {
        struct {
            u32 tepthd      :   12;     //touch ear press threshold
            u32 resv3       :   4;
            u32 terthd      :   12;     //touch ear release threshold
            u32 tefathd     :   4;
        };
        u32 reg_tepthd;
    };
    union {
        struct {
            u32 testhd      :   12;     //touch ear smaller threshold
            u32 resv5       :   4;
            u32 telthd      :   12;     //touch ear larger threshold
            u32 resv6       :   4;
        };
        u32 reg_teethd;
    };
} tkey_cfg_t;

///需要备份在全局变量的寄存器
typedef struct {
    u32 tkcdpr0;
    u32 tkcdpr1;
    u32 tktmr;
    u32 tkpthd;
    u32 tkethd;
    u32 tetmr;
} tk_reg_cb_t;
extern tk_reg_cb_t tk_reg_cb;

///用于蓝牙SPP调试
typedef struct {
    u8  te_exp;                     //touch ear exception pending
    u8  tk_exp;                     //touch key exception pending
    u16 tebcnt;
    u16 tkbcnt;
volatile u32 flag;
} tk_pnd_cb_t;
extern tk_pnd_cb_t tk_pnd_cb;

typedef struct {
    u8  ch;                     //channel index
    u8  cnt;
    u8  limit;                  //方差阈值
    u8  stable_cnt;
    u8  te_flag;                //是否为入耳检测
    u8  range_thresh;           //rang校准的上限阈值
    u16 avg;                    //平均值
    u16 buf[8];
    u32 anov_cnt;               //满足方差条件计数
    psfr_t bcnt_sfr;            //BCNT寄存器
    u8  fil_except;
    u8  range_en;               //是否使能range校准
    u16 to_cnt;                 //定时校准时间
    u32 tmr_cnt;                //8ms单位

    u32 thd_save;
    s8  thd_avg;
    u8  thd_delta;
    u8  dcnt;
    u8  avg_exp_en;
    s16 delta;
    u8  slide_ch;
    u8  resv0;
} tk_cb_t;

extern tk_cb_t tk_cb;
extern tk_cb_t te_cb;

typedef struct {
    u8  tkey_sta;
    u32 tkey_ticks;
    tk_cb_t *tkey;
} tk_multi_cb_t;

#if USER_TKEY_SLIDE_OLD
///2点滑动处理
typedef struct {
    u8  tkey_sta;
    u8  press_cnt;
    u8  release_cnt;
    u8  release;
    u8  slide;
    u8  press_flag;
    u8  slide_interval_min;
    u8  slide_interval_max;
    u32 tkey_ticks;
    u32 release_tick;
} tk_slide_cb_t;
#else
///2点滑动处理
typedef struct {
    u8  tkey_sta;
    u8  press_interval;
    u8  release_cnt;
    u8  release;
    u8  slide;
    u8  press_flag;
    u8  slide_interval_min;
    u8  slide_interval_max;
    tk_multi_cb_t *tk_slide;
    u32 release_tick;
} tk_slide_cb_t;
#endif
extern tk_slide_cb_t slide_cb[2];

//滑动键值处理结构体
typedef struct {
    u16 key_val;
    u32 slide_cnt;
    u8  slide_timeout;
    u16 delay;          //滑动检测延时
} key_slide_cb_t;
extern key_slide_cb_t key_slide_cb;

int tkey_init(void *tkey_cfg, u32 first_pwron);
void spp_inpcon_tx(void);
int te_temp_bcnt_calibration(u16 cur_cnt, u16 prev_cnt);
void tkey_bcnt_range_exception(tk_cb_t *s, u16 tkcnt);
u32 tkey_tcnt_variance_calc(tk_cb_t *s, u16 tkcnt);
void tkey_sw_reset(void);
#endif // _API_TKEY_H

