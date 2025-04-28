#include "include.h"
#include "func.h"

#if FUNC_AUX_EN

AT(.text.func.aux.msg)
void func_aux_message(u16 msg)
{
    switch (msg) {
        case KU_PLAY:
        case KU_PLAY_USER_DEF:
        case KU_PLAY_PWR_USER_DEF:
            func_aux_pause_play();
            break;

        case EVT_LINEIN_REMOVE:
            func_cb.sta = FUNC_BT;
            break;

        ///长按按键处理
        case KL_PLAY_USER_DEF:
        case KL_PLAY_PWR_USER_DEF:
            user_def_key_msg(xcfg_cb.user_def_kl_sel);
            break;

        ///双击按键处理
        case KD_PLAY_USER_DEF:
        case KD_PLAY_PWR_USER_DEF:
            user_def_key_msg(xcfg_cb.user_def_kd_sel);
            break;

        ///三击按键处理
        case KTH_PLAY_USER_DEF:
        case KTH_PLAY_PWR_USER_DEF:
            user_def_key_msg(xcfg_cb.user_def_kt_sel);
        break;

        ///四击按键处理
        case KFO_PLAY_USER_DEF:
        case KFO_PLAY_PWR_USER_DEF:
            user_def_key_msg(xcfg_cb.user_def_kfour_sel);
            break;

        ///五击按键处理
        case KFI_PLAY_USER_DEF:
        case KFI_PLAY_PWR_USER_DEF:
            user_def_key_msg(xcfg_cb.user_def_kfive_sel);
            break;

        default:
            func_message(msg);
            break;
    }
}
#endif // FUNC_AUX_EN
