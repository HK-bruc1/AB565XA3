#include "include.h"
#include "func.h"
#include "func_speaker.h"

#if FUNC_SPEAKER_EN

AT(.com_text.speaker)
void speaker_sdadc_process(u8 *ptr,u32 samples,int ch_mode)
{
    sdadc_pcm_2_dac(ptr, samples, ch_mode);
}

AT(.text.bsp.speaker)
void func_speaker_start(void)
{
    dac_fade_wait();                    //等待淡出完成
    audio_path_init(AUDIO_PATH_SPEAKER);
    audio_path_start(AUDIO_PATH_SPEAKER);
    dac_fade_in();
}

AT(.text.bsp.speaker)
static void func_speaker_enter(void)
{
    func_speaker_start();
}

AT(.text.bsp.speaker)
void func_speaker_stop(void)
{
    dac_fade_out();
    dac_fade_wait();                    //等待淡出完成
    audio_path_exit(AUDIO_PATH_SPEAKER);
}

static void func_speaker_exit(void)
{
    func_speaker_stop();
    func_cb.last = FUNC_SPEAKER;
}

AT(.text.func.speaker)
void func_speaker_process(void)
{
    qtest_other_usage_process();
    func_process();
}

AT(.text.func.speaker)
void func_speaker(void)
{
    printf("%s\n", __func__);

    func_speaker_enter();

    while (func_cb.sta == FUNC_SPEAKER) {
        func_speaker_process();
        func_speaker_message(msg_dequeue());
    }

    func_speaker_exit();
}
#endif
