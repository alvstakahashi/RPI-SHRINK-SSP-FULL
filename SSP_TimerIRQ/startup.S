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
#ifdef TOPPERS_USE_BCM2837
_initial_vector_start:
	ldr	pc, _vec_Reset
	ldr	pc, _vec_Undef
	ldr	pc, _vec_SWI
	ldr	pc, _vec_PrefAbort
	ldr	pc, _vec_DataAbort
	ldr	pc, _vec_Reserved
	ldr	pc, _vec_IRQ
	ldr	pc, _vec_FIQ
_vec_Reset:		.word	_reset
_vec_Undef:		.word	_hangup
_vec_SWI:		.word	_hangup
_vec_PrefAbort:	.word	_hangup
_vec_DataAbort:	.word	_hangup
_vec_Reserved:	.word	_hangup
_vec_IRQ:		.word	_IRQ_iterrupt
//_vec_IRQ:		.word	__kernel_default_int_handler_entry
_vec_FIQ:		.word	_hangup
_initial_vector_end:
#endif
_reset:
#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
	/*
	 *  セキュア解除
	 *  config.txt に disable_commandline_tags=1 を記載すること．
	 */
	mrc p15, 0, r0, c1, c1, 0
	orr r0, r0, #0x1
	mcr p15, 0, r0, c1, c1, 0

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

.global _hangup
_hangup:
	// IRQ,FIQ disable, Thumb disable, SVC mode 
	ldr	r0, =0x000000d3
	msr	cpsr, r0
	b .

_IRQ_iterrupt:
	/* 
	 * IRQモードで実行される
　　　　 * スタックはスーパーバイザーモードにコンテキスト
	 * を保存
	 */
	sub   lr, lr, #4 			/* 戻り先アドレス  */
	srsdb #CPSR_SVC! 			/*lr(戻り先) spsr(cpsr_svc)を	*/
						/* スーパーバイザーモードのスタックへ保存	*/

						/*
						 *  割込み前のモード(スーパーバイザーモード)へ
						 *  移行し，コンテキストを保存する
						 */
	msr   cpsr_cxsf, #(CPSR_SVC AOR CPSR_CPULOCK AOR CPSR_ALWAYS_SET)
	stmfd sp!, {r0-r12,lr}				/* 全部保存する								*/
//	;----------------------------------------------------------
//	;ここまでで、スーパーバイザースタックに以下のように積まれる
//	;
//	;           例外フレーム(ARMv6-7)            offset
//	;      ------------------------------
//	;     |            R0-R12            |  0　
//	;      ------------------------------
//	;     |          LR_svc              |  
//	;      ------------------------------
//	;     |     例外前のPC(戻り先)       |  
//	;      ------------------------------
//	;     |        例外前のCPSR          |  
//	;      ------------------------------
//	;-------------------------------------------------------------
	// C言語の割り込みハンドラ呼び出し
	bl	IRQ_handler
	/*
	 *  割込み処理からのリターンにより，CPUロック解除状態に移行するよ
	 *  うにする．ARMはCPSRのIRQビットによってCPUロック状態を表してい
	 *  るため，そのままリターンすればよい．
	 */
	ldmfd sp!, {r0-r12,lr}			//; 全部戻す
	rfeia sp!



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
// ベクターテーブル設定用
#ifndef TOPPERS_USE_BCM2837
	.global  _initial_vector_start
_initial_vector_start:
	ldr	pc, _vec_Reset
	ldr	pc, _vec_Undef
	ldr	pc, _vec_SWI
	ldr	pc, _vec_PrefAbort
	ldr	pc, _vec_DataAbort
	ldr	pc, _vec_Reserved
	ldr	pc, _vec_IRQ
	ldr	pc, _vec_FIQ
_vec_Reset:		.word	_start
_vec_Undef:		.word	_hangup
_vec_SWI:		.word	_hangup
_vec_PrefAbort:	.word	_hangup
_vec_DataAbort:	.word	_hangup
_vec_Reserved:	.word	_hangup
_vec_IRQ:		.word	_IRQ_iterrupt
//_vec_IRQ:		.word	__kernel_default_int_handler_entry
_vec_FIQ:		.word	_hangup
	.global  _initial_vector_end
_initial_vector_end:
	mov r0,r0
// 最後に何か命令を置かないと_initial_vector_endラベルが消える
#endif
