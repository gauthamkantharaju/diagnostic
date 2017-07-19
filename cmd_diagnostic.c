#include "ddr3_test.h"
#include "uart_test.h"
#include "C_Types.h"
#include "spi_test.h"
#include "I2C.h"
#include "GPIO_8148.h"
#include "Gpmc_8148.h"
#include <command.h>


void Display_Str(char *String);

#define PROJECT_STR 	"		DC4 Test Board S/W		 \n"
#define DDR_STR			" 1: DDR3 related test"
#define UART_STR		" 2: UART related test"
#define SPI_STR			" 3: SPI related test"
#define GPIO_STR		" 4: GPIO related test"
#define I2C_STR			" 5: I2C related test"
#define I2S_STR			" 6: I2S related test"
#define NOR_STR			" 7: NOR Flash related test"
#define EXIT_STR		" 8: To exit"
#define OPT_STR			" Enter your option: "


#define WR_MEM_32(addr, data) *((UNS32 *) addr) = (UNS32) data
  
UNS8 ucBuffer[1024] = {0};
UNS8 ucStrBuff[50] = " \r\n Hello This is EDMA Test";

UNS32 GetHexData()
{
	UNS8 ucSizeIndex = 8, ucData = 0;
	UNS32 DataLength = 0;

	Display_Str(" Value : ");
	WriteByte(UART0_BASE_ADDRESS, '0');
	WriteByte(UART0_BASE_ADDRESS, 'x');
	while(ucSizeIndex--)
	{
		ucData = ReadByte(UART0_BASE_ADDRESS);
		if(ucData == 0x0D)
			break;
		else if(ucData == 'A' || ucData == 'B' || ucData == 'C' || ucData == 'D' || ucData == 'E' || ucData == 'F')	
			ucData -= ('A' - 10);
		else if(ucData == 'a' || ucData == 'b' || ucData == 'c' || ucData == 'd' || ucData == 'e' || ucData == 'f')	 
			ucData -= ('a' - 10);
		else
			ucData -= 0x30;

		DataLength = (DataLength << 4) | ucData;
	}

	return DataLength;
}

UNS16 ReadC()
{
	UNS16 usData = ReadByte(UART0_BASE_ADDRESS);

	if(usData == 0x0D)
		return 0x30;

	return usData;
}

void Display_Str(char *String)
{
	WriteByte(UART0_BASE_ADDRESS, '\r');
	WriteByte(UART0_BASE_ADDRESS, '\n');
	while(*String++)
		WriteByte(UART0_BASE_ADDRESS, *String);
}

