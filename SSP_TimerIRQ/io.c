/************************************************************************/
/*      GCC/LLCM ARM Baremetal Software / (C) Toshio MURAI  2012-2016   */
/*      This is Free Software.   You can redistribute this software     */
/*      and/or modify it ,under the terms of the GNU General Public     */
/*      License version 3, or (at your option) any later version.       */
/*      or under the terms of the BSD 3-Clause License.	(Dual Licence)  */
/************************************************************************/
#include "io.h"
void initializeJtagPin(){ 
	volatile static int wait=0;
	struct rpiGpio *rpiGpio = RPI_GPIO_BASE;

	// ALT4=3(011) ALT5=2(010)                                 P1 JTAG
	rpiGpio->GPFSEL2 |= (3 << (22-20)*3);	//06 GPIO22 (ALT4) 15-03TRST
	rpiGpio->GPFSEL0 |= (2 << (04-00)*3);	//12 GPIO04 (ALT5) 07-05TDI
	rpiGpio->GPFSEL2 |= (3 << (27-20)*3);	//21 GPIO27 (ALT4) 13-07TMS
	rpiGpio->GPFSEL2 |= (3 << (25-20)*3);	//15 GPIO25 (ALT4) 22-09TCK
	rpiGpio->GPFSEL2 |= (3 << (24-20)*3);	//12 GPIO24 (ALT4) 18-13TDO

}

