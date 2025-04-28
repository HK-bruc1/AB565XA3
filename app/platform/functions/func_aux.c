#include "include.h"
#include "func.h"
#include "func_aux.h"

#if FUNC_AUX_EN
extern func_aux_t f_aux;

AT(.com_text.aux)
void aux_sdadc_process(u8 *ptr,u32 samples,int ch_mode)
{
    //print_r16(ptr, 8);
    sdadc_pcm_2_dac(ptr, samples, ch_mode);
}

AT(.text.bsp.aux)
void func_aux_stop(void)
{
    if (!f_aux.start) {
        return;
    }
//    printf("%s\n", __func__);
    dac_fade_out();
    dac_fade_wait();                    //等待淡出完成
    audio_path_exit(AUDIO_PATH_AUX);
    f_aux.start = 0;
}

AT(.text.bsp.aux)
void func_aux_start(void)
{
    if (f_aux.start) {
        return;
    }
//    printf("%s\n", __func__);
    dac_fade_wait();                    //等待淡出完成
    audio_path_init(AUDIO_PATH_AUX);
    audio_path_start(AUDIO_PATH_AUX);
    dac_fade_in();
    f_aux.start = 1;
}

AT(.text.bsp.aux)
bool aux_is_start(void)
{
    return f_aux.start;
}

AT(.text.func.aux)
void func_aux_pause_play(void)
{
    if (f_aux.pause) {
        led_aux_play();
        func_aux_start();
        f_aux.pause = 0;
    } else {
        led_aux_idle();
        func_aux_stop();
        f_aux.pause = 1;
    }
}

AT(.text.func.aux)
void func_aux_setvol_callback(u8 dir)
{
    if (f_aux.pause) {
        func_aux_pause_play();
    }
}

AT(.text.func.aux)
void func_aux_recover_play(u8 index)
{
    if (index == T_WARNING_MAX_VOL) {
        f_aux.tick = tick_get();
        f_aux.delay_play = 1;                       //修正HOLD住按键，连续音量语音中间漏出AUX音乐声音的问题
    } else {
        func_aux_start();
    }
}

AT(.text.func.aux)
void func_aux_process(void)
{
    if ((f_aux.delay_play) && tick_check_expire(f_aux.tick, 600)) {
        f_aux.delay_play = 0;
        if (!f_aux.pause) {
            func_aux_start();
        }
    }
    func_process();
}

static void func_aux_enter(void)
{
    if (!is_linein_enter_enable()) {
        func_cb.sta = FUNC_NULL;
        return;
    }

    memset(&f_aux, 0, sizeof(func_aux_t));
    func_cb.set_vol_callback = func_aux_setvol_callback;
    msg_queue_clear();

#if WARNING_FUNC_AUX
    sys_warning_play(T_WARNING_AUX_MODE, PIANO_AUX_MODE);
#endif // WARNING_FUNC_AUX
    func_aux_start();
    led_aux_play();
}

static void func_aux_exit(void)
{
    func_aux_stop();
    func_cb.last = FUNC_AUX;
}

AT(.text.func.aux)
void func_aux(void)
{
    printf("%s\n", __func__);

    func_aux_enter();

    while (func_cb.sta == FUNC_AUX) {
        func_aux_process();
        func_aux_message(msg_dequeue());
    }

    func_aux_exit();
}
#else
bool aux_is_start(void)
{
    return false;
}

void func_aux_stop(void)
{
}

void func_aux_recover_play(u8 index)
{
}
#endif  //FUNC_AUX_EN
