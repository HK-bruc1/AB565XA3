/**************************************************************************
 *                  TIMER使用例程
 * 1. 只能使用timer2, timer3，其它timer已经被系统使用
 * 2. timer2只有counter功能，timer3有counter/capture/pwm功能
 *
 **************************************************************************/

#include "include.h"

/*

///高优先级中断(一定要加FIQ属性)
AT(.com_text.timer2) FIQ
void timer2_isr(void)
{
    TMR2CPND = BIT(9);                              //clear timer2 overflow pending

}

///这里只是示例怎么用高优先级中断，最好使用低优先级中断。
void timer2_init(void)
{
    sys_irq_init(IRQ_TMR2_VECTOR, 1, timer2_isr);   //注册高优先级中断
    TMR2CON =  BIT(7);                              //Timer overflow interrupt enable
	TMR2CNT = 0;
	TMR2PR  = 50000 - 1;                            //50ms period
	TMR2CON |= BIT(2) | BIT(0);                     //Timer works in Counter Mode
}

///低优先级中断(不用加FIQ属性)
AT(.com_text.timer3)
void timer3_isr(void)
{
    if (TMR3CON & BIT(16)) {                        //timer3 overflow pending
        TMR3CPND = BIT(16);                         //clear timer3 overflow pending

    }
}

void timer3_counter_init(void)
{
    sys_irq_init(IRQ_TMR3_VECTOR, 0, timer3_isr);   //注册低优先级中断
    TMR3CON =  BIT(7);                              //Timer overflow interrupt enable
	TMR3CNT = 0;
	TMR3PR  = 5000 - 1;                             //5ms period
	TMR3CON |= BIT(2) | BIT(0);                     //Timer works in Counter Mode
}

//timer3 pwm(pwm0: PA6, pwm1: PA7, pwm2: PB3)
void timer3_pwm_init(void)
{
    GPIOADE |= BIT(6) | BIT(7);
    GPIOADIR &= ~(BIT(6) | BIT(7));
    GPIOBDE |= BIT(3);
    GPIOBDIR &= ~BIT(3);
    FUNCMCON2 = (1 << 8);

	TMR3CNT = 0;
	TMR3PR  = (1000*3)-1;
	TMR3DUTY0 = 1000 - 1;                           //pwm0 duty, low level: TMR3DUTY0 + 1, high level: TMR3PR-TMR3DUTY0+1
	TMR3DUTY1 = 2000 - 1;                           //pwm1 duty
	TMR3DUTY2 = 500 - 1;                            //pwm2 duty
	TMR3CON  = BIT(9) | BIT(10) | BIT(11);          //timer3 pwm0(bit9), pwm1(bit10), pwm2 enable(bit11)
	TMR3CON |= BIT(2);                              //timer increase clk, 1:select xosc26_div 1MHz, 0:select sysclk 26MHz
	TMR3CON |= BIT(0);                              //Timer enable
}

*/
