#include "config.h"
setflash(1, FLASH_SIZE, 1, 0, 0);
setspace(0x2000);
setid(11111111-1111-1111-1111-111111111111);
make(dcf_buf, header.bin, app.bin, res.bin, xcfg.bin, updater.bin);
save(dcf_buf, app.dcf);
