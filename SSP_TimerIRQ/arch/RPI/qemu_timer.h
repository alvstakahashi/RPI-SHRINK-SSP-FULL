
#ifndef QEMU_TIMER_H
#define QEMU_TIMER_H

extern uint32_t cntfrq;
extern void qemu_timer_init(void);
extern uint32_t read_core0timer_pending(void);
extern void write_cntv_tval(uint32_t val);
#endif