#ifndef _PORT_HALL_SENSOR_H
#define _PORT_HALL_SENSOR_H

#if BT_HALL_SENSOR_EN
void hall_sensor_init(void);
void hall_sensor_detect(void);
void hall_trigger_to_pwrdwn(void);
#else
#define hall_sensor_init()
#define hall_sensor_detect()
#define hall_trigger_to_pwrdwn()
#endif // BT_HALL_SENSOR_EN

#endif // _PORT_HALL_SENSOR_H
