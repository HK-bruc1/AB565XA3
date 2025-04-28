#include "include.h"
#include "bsp_188led_driver.h"

#if LED_188LED_DISP_EN
void bsp_188led_charge_tmr2_init(void);
void bsp_188led_disp_set(void);
void bsp_188led_disp_scan(void);
u16 get_vbat_val(void);
extern u32 user_timer_isr_en;

AT(.com_text.led)
void bsp_188led_gpio_init(void)
{
//    printf("bsp_188led_init\n");
    GPIOADE &= ~(BIT(6) | BIT(7));
    GPIOADIR |= BIT(6)| BIT(7);
    GPIOBDE &= ~(BIT(3) | BIT(4));
    GPIOBDIR |= BIT(3)| BIT(4);
    GPIOEDE &= ~BIT(7);
    GPIOEDIR |= BIT(7);
    GPIOADRV &= ~(BIT(6) | BIT(7));
    GPIOBDRV &= ~(BIT(3) | BIT(4));
    GPIOEDRV &= ~BIT(7);
    GPIOACLR |= (BIT(6) | BIT(7));
    GPIOBCLR |= (BIT(3) | BIT(4));
    GPIOECLR |= BIT(7);

}

void bsp_188led_var_init(void)
{
    memset(&sys_188led, 0, sizeof(sys_188led_t));
    bsp_188led_charge_tmr2_init();
    sys_188led.disp_sta = BIT(0);
    sys_188led.disp_on = 1;
    sys_188led.disp_value = 188;
}

AT(.com_text.led)
void bsp_188led_get_bat_level(void)
{
    u16 bat_level = bt_get_bat_level(100);
    sys_188led.disp_value = (bat_level > 100) ? 100 : bat_level;
}

AT(.com_text.led)
void bsp_188led_led0_set_on(void)
{
    if(sys_188led.led_0) {
        GPIOEDE |= BIT(7);
        GPIOEDIR &= ~BIT(7);
        GPIOEDRV |= BIT(7);
        GPIOESET |= BIT(7);
        if(sys_188led.led_0 & BIT(0)) {
            GPIOBDE |= BIT(3);
            GPIOBDIR &= ~BIT(3);
        }
        if(sys_188led.led_0 & BIT(1)) {
            GPIOBDE |= BIT(4);
            GPIOBDIR &= ~BIT(4);
        }
    }
}

AT(.com_text.led)
void bsp_188led_led1_set_on(void)
{
    if(sys_188led.led_1) {
        GPIOADE |= BIT(6);
        GPIOADIR &= ~BIT(6);
        GPIOADRV |= BIT(6);
        GPIOASET |= BIT(6);
        if(sys_188led.led_1 & BIT(0)) {
            GPIOADE |= BIT(7);
            GPIOADIR &= ~BIT(7);
        }
        if(sys_188led.led_1 & BIT(1)) {
            GPIOBDE |= BIT(3);
            GPIOBDIR &= ~BIT(3);
        }
        if(sys_188led.led_1 & BIT(2)) {
            GPIOBDE |= BIT(4);
            GPIOBDIR &= ~BIT(4);
        }
    }
}

AT(.com_text.led)
void bsp_188led_led2_set_on(void)
{
    if(sys_188led.led_2) {
        GPIOADE |= BIT(7);
        GPIOADIR &= ~BIT(7);
        GPIOADRV |= BIT(7);
        GPIOASET |= BIT(7);
        if(sys_188led.led_2 & BIT(0)) {
            GPIOADE |= BIT(6);
            GPIOADIR &= ~BIT(6);
        }
        if(sys_188led.led_2 & BIT(1)) {
            GPIOBDE |= BIT(3);
            GPIOBDIR &= ~BIT(3);
        }
        if(sys_188led.led_2 & BIT(2)) {
            GPIOBDE |= BIT(4);
            GPIOBDIR &= ~BIT(4);
        }
    }
}

