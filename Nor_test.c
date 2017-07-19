/*
 * Nor_test.c
 *
 *  Created on: Mar 2, 2012
 *      Author: sailesh.myneni
 */

#include "C_Types.h"
#include "Gpmc_8148.h"
#include "ML_UARTdefineC.h"
#include"GPIO_8148.h"

#define A0_TARGET

void NOR_HardReset()
{
	NOR_Flash_PwrUp();
}

UNS16 HypTerWrite16(UNS16 Regval)
{
		UNS16 i,Temp;
		UNS16 Var;
		i=4;
		Temp= Regval;
		WriteByte(UART0_BASE_ADDRESS,'0');
		WriteByte(UART0_BASE_ADDRESS,'x');
		while(i--)
		{
			Var=Temp &(0xF000);
			Temp=Temp<<4;
			Var=Var>>12;
			if(Var<=0x09)
				Var+=0x30;
			else
				Var+=0x37;
			WriteByte(UART0_BASE_ADDRESS, Var);
		}
		return(Regval);
}

UNS16 HypTerWrite8(UNS8 Regval)
{
		UNS8 i,Temp;
		UNS8 Var;
		i=2;
		Temp= Regval;
		WriteByte(UART0_BASE_ADDRESS,'0');
		WriteByte(UART0_BASE_ADDRESS,'x');
		while(i--)
		{
			Var=Temp &(0xF0);
			Temp=Temp<<4;
			Var=Var>>4;
			if(Var<=0x09)
				Var+=0x30;
			else
				Var+=0x37;
			WriteByte(UART0_BASE_ADDRESS, Var);
		}
		return(Regval);
}

void NOR_Test_Terminal()
{

	UNS32 uiBlock_num=0;
	UNS32 uiPage_num=0;
	UNS32 uiOffset=0;
	UNS32 IsExit = 0;
	UNS16 ucBuffer[1024];
	UNS16 ucReadBuffer[1024];
	UNS16 UcReadData[1024];
	UNS16 ID[5];
	UNS16 uiCount=0;

#ifdef A0_TARGET
	/* Code for NOR Flash */
	/*To enable the GPIO functionality to operate at 3.3V instead of 1.8V.
	This is applicable for GPIO Bank 1 pins GP1[7], GP1[8], GP1[9] and GP1[10].
	Values are set as per TI suggestion.
	*/
	*pMLB_SIG_IO_CTRL |= 0x4;
	*pMLB_DAT_IO_CTRL |= 0x4;

/* Should be called after GPIO, PRCM configuration */	
	NOR_HardReset();
#endif


	while(!IsExit)
	{
		Display_Str(" ");
		Display_Str(" 0: Read the NOR Manufacture-ID & Device-ID ");
		Display_Str(" 1: NOR Read-Write");
		Display_Str(" 2: Exit");
		Display_Str(" Enter your option: ");

		switch((ReadC() - 0x30))
		{
		case 0x00:

			DC4_NORdevice_Info(ID);
			Display_Str("");
			Display_Str(" NOR Manufacture-ID is:");
			HypTerWrite8(ID[0]);

			Display_Str("");
			Display_Str(" NOR Device-ID1 is:");
			HypTerWrite16(ID[1]);

			Display_Str("");
			Display_Str(" NOR Device-ID2 is:");
			HypTerWrite16(ID[2]);

			Display_Str("");
			Display_Str(" NOR Device-ID2 is:");
			HypTerWrite16(ID[3]);

			Display_Str(" ");
			Display_Str(" NOR-Flash device is alive");

			break;

		case 0x01:

			for(uiCount=0; uiCount<1024; uiCount++)
			{
				ucBuffer[uiCount] = 0xABCD;
			}

			DC4_Nor_Write(uiBlock_num , uiPage_num ,uiOffset, ucBuffer);
			DC4_Nor_Read(uiBlock_num , uiPage_num ,uiOffset,ucReadBuffer);

			for(uiCount = 0;uiCount<1024 ; uiCount++)
				if(ucReadBuffer[uiCount] != ucBuffer[uiCount])
					break;

			if(uiCount == 1024)
			{
				Display_Str(" ");
				Display_Str(" NOR Read Write successfull ");
			}
			else
			{
				Display_Str(" ");
				Display_Str(" NOR Read Write unsuccessfull ");
			}
			break;


		case 0x02:

			IsExit = 1;
			break;

		default:
			Display_Str(" select proper option");
			break;
		}
	}

}



