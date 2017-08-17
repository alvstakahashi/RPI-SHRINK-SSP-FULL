/*
 *  TOPPERS/SSP Kernel
 *      Smallest Set Profile Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2009 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2010 by Naoki Saito
 *             Nagoya Municipal Industrial Research Institute, JAPAN
 *  Copyright (C) 2010 by Meika Sugimoto
 *
 *  ��L���쌠�҂́C�ȉ��� (1)�`(4) �̏����𖞂����ꍇ�Ɍ���C�{�\�t�g�E�F
 *  �A�i�{�\�t�g�E�F�A����ς������̂��܂ށD�ȉ������j���g�p�E�����E��ρE
 *  �Ĕz�z�i�ȉ��C���p�ƌĂԁj���邱�Ƃ𖳏��ŋ����D
 *  (1) �{�\�t�g�E�F�A���\�[�X�R�[�h�̌`�ŗ��p����ꍇ�ɂ́C��L�̒��쌠
 *      �\���C���̗��p��������щ��L�̖��ۏ؋K�肪�C���̂܂܂̌`�Ń\�[�X
 *      �R�[�h���Ɋ܂܂�Ă��邱�ƁD
 *  (2) �{�\�t�g�E�F�A���C���C�u�����`���ȂǁC���̃\�t�g�E�F�A�J���Ɏg�p
 *      �ł���`�ōĔz�z����ꍇ�ɂ́C�Ĕz�z�ɔ����h�L�������g�i���p�҃}
 *      �j���A���Ȃǁj�ɁC��L�̒��쌠�\���C���̗��p��������щ��L�̖���
 *      �؋K����f�ڂ��邱�ƁD
 *  (3) �{�\�t�g�E�F�A���C�@��ɑg�ݍ��ނȂǁC���̃\�t�g�E�F�A�J���Ɏg�p
 *      �ł��Ȃ��`�ōĔz�z����ꍇ�ɂ́C���̂����ꂩ�̏����𖞂������ƁD
 *    (a) �Ĕz�z�ɔ����h�L�������g�i���p�҃}�j���A���Ȃǁj�ɁC��L�̒���
 *        ���\���C���̗��p��������щ��L�̖��ۏ؋K����f�ڂ��邱�ƁD
 *    (b) �Ĕz�z�̌`�Ԃ��C�ʂɒ�߂��@�ɂ���āCTOPPERS�v���W�F�N�g�ɕ�
 *        �����邱�ƁD
 *  (4) �{�\�t�g�E�F�A�̗��p�ɂ�蒼�ړI�܂��͊ԐړI�ɐ����邢���Ȃ鑹�Q
 *      ������C��L���쌠�҂����TOPPERS�v���W�F�N�g��Ɛӂ��邱�ƁD�܂��C
 *      �{�\�t�g�E�F�A�̃��[�U�܂��̓G���h���[�U����̂����Ȃ闝�R�Ɋ��
 *      ������������C��L���쌠�҂����TOPPERS�v���W�F�N�g��Ɛӂ��邱�ƁD
 *
 *  �{�\�t�g�E�F�A�́C���ۏ؂Œ񋟂���Ă�����̂ł���D��L���쌠�҂���
 *  ��TOPPERS�v���W�F�N�g�́C�{�\�t�g�E�F�A�Ɋւ��āC����̎g�p�ړI�ɑ΂�
 *  ��K�������܂߂āC�����Ȃ�ۏ؂��s��Ȃ��D�܂��C�{�\�t�g�E�F�A�̗��p
 *  �ɂ�蒼�ړI�܂��͊ԐړI�ɐ����������Ȃ鑹�Q�Ɋւ��Ă��C���̐ӔC��
 *  ��Ȃ��D
 *
 */
#include <stddef.h>
#include <limits.h>
#include <stdio.h>

#include "kernel_impl.h"
#include "task.h"

#include "kernel_cfg.h"



/*
 *  �g���[�X���O�}�N���̃f�t�H���g��`
 */

#ifndef LOG_DSP_ENTER
	#define LOG_DSP_ENTER(tskidx)