void Pinmux()
{
	//Code for UART PinMux
	*((VUNS32 *)0x48140914) = 0x01;//UART0 TX
	*((VUNS32 *)0x48140918) = 0x01;//UART0 RX

	*((VUNS32 *)0x4814092C) = 0x04;//UART1 TX
	*((VUNS32 *)0x48140930) = 0x04;//UART1 RX

	*((VUNS32 *)0x4814091C) = 0x02;//UART4 RX
	*((VUNS32 *)0x48140920) = 0x02;//UART4 TX

	//Code for GPIO PinMux
	*((VUNS32 *)0x4814093C) = 0x80;//GPIO GP1[6]
	*((VUNS32 *)0x4814095C) = 0x80;//GPIO GP1[26]

	//Code for SPI PinMux
	*((VUNS32 *)0x48140940) = 0x01;//SPI[0] CS0
	*((VUNS32 *)0x48140944) = 0x01;//SPI[0]_SCLK
	*((VUNS32 *)0x48140948) = 0x01;//SPI[0]_D0
	*((VUNS32 *)0x4814094C) = 0x01;//SPI[0]_D1

	//Code for I2C PinMux
#if 0
	*((VUNS32 *)0x48140C18) = 0x01;//I2C[0] SCL
	*((VUNS32 *)0x48140C1C) = 0x01;//I2C[0] SDA

	*((VUNS32 *)0x48140934) = 0x01;//I2C[1] SCL
	*((VUNS32 *)0x48140938) = 0x01;//I2C[1] SDA

	*((VUNS32 *)0x48140924) = 0x20;//I2C[2] SCL
	*((VUNS32 *)0x48140928) = 0x20;//I2C[2] SDA

	*((VUNS32 *)0x48140910) = 0x20;//I2C[3] SCL
	*((VUNS32 *)0x4814090C) = 0x20;//I2C[3] SDA
#endif
	//Code for McASP PinMux
	*((VUNS32 *)0x48140898) = 0x01;//MCA[2]_ACLKX
	*((VUNS32 *)0x4814089C) = 0x01;//MCA[2]_AFSX
	*((VUNS32 *)0x481408A0) = 0x01;//MCA[2]_AXR[0]
	*((VUNS32 *)0x481408A4) = 0x01;//MCA[2]_AXR[1]

/* code for NOR flash related */ 
	WR_MEM_32(PINCNTL64, 	0x00040080);  	/*GP1[9] NOR -Reset release GPIO pin for A0 board*/		    

	/* GPMC_A[0]_MUX0 */
	WR_MEM_32(PINCNTL243,    0x000400010);

	/* GPMC_A[1]_MUX0 */
	WR_MEM_32(PINCNTL244,    0x000400010);

	/* GPMC_A[2]_MUX0 */
	WR_MEM_32(PINCNTL245,    0x000400010);

	/* GPMC_A[3]_MUX0 */
	WR_MEM_32(PINCNTL246,    0x000400010);

	/* GPMC_A[4]_MUX0 */
	WR_MEM_32(PINCNTL247,    0x000400010);

	/* GPMC_A[5]_MUX0 */
	WR_MEM_32(PINCNTL248,    0x000400010);

	/* GPMC_A[6]_MUX0 */
	WR_MEM_32(PINCNTL249,    0x000400010);

	/* GPMC_A[7]_MUX0 */
	WR_MEM_32(PINCNTL250,    0x000400010);

	/* GPMC_A[8]_MUX0 */
	WR_MEM_32(PINCNTL251,    0x000400010);

	/* GPMC_A[9]_MUX0 */
	WR_MEM_32(PINCNTL252,    0x000400010);

	/* GPMC_A[10]_MUX0 */
	WR_MEM_32(PINCNTL253,    0x000400010);

	/* GPMC_A[11]_MUX0 */
	WR_MEM_32(PINCNTL254,    0x000400010);

	/* GPMC_A[12]_MUX0 */
	WR_MEM_32(PINCNTL255,    0x000400010);

	/* GPMC_A[13]_MUX0 */
	WR_MEM_32(PINCNTL256,    0x000400010);

	/* GPMC_A[14]_MUX0 */
	WR_MEM_32(PINCNTL257,    0x000400010);

	/* GPMC_A[15]_MUX0 */
	WR_MEM_32(PINCNTL258,    0x000400010);



	/* GPMC_A[16] */
	WR_MEM_32(PINCNTL105,    0x000500001);

	/* GPMC_A[17] */
	WR_MEM_32(PINCNTL106,    0x000500001);

	/* GPMC_A[18] */
	WR_MEM_32(PINCNTL107,    0x000500001);

	/* GPMC_A[19] */
	WR_MEM_32(PINCNTL108,    0x000500001);

	/* GPMC_A[20]_MUX0 */
	WR_MEM_32(PINCNTL109,    0x000500001);

	/* GPMC_A[21]_MUX0 */
	WR_MEM_32(PINCNTL110,    0x000500001);

	/* GPMC_A[22]_MUX0 */
	WR_MEM_32(PINCNTL111,    0x000500001);

	/* GPMC_A[23]_MUX0 */
	WR_MEM_32(PINCNTL112,    0x000500001);


	/* GPMC_A[24]*/
	WR_MEM_32(PINCNTL124,    0x000500002);

	/* GPMC_A[25] */
	WR_MEM_32(PINCNTL123,    0x000500002);


	/* GPMC_D[0] */
	WR_MEM_32(PINCNTL89,    0x000500001);

	/* GPMC_D[1] */
	WR_MEM_32(PINCNTL90,    0x000500001);

	/* GPMC_D[2] */
	WR_MEM_32(PINCNTL91,    0x000500001);

	/* GPMC_D[3] */
	WR_MEM_32(PINCNTL92,    0x000500001);

	/* GPMC_D[4] */
	WR_MEM_32(PINCNTL93,    0x000500001);

	/* GPMC_D[5] */
	WR_MEM_32(PINCNTL94,    0x000500001);

	/* GPMC_D[6] */
	WR_MEM_32(PINCNTL95,    0x000500001);

	/* GPMC_D[7] */
	WR_MEM_32(PINCNTL96,    0x000500001);

	/* GPMC_D[8] */
	WR_MEM_32(PINCNTL97,    0x000500001);

	/* GPMC_D[9] */
	WR_MEM_32(PINCNTL98,    0x000500001);

	/* GPMC_D[10] */
	WR_MEM_32(PINCNTL99,    0x000500001);

	/* GPMC_D[11] */
	WR_MEM_32(PINCNTL100,    0x000500001);

	/* GPMC_D[12] */
	WR_MEM_32(PINCNTL101,    0x000500001);

	/* GPMC_D[13] */
	WR_MEM_32(PINCNTL102,    0x000500001);

	/* GPMC_D[14] */
	WR_MEM_32(PINCNTL103,    0x000500001);

	/* GPMC_D[15] */
	WR_MEM_32(PINCNTL104,    0x000500001);



	/* GPMC_CS[0] */
	WR_MEM_32(PINCNTL122,    0x000500001);

	/* GPMC_CS[3] */
	WR_MEM_32(PINCNTL125,    0x000500001);

	/* GPMC_CS[4] */
	WR_MEM_32(PINCNTL126,    0x000500001);

	/* GPMC_CLK */
	WR_MEM_32(PINCNTL127,    0x000500001);

	/* GPMC_ADV_ALE */
	WR_MEM_32(PINCNTL128,    0x000500001);

	/* GPMC_OE_RE */
	WR_MEM_32(PINCNTL129,    0x000500001);

	/* GPMC_WE */
	WR_MEM_32(PINCNTL130,    0x000500001);

	/* GPMC_BE[0]_CLE */
	WR_MEM_32(PINCNTL131,    0x000500001);

	/* GPMC_BE[1] */
	WR_MEM_32(PINCNTL132,    0x000500001);

	/* GPMC_WAIT[0] */
	WR_MEM_32(PINCNTL133,    0x000500001);
}

