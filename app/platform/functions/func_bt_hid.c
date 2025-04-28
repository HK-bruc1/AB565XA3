#include "include.h"
#include "func.h"
#include "func_bt_hid.h"
#if FUNC_BTHID_EN

AT(.com_text.bthid)
bool is_bthid_mode(void)
{
    if ((sys_cb.bthid_mode) && (xcfg_cb.func_bthid_en)) {
        return true;
    }
    return false;
}

AT(.text.func.bt)
static void func_bthid_process(void)
{
    qtest_other_usage_process();
    func_process();
    func_bt_status();

    if(sleep_process(bt_is_sleep)) {
        f_bt.disp_status = 0xff;
    }
}

AT(.text.func.bt)
static void func_bthid_enter(void)
{
    sys_cb.bthid_mode = 1;
    dis_auto_pwroff();
    led_bt_idle();
//    func_bt_enter_display();

#if WARNING_FUNC_BT
    sys_warning_play(T_WARNING_CAMERA_MODE, PIANO_CAMERA_MODE);
#endif // WARNING_FUNC_BT

    f_bt.disp_status = 0xff;
    f_bt.hid_menu_flag = 0;
    f_bt.hid_discon_flag = 0;
#if !BT_BACKSTAGE_EN
    f_bt.warning_status = 0;
    bsp_bt_init();
#endif
}

AT(.text.func.bt)
static void func_bthid_exit(void)
{
    dac_fade_out();
//    func_bt_exit_display();
    bt_disconnect();
    bt_off();
    func_cb.last = FUNC_BTHID;
    sys_cb.bthid_mode = 0;
}

AT(.text.func.bt)
void func_bthid(void)
{
    if (!xcfg_cb.func_bthid_en) {
        func_cb.sta = FUNC_NULL;
        func_cb.last = FUNC_BTHID;
        return;
    }

    printf("%s\n", __func__);

    func_bthid_enter();

    while (func_cb.sta == FUNC_BTHID) {
        func_bthid_process();
        func_bthid_message(msg_dequeue());
//        func_bt_display();
    }

    func_bthid_exit();
}
#endif //FUNC_BTHID_EN
