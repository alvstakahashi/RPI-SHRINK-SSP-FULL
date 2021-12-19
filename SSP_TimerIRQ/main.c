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

	qemu_timer_init();

	sta_ker();

	while(1);
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