AT(.com_text.led)
void bsp_188led_led3_set_on(void)
{
    if(sys_188led.led_3) {
        GPIOBDE |= BIT(3);
        GPIOBDIR &= ~BIT(3);
        GPIOBDRV |= BIT(3);
        GPIOBSET |= BIT(3);
        if(sys_188led.led_3 & BIT(0)) {
            GPIOEDE |= BIT(7);
            GPIOEDIR &= ~BIT(7);
        }
        if(sys_188led.led_3 & BIT(1)) {
            GPIOADE |= BIT(6);
            GPIOADIR &= ~BIT(6);
        }
        if(sys_188led.led_3 & BIT(2)) {
            GPIOADE |= BIT(7);
            GPIOADIR &= ~BIT(7);
        }
        if(sys_188led.led_3 & BIT(3)) {
            GPIOBDE |= BIT(4);
            GPIOBDIR &= ~BIT(4);
        }
    }
}

AT(.com_text.led)
void bsp_188led_led4_set_on(void)
{
    if(sys_188led.led_4) {
        GPIOBDE |= BIT(4);
        GPIOBDIR &= ~BIT(4);
        GPIOBDRV |= BIT(4);
        GPIOBSET |= BIT(4);
        if(sys_188led.led_4 & BIT(0)) {
            GPIOEDE |= BIT(7);
            GPIOEDIR &= ~BIT(7);
        }
        if(sys_188led.led_4 & BIT(1)) {
            GPIOADE |= BIT(6);
            GPIOADIR &= ~BIT(6);
        }
        if(sys_188led.led_4 & BIT(2)) {
            GPIOADE |= BIT(7);
            GPIOADIR &= ~BIT(7);
        }
        if(sys_188led.led_4 & BIT(3)) {
            GPIOBDE |= BIT(3);
            GPIOBDIR &= ~BIT(3);
        }
    }
}

AT(.com_text.led)
void bsp_188led_disp_value_parse_0(u8 num)
{
    if(num) {
        sys_188led.led_0 = 0;
        sys_188led.led_1 = 0;
        sys_188led.led_2 = 0;
        sys_188led.led_3 = 0;
        sys_188led.led_4 = BIT(0) | BIT(1);
    } else {
        sys_188led.led_0 = 0;
        sys_188led.led_1 = 0;
        sys_188led.led_2 = 0;
        sys_188led.led_3 = 0;
        sys_188led.led_4 = 0;
    }
}

AT(.com_text.led)
void bsp_188led_disp_value_parse_1(u8 num)
{
    switch (num) {
        case 0:
            sys_188led.led_2 |= BIT(0);
            sys_188led.led_3 |= BIT(0) | BIT(1) | BIT(2) | BIT(3);
            sys_188led.led_4 |= BIT(3);
            break;

        case 1:
            sys_188led.led_3 |= BIT(0);
            sys_188led.led_4 |= BIT(3);
            break;

        case 2:
            sys_188led.led_2 |= BIT(0);
            sys_188led.led_3 |= BIT(0) | BIT(1) | BIT(3);
            sys_188led.led_4 |= BIT(2);
            break;

        case 3:
            sys_188led.led_3 |= BIT(0) | BIT(1) | BIT(3);
            sys_188led.led_4 |= BIT(2) | BIT(3);
            break;

        case 4:
            sys_188led.led_3 |= BIT(0) | BIT(2);
            sys_188led.led_4 |= BIT(2) | BIT(3);
            break;

        case 5:
            sys_188led.led_3 |= BIT(1) | BIT(2) | BIT(3);
            sys_188led.led_4 |= BIT(2) | BIT(3);
            break;

        case 6:
            sys_188led.led_2 |= BIT(0);
            sys_188led.led_3 |= BIT(1) | BIT(2) | BIT(3);
            sys_188led.led_4 |= BIT(2) | BIT(3);
            break;

        case 7:
            sys_188led.led_3 |= BIT(0) | BIT(1);
            sys_188led.led_4 |= BIT(3);
            break;

        case 8:
            sys_188led.led_2 |= BIT(0);
            sys_188led.led_3 |= BIT(0) | BIT(1) | BIT(2) | BIT(3);
            sys_188led.led_4 |= BIT(2) | BIT(3);
            break;

        case 9:
            sys_188led.led_3 |= BIT(0) | BIT(1) | BIT(2) | BIT(3);
            sys_188led.led_4 |= BIT(2) | BIT(3);
            break;

        default:
            break;
    }
}

