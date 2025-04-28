#include "include.h"

#if BT_HALL_SENSOR_EN
gpio_t hall_gpio AT(.buf.gpio);

void hall_sensor_init(void)
{
    gpio_t *g = &hall_gpio;
    bsp_gpio_cfg_init(g, BT_HALL_SENSOR_GPIO_SEL);

    if (g->sfr == NULL) {
        return;
    }
    xcfg_cb.pwron_press_time = 0;       //霍尔传感器,磁铁离开,PWRKEY产生300ms左右低电平，故需要设置成100ms长按开机。
    g->sfr[GPIOxDE]  |= BIT(g->num);
    g->sfr[GPIOxPU]  |= BIT(g->num);
    g->sfr[GPIOxDIR] |= BIT(g->num);
}

//霍尔传感器，靠近磁铁，输出低电平，需要关机
AT(.com_text.detect)
void hall_sensor_detect(void)
{
    gpio_t *g = &hall_gpio;
    static u8 off_cnt = 0;

    if (g->sfr == NULL) {
        return;
    }

    if (g->sfr[GPIOx] & BIT(g->num)) {
        off_cnt = 0;
    } else {
        if (off_cnt < 20) {
            off_cnt++;
        } else if (off_cnt == 20) {
            off_cnt = 30;
            sys_cb.hall_2_pwrdwn_flag = 1;
        }
    }
}

void hall_trigger_to_pwrdwn(void)
{
    gpio_t *g = &hall_gpio;
    if (g->sfr == NULL) {
        return;
    }

    if (sys_cb.hall_2_pwrdwn_flag) {
        sys_cb.pwrdwn_tone_en = 1;
        func_cb.sta = FUNC_PWROFF;
        sys_cb.hall_2_pwrdwn_flag = 0;
    }
}
#endif // BT_HALL_SENSOR_EN