#endif /* LOG_DSP_ENTER */

#ifndef LOG_DSP_LEAVE
	#define LOG_DSP_LEAVE(tskidx)
#endif /* LOG_DSP_ENTER */


extern const intptr_t	tinib_exinf[];			/* �^�X�N�̊g����� */
extern const TASK    	tinib_task[];			/* �^�X�N�̋N���Ԓn */
extern const uint_t  	tinib_epriority[];		/* �^�X�N�̎��s���D��x��x�i�����\���j */



#define TOPPERS_tskini

#ifdef TOPPERS_tskini
/*-------------------------------------------
 * �f�B�X�p�b�`���[�̃R���e�L�X�g
 */
jmp_buf disp_ctx;		//�f�B�X�p�b�`���R���e�L�X�g

/*
 * ���s��ԃ^�X�N�̋N�����D��x
 */
uint_t runtsk_ipri;

/*
 *  ���f�B�L���[�T�[�`�̂��߂̃r�b�g�}�b�v
 */
volatile uint_t	ready_primap;

/*
 *  �^�X�N�f�B�X�p�b�`�N���v���t���O
 */
bool_t	reqflg;

/*
 *  �f�B�X�p�b�`�֎~���
 */
bool_t	disdsp;

/*
 * �O��@���stask
 */
intptr_t last_ipri;

/*
 *  read_primap�̏���l
 */
extern const uint_t init_rdypmap;

#endif /* TOPPERS_tskini */

#define TOPPERS_get_ipri_self
#ifdef TOPPERS_get_ipri_self

/*
 *  �^�X�NID����N�����D��x�����o�����߂̃}�N��
 */
uint_t
get_ipri_self(ID tskid)
{
	uint_t inipri;

	if(tskid != TSK_SELF)
	{
		inipri = (uint_t)((tskid) - TMIN_TSKID);
	}
	else
	{
		inipri = runtsk_ipri;
	}
	return inipri;
}

#endif /* TOPPERS_get_ipri_self */

#define TOPPERS_get_ipri
#ifdef TOPPERS_get_ipri

uint_t
get_ipri(ID tskid)
{
	return (uint_t)(tskid - TMIN_TSKID);
}

#endif /* TOPPERS_get_ipri */

/*
 *  �r�b�g�}�b�v�T�[�`�֐�
 *
 *  bitmap����1�̃r�b�g�̓��C�ł����ʁi�E�j�̂��̂��T�[�`���C���̃r�b
 *  �g�ԍ���Ԃ��D�r�b�g�ԍ��́C�ŉ��ʃr�b�g��0�Ƃ���Dbitmap��0���w��
 *  ���Ă͂Ȃ�Ȃ��D���̊֐��ł́Cbitmap��8�r�b�g�ł��邱�Ƃ����肵�C
 *  uint8_t�^�Ƃ��Ă���D
 *
 *  �r�b�g�T�[�`���߂����v���Z�b�T�ł́C�r�b�g�T�[�`���߂��g���悤��
 *  ��������������ǂ��ꍇ������D���̂悤�ȏꍇ�ɂ́C�^�[�Q�b�g
 *  �ˑ����Ńr�b�g�T�[�`���߂��g����bitmap_search���`���C
 *  OMIT_BITMAP_SEARCH���}�N����`����΂悢�D�܂��C�r�b�g�T�[�`���߂�
 *  �T�[�`���t�Ȃǂ̗��R�ŗD��x�ƃr�b�g�Ƃ̑Ή���ύX�������ꍇ��
 *  �́CPRIMAP_BIT���}�N����`����΂悢�D
 *
 *  �܂��C�W�����C�u������ffs������Ȃ�C���̂悤�ɒ�`���ĕW�����C�u
 *  �������g��������ǂ��\��������D
 *		#define	bitmap_search(bitmap) (ffs(bitmap) - 1)
 */
#ifndef PRIMAP_BIT
#define	PRIMAP_BIT(pri)		(1U << (pri))
#endif /* PRIMAP_BIT */


#ifndef OMIT_BITMAP_SEARCH




