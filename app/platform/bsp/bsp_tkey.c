#include "include.h"

#define USER_TKEY_MROM_EN           1       //使用rom tkey process

extern tk_cb_t tk_cb;
extern tk_cb_t te_cb;
extern te_cali_cb_t te_cali_cb;
extern u16 tkcnt_tmr[4];       //每个通路的实时TKCNT
extern u8  tk_ch_temp;
extern tk_cb_t tk1_cb;
#if (USER_TKEY_SLIDE || USER_TKEY_SHORT_SLIDE || USER_TKEY_MULTI_EN)
u32 tkbcnt_tmr[3];
tk_multi_cb_t tk_multi_cb;
tk_multi_cb_t tk1_multi_cb;
#endif
#if USER_TKEY_PB3_EN
tk_cb_t tk2_cb;
tk_multi_cb_t tk2_multi_cb;
#endif
#if USER_TKEY_PA7_EN
tk_cb_t tk3_cb;
tk_multi_cb_t tk3_multi_cb;
#endif
#if USER_TKEY_SLIDE
key_slide_cb_t key_slide_cb;
#endif

int s_abs(int x);
void tkey_slide_msg(u8 silde_up);
u8 tkey_slide_is_pressed(tk_cb_t *s, tk_cb_t *p);
void bsp_charge_bcnt_calibration_m(u32 min_avg_cnt);
u8 tkey_multi_is_pressed(void);
void tkey_bcnt_range_exception_fix(tk_multi_cb_t *tkm, u16 tkcnt, u8 tk_flag);
void bsp_tkey_spp_tx_m(void);
bool tkey_is_pressed_m(void);
u8 bsp_tkey_scan_m(void);
void key_multi_press_clr(void);
void tkey_slide_up_down_check(tk_slide_cb_t *ts, tk_slide_cb_t *tp);
u8 bsp_short_slide_tkey_scan(void);

AT(.rodata.tkey)
const char tk_fil_high_tbl[8] = {
    4, 6, 8, 10, 12, 13, 14, 15
};

#if USER_TKEY
#if (USER_TKEY_SLIDE || USER_TKEY_SHORT_SLIDE)
AT(.com_text.tkey.isr)
void tkey_slide_message_do(tk_cb_t *tkey_slide)
{
    tk_multi_cb_t *tkm = slide_cb[0].tk_slide;

    if (tkey_slide->ch == tkm->tkey->ch) {
        tkey_slide_msg(1);             //上滑
    } else {
        tkey_slide_msg(0);
    }
}

AT(.com_text.tkey.isr)
void tkey_slide_set_release(tk_multi_cb_t *tkm)
{
    tk_slide_cb_t *ts0 = &slide_cb[0];
    tk_slide_cb_t *ts1 = &slide_cb[1];

    if (ts0->tk_slide == tkm) {
        ts0->release_tick = tick_get();
        ts0->release = 1;
    } else if (ts1->tk_slide == tkm) {
        ts1->release_tick = tick_get();
        ts1->release = 1;
    }
}
#endif

#if USER_TKEY_SHORT_SLIDE
#if USER_TKEY_SHORT_SLIDE_DEBUG_EN
AT(.rodata.tkey)
const char tkcnt_str[] = "[TK] State(%01x), TKBCNT(%03x):\n";
AT(.rodata.tkey)
const char tecnt_str[] = "[TE] State(%01x), TEBCNT(%03x):\n";
AT(.rodata.tkey)
const char tk1cnt_str[] = "[TK1] State(%01x), TKBCNT(%03x):\n";

//通过蓝牙SPP打印输出, 避免UART飞线干扰
void spp_slide_tkcnt_tx(void)
{
    u16 tk_buf[8], te_buf[8];
    int c_len = 0, i;

    if (spp_tkcb.tk_print_sta) {
        memset(spp_tkcb.spp_buf, 0, sizeof(spp_tkcb.spp_buf));
        memcpy(tk_buf, &spp_tkcb.tk_buf[(spp_tkcb.tk_print_sta - 1)*8], 16);
        memcpy(te_buf, &spp_tkcb.te_buf[(spp_tkcb.tk_print_sta - 1)*8], 16);
        spp_tkcb.tk_print_sta = 0;
        if (tk_cb.ch != 0xff) {
            c_len += sprintf(spp_tkcb.spp_buf + c_len, tkcnt_str, tk_multi_cb.tkey_sta, (u16)(TKBCNT & 0xfff));
            for (i = 0; i < 8; i++) {
                c_len += sprintf(spp_tkcb.spp_buf + c_len, "%03x ", tk_buf[i] & 0xfff);
            }
            c_len += sprintf(spp_tkcb.spp_buf + c_len, "\n");
        }
        if (tk1_cb.ch != 0xff) {
            c_len += sprintf(spp_tkcb.spp_buf + c_len, tk1cnt_str, tk1_multi_cb.tkey_sta, (u16)(tk1_cb.bcnt_sfr[0] & 0xfff));
            for (i = 0; i < 8; i++) {
                c_len += sprintf(spp_tkcb.spp_buf + c_len, "%03x ", tk1_cb.buf[i] & 0xfff);
            }
            c_len += sprintf(spp_tkcb.spp_buf + c_len, "\n");
        }
//#if USER_TKEY_INEAR
//        if (te_cb.ch != 0xff) {
//            c_len += sprintf(spp_tkcb.spp_buf + c_len, tecnt_str, tkey_is_inear(), (u16)(TEBCNT & 0xfff));
//            for (i = 0; i < 8; i++) {
//                c_len += sprintf(spp_tkcb.spp_buf + c_len, "%03x ", te_buf[i] & 0xfff);
//            }
//            c_len += sprintf(spp_tkcb.spp_buf + c_len, "\n");
//        }
//#endif
        bt_spp_tx((uint8_t *)spp_tkcb.spp_buf, c_len);
    }
}

