#include "include.h"
#include "bsp_param.h"

#define TRACE_EN                0

#if TRACE_EN
#define TRACE(...)              printf(__VA_ARGS__)
#else
#define TRACE(...)
#endif

#define CM_SIZE         0x2000          //参数区至少8k
#define CM_START        (FLASH_SIZE - CM_SIZE)

void param_sync_hook(void);
u32 bsp_get_rand_key(void);
typedef void (*param_write_func)(u8 *buf, u32 addr, uint len);
typedef void (*param_read_func)(u8 *buf, u32 addr, uint len);

///CM Init时，判断该Page参数是否有效，有效则加载
bool cm_loadparam(void *buff, uint page)
{
    return true;
}

///CM Init时，如果找不到有效的参数，则进行出厂值设置
void cm_factory(void *buff, uint page)
{
    u8 *buf = buff;
    memset(buf, 0, 250);

    TRACE("cm_factory: %d\n", page);
    if (page == SYS_CM_PAGE_NUM) {
        //系统参数初始化
        sys_cb.vol = SYS_INIT_VOLUME + 1;

        if (xcfg_cb.lang_id == LANG_EN_ZH) {
            sys_cb.lang_id = 0;             //出厂默认英文
        } else if (xcfg_cb.lang_id == LANG_ZH_EN) {
            sys_cb.lang_id = 1;             //出厂默认中文
        } else {
            sys_cb.lang_id = xcfg_cb.lang_id;
        }

        buf[PARAM_SYS_VOL] = sys_cb.vol;
        if (sys_cb.lang_id) {
            buf[PARAM_LANG_ID] = BIT(3);
        }
        PUT_LE32(buf + PARAM_RANDOM_KEY, sys_cb.rand_seed);
    }
}

void cm_param_init(void)
{
    param_cb.read  = (param_read_func)cm_read;
    param_cb.write = (param_write_func)cm_write;
    param_cb.sync  = cm_sync;
    cm_init(MAX_CM_PAGE, CM_START, CM_SIZE);
    sys_cb.cm_factory = 0;
}