int diagnostic()
{
	UNS32 iIndex=1;

	/* Enabling all the module PLL's and PRCM */
	/* to unlock the MMR and PLL Sub System registers*/
	DC4_PLL_MMR_UNLOCK();

	Pinmux();

	/* Power management for the PLLSS module */
	//*pCONTROl_SYSCONFIG = 0x2;

#if 0
	/* Other module related PLL configuration i.e. DSP, L3, DDR etc.. */
	DC4_PLL_PERIPHERALS_SOURCE_SEL(CLK_SRC);
	DC4_PLL_ARM(ARMPLL, 0, 30, 0); /*Values of N, M and M2 to achieve 600MHz*/
	DC4_PLL_Peripherals(DSPPLL, 0, 50, 2, ADPLLJ_CLKCRTL_HS2); /* Values of N, M and M2 to achieve 500MHz*/
	DC4_PLL_Peripherals(L3PLL, 0, 50, 5, ADPLLJ_CLKCRTL_HS2); /* Values of N, M and M2 to achieve 200MHz*/
	DC4_PLL_Peripherals(DDRPLL, 19, 666, 2, ADPLLJ_CLKCRTL_HS2); /* Values of N, M and M2 to achieve 333MHz*/
	DC4_PLL_Peripherals(USBPLL, 0, 240, 5, ADPLLJ_CLKCRTL_HS2 + ADPLLJ_CLKCRTL_CLKDCO); /* Values of N, M and M2 to achieve 960MHz*/


	DC4_PRCM_CM_CLKSEL(CM_CLKSEL);
	DC4_PRCM_CM_DEFAULT(CM_DEFAULT);
	DC4_PRCM_CM_ALWON(CM_ALWON);
#endif

	/* ALL module Initialization */
	DC4_UART_Config(UART0_BASE_ADDRESS, 115200);
	DC4_UART_Config(UART4_BASE_ADDRESS, 115200);
	//DC4_DDR3_Init();
	DC4_SPI_Init();
	DC4_I2C_EnableController();
	DC4_Gpmc_init(0);

	Config_InteriorImager();
	Config_ForwardImager();

	WriteByte(UART0_BASE_ADDRESS, 0x0C);
	Display_Str(PROJECT_STR);

	while(iIndex)
	{
		/* To clear the screen */
		WriteByte(UART0_BASE_ADDRESS, 0x0C);
		Display_Str(" ");
		Display_Str(DDR_STR);
		Display_Str(UART_STR);
		Display_Str(SPI_STR);
		Display_Str(GPIO_STR);
		Display_Str(I2C_STR);
		Display_Str(I2S_STR);
		Display_Str(NOR_STR);
		Display_Str(EXIT_STR);
		Display_Str(" 9: Clear Screen");
		Display_Str(OPT_STR);

		switch((ReadByte(UART0_BASE_ADDRESS) - 0x30))
		{
			case 1:
				DDR_Test_Terminal();
				break;
			case 2:
				UART_Test_Terminal();
				break;
			case 3:
				SPI_Test_Terminal();
				break;
			case 4:
				GPIO_Test_Terminal();
				break;
			case 5:
				I2C_Test_Terminal();
				break;
			case 6:
				I2S_Test_Terminal();
				break;

			case 7:
				NOR_Test_Terminal();
				break;
			case 8:
				Display_Str(" Exiting from Diagnostic test software");
				printf("\n");				
				iIndex = 0;
				break;
				
			case 0x09:
				WriteByte(UART0_BASE_ADDRESS, 0x0C);
				Display_Str(PROJECT_STR);
				break;
		}		
	}

	return 0;
}



U_BOOT_CMD(
	BigHill,	1,	0,	diagnostic,
	"test the diagnostic test cases of different periherals",
	"[args..]\n"
	"    - test the diagnostic test cases of different periherals; \\c suppresses newline"
);