void bsp_slide_tkey_spp_tx(void)
{
    if (xcfg_cb.user_tkey_debug_en) {
        spp_slide_tkcnt_tx();                 //输出TKCNT
//        if (tick_check_expire(spp_tkcb.ticks, 100)) {
//            spp_tkcb.ticks = tick_get();
//            spp_inpcon_tx();                //输出一些状态
//        }
    }
}
#endif

//AT(.com_rodata.text)
//const char print_release_str[] = "===> tkey_release_ticks (%d)\n";

AT(.com_text.bsp.key)
u16 tkey_short_slide_process(u16 key_val)
{
    u16 key_return = NO_KEY;
    tk_slide_cb_t *ts = &slide_cb[0];
    tk_slide_cb_t *tp = &slide_cb[1];
    tk_multi_cb_t *s = slide_cb[0].tk_slide;
    tk_multi_cb_t *p = slide_cb[1].tk_slide;

    static u8  slide_press_flag = 0;
    static u32 slide_press_ticks = 0;

    if ((s->tkey->ch == 0xff) || (p->tkey->ch == 0xff)) {
        return key_val;
    }

    //多击判断时间内不判断滑动
    if (slide_press_flag) {
        if (tick_check_expire(slide_press_ticks, xcfg_cb.double_key_time * 100 + 200)) {
            slide_press_flag = 0;                           //多击判断后清flag
            slide_press_ticks = 0;
        } else if (key_val) {
            slide_press_ticks = tick_get();
        }
        return key_val;
    }

    if (ts->release_tick < tp->release_tick) {
        ts = &slide_cb[1];
        tp = &slide_cb[0];
        s = slide_cb[1].tk_slide;
        p = slide_cb[0].tk_slide;
    }

    u32 tkey_release_ticks = ts->release_tick - tp->release_tick;

    if (((key_val & 0xff00) == KEY_SHORT_UP) && (!slide_press_flag)) {
        if (tkey_release_ticks > ts->slide_interval_min && tkey_release_ticks < ts->slide_interval_max * 2) {
            tkey_slide_message_do(s->tkey);
        } else if (tkey_release_ticks < ts->press_interval) {
            key_return = key_val;
            slide_press_flag = 1;                           //控制多击判断时间内不判断滑动
            slide_press_ticks = tick_get();
        }
//        printf(print_release_str, tkey_release_ticks);
    } else {
        key_return = key_val;
    }

    //添加按键提示音
//    if (key_return == KU_PLAY_PWR_USER_DEF) {
//        msg_enqueue(EVT_KEY_VOICE);
//    }

    return key_return;
}
#endif

#if USER_TKEY_SLIDE
AT(.com_text.bsp.key)
void tkey_slide_clr_all_param(tk_slide_cb_t *ts)
{
    ts->tk_slide->tkey_ticks = 0;
    ts->release = 0;
    ts->press_flag = 0;
    ts->slide = 0;
}

//AT(.com_rodata.text)
//const char slide_press_str1[] = "==> slide save (%x)\n";
//AT(.com_rodata.text)
//const char slide_press_str2[] = "==> slide timeout\n";
//AT(.com_rodata.text)
//const char slide_press_str4[] = "==> slide timeout release (0x%04x)\n";
//AT(.com_rodata.text)
//const char slide_press_str5[] = "==> slide press\n";
//AT(.com_rodata.text)
//const char slide_press_str6[] = "==> slide cnt reint\n";

