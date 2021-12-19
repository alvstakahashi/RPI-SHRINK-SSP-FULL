#include <kernel.h>
#include "kernel_cfg.h"

#include "rpi_lib/rpi.h"
#include "rpi_peripherals.h"
#include <stdio.h>
#include <stdint.h>

#include "kernel_impl.h"
#include "target_kernel.h"

#if defined(RPIBPLUS) || defined(RPI2) || defined(RPI3)
#define	LED_ACT_PIN	47	//TYPE B PLUS
#else
#define LED_ACT_PIN	16	//TYPE B
#endif

#define IRQ_TIMER_C1  (1 << 1)
#define IRQ_TIMER_C3  (1 << 3)


#define CORE0_IRQ_SOURCE 	0x40000060
#define INT_SRC_GPU 		0x00000100			//なぜかタイマー割り込み

volatile int count = 0;


int setup(void)
{
	initializeJtagPin();
	rpi_init();

	// 起動確認用
	pinMode(LED_ACT_PIN,OUTPUT);
	digitalWrite(LED_ACT_PIN, LOW);

#if 0
	// すべての割り込み不許可
//	*INTERRUPT_DISABLE_BASIC_IRQS = 0xffffffff;
//	*INTERRUPT_DISABLE_IRQS1 = 0xffffffff;
//	*INTERRUPT_DISABLE_IRQS2 = 0xffffffff;
//	*INTERRUPT_FIQ_CTRL = 0;

	// タイマ割り込み設定
	*INTERRUPT_ENABLE_BASIC_IRQS = 0x01;

	// 設定のため一旦タイマー停止
	*TIMER_CONTROL &= 0xffffff00;

	// timer clock を1MHzに設定
	//（0xF9=249: timer clock=250MHz/(249+1)）
	*TIMER_PREDIVIDER = 0x000000F9;

	// タイマー値設定(4sec)
//	*TIMER_LOAD = 4000000-1;
//	*TIMER_RELOAD = 4000000-1;
	// タイマー値設定(1msec)
	*TIMER_LOAD = 10-1;
	*TIMER_RELOAD = 10-1;

	// 割り込みフラグをクリア
	*TIMER_IRQ_CLR = 0;

	// タイマー開始
	// Timer enable, 32bit Timer
	*TIMER_CONTROL |= 0x000000A2;

	// 割り込み許可
	*INTERRUPT_ENABLE_BASIC_IRQS = 0x01;

	// IRQ許可
//	enable_IRQ();
//#else
	// タイマ割り込み設定
	*INTERRUPT_DISABLE_BASIC_IRQS = 0x01;
  // disable IRQ
  //IRQ_DISABLE_BASIC = 1;
  
  // set timer
  uint32_t now = *SYST_CLO;
  *SYST_C1 = now +1000;
 // enable IRQ
//  set_vbar(&exception_vector);
//  IRQ_ENABLE1 = (IRQ_TIMER_C1 );
//  __asm volatile("mrs r0, cpsr \n"
//                "bic r0, r0, #0x80 \n"
//                 "msr cpsr_c, r0 \n");
	// 割り込み許可
	//Set_vector();
	*SYST_CS = 0x02;


	*INTERRUPT_ENABLE_BASIC_IRQS = (IRQ_TIMER_C1  );;
#endif
	kernel_main();

	sta_ker();

	while(1);
//
//	return 0;
}
void main(intptr_t arg)
{
	printf("main here\n");
	act_tsk(TASK3_ID);
	act_tsk(TASK2_ID);
	printf("main end\n");
}
void task2(intptr_t arg)
{
	int toggle= 0;
	printf("task2 RUNNING-----------------------------------------------------\n");
	for(;;)
	{
		if ((toggle ^= 1) != 0)
		{
			printf("TASK2 LED: ON count= %d\n",count);
			digitalWrite(LED_ACT_PIN, HIGH);
		}
		else
		{
			printf("TASK2 LED: OFF count= %d\n",count);
			digitalWrite(LED_ACT_PIN, LOW);
		}
		dly_tsk(1000);
	}
}

void task3(void)
{
	printf("task3-----------------\n");
	for(;;)
	{
		dly_tsk(500);
		count++;
		printf("task3-----------------\n");
	}
}


//extern void enable_irq(void);
//extern void disable_irq(void);
//extern void io_halt(void);

// Memory-Mapped I/O output
static inline void mmio_write(uint32_t reg, uint32_t data)
{
    *(volatile uint32_t*)reg = data;
}
 
// Memory-Mapped I/O input
static inline uint32_t mmio_read(uint32_t reg)
{
    return *(volatile uint32_t*)reg;
}



#define CORE0_TIMER_IRQCNTL 0x40000040
#define CORE0_IRQ_SOURCE 0x40000060

void routing_core0cntv_to_core0irq(void)
{
    mmio_write(CORE0_TIMER_IRQCNTL, 0x08);
}

uint32_t read_core0timer_pending(void)
{
    uint32_t tmp;
    tmp = mmio_read(CORE0_IRQ_SOURCE);
    return tmp;
}

uint32_t cntfrq = 0;

void enable_cntv(void)
{
    uint32_t cntv_ctl;
    cntv_ctl = 1;
	asm volatile ("mcr p15, 0, %0, c14, c3, 1" :: "r"(cntv_ctl) ); // write CNTV_CTL
}

void disable_cntv(void)
{
    uint32_t cntv_ctl;
    cntv_ctl = 0;
	asm volatile ("mcr p15, 0, %0, c14, c3, 1" :: "r"(cntv_ctl) ); // write CNTV_CTL
}

uint64_t read_cntvct(void)
{
	uint64_t val;
	asm volatile("mrrc p15, 1, %Q0, %R0, c14" : "=r" (val));
	return (val);
}

uint64_t read_cntvoff(void)
{
	uint64_t val;
    asm volatile("mrrc p15, 4, %Q0, %R0, c14" : "=r" (val));
	return (val);
}

uint32_t read_cntv_tval(void)
{
    uint32_t val;
	asm volatile ("mrc p15, 0, %0, c14, c3, 0" : "=r"(val) );
    return val;
}

void write_cntv_tval(uint32_t val)
{
	asm volatile ("mcr p15, 0, %0, c14, c3, 0" :: "r"(val) );
    return;
}

uint32_t read_cntfrq(void)
{
    uint32_t val;
	asm volatile ("mrc p15, 0, %0, c14, c0, 0" : "=r"(val) );
    return val;
}

void c_irq_handler(void)
{
    if (read_core0timer_pending() & 0x08 ) {
        write_cntv_tval(cntfrq/1000);    // clear cntv interrupt and set next 1sec timer.

    }
    return;
}

void kernel_main(void)
{
    uint32_t val;

    cntfrq = read_cntfrq();

    write_cntv_tval(cntfrq/1000);    // clear cntv interrupt and set next 1 sec timer.
    val = read_cntv_tval();
 
    routing_core0cntv_to_core0irq();
    enable_cntv(); 
  //  enable_irq();

}
