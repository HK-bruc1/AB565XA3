#include "include.h"

AT(.rodata.func.table)
const u8 func_sort_table[] = {
#if FUNC_BT_EN
    FUNC_BT,
#endif

#if FUNC_BTHID_EN
    FUNC_BTHID,
#endif

#if FUNC_AUX_EN
    FUNC_AUX,
#endif

#if FUNC_SPEAKER_EN
    FUNC_SPEAKER,
#endif
};

AT(.text.func)
u8 get_funcs_total(void)
{
    return sizeof(func_sort_table);
}

uint getcfg_bt_fcc_io(void)
{
    return xcfg_cb.bt_fcc_io;
}

#if SYS_SUPPORT_DONGLE_EN
AT(.text.dongle)
u8 *getcfg_soft_key(void)
{
    return xcfg_cb.soft_key;
}
#endif