/*
 *  �����ݔ����񐔂�ۑ�����ϐ�
 */
volatile  uint16_t	intnest;


static const uint8_t bitmap_search_table[] = { 0U, 1U, 0U, 2U, 0U, 1U, 0U,
												3U, 0U, 1U, 0U, 2U, 0U, 1U, 0U };



#pragma inline (bitmap_search)
static  uint_t bitmap_search(uint_t bitmap)
{
	uint_t	n = 0U;

	if ((bitmap & 0x0fU) == 0U) {
		bitmap >>= 4U;
		n += 4U;
	}
	return (n + bitmap_search_table[(bitmap & 0x0fU) - 1U]);
}

#endif /* OMIT_BITMAP_SEARCH */

/*
 *  �D��x�r�b�g�}�b�v���󂩂̃`�F�b�N
 */
#pragma inline (primap_empty)
static bool_t primap_empty(void)
{
	return (ready_primap == 0U);
}

/*
 *  �w�肵���D��x�̗D��x�r�b�g�}�b�v���Z�b�g����Ă��邩�ǂ����̃`�F�b�N
 */
#pragma inline (primap_test)
static bool_t primap_test(uint_t pri)
{
	return ((ready_primap & PRIMAP_BIT(pri)) != 0U);
}

/*
 *  �D��x�r�b�g�}�b�v�̃T�[�`
 */
#pragma inline (primap_search)
static uint_t primap_search(void)
{
	return bitmap_search((uint_t)ready_primap);
}

/*
 *  �D��x�r�b�g�}�b�v�̃Z�b�g
 */
#pragma inline (primap_set)
static void primap_set(uint_t pri)
{
	ready_primap |= PRIMAP_BIT(pri);
}

/*
 *  �D��x�r�b�g�}�b�v�̃N���A
 */
#pragma inline (primap_clear)
static void primap_clear(uint_t pri)
{
	ready_primap &= ~PRIMAP_BIT(pri);
}

/*
 *  �ō��D�揇�ʃ^�X�N�̃T�[�`
 */
#define TOPPERS_tsksched

#ifdef TOPPERS_tsksched

uint_t
search_schedtsk(void)
{
	return primap_search();
}

#endif /* TOPPERS_tsksched */

/*
 * �w�肵���N�����D��x�̃^�X�N�����s�ł����Ԃ��ǂ����̃e�X�g
 *
 * ���s�ł���ꍇ��true��Ԃ��D
 */
bool_t
test_dormant(uint_t ipri)
{
	return !primap_test(ipri);
}

/*
 *  �^�X�N�Ǘ����W���[���̏���
 */

#define TOPPERS_tskini

#ifdef TOPPERS_tskini

void
initialize_task(void)
{
	intptr_t ipri;
	/* ���f�B�L���[�̃r�b�g�}�b�v���� */
	ready_primap = init_rdypmap;

	/* ���s���D��x�̏��� */
	runtsk_ipri = IPRI_NULL;

	last_ipri = 0xff;		//���肦�Ȃ��l�ɂ���

	/* �^�X�N�R���e�L�X�g�̐ݒ�*/
	for(ipri = 0;ipri <  TNUM_TSKID;ipri++)
	{
		if (primap_test(ipri))
		{
			make_ctx(ipri);
		}
		task_wait[ipri] = 0;
		tskTout[ipri] = 0;
	}
	/* �����݋֎~�t���O�̏��� */
	disdsp = false;
}

#endif /* TOPPERS_tskini */

/*-----------------------------------------------------
 *  �R���e�L�X�g�̏��������Ă���					takahashi
 */
static jmp_buf jmpp;		//���̃��[�`���̃Z�[�u�p