//滑动按键检测
AT(.com_text.bsp.key)
u16 bsp_tkey_slide_process(u16 key_val)
{
    u16 key_return = NO_KEY;
    u8 slide_clr = 0;
    tk_slide_cb_t *ts, *tp;

    ts = &slide_cb[0];
    tp = &slide_cb[1];

    if (!ts->press_flag && !tp->press_flag) {
        return key_val;                      //非滑动的IO直接返回值
    }

    if (key_val) {
        if (key_val & 0xff00) {
            if (((key_val & 0xff00) == KEY_LONG) || ((key_val & 0xff00) == KEY_LONG_UP) || ((key_val & 0xff00) == KEY_HOLD) || ((key_val & 0xff00) == KEY_LHOLD)) {
                key_return = key_val;                               //长按抬起直接返回键值
                slide_clr = 1;
            } else {
                key_slide_cb.key_val = key_val;                     //非长按抬起, 先保存下来
//                printf(slide_press_str1, key_slide_cb.key_val);
            }
        } else {
            key_return = key_val;                                   //按下未抬起, 直接返回
            if (key_slide_cb.slide_cnt) {
                key_slide_cb.slide_cnt = key_slide_cb.delay;        //刷新超时时间, 多击情况需要
//                printf(slide_press_str6);
            }
        }
    }

    if (key_slide_cb.slide_cnt) {
        key_slide_cb.slide_cnt--;
    }

    if (key_slide_cb.slide_cnt == 1) {
        key_slide_cb.slide_timeout = 1;                             //超时未判断到滑动，返回保存的值
//        printf(slide_press_str2);
    }

    if (key_slide_cb.slide_timeout) {
        if ((ts->press_flag && ts->release) || (tp->press_flag && tp->release)) {
            slide_clr = 1;
            if (ts->slide || tp->slide) {
#if USER_MULTI_PRESS_EN
                key_multi_press_clr();                               //清掉多击判断
#endif
            } else {
                key_return = key_slide_cb.key_val;                   //超时没有判断滑动，返回保存的抬起按键
//                printf(slide_press_str4, key_return);
            }
        }
    }

    //第一个IO抬起后才开始计时滑动
    if ((ts->press_flag && ts->release) || (tp->press_flag && tp->release)) {
        if (key_slide_cb.slide_cnt == 0) {
            key_slide_cb.slide_cnt = key_slide_cb.delay;            //重置滑动超时时间
            key_slide_cb.key_val = NO_KEY;
#if USER_MULTI_PRESS_EN
            key_multi_press_clr();                                  //清掉多击判断
#endif
//            printf(slide_press_str5);
        }
    }

    if (slide_clr) {
        key_slide_cb.slide_cnt = 0;
        key_slide_cb.slide_timeout = 0;
        key_slide_cb.key_val = NO_KEY;
        tkey_slide_clr_all_param(ts);
        tkey_slide_clr_all_param(tp);
    }

    return key_return;
}

#endif

#if (USER_TKEY_MULTI_DEBUG_EN || USER_TKEY_SLIDE_DEBUG_EN)
//AT(.com_rodata.text)
//const char print_multi_tkey_str[] = "TKM -> CH[%d], STA[%d], TKCNT[0x%04x], TKBCNT[0x%04x]\n";

AT(.com_text.tkey.debug)
void tkey_multi_debug_info(u32 tk_ch, u16 tkcnt)
{
//    static u32 tkey_print_cnt = 0;
//
//    tkey_print_cnt++;
//    if (tkey_print_cnt % 11 == 0) {
//        tkey_print_cnt = 0;
//        spp_tkcb.tk_print_sta = 1;
//        u16 tkbcnt = TKBCNT & 0xfff;
//        if (tk_ch) {
//            tkbcnt = tkbcnt_tmr[tk_ch - 1];
//        }
//        printf(print_multi_tkey_str, tk_ch, tkey_multi_is_pressed(), tkcnt, tkbcnt);
//    }
}

AT(.rodata.tkey)
const char tkcnt_str[] = "[TK%d] Sta %01x TKB %03x\n";
AT(.rodata.tkey)
const char tecnt_str[] = "[TE] Sta %01x TEB %03x\n";

//通过蓝牙SPP打印输出, 避免UART飞线干扰
void spp_multi_tkcnt_tx(void)
{
    int c_len = 0, i;

    if (spp_tkcb.tk_print_sta) {
        memset(spp_tkcb.spp_buf, 0, sizeof(spp_tkcb.spp_buf));
        spp_tkcb.tk_print_sta = 0;
        if (tk_cb.ch != 0xff) {
            c_len += sprintf(spp_tkcb.spp_buf + c_len, tkcnt_str, tk_cb.ch, tkey_get_key(), (u16)(TKBCNT & 0xfff));
            for (i = 0; i < 2; i++) {
                c_len += sprintf(spp_tkcb.spp_buf + c_len, "%03x ", tk_cb.buf[i] & 0xfff);
            }
            c_len += sprintf(spp_tkcb.spp_buf + c_len, "\n");
        }
#if (USER_TKEY_PB4_EN || USER_TKEY_SLIDE_DEBUG_EN)
        if (tk1_cb.ch != 0xff) {
            c_len += sprintf(spp_tkcb.spp_buf + c_len, tkcnt_str, tk1_cb.ch, tkey_multi_is_pressed(), (u16)(tk1_cb.bcnt_sfr[0] & 0xfff));
            for (i = 0; i < 2; i++) {
                c_len += sprintf(spp_tkcb.spp_buf + c_len, "%03x ", tk1_cb.buf[i] & 0xfff);
            }
            c_len += sprintf(spp_tkcb.spp_buf + c_len, "\n");
        }
#endif
#if USER_TKEY_PB3_EN
        if (tk2_cb.ch != 0xff) {
            c_len += sprintf(spp_tkcb.spp_buf + c_len, tkcnt_str, tk2_cb.ch, tkey_multi_is_pressed(), (u16)(tk2_cb.bcnt_sfr[0] & 0xfff));
            for (i = 0; i < 2; i++) {
                c_len += sprintf(spp_tkcb.spp_buf + c_len, "%03x ", tk2_cb.buf[i] & 0xfff);
            }
            c_len += sprintf(spp_tkcb.spp_buf + c_len, "\n");
        }
#endif
#if USER_TKEY_PA7_EN
        if (tk3_cb.ch != 0xff) {
            c_len += sprintf(spp_tkcb.spp_buf + c_len, tkcnt_str, tk3_cb.ch, tkey_multi_is_pressed(), (u16)(tk3_cb.bcnt_sfr[0] & 0xfff));
            for (i = 0; i < 2; i++) {
                c_len += sprintf(spp_tkcb.spp_buf + c_len, "%03x ", tk3_cb.buf[i] & 0xfff);
            }
            c_len += sprintf(spp_tkcb.spp_buf + c_len, "\n");
        }
#endif
//#if USER_TKEY_INEAR
//        if (te_cb.ch != 0xff) {
//            c_len += sprintf(spp_tkcb.spp_buf + c_len, tecnt_str, tkey_is_inear(), (u16)(TEBCNT & 0xfff));
//            for (i = 0; i < 4; i++) {
//                c_len += sprintf(spp_tkcb.spp_buf + c_len, "%03x ", te_cb.buf[i] & 0xfff);
//            }
//            c_len += sprintf(spp_tkcb.spp_buf + c_len, "\n");
//        }
//#endif
//        printf("---> spp_tx : len (%d)\n", c_len);
        bt_spp_tx((uint8_t *)spp_tkcb.spp_buf, c_len);
    }
}

