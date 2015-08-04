#include <kernel.h>
#include "kernel_cfg.h"

#include "rpi_lib/rpi.h"
#include <stdio.h>
#include <stdint.h>

#include "kernel_impl.h"
#include "target_kernel.h"

#ifdef RPIBPLUS
#define	LED_ACT_PIN	47	//TYPE B PLUS
#elif RPI2
#define LED_ACT_PIN	47	// PRI2
#else
#define LED_ACT_PIN	16	//TYPE B
#endif


extern void _kernel_handler(INTHDR userhandler);
extern void signal_time(void);

// IRQ割り込みハンドラ
void IRQ_handler(void)
{
	// Basic IRQ pendingをチェック
	if(*INTERRUPT_IRQ_BASIC_PENDING & 0x01 != 0)
	{
		// タイマー割り込み
        // 割り込みフラグクリア
        *TIMER_IRQ_CLR = 0;

		_kernel_handler(signal_time);
	}
	return;
}


void task1(intptr_t arg)
{
	printf("task1 here\n");
	sta_cyc(RUBY_CYC3);
	sta_cyc(RUBY_CYC4);
	printf("task1 end\n");
}
void task2(intptr_t arg)
{
	int toggle= 0;
	printf("task2 --------------\n");
	printf("LED: ON\n");
	digitalWrite(LED_ACT_PIN, HIGH);
}

void task3(void)
{
	printf("task3-----------------\n");
	printf("LED: OFF\n");
	digitalWrite(LED_ACT_PIN, LOW);
}

void mrb_ssp_cyclic_call(intptr_t arg)
{
	if (arg == 3)
	{
		iact_tsk(TASK2_ID);
	}
	else
	{
		iact_tsk(TASK3_ID);
	}		
}
void mrb_ssp_alarm_call(intptr_t arg)
{
	iact_tsk(TASK3_ID);
}
