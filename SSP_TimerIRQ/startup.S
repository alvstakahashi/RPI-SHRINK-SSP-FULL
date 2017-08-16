#define TOPPERS_MACRO_ONLY
#define TOPPERS_ASM_MACRO
#define UINT_C(val)     (val)        /* uint_t型の定数を作るマクロ */
#define ULONG_C(val)    (val)        /* ulong_t型の定数を作るマクロ */

#include "target_asm.inc"

#ifdef TOPPERS_USE_BCM2837
#define CPSR_SVC_MODE	UINT_C(0x13)
#define AOR				|
/*
 *  CPSRの割込み禁止ビット
 */
//#define CPSR_INT_MASK	UINT_C(0xc0)
#define CPSR_IRQ_BIT	UINT_C(0x80)
#define CPSR_FIQ_BIT	UINT_C(0x40)

/*
 *  FIQとIRQの両方を禁止するCPSRのビットパターン
 */
#define CPSR_FIQ_IRQ_BIT	(CPSR_FIQ_BIT AOR CPSR_IRQ_BIT)

#endif


@ startup
	.global _start
	.align
_start:
_reset:
#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
	/*
	 *  セキュア解除
	 *  config.txt に disable_commandline_tags=1 を記載すること．
	 */

#if defined(TOPPERS_USE_BCM2837)
	/*
	 *  キャッシュ無効化
	 *  ブートローダが有効にしているので無効化．
	 */
	mov r12,#0
	mcr p15, 0, r12, c7, c10, 1
	dsb
	mov r12, #0
	mcr p15, 0, r12, c7, c5, 0
	mov r12, #0
	mcr p15, 0, r12, c7, c5, 6
	dsb
	isb
#endif

	/* コア0のみ動作 */
	mrc  p15, 0, r0, c0, c0, 5
	mov  r1, #0xF
	ands r0, r0, r1
	beq  start_core0
ALABEL(start_coren)
	wfi
	b    start_coren
ALABEL(start_core0)
#endif

	// IRQモードのスタックポインタ設定
	// IRQ,FIQ disable, Thumb disable, IRQ mode
	ldr	r0, =0x000000d2
	msr	cpsr, r0
	ldr	sp, =0x00008000

	// SVCモードのスタックポインタ設定
	// IRQ,FIQ disable, Thumb disable, SVC mode
	ldr	r0, =0x000000d3
	msr	cpsr, r0
	ldr	sp, =0x06400000

	// setup呼び出し
	bl setup

	// 終了（無限ループ）
	b .

.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.globl dummy
dummy:
    bx lr

.globl GETPC
GETPC:
    mov r0,lr
    bx lr

.globl GETCPSR
GETCPSR:
    mrs r0,cpsr
    bx lr