void bsp_multi_tkey_spp_tx(void)
{
    if (xcfg_cb.user_tkey_debug_en) {
        spp_multi_tkcnt_tx();                 //输出TKCNT
//        if (tick_check_expire(spp_tkcb.ticks, 100)) {
//            spp_tkcb.ticks = tick_get();
//            spp_inpcon_tx();                //输出一些状态
//        }
    }
}
#endif

void bsp_tkey_spp_tx(void)
{
#if (USER_TKEY_MULTI_DEBUG_EN || USER_TKEY_SLIDE_DEBUG_EN)
    bsp_multi_tkey_spp_tx();
#elif USER_TKEY_SHORT_SLIDE_DEBUG_EN
    bsp_slide_tkey_spp_tx();
#else
    bsp_tkey_spp_tx_m();
#endif
}

//中断回调函数
AT(.com_text.tkey.isr)
int tkey_tkcnt_isr_hook(u32 tk_ch, u16 tkcnt)
{
#if (USER_TKEY_SLIDE || USER_TKEY_SHORT_SLIDE || USER_TKEY_MULTI_EN)
    if (tk_ch == te_cb.ch) {
        tkey_bcnt_range_exception(&te_cb, tkcnt);
#if USER_TKEY_DEBUG_EN
        spp_tkcb.te_buf[spp_tkcb.tk_idx] = tkcnt;
#endif
    } else if (tk_ch == tk_cb.ch) {
        tkey_bcnt_range_exception_fix(&tk_multi_cb, tkcnt, 1);
#if USER_TKEY_DEBUG_EN
        spp_tkcb.tk_buf[spp_tkcb.tk_idx] = tkcnt;
#endif
    } else if (tk_ch == tk1_cb.ch) {
       tkey_bcnt_range_exception_fix(&tk1_multi_cb, tkcnt, 0);
    }
#if USER_TKEY_PB3_EN
    else if (tk_ch == tk2_cb.ch) {
       tkey_bcnt_range_exception_fix(&tk2_multi_cb, tkcnt, 0);
    }
#endif
#if USER_TKEY_PA7_EN
    else if (tk_ch == tk3_cb.ch) {
       tkey_bcnt_range_exception_fix(&tk3_multi_cb, tkcnt, 0);
    }
#endif
#if USER_TKEY_SLIDE
    if (tk_ch == slide_cb[0].tk_slide->tkey->ch) {
        tkey_slide_up_down_check(&slide_cb[0], &slide_cb[1]);
    } else if (tk_ch == slide_cb[1].tk_slide->tkey->ch) {
        tkey_slide_up_down_check(&slide_cb[1], &slide_cb[0]);
    }
#endif
#if USER_TKEY_DEBUG_EN
    if (tk_ch == spp_tkcb.ch) {
        spp_tkcb.tk_idx++;
        if (spp_tkcb.tk_idx == 8) {
            spp_tkcb.tk_print_sta = 1;
        } else if (spp_tkcb.tk_idx >= 16) {
            spp_tkcb.tk_print_sta = 2;
            spp_tkcb.tk_idx = 0;
        }
    }
#endif
#if USER_TKEY_MULTI_DEBUG_EN
    tkey_multi_debug_info(tk_ch, tkcnt);
#endif
    return 1;
#elif !USER_TKEY_MROM_EN
    if (tk_ch == te_cb.ch) {
        tkey_bcnt_range_exception(&te_cb, tkcnt);
#if USER_TKEY_DEBUG_EN
        spp_tkcb.te_buf[spp_tkcb.tk_idx] = tkcnt;
#endif
    } else if (tk_ch == tk_cb.ch) {
        tkey_bcnt_range_exception(&tk_cb, tkcnt);
        tkey_timeout_calibration(&tk_cb, tkcnt);
#if USER_TKEY_DEBUG_EN
        spp_tkcb.tk_buf[spp_tkcb.tk_idx] = tkcnt;
#endif
    }
#if USER_TKEY_SLIDE_OLD
    else if (tk_ch == tk1_cb.ch) {
       tkey_bcnt_range_exception(&tk1_cb, tkcnt);
    }
#endif
#if USER_TKEY_DEBUG_EN
    if (tk_ch == spp_tkcb.ch) {
        spp_tkcb.tk_idx++;
        if (spp_tkcb.tk_idx == 8) {
            spp_tkcb.tk_print_sta = 1;
        } else if (spp_tkcb.tk_idx >= 16) {
            spp_tkcb.tk_print_sta = 2;
            spp_tkcb.tk_idx = 0;
        }
    }
#endif
	return 1;
#else
	return 0;
#endif
}

