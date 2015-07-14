/* kernel_cfg.c */
//#include "kernel_int.h"
#include "kernel_impl.h"
#include "task.h"
#include "kernel_unrename.h"

#include "kernel_cfg.h"
#include "sample1.h"

/*
 *  Task Management Functions
 */

const ID _kernel_tmax_tskid = (TMIN_TSKID + TNUM_TSKID - 1);

const ATR     	_kernel_tinib_tskatr[TNUM_TSKID]    = {(TA_ACT),(TA_NULL)};
const uint_t	_kernel_init_rdypmap = 1U;
const intptr_t	_kernel_tinib_exinf[TNUM_TSKID]     = {(intptr_t)(0),(intptr_t)(0)};
const TASK    	_kernel_tinib_task[TNUM_TSKID]      = {(task1),(task2)};
const uint_t  	_kernel_tinib_epriority[TNUM_TSKID] = {INT_PRIORITY(1),INT_PRIORITY(1)};


/*
 *  Stack Area for System
 */

#define TOPPERS_ISTKSZ		DEFAULT_ISTKSZ
static STK_T          		_kernel_stack[COUNT_STK_T(TOPPERS_ISTKSZ)];
#define TOPPERS_STK   		_kernel_stack
#define TOPPERS_STKSZ		ROUND_STK_T(TOPPERS_ISTKSZ)


const SIZE		_kernel_stksz = TOPPERS_STKSZ;
STK_T *const	_kernel_stk = TOPPERS_STK;

/*
 *  ��^�X�N�R���e�L�X�g�p�̃X�^�b�N�����l
 */

#define TOPPERS_ISTKPT( istk, istksz ) (( STK_T * )(( char * )( istk ) + ( istksz )))

#ifdef TOPPERS_ISTKPT
STK_T *const	_kernel_istkpt = TOPPERS_ISTKPT(TOPPERS_STK, TOPPERS_STKSZ);
#endif /* TOPPERS_ISTKPT */

/*
 *  Module Initialization Function
 */

void
_kernel_initialize_object(void)
{
	_kernel_initialize_task();			//�K�v
//	_kernel_initialize_interrupt();		
//	_kernel_initialize_exception();		
}


#ifndef TARGET_COPYRIGHT
#define TARGET_COPYRIGHT
#endif /* TARGET_COPYRIGHT */




