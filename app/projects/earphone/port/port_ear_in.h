#ifndef _PORT_EAR_IN_H
#define _PORT_EAR_IN_H

#if BT_EARIN_DETECT_EN
void earin_init(void);
void earin_detect(void);
void earin_sniff_gpiode_set(void);
bool earin_is_wakeup(void);
#else
#define earin_init()
#define earin_detect()
#define void earin_sniff_gpiode_set()
#define earin_is_wakeup()                   (0)
#endif // BT_EARIN_DETECT_EN

#endif