#if (USER_TKEY_SLIDE || USER_TKEY_MULTI_EN)
AT(.com_text.bsp.tkey)
u8 tkey_multi_is_pressed(void)
{
    u8 multi_press = 0;

    if (tk1_multi_cb.tkey_sta) {
        multi_press = 1;
    }
#if USER_TKEY_PB3_EN
    if (tk2_multi_cb.tkey_sta) {
        multi_press = 2;
    }
#endif
#if USER_TKEY_PA7_EN
    if (tk3_multi_cb.tkey_sta) {
        multi_press = 3;
    }
#endif
    return multi_press;
}

//设置触摸通道的键值
AT(.com_text.bsp.tkey)
u8 bsp_tkey_multi_scan(void)
{
    u8 key = NO_KEY;
    u8 multi_press = tkey_multi_is_pressed();

    if (multi_press == 1) {
//        key = KEY_PLAY_USER_DEF;
        key = NO_KEY;                         //TK1(PB4)
    }
#if USER_TKEY_PB3_EN
    if (multi_press == 2) {
        key = NO_KEY;                         //TK2(PB3)
    }
#endif
#if USER_TKEY_PA7_EN
    if (multi_press == 3) {
        key = NO_KEY;                         //TK3(PA7)
    }
#endif
    return key;
}
#endif

#if (USER_TKEY_SLIDE || USER_TKEY_SHORT_SLIDE || USER_TKEY_MULTI_EN)
AT(.com_text.tkey)
bool tkey_is_pressed(void)
{
    if (tk_cb.ch == 0xff) {
        return false;
    }
    if (!tkey_get_key()) {
        return false;
    }
    return true;
}

AT(.com_text.bsp.tkey)
u8 bsp_tkey_scan(void)
{
    u8 key = NO_KEY;

    if (tkey_is_pressed()) {
        key = (sys_cb.tkey_pwrdwn_en) ? KEY_PLAY_PWR_USER_DEF : KEY_PLAY_USER_DEF;  //wko
    }
#if (USER_TKEY_SLIDE || USER_TKEY_MULTI_EN)
    else {
        key = bsp_tkey_multi_scan();
    }
#endif
    return key;
}

#if USER_TKEY_SHORT_SLIDE
AT(.com_text.tkey)
u8 tkey_short_slide_is_pressed(void)
{
    tk_multi_cb_t *s = slide_cb[0].tk_slide;
    tk_multi_cb_t *p = slide_cb[1].tk_slide;

    if (s->tkey->ch == 0xff || p->tkey->ch == 0xff) {
        return 0;
    }
    if (s->tkey_sta && p->tkey_sta) {
        return 2;                             //同时按下判断按下
    }
    if (!s->tkey_sta && !p->tkey_sta) {
        return 0;                             //同时释放判断释放
    }
    return 1;
}

AT(.com_text.bsp.tkey)
u8 bsp_short_slide_tkey_scan(void)
{
    static u8 key_scan_val = NO_KEY;

    if (tkey_short_slide_is_pressed() == 2) {
        key_scan_val = (sys_cb.tkey_pwrdwn_en) ? KEY_PLAY_PWR_USER_DEF : KEY_PLAY_USER_DEF;  //wko
    } else if (tkey_short_slide_is_pressed() == 0) {
        key_scan_val = NO_KEY;
    }

    return key_scan_val;
}
#endif
#else
AT(.com_text.tkey)
bool tkey_is_pressed(void)
{
    return tkey_is_pressed_m();
}

AT(.com_text.bsp.tkey)
u8 bsp_tkey_scan(void)
{
    return bsp_tkey_scan_m();
}
#endif

AT(.text.bsp.tkey)
void bsp_charge_bcnt_calibration(u32 min_avg_cnt)
{
    bsp_charge_bcnt_calibration_m(min_avg_cnt);
#if (USER_TKEY_SLIDE || USER_TKEY_SHORT_SLIDE || USER_TKEY_MULTI_EN)
    u16 tkbcnt;

    if (tk1_cb.ch != 0xff) {
        if (tk1_cb.anov_cnt > min_avg_cnt) {             //tkcnt是否稳定？
            tkbcnt = tk1_cb.bcnt_sfr[0] & 0xfff;
            if (s_abs((s16)tkbcnt - (s16)tk1_cb.avg) > 2) {
                tk1_cb.bcnt_sfr[0] = tk1_cb.avg;
            }
        }
    }
#if USER_TKEY_PB3_EN
    if (tk2_cb.ch != 0xff) {
        if (tk2_cb.anov_cnt > min_avg_cnt) {             //tkcnt是否稳定？
            tkbcnt = tk2_cb.bcnt_sfr[0] & 0xfff;
            if (s_abs((s16)tkbcnt - (s16)tk2_cb.avg) > 2) {
                tk2_cb.bcnt_sfr[0] = tk2_cb.avg;
            }
        }
    }
#endif
#if USER_TKEY_PA7_EN
    if (tk3_cb.ch != 0xff) {
        if (tk3_cb.anov_cnt > min_avg_cnt) {             //tkcnt是否稳定？
            tkbcnt = tk3_cb.bcnt_sfr[0] & 0xfff;
            if (s_abs((s16)tkbcnt - (s16)tk3_cb.avg) > 2) {
                tk3_cb.bcnt_sfr[0] = tk3_cb.avg;
            }
        }
    }
#endif
#endif
}