void make_ctx(uint_t ipri_prm)
{
	intptr_t debugtemp = 0;
	uint_t ipri = ipri_prm;
	intptr_t task_stackadr;
//	t_lock_cpu();
	if (setjmp(jmpp) == 0)	//�����ɖ߂�p
	{
		//����
		// �^�X�N�X�^�b�N�ɐ؂�ւ���
		task_stackadr = TOPPERS_TASKSTKPT(ipri);
		set_task_stack(task_stackadr);
#if 0
		__asm__( "mov sp,%[Rs1]"
				 ::[Rs1]"r"(task_stackadr));
		__asm__("mov   %[Rd],sp"
				:[Rd]"=r"(debugtemp));
		printf("stack data = %08x\n",debugtemp);
#endif
		if (setjmp(task_ctx[ipri]) == 0)
		{
			/*�o�^�����ꍇ*/
			longjmp(jmpp,1);	//�߂�
		}
		else
		{
			t_lock_cpu();
			ipri = runtsk_ipri;			//longjmp�Ŗ߂��ė������͕s��
			/* �^�X�N�N���� */
			ipl_maskClear();
			t_unlock_cpu();
			/* �^�X�N�ɗ��܂���*/
			/* �^�X�N���s�J�n */
			(*((TASK)(tinib_task[ipri])))(tinib_exinf[ipri]);
   			t_lock_cpu();
			ipri = runtsk_ipri;			//longjmp�Ŗ߂��ė������͕s��

			disdsp = false;
			/* �r�b�g�}�b�v�N���A�D */
			primap_clear(ipri);
			//�^�X�N���I������ꍇ�ǂ�����̂��H--> ���̂��Ƃ́@dispacher()�ɍs��
			longjmp(disp_ctx,1);			//sta_ker�̑����ɍs��
		}
	}
	else
	{
		//�o�^�I��
//		t_unlock_cpu();
	}
}
/*
 *  ipri : �N���Ώۃ^�X�N�̋N�����D��x(�����\��)
 */
#define TOPPERS_tskact
#ifdef TOPPERS_tskact

bool_t
make_active(uint_t ipri)
{
	bool_t dsp;

	primap_set(ipri);

	if(ipri < runtsk_ipri) {
		dsp = !disdsp;
	}
	else {
		dsp = false;
	}
	make_ctx(ipri);
	return dsp;
}

#endif /* TOPPERS_tskact */


/*
 *  apri : ���s�J�n�^�X�N�̋N�����D��x
 *  �Ăяo�������F CPU���b�N
 */
#define TOPPERS_tsk_dsp
#ifdef TOPPERS_tsk_dsp
void dispatch(intptr_t ipri)
{
	last_ipri = ipri;
	runtsk_ipri = ipri;
	longjmp(task_ctx[ipri],1);
}

#define TOPPERS_tskrun

#ifdef TOPPERS_tskrun

void
run_task(uint_t ipri)
{
	dispatch((intptr_t)ipri);
}

#endif /* TOPPERS_tskrun */

/*
 *  ���̊֐��͑S�����݃��b�N��ԂƓ����̏�Ԃ� sta_ker ����Ă΂��
 */


void
dispatcher(void)
{
	do {
		if(!primap_empty()) {
			/* �^�X�N�̊J�n */
			//run_task(search_schedtsk());
			dispatch(search_schedtsk());		//���ꂩ��͋A���Ă��Ȃ�
		}
		else {
			last_ipri = 0xff;
			idle_loop();
		}
	} while(true);
}
ER
wai_tsk(void)
{
	ER		ercd;
	uint_t	tskpri;

//	LOG_ACT_TSK_ENTER(tskid);
//	CHECK_TSKCTX_UNL();
//	CHECK_TSKID_SELF(tskid);

	tskpri = get_ipri_self(TSK_SELF);
	//tskpri = runtsk_ipri;
	t_lock_cpu();
	task_wait[tskpri] = 1;		//wait���
	primap_clear(tskpri);		//���f�BQ����폜

	//�����̃R���e�L�N�X�g��o�^
	if (setjmp(task_ctx[tskpri]) == 0)
	{
		/*�o�^�����ꍇ*/
		longjmp(disp_ctx,1);			//sta_ker�̑����ɍs��
	}
	else
	{
		// �^�X�N���A�����ꍇ
		ipl_maskClear();
		t_unlock_cpu();
		return(ercd);
	}

	t_unlock_cpu();


	return(ercd);
}

