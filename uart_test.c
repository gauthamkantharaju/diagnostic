/* UART Test case functions */
#include "uart_test.h"
#include "common.h"

unsigned char ucBufferGPS[1024] = {0};

void UART_Test_Terminal()
{
	UNS32 uiUARTBaseAddr[6] = {UART0_BASE_ADDRESS, UART1_BASE_ADDRESS, UART2_BASE_ADDRESS,
				   UART3_BASE_ADDRESS, UART4_BASE_ADDRESS, UART5_BASE_ADDRESS};
	UNS32 uiDatasize[6] = {(4 * BYTES), (8 * BYTES), (16 * BYTES), (32 * BYTES), (64 * BYTES), 0x0};
	UNS32 uiGPSDatasize[5] = {(1 * MB), (2 * MB), (3 * MB), (4 * MB), (5 * MB)};
	UNS32 uiPattern[4] = {0xA5A5A5A5, 0x5A5A5A5A, 0x01234567, 0x0};
	UNS8 *ucTransBuffer, *ucRecvBuffer;
	UNS32 uiUARTController = 0, uiBaud = 0, ucSizeSelect = 0;
	UNS32 uiUARTBaud[5] = {9600, 19200, 38400, 57600, 115200};
	UNS8 ucPatterSelect=0;
//	FILE *fp;
	UNS8 IsExit = 0;
	UNS32 iIndex;


	while(!IsExit)
	{
		Display_Str(" ");
		Display_Str(" 0: UART Baudrate Configuration");
		Display_Str(" 1: Transmitting Data ");
		Display_Str(" 2: Receiving Data ");
		Display_Str(" 3: GPS interface test ");
		Display_Str(" 4: Exit from UART menu ");
		Display_Str(" Enter your option: ");

		switch((ReadC() - 0x30))
		{
		case 0x00:
			Display_Str(" Select UART Controller");
			Display_Str(" UART1 and UART0 baudrate is fixed.");			
			Display_Str(" Baudrate change for UART4 controller i.e. interfaced to VIOP");
			//uiUARTController = (ReadC() - 0x30);
			Display_Str(" Select UART4 baudrate ");
			Display_Str(" [0-9600, 1-19200, 2-38400, 3-57600, 4-115200]: ");
			uiBaud = (ReadC() - 0x30);
			DC4_UART_Config(uiUARTBaseAddr[4], uiUARTBaud[uiBaud]);

			break;

		case 0x01:
			Display_Str(" Select UART Controller");
			Display_Str(" [0-UART0, 4-UART4]: ");
			uiUARTController = (ReadC() - 0x30);
#if 0
			Display_Str(" Select Data Size[0-4B, 1-8B, 2-16B, 3-32B, 4-64B, 5-User Input]: ");
			ucSizeSelect = (ReadC() - 0x30);
			if(ucSizeSelect == 0x5)
				uiDatasize[ucSizeSelect] = GetHexData();

			Display_Str(" Select Pattern[0-0xA5A5A5A5, 1- 0x5A5A5A5A, 2-0x01234567, 3-User Input]: ");
			ucPatterSelect = (ReadC() - 0x30);
			if(ucPatterSelect == 3)
				uiPattern[ucPatterSelect] = GetHexData();

			ucTransBuffer = malloc(sizeof(char) * uiDatasize[ucSizeSelect]);
			memset(ucTransBuffer, uiPattern[ucPatterSelect], uiDatasize[ucSizeSelect]);
			free(ucTransBuffer);
#endif
			printf("\n");
			DC4_UART_Write(uiUARTBaseAddr[uiUARTController], "Diagnostic test software UART Test", 35);		

			break;

		case 0x02:
			Display_Str(" Select UART Controller");
			Display_Str(" [0-UART0, 4-UART4]: ");
			uiUARTController = (ReadC() - 0x30);
			Display_Str(" Select Data Size[0-4B, 1-8B, 2-16B, 3-32B, 4-64B, 5-User Input]: ");
			ucSizeSelect = (ReadC() - 0x30);
			if(ucSizeSelect == 0x5)
				uiDatasize[ucSizeSelect] = GetHexData();

			if(uiUARTBaseAddr[uiUARTController] == UART0_BASE_ADDRESS)
				printf(" \n Please enter the data that has to be received from UART0 controller: ");

			ucRecvBuffer = malloc(sizeof(char) * uiDatasize[ucSizeSelect]);
			DC4_UART_Read(uiUARTBaseAddr[uiUARTController], ucRecvBuffer, uiDatasize[ucSizeSelect]);

#if 0
			/* Logging the captured/Received data into a file */
			fp = fopen("Received.txt", "r");
			fwrite(ucRecvBuffer, 1, uiDatasize[ucSizeSelect], fp);
			fclose(fp);
#endif
			printf("\n");			
			for(iIndex=0; iIndex < uiDatasize[ucSizeSelect]; iIndex++)
				printf("%c ", ucRecvBuffer[iIndex]);

			printf("\n");

			free(ucRecvBuffer);
			break;

		case 0x03:
			Display_Str(" Please make sure that active GPS antenna is connected ");
			Display_Str(" Select Data Size[0-1MB, 1-MB, 2-3MB, 3-4MB, 4-5MB]: ");
			ucSizeSelect = (ReadC() - 0x30);

			/* Configuring UART1 for 9600*/
			DC4_UART_Config(uiUARTBaseAddr[UART_GPS_CONTROLLER], uiUARTBaud[GPS_BAUDRATE]);

			//ucRecvBuffer = malloc(sizeof(char) * uiGPSDatasize[ucSizeSelect]);

			DC4_UART_Read(uiUARTBaseAddr[UART_GPS_CONTROLLER], ucBufferGPS, GPS_MAX_DATA);

#if 0
			fp = fopen("GPS.txt", "w+");
			fwrite(ucBufferGPS, 1, 1024, fp);
			fclose(fp);
			free(ucRecvBuffer);
#endif
			for(iIndex=0; iIndex < GPS_MAX_DATA; iIndex++)
				printf("%c \t", ucBufferGPS[iIndex]);

			printf("\n");

			break;

		case 0x04:
			IsExit = 1;
			break;
		}
	}
}


