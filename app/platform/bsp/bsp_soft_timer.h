#ifndef __BSP_SOFT_TIMER_H
#define __BSP_SOFT_TIMER_H

#if LE_CHARGE_BOX

typedef void *soft_timer_p;

typedef void (*soft_timer_handler_t)(void*);


typedef enum {
    TIMER_SINGLE_SHOT,
    TIMER_REPEATED,
} soft_timer_mode;


void soft_timer_init(void);
void soft_timer_run(void);
int soft_timer_create(void** p_timer_id,uint32_t timeout_value_ms, soft_timer_mode mode,soft_timer_handler_t timeout_handler);
int soft_timer_stop(void* timer_id);
int soft_timer_restart(void* timer_id,uint32_t timeout_value_ms);
int soft_timer_start(void* timer_id);
int soft_timer_delete(void* timer_id);
#endif

#endif // __BSP_SOFT_TIMER_H