ER
go_tsk(ID tskid)
{
	ER		ercd;
	uint_t	self_tskpri;	//�Ăяo���^�X�N
	uint_t	ipri;			//go ����^�X�N


//	LOG_ACT_TSK_ENTER(tskid);
//	CHECK_TSKCTX_UNL();
//	CHECK_TSKID_SELF(tskid);

	ipri = get_ipri(tskid);
	self_tskpri = get_ipri_self(TSK_SELF);
	t_lock_cpu();
	if (task_wait[ipri] == 0)
	{
		t_unlock_cpu();
		return(E_OBJ);
	}

	task_wait[ipri] = 0;		//wait��ԉ���
	primap_set(ipri);		//���f�BQ�ǉ�



	//�����̃R���e�L�N�X�g��o�^
	if (setjmp(task_ctx[self_tskpri]) == 0)
	{
		/*�o�^�����ꍇ*/
		longjmp(disp_ctx,1);			//sta_ker�̑����ɍs��
	}
	else
	{
		// �^�X�N���A�����ꍇ
		ipl_maskClear();
		t_unlock_cpu();
		return(ercd);
	}

	t_unlock_cpu();


	return(ercd);
}

#endif /* TOPPERS_tsk_dsp */

void isig_tim()
{
	int tskid;

//	i_lock_cpu();

	for(tskid = 0 ; tskid < TNUM_TSKID; tskid++)
	{
		if (tskTout[tskid] != 0)
		{
			if (--tskTout[tskid] == 0)
			{
				task_wait[tskid] = 0;		//wait��ԉ���
				primap_set(tskid);		//���f�BQ�ǉ�
				reqflg = 1;
			}
		}
	}
//	i_unlock_cpu();
}

#define MAXTOUT 0xfffffffe

ER
dly_tsk(RELTIM dlytim)
{

	ER		ercd;
	uint_t	tskpri;

	if (dlytim > MAXTOUT)
	{
		return(E_PAR);
	}

//	LOG_ACT_TSK_ENTER(tskid);
//	CHECK_TSKCTX_UNL();
//	CHECK_TSKID_SELF(tskid);

	tskpri = get_ipri_self(TSK_SELF);
	//tskpri = runtsk_ipri;
	t_lock_cpu();
	task_wait[tskpri] = 1;		//wait���
	primap_clear(tskpri);		//���f�BQ����폜
	tskTout[tskpri] = dlytim+1;

	//�����̃R���e�L�N�X�g��o�^
	if (setjmp(task_ctx[tskpri]) == 0)
	{
		/*�o�^�����ꍇ*/
		longjmp(disp_ctx,1);			//sta_ker�̑����ɍs��
	}
	else
	{
		// �^�X�N���A�����ꍇ
		ipl_maskClear();
		t_unlock_cpu();
#if 0
		printf("dly_tsk End\n");
#endif
		return(ercd);
	}

	t_unlock_cpu();


	return(ercd);
}

void handler(INTHDR userhandler)
{
	volatile static intptr_t newtskipi;

	intnest++;						//���荞�݃l�X�g���C���N�������g
//	i_unlock_cpu();					//���荞�݋���
	(*userhandler)();				//���[�U�[�n���h���Ăяo��
//	i_lock_cpu();					//���荞�ݕs��
	intnest--;						//���荞�݃l�X�g���f�N�������g

	if (intnest == 0)				//���d���荞�ݒ��łȂ�
	{
		if (reqflg !=0)				//�X�P�W���[���K�v
		{
			reqflg = 0;
			newtskipi = search_schedtsk();			//���Ƀf�B�X�p�b�`�����^�X�NID
			if ((last_ipri != 0xff) && (last_ipri != newtskipi))
			{
				// RUN���ɍ��D��x�̃^�X�N�ɐ؂�ւ��ꍇ
				if (setjmp(task_ctx[last_ipri]) == 0)
				{
					// ���D��x�̃^�X�N�Ƀf�B�X�p�b�`
					dispatch(newtskipi);	//����̓��^�[�����Ȃ�
				}
			}
		}
	}
}