void bsp_tkey_channel_set(void)
{
    int i;

#if !USER_TKEY_PWRKEY_EN
    if (xcfg_cb.user_tkey)
#endif
    {
        tk_cb.ch = USER_TKEY_CH_SEL;                    //wko
        spp_tkcb.ch = USER_TKEY_CH_SEL;
    }

#if USER_TKEY_PB5_DISABLE
    tk_cb.ch = 0;                                       //初始化需要
#endif

#if USER_TKEY_INEAR
    if (xcfg_cb.user_tkey_inear) {
        if (xcfg_cb.earin_gpio_sel == 2) {              //PA7
            te_cb.ch = 3;
        } else if (xcfg_cb.earin_gpio_sel == 3) {       //PB3
            te_cb.ch = 2;
        } else if (xcfg_cb.earin_gpio_sel == 4) {       //PB4
            te_cb.ch = 1;
        }
    }
#endif

#if USER_TKEY_SLIDE_OLD
    u8 tk1_ch = 0xff;

    if (xcfg_cb.tkey_slide_gpio_sel) {
        if (xcfg_cb.tkey_slide_gpio_sel == 1) {         //PA7
            tk1_ch = 3;
        } else if (xcfg_cb.tkey_slide_gpio_sel == 2) {  //PB3
            tk1_ch = 2;
        } else if (xcfg_cb.tkey_slide_gpio_sel == 3) {  //PB4
            tk1_ch = 1;
        }
        tk1_cb.ch = tk1_ch;
        if (tk1_ch != 0xff) {
            tk_cb.slide_ch = 0;
            tk1_cb.slide_ch = 1;
        }
    }
#endif

#if (USER_TKEY_SLIDE || USER_TKEY_SHORT_SLIDE)
    if (xcfg_cb.tkey_slide_gpio_sel) {
        if (xcfg_cb.tkey_slide_gpio_sel == 1) {          //PA7
            tk1_cb.ch = 3;
        } else if (xcfg_cb.tkey_slide_gpio_sel == 2) {   //PB3
            tk1_cb.ch = 2;
        } else if (xcfg_cb.tkey_slide_gpio_sel == 3) {   //PB4
            tk1_cb.ch = 1;
        }
    }
#endif

#if USER_TKEY_MULTI_EN
    tk1_cb.ch = 0xff;                                   //去掉滑动配置
#endif
#if USER_TKEY_PB4_EN
    tk1_cb.ch = 1;
#endif
#if USER_TKEY_PB3_EN
    tk2_cb.ch = 2;
#endif
#if USER_TKEY_PA7_EN
    tk3_cb.ch = 3;
#endif

    if (te_cb.ch != 0xff) {                             //入耳多开一个内部通路用于校准
        spp_tkcb.ch = te_cb.ch;
        for (i = 0; i < 4; i++) {
            if (i != te_cb.ch && i != tk_cb.ch && i != tk1_cb.ch
#if USER_TKEY_PB5_DISABLE
                && i                                    //PowerKey不能开TouchKey
#endif
#if USER_TKEY_PB3_EN
                && i != tk2_cb.ch
#endif
#if USER_TKEY_PA7_EN
                && i != tk3_cb.ch
#endif
                ) {
                tk_ch_temp = i;
            }
        }
    }
}

