#include "C_Types.h"
#include "I2C.h"
#include "McASP_LLD.h"
#include "McASP_HLD.h"

void I2S_Test_Terminal()
{
	UNS16 usSpeed[2] = {100, 400};
	UNS8 ucRegAddr = 0, ucRegData = 0;
	UNS8 IsExit = 0;

	DC4_I2C_init(I2C_CONTROLLER_1, 100, 0x25);

	DC4_McASP_EnableController(McASP_CONTROLLER_2);

	while(!IsExit)
	{
		Display_Str(" ");
		Display_Str(" 0: Play Sine Tone ");
		Display_Str(" 1: Record & PLayback");
		Display_Str(" 2: Read Device Id");
		Display_Str(" 3: Exit");
		Display_Str(" Enter your option: ");

		switch((ReadC() - 0x30))
		{
		case 0x00:
			TEST_audio(McASP_CONTROLLER_2);
			break;

		case 0x01:
			TEST_audio_loopback(McASP_CONTROLLER_2);
			break;

		case 0x02:
			break;
		case 0x03:
			IsExit = 1;
			break;
		default:
			Display_Str(" select proper option");
			break;
		}
	}

}


void I2S_Test()
{
#if 0
	UNS32 uiCntrlAddr = 0, uiBusFreq = 0;
	UNS16 usDataLen = 0, usSlaveAddr = 0;
	UNS8 ucSubCmd1 = 0, ucSubCmd2 = 0, *ucDataBuffer, ucDataPattern = 0;

	ucSubCmd1 = get_sub_cmd1(pPacket);
	uiCntrlAddr = uiCntrl_AddrTable[ucSubCmd1];

	ucSubCmd2 = get_sub_cmd2(pPacket);
	ucDataBuffer = get_data_location(pPacket);


	switch(ucSubCmd2)
	{
		case 0x01:
			uiBusFreq = *((UNS16*)&ucDataBuffer[0]);
			DC4_I2C_init(uiCntrlAddr, uiBusFreq, 0x25);
			break;

		case 0x02:
			usSlaveAddr = *((UNS16*)&ucDataBuffer[0]);
			usDataLen = *((UNS16*)&ucDataBuffer[2]);
			ucDataPattern = ucDataBuffer[4];
			ucDataBuffer = (UNS8*)malloc(sizeof(UNS8) * usDataLen);
			memset(ucDataBuffer, ucDataPattern, usDataLen);
			DC4_I2C_ReadrWrite(uiCntrlAddr, usSlaveAddr, ucDataBuffer, &usDataLen, (I2C_WRITE | I2C_START | I2C_STOP));
			free(ucDataBuffer);
			break;

		case 0x03:
			usSlaveAddr = *((UNS16*)&ucDataBuffer[0]);
			usDataLen = *((UNS16*)&ucDataBuffer[2]);
			DC4_I2C_ReadrWrite(uiCntrlAddr, usSlaveAddr, &ucDataBuffer[4], &usDataLen, (I2C_WRITE | I2C_START | I2C_STOP));
			break;

		case 0x04:
			usSlaveAddr = *((UNS16*)&ucDataBuffer[0]);
			usDataLen = *((UNS16*)&ucDataBuffer[2]);
			DC4_I2C_ReadrWrite(uiCntrlAddr, usSlaveAddr, &ucDataBuffer[4], &usDataLen, (I2C_WRITE | I2C_READ | I2C_STOP));
			break;
	}
#endif
}

