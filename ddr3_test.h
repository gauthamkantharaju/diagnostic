#ifndef DDR3_TEST_H_
#define DDR3_TEST_H_

#include "C_Types.h"
#include "ML_DDRdefineC.h"
#include "ML_PLLstructC.h"
#include "ML_PLLdefineC.h"

#define PLL_Config 	0x00
#define Pattern_RW 	0x01
#define UserData_RW 0x02
#define ADPLLJ_CLKCRTL_HS2       0x00000801 //HS2 Mode,TINTZ =1  --used by all PLL's except HDMI

#define OSC0 20
#define DATABUSLENGTH	 32

typedef struct data
{
	UNS8  bankAddr;
	UNS8  wordLength;
	UNS32 size;
	UNS8  pattern;
}stDDRData;

void DDR_Test_Terminal();
void DDR3_Test();
void DDR3_PLL_Config(UNS32 clk);
void DC4_DDR_Write_Data(unsigned int uiController_BaseAddr, unsigned int uiBank_Offset, unsigned int uiDataLen, unsigned char *uiPattern, unsigned char ucAccessLength);
unsigned int DC4_DDR_Read_Data(unsigned int uiController_BaseAddr, unsigned int uiBank_Offset, unsigned int uiDataLen, unsigned char *uiPattern, unsigned char ucAccessLength);
UNS32 HypTerWrite( UNS32 Memadd);

#endif /* DDR3_TEST_H_ */
