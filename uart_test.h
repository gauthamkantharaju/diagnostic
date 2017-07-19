#ifndef UART_TEST_H_
#define UART_TEST_H_

#include "ML_UARTdefineC.h"
#include "ML_PLLdefineC.h"
#include "ML_PLLstructC.h"
#include "C_Types.h"

typedef struct _data
{
	UNS32 size;
	UNS8 data;
}stUartData;

#define GPS_BAUDRATE 		0
#define UART_GPS_CONTROLLER 	1
#define GPS_MAX_DATA		1024

void UART_Test();
void UART_Test_Terminal();


#endif /* UART_TEST_H_ */
