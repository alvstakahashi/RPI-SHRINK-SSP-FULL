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


void set_vector_table(void){
	extern void *_initial_vector_start;
	extern void *_initial_vector_end;
	// volatileをつけないと最適化に消される（涙目）
	volatile unsigned int *vec = 0;
	volatile unsigned int *p;
	volatile unsigned int *s = (unsigned int *)&_initial_vector_start;
	volatile unsigned int *e = (unsigned int *)&_initial_vector_end;

	printf("Vector table check\n");
	printf("Addr : Hex\n");
	for (p = s; p < e; p++) {
		*vec = *p;
		printf("0x%02x : 0x%08x\n",vec,*vec);
		vec++;
	}
}

int setup(void)
{
	rpi_init();

	// 起動確認用
	pinMode(LED_ACT_PIN,OUTPUT);
	digitalWrite(LED_ACT_PIN, LOW);

	// ベクタテーブルセット
	set_vector_table();

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

	// 割り込み許可
	*INTERRUPT_ENABLE_BASIC_IRQS = 0x01;

	// IRQ許可
	enable_IRQ();

	sta_ker();

	while(1);
//
//	return 0;
}
