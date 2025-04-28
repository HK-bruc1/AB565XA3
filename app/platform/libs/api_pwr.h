#ifndef _API_PWR_H
#define _API_PWR_H

#define RI_VIO_TC(bit)                       RTCCON4 = (RTCCON4 & ~(3<<28)) | ((bit)<<28)

void pmu_init(u8 buck_en);
void set_buck_mode(u8 buck_en);
void pmu_vdd11_select_vrtc11(void);
#endif