void bsp_tkey_var_init(void)
{
    memset(&tk_cb, 0, sizeof(tk_cb));
    memset(&tk1_cb, 0, sizeof(tk_cb));
#if (USER_TKEY_SLIDE || USER_TKEY_SHORT_SLIDE || USER_TKEY_MULTI_EN)
    memset(&tkbcnt_tmr, 0, sizeof(tkbcnt_tmr));
    memset(&tk_multi_cb, 0, sizeof(tk_multi_cb_t));
    memset(&tk1_multi_cb, 0, sizeof(tk_multi_cb_t));
    tk_multi_cb.tkey = &tk_cb;
    tk1_multi_cb.tkey = &tk1_cb;
#if USER_TKEY_SLIDE
    memset(&key_slide_cb, 0, sizeof(key_slide_cb));
#endif
#endif
#if USER_TKEY_PB3_EN
    memset(&tk2_cb, 0, sizeof(tk2_cb));
    memset(&tk2_multi_cb, 0, sizeof(tk_multi_cb_t));
    tk2_multi_cb.tkey = &tk2_cb;
#endif
#if USER_TKEY_PA7_EN
    memset(&tk3_cb, 0, sizeof(tk3_cb));
    memset(&tk3_multi_cb, 0, sizeof(tk_multi_cb_t));
    tk3_multi_cb.tkey = &tk3_cb;
#endif
    memset(&te_cb, 0, sizeof(te_cb));
    memset(&te_cali_cb, 0, sizeof(te_cali_cb));
#if USER_TKEY_DEBUG_EN
    memset(&spp_tkcb, 0, sizeof(spp_tkcb));
    spp_tkcb.ch = 0xff;
#endif
    tk_cb.limit = 32;
    tk_cb.range_thresh = (u8)xcfg_cb.tkrthresh * 2 - 1; //TKRTHRESH;
    tk_cb.ch = 0xff;
    tk_cb.slide_ch = 0xff;
    tk_cb.fil_except = FIL_EXCEPT;
    tk_cb.bcnt_sfr = (psfr_t)(&TKBCNT);
    tk_cb.range_en = 0;                                 //使用硬件的校准
    tk_cb.to_cnt = TO_EXCEPT;                           //10秒
    if (xcfg_cb.bt_pwrkey_nsec_discover > 4) {
        tk_cb.to_cnt = 1875;                            //15秒
    }

#if USER_TKEY_SLIDE_OLD
    memset(&slide_cb[0], 0, sizeof(tk_slide_cb_t) * 2);
    slide_cb[0].slide_interval_min = xcfg_cb.tkey_slide_interval_min_sel * 5 + 15;
    slide_cb[0].slide_interval_max = 250;               //250ms
    slide_cb[1].slide_interval_min = slide_cb[0].slide_interval_min;
    slide_cb[1].slide_interval_max = slide_cb[0].slide_interval_max;
#endif
#if (USER_TKEY_SLIDE || USER_TKEY_SHORT_SLIDE)
    memset(&slide_cb[0], 0, sizeof(tk_slide_cb_t) * 2);
    slide_cb[0].slide_interval_min = xcfg_cb.tkey_slide_interval_min_sel * 5 + 15;
    slide_cb[0].slide_interval_max = 125;                     //250ms, 单位2ms
    slide_cb[0].press_interval = 10;
    memcpy(&slide_cb[1], &slide_cb[0], sizeof(tk_slide_cb_t));
    slide_cb[0].tk_slide = &tk_multi_cb;
    slide_cb[1].tk_slide = &tk1_multi_cb;
#if USER_TKEY_SLIDE
    key_slide_cb.delay = key_cb.delay + key_cb.delay / 2;
#endif
#endif
    memcpy(&tk1_cb, &tk_cb, sizeof(tk_cb_t));
#if USER_TKEY_PB3_EN
    memcpy(&tk2_cb, &tk_cb, sizeof(tk_cb_t));
#endif
#if USER_TKEY_PA7_EN
    memcpy(&tk3_cb, &tk_cb, sizeof(tk_cb_t));
#endif
    te_cb.limit = 36;
    te_cb.te_flag = 1;
    te_cb.range_thresh = (u8)xcfg_cb.terthresh * 2 - 2; //TERTHRESH - 2
    te_cb.ch = 0xff;
    te_cb.slide_ch = 0xff;
    te_cb.bcnt_sfr = (psfr_t)(&TEBCNT);
    te_cb.fil_except = EAR_FIL_EXCEPT;
    te_cb.range_en = 1;

    tk_ch_temp = 0xff;
}
#endif // USER_TKEY

