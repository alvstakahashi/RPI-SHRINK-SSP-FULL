#include <kernel.h>
#include "kernel_cfg.h"

#include "rpi_lib/rpi.h"
#include <stdio.h>
#include <stdint.h>

#include "kernel_impl.h"
#include "target_kernel.h"

#if defined(RPIBPLUS) || defined(RPI2) || defined(RPI3)
#define	LED_ACT_PIN	47	//TYPE B PLUS
#else
#define LED_ACT_PIN	16	//TYPE B
#endif


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

	// すべての割り込み不許可
	*INTERRUPT_DISABLE_BASIC_IRQS = 0xffffffff;
	*INTERRUPT_DISABLE_IRQS1 = 0xffffffff;
	*INTERRUPT_DISABLE_IRQS2 = 0xffffffff;
	*INTERRUPT_FIQ_CTRL = 0;

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
	*TIMER_LOAD = 1000-1;
	*TIMER_RELOAD = 1000-1;

	// 割り込みフラグをクリア
	*TIMER_IRQ_CLR = 0;

	// タイマー開始
	// Timer enable, 32bit Timer
	*TIMER_CONTROL |= 0x000000A2;
#ifdef RPI3
	target_hrt_initialize(0);
#endif

	// 割り込み許可
	*INTERRUPT_ENABLE_BASIC_IRQS = 0x01;

	// IRQ許可
	enable_IRQ();

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

