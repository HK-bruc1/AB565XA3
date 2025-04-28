#include "include.h"
#include "func.h"
#include "func_speaker.h"

#if FUNC_SPEAKER_EN
void func_speaker_message(u16 msg){
    switch (msg) {
    case KU_PLAY:
        break;

    default:
        func_message(msg);
        break;
    }
}
#endif // FUNC_SPEAKER_EN

