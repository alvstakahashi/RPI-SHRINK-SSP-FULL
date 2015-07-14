#include "rpi.h"

void rpi_init(void){
	// bssのクリア
	clearBss();
	// システムタイマーを初期化
	init_syst();
	// GPIOをすべてINPUT_PULLUPに設定
//	init_gpio();


	// UARTを有効
	Serial_begin(115200);
}
