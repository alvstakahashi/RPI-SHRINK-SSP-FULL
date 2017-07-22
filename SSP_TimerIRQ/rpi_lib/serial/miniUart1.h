//-------------------------------------------------------------------------
//  miniUart1 driver  extern definition miniUart1.h
//  raspberry pi3 ��p�@UART�h���C�o�@
//  rpi_lib��serial�h���C�o�C���^�[�t�F�[�X�̉��ʑw�h���C�o
//-------------------------------------------------------------------------
#ifndef __H_MINIUART1__
#define __H_MINIUART1__
extern unsigned int uart_lcr ( void );
extern unsigned int uart_recv ( void );
extern unsigned int uart_check ( void );
extern void uart_send ( unsigned int c );
extern void uart_flush ( void );
extern void uart_init ( void );
extern void hexstrings ( unsigned int d );
extern void hexstring ( unsigned int d );
#endif
