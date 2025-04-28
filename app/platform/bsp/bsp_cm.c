#include "include.h"
#include "bsp_param.h"

#define TRACE_EN                0

#if TRACE_EN
#define TRACE(...)              printf(__VA_ARGS__)
#else
#define TRACE(...)
#endif

#define CM_SIZE         0x2000          //����������8k
#define CM_START        (FLASH_SIZE - CM_SIZE)

void param_sync_hook(void);
u32 bsp_get_rand_key(void);
typedef void (*param_write_func)(u8 *buf, u32 addr, uint len);
typedef void (*param_read_func)(u8 *buf, u32 addr, uint len);

///CM Initʱ���жϸ�Page�����Ƿ���Ч����Ч�����
bool cm_loadparam(void *buff, uint page)
{
    return true;
}

///CM Initʱ������Ҳ�����Ч�Ĳ���������г���ֵ����
void cm_factory(void *buff, uint page)
{
    u8 *buf = buff;
    memset(buf, 0, 250);

    TRACE("cm_factory: %d\n", page);
    if (page == SYS_CM_PAGE_NUM) {
        //ϵͳ������ʼ��
        sys_cb.vol = SYS_INIT_VOLUME + 1;

        if (xcfg_cb.lang_id == LANG_EN_ZH) {
            sys_cb.lang_id = 0;             //����Ĭ��Ӣ��
        } else if (xcfg_cb.lang_id == LANG_ZH_EN) {
            sys_cb.lang_id = 1;             //����Ĭ������
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