void bsp_tkey_init(void)
{
    int res;
    bsp_tkey_var_init();
#if USER_TKEY
    bsp_tkey_channel_set();
    if (POWKEY_10S_RESET && tk_cb.ch == 0) {
        tk_cb.to_cnt = 1875;                                //15秒
    }
    if ((tk_cb.ch != 0xff) ||  (te_cb.ch != 0xff)) {
        tkey_cfg_t tkey_cfg_v;
        tkey_ch_t tk_ch_cfg, te_ch_cfg, tp_ch_cfg;
        tkey_ch_t tk1_ch_cfg;
#if USER_TKEY_PB3_EN
        tkey_ch_t tk2_ch_cfg;
#endif
#if USER_TKEY_PA7_EN
        tkey_ch_t tk3_ch_cfg;
#endif
        memcpy(&tk_ch_cfg, &tkey0, sizeof(tkey_ch_t));
        memcpy(&te_ch_cfg, &tkey_inear, sizeof(tkey_ch_t));
        memcpy(&tkey_cfg_v, &tkey_cfg, sizeof(tkey_cfg_t));
        if (tk_cb.ch != 0xff) {                             //touch key configure
            tk_ch_cfg.cdpr = (u16)xcfg_cb.tkey_cdpr * 10 + 40;
            tk_ch_cfg.itrim = (u32)xcfg_cb.tk_charge_current;
            tkey_cfg_v.key[tk_cb.ch] = &tk_ch_cfg;
            tkey_cfg_v.fil_high = tk_fil_high_tbl[xcfg_cb.tkey_fil_high];
            tkey_cfg_v.tkpthd = (u8)xcfg_cb.tkpthresh * 2;  //TKPTHRESH
            tkey_cfg_v.tkrthd = (u8)xcfg_cb.tkrthresh * 2;  //TKRTHRESH
            tkey_cfg_v.tklthd = tkey_cfg_v.tkpthd * 10;     //LARGETHD
            tkey_cfg_v.to_except = tk_cb.to_cnt;            //timeout except time
            tkey_cfg_v.tkvthd = tk_cb.limit;
            tkey_cfg_v.tkarthd = tk_cb.range_thresh;
            tk_cb.avg_exp_en = xcfg_cb.tk_avg_exp_en;
            tk_cb.thd_save = tkey_cfg_v.reg_tkpthd;
            tk_cb.thd_avg = 8;
            if (tkey_cfg_v.tkrthd > 10) {
                tk_cb.thd_avg = tkey_cfg_v.tkrthd - 2;
            }
            tk_cb.thd_delta = tk_cb.thd_avg - 3;
            if (tk1_cb.ch != 0xff) {
                memcpy(&tk1_ch_cfg, &tk_ch_cfg, sizeof(tkey_ch_t));
                tk1_ch_cfg.type = 3;
                tkey_cfg_v.key[tk1_cb.ch] = &tk1_ch_cfg;
                tk1_cb.to_cnt = tk_cb.to_cnt;
#if (USER_TKEY_SLIDE || USER_TKEY_SHORT_SLIDE || USER_TKEY_MULTI_EN)
                tk1_cb.bcnt_sfr = &tkbcnt_tmr[tk1_cb.ch - 1];
#endif
            }
#if USER_TKEY_PB3_EN
            if (tk2_cb.ch != 0xff) {
                memcpy(&tk2_ch_cfg, &tk_ch_cfg, sizeof(tkey_ch_t));
                tk2_ch_cfg.type = 3;
                tkey_cfg_v.key[tk2_cb.ch] = &tk2_ch_cfg;
                tk2_cb.to_cnt = tk_cb.to_cnt;
                tk2_cb.bcnt_sfr = &tkbcnt_tmr[tk2_cb.ch - 1];
            }
#endif
#if USER_TKEY_PA7_EN
            if (tk3_cb.ch != 0xff) {
                memcpy(&tk3_ch_cfg, &tk_ch_cfg, sizeof(tkey_ch_t));
                tk3_ch_cfg.type = 3;
                tkey_cfg_v.key[tk3_cb.ch] = &tk3_ch_cfg;
                tk3_cb.to_cnt = tk_cb.to_cnt;
                tk3_cb.bcnt_sfr = &tkbcnt_tmr[tk3_cb.ch - 1];
            }
#endif
        }
        if (te_cb.ch != 0xff) {                             //touch ear configure
            te_ch_cfg.cdpr = (u16)xcfg_cb.te_cdpr * 20;
            te_ch_cfg.itrim = (u32)xcfg_cb.te_charge_current;
            tkey_cfg_v.key[te_cb.ch] = &te_ch_cfg;
            tkey_cfg_v.tepthd = (u8)xcfg_cb.tepthresh * 2;  //TEPTHRESH
            tkey_cfg_v.terthd = (u8)xcfg_cb.terthresh * 2;  //TERTHRESH
            tkey_cfg_v.tefathd = (u8)xcfg_cb.terthresh * 2; //TERTHRESH
            tkey_cfg_v.telthd = tkey_cfg_v.tepthd * 10;     //TE_LARGETHD
        }
        if (tk_ch_temp != 0xff) {
            memcpy(&tp_ch_cfg, &tkey_temp_cali_ch, sizeof(tkey_ch_t));
            tp_ch_cfg.itrim = (u32)xcfg_cb.te_charge_current;
            tkey_cfg_v.key[tk_ch_temp] = &tp_ch_cfg;
        }
#if USER_TKEY_PB5_DISABLE
        tk_cb.ch = 0xff;
#endif
#if (USER_TKEY_PB5_DISABLE || USER_TKEY_PWRKEY_EN)
        tkey_cfg_v.key[0] = NULL;                           //关PB5的TouchKey
#endif
        res = tkey_init((void *)&tkey_cfg_v, sys_cb.rtc_first_pwron);
        TKCON &= ~BIT(26);

        if (tk_cb.ch != 0xff) {
            if (!res || sys_cb.inbox_wken_flag || (RTCCON9 & BIT(4)) || (!(RTCCON9 & BIT(6)))) {
                delay_5ms(5);
                TKBCNT = tkcnt_tmr[tk_cb.ch];
            }
        }
#if (USER_TKEY_SLIDE || USER_TKEY_SHORT_SLIDE || USER_TKEY_MULTI_EN)
        delay_5ms(5);
        if (tk1_cb.ch != 0xff) {
            tk1_cb.bcnt_sfr[0] = tkcnt_tmr[tk1_cb.ch];
        }
#endif
#if USER_TKEY_PB3_EN
        if (tk2_cb.ch != 0xff) {
            tk2_cb.bcnt_sfr[0] = tkcnt_tmr[tk2_cb.ch];
        }
#endif
#if USER_TKEY_PA7_EN
        if (tk3_cb.ch != 0xff) {
            tk3_cb.bcnt_sfr[0] = tkcnt_tmr[tk3_cb.ch];
        }
#endif
        if (te_cb.ch != 0xff) {
            delay_5ms(4);
            TEBCNT = tkcnt_tmr[te_cb.ch];
        }
    } else {
        tkey_sw_reset();
    }
#else
    tkey_sw_reset();
#endif // USER_TKEY
}


