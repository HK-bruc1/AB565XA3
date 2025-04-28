#ifndef _API_QTEST_H
#define _API_QTEST_H

#define SW_QTEST_NORMAL                 2
#define SW_QTEST_ONLY_PAIR              3

bool qtest_init(u8 rst_source);
void qtest_step_process(void);
void qtest_create_env(void);
void qtest_process(void);
u8 get_qtest_mode(void);
void bsp_tbox_qest_packet_recv(u8 data);
void send_msg2tbox(u8 *buf,u16 len);

bool qtest_feature_is_pickup_pwroff(void);
bool qtest_feature_is_pickup_rst(void);
#endif // _API_QTEST_H