AT(.com_text.led)
void bsp_188led_disp_value_parse_2(u8 num)
{
    switch (num) {
        case 0:
            sys_188led.led_0 |= BIT(0) | BIT(1);
            sys_188led.led_1 |= BIT(0) | BIT(1) | BIT(2);
            sys_188led.led_2 |= BIT(1);
            break;

        case 1:
            sys_188led.led_1 |= BIT(2);
            sys_188led.led_2 |= BIT(1);
            break;

        case 2:
            sys_188led.led_0 |= BIT(0) | BIT(1);
            sys_188led.led_1 |= BIT(1) | BIT(2);
            sys_188led.led_2 |= BIT(2);
            break;

        case 3:
            sys_188led.led_0 |= BIT(0);
            sys_188led.led_1 |= BIT(1) | BIT(2);
            sys_188led.led_2 |= BIT(1) | BIT(2);
            break;

        case 4:
            sys_188led.led_1 |= BIT(0) | BIT(2);
            sys_188led.led_2 |= BIT(1) | BIT(2);
            break;

        case 5:
            sys_188led.led_0 |= BIT(0);
            sys_188led.led_1 |= BIT(0) | BIT(1);
            sys_188led.led_2 |= BIT(1) | BIT(2);
            break;

        case 6:
            sys_188led.led_0 |= BIT(0) | BIT(1);
            sys_188led.led_1 |= BIT(0) | BIT(1);
            sys_188led.led_2 |= BIT(1) | BIT(2);
            break;

        case 7:
            sys_188led.led_1 |= BIT(1) | BIT(2);
            sys_188led.led_2 |= BIT(1);
            break;

        case 8:
            sys_188led.led_0 |= BIT(0)| BIT(1);
            sys_188led.led_1 |= BIT(0) | BIT(1) | BIT(2);
            sys_188led.led_2 |= BIT(1) | BIT(2);
            break;

        case 9:
            sys_188led.led_0 |= BIT(0);
            sys_188led.led_1 |= BIT(0) | BIT(1) | BIT(2);
            sys_188led.led_2 |= BIT(1) | BIT(2);
            break;

        default:
            break;
    }
}

AT(.com_text.led)
void bsp_188led_disp_rest(void)
{
    sys_188led.delay_off_cnt = 0;
    sys_188led.disp_on = 0;
    sys_188led.curr_disp_value = -1;
    bsp_188led_gpio_init();
}

AT(.com_text.led)
void bsp_188led_disp_delay_off_check(void)
{
    if(CHARGE_DC_IN()) {
        return;
    }
    if(sys_188led.delay_off_cnt == LED_188LED_DISP_OFF_DELAY) {
        bsp_188led_disp_rest();
    }
}

AT(.com_text.led)
void bsp_188led_disp_set_on(void)
{
    sys_188led.delay_off_cnt = 0;
    sys_188led.disp_on = 1;
}

void bsp_188led_charge_full_disp(void)
{
    sys_188led.charge_full = 1;
    sys_188led.charge_full_cnt = LED_188LED_DISP_CHARGE_FULL;
}

AT(.com_text.led)
void bsp_188led_charge_full_disp_off(void)
{
    if(sys_188led.charge_full_cnt) {
        sys_188led.charge_full_cnt--;
        if(sys_188led.charge_full_cnt == 0) {
            bsp_188led_disp_rest();
        }
    }
}

