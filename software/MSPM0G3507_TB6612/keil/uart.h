#ifndef _UART_H_
#define _UART_H_

#include "ti_msp_dl_config.h"
#include "wit_imu.h"


void UART_Init(void);
void UART0_Send_Bytes(uint8_t *buf, int len);
void UART0_Text(void);



#endif
