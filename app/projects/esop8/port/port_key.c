#include "include.h"

///与配置工具下拉选项一一对应
//AT(.com_text.key.table)
//const u8 key_config_table[] = {
//    NO_KEY,                     //[0]   None
//    KEY_PLAY,                   //[1]   P/P
//    KEY_POWER,                  //[2]   PWR
//    KEY_NEXT_VOL_UP,            //[3]   NEXT/VOL+           短按NEXT功能, 长按VOL+
//    KEY_PREV_VOL_DOWN,          //[4]   PREV/VOL-           短按PREV,     长按VOL-
//    KEY_NEXT_VOL_DOWN,          //[5]   NEXT/VOL-           短按NEXT,     长按VOL-
//    KEY_PREV_VOL_UP,            //[6]   PREV/VOL+           短按PREV功能, 长按VOL+
//    KEY_VOL_UP_NEXT,            //[7]   VOL+/NEXT           短按VOL+,     长按NEXT
//    KEY_VOL_DOWN_PREV,          //[8]   VOL-/PREV           短按VOL-,     长按PREV
//    KEY_VOL_DOWN_NEXT,          //[9]   VOL-/NEXT           短按VOL-,     长按NEXT
//    KEY_VOL_UP_PREV,            //[10]  VOL+/PREV           短按VOL+,     长按PREV
//    KEY_VOL_UP,                 //[11]  VOL+
//    KEY_VOL_DOWN,               //[12]  VOL-
//    KEY_NEXT,                   //[13]  NEXT
//    KEY_PREV,                   //[14]  PREV
//    KEY_MODE,                   //[15]  MODE
//    KEY_HSF,                    //[16]  HSF
//    KEY_MODE_PWR,               //[17]  MODE/PWR            短按MODE,     长按POWER
//    KEY_SIRI,                   //[18]  SIRI
//    KEY_HOME,                   //[19]  IOS HOME
//    KEY_PLAY_PWR_USER_DEF,      //[20]  PP/PWR/USER_DEF     短按PLAY,     长按POWER,  长按/双击/三击用户自定义
//    KEY_PLAY_USER_DEF,          //[21]  PP/USER_DEF         短按PLAY,                 长按/双击/三击用户自定义
//    KEY_VOL_UP_DOWN,            //[22]  VOL+/VOL-           短按VOL+,     长按VOL-
//};


//新增多击按键, 返回true代表支持多击检测。
AT(.com_text.key)
bool check_double_key_hook(u16 key)
{
//    if (key == KU_POWER) {
//        return true;
//    }
    return false;
}

#if USER_ADKEY
///以0xff结束
AT(.rodata.adkey.table)
const adkey_tbl_t adkey_table[] = {
    {0x0A, KEY_PLAY},           //PLAY      0
    {0x40, KEY_VOL_UP},         //VOL+      2K
    {0x6A, KEY_NEXT},           //NEXT      5.1K
    {0x94, KEY_PREV},           //MODE      10K
    {0xC1, KEY_HSF},            //HSF       20K
    {0xE0, KEY_VOL_DOWN},       //VOL-      56K
    {0xFF, NO_KEY},             //          END
};
#endif
