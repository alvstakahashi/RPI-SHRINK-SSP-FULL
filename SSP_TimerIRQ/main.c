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

volatile int tim_count = 0;
volatile int tim_flag = 0;

void isig_tim()
{
	if (tim_count++ > 500)
	{
		if (tim_flag == 0)
		{
			iact_tsk(TASK3_ID);
			tim_flag = 1;
		}
		else
		{
			iact_tsk(TASK2_ID);
			tim_flag = 0;
		}
		tim_count = 0;
	}
}

int setup(void)
{
	initializeJtagPin();
	rpi_init();

	// 起動確認用
	pinMode(LED_ACT_PIN,OUTPUT);
	digitalWrite(LED_ACT_PIN, LOW);

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
	*TIMER_LOAD = 1000-1;
	*TIMER_RELOAD = 1000-1;

	// 割り込みフラグをクリア
	*TIMER_IRQ_CLR = 0;

	// タイマー開始
	// Timer enable, 32bit Timer
	*TIMER_CONTROL |= 0x000000A2;

	// 割り込み許可
	*INTERRUPT_ENABLE_BASIC_IRQS = 0x01;

	// IRQ許可
//	enable_IRQ();

	sta_ker();

	while(1);
//
//	return 0;
}
void main(intptr_t arg)
{
	printf("main here\n");
//	act_tsk(TASK3_ID);
//	act_tsk(TASK2_ID);
	printf("main end\n");
}
void task2(intptr_t arg)
{
	static	int toggle= 0;
	printf("task2 RUNNING-----------------------------------------------------\n");
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
		//dly_tsk(100000);
	}
}

void task3(void)
{
	{
		//dly_tsk(50000);
		count++;
		printf("task3-----------------\n");
	}
}