AT(.com_text.led)
void bsp_188led_charge_disp(void)
{
    if(sys_188led.charge_full) {
        if(!sys_188led.disp_on && sys_188led.charge_full_cnt) {         //充满电的时候正好灭灯状态的情况
            bsp_188led_disp_set_on();
        }
        return;
    }
    if(sys_188led.disp_on) {
        bsp_188led_disp_rest();
    }else {
        bsp_188led_disp_set_on();
    }
}

AT(.com_text.led)
void bsp_188led_charge_tmr2_isr(void)
{
    TMR2CPND = BIT(9);
    sys_188led.cnt++;
    if((sys_188led.cnt % 5) == 0) {          //5ms
        bsp_188led_disp_set();
        bsp_188led_charge_full_disp_off();
        bsp_188led_disp_delay_off_check();
    }
    if(sys_188led.cnt == 1000) {
        if((user_timer_isr_en & BIT(1)) == 0) {
            sys_cb.vbat = get_vbat_val();
        }
        bsp_188led_get_bat_level();
        if(!CHARGE_DC_IN()) {
            sys_188led.cnt = 0;
        }
    }
    bsp_188led_disp_scan();
    if(CHARGE_DC_IN()) {
        if(sys_188led.cnt == LED_188LED_DISP_CHARGE) {
            bsp_188led_charge_disp();
        }else if(sys_188led.cnt == (LED_188LED_DISP_CHARGE * 2)) {
            bsp_188led_charge_disp();
            sys_188led.cnt = 0;
        }else if(sys_188led.cnt > 2*LED_188LED_DISP_CHARGE) {
            sys_188led.cnt = 0;
        }
    }
}

void bsp_188led_charge_tmr2_init(void)
{
    register_isr(IRQ_TMR2_VECTOR, bsp_188led_charge_tmr2_isr);
	TMR2CON =  BIT(7);              //Timer overflow interrupt enable
	TMR2CNT = 0;
	TMR2PR  = 1000000 / (5 * 200) - 1;   //1ms, select xosc26_div 1M clk
	TMR2CON |= BIT(2) | BIT(0);     //Timer works in Counter Mode
	PICPR &= ~BIT(IRQ_TMR2_VECTOR);
	PICEN |= BIT(IRQ_TMR2_VECTOR);
}

AT(.com_text.led)
void bsp_188led_disp_set(void)
{
    sys_188led.delay_off_cnt++;
    if(!sys_188led.disp_on) {
        return;
    }
    if(sys_188led.curr_disp_value == sys_188led.disp_value) {
        return;
    }
    sys_188led.curr_disp_value = sys_188led.disp_value;
    u8 num_0 = sys_188led.disp_value / 100;
    u8 num_1 = (sys_188led.disp_value / 10) % 10;
    u8 num_2 = sys_188led.disp_value % 10;

    bsp_188led_disp_value_parse_0(num_0);
    bsp_188led_disp_value_parse_1(num_1);
    bsp_188led_disp_value_parse_2(num_2);
}

AT(.com_text.led)
void bsp_188led_disp_scan(void)
{
    if(!sys_188led.disp_on) {
        return;
    }
    bsp_188led_gpio_init();
    switch (sys_188led.disp_sta) {
        case BIT(0):
            sys_188led.disp_sta = BIT(1);
            bsp_188led_led0_set_on();
            break;

        case BIT(1):
            sys_188led.disp_sta = BIT(2);
            bsp_188led_led1_set_on();
            break;

        case BIT(2):
            sys_188led.disp_sta = BIT(3);
            bsp_188led_led2_set_on();
            break;

        case BIT(3):
            sys_188led.disp_sta = BIT(4);
            bsp_188led_led3_set_on();
            break;

        case BIT(4):
            sys_188led.disp_sta = BIT(0);
            bsp_188led_led4_set_on();
            break;

        default:
            break;
    }
}

#endif
