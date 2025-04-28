#ifndef _FUNC_H
#define _FUNC_H

#include "func_bt.h"
#include "func_bt_hid.h"
#include "func_lowpwr.h"
#include "func_aux.h"
#include "func_speaker.h"

//task number
enum {
    FUNC_NULL = 0,
    FUNC_BT,
    FUNC_BTHID,
    FUNC_AUX,
    FUNC_PWROFF,
    FUNC_SLEEPMODE,
    FUNC_BT_DUT,                        //DUT测试模式
    FUNC_SPEAKER,
    FUNC_BT_FCC,
};

//task control block
typedef struct {
    u8 sta;                                         //cur working task number
    u8 last;                                        //lask task number
    void (*set_vol_callback)(u8 dir);               //设置音量的回调函数，用于各任务的音量事件处理。
} func_cb_t;
extern func_cb_t func_cb;

extern const u8 func_sort_table[];     //任务切换排序table

u8 get_funcs_total(void);
void func_process(void);
void func_message(u16 msg);

void func_run(void);
void func_music(void);
void func_bt(void);
void func_bthid(void);
void func_aux(void);

#endif // _FUNC_H
