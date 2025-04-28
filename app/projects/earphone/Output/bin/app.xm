#include "config.h"
depend(0x01030500);
setflash(1, FLASH_SIZE, 1, 0, 0);
setspace(0x2000);
#if SYS_SUPPORT_DONGLE_EN
setauth(0xDF1655BC, soft_key);    //加密狗
#endif
setid(11111111-1111-1111-1111-111111111111);
make(dcf_buf, header.bin, app.bin, res.bin, xcfg.bin, updater.bin);
save(dcf_buf, app.dcf);
