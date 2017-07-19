/* DDR3 Test case functions */
#include "ddr3_test.h"
#include "ML_UARTdefineC.h"
#include "C_Types.h"
#include "common.h"

void DDR_Test_Terminal()
{
	UNS32 uiController_Addr[2] = {DDR0_BASE_ADDR, DDR1_BASE_ADDR};
	UNS32 uiBank[9] = 	{0x0000C000, 0x01000000, 0x02000000,
						 0x03000000, 0x04000000, 0x05000000,
						 0x06000000, 0x07000000
						};
	UNS32 uiPLLMhz[4] = {(100 * MHz), (200 * MHz), (300 * MHz), (400 * MHz)};
	UNS8 ucBankSelect;
	UNS8 ucCntrlSelect;
	UNS8 ucPLL = 0;
	UNS8 ucPLLMulVal = 0;
	UNS8 IsExit = 0;
	UNS32 Index;
	VUNS32 * MemAddr;
	UNS32 MemDiff;
	UNS32 i=0, ErrLoc=0, Temp=0;


	while(!IsExit)
	{
		Display_Str(" ");
		Display_Str(" 0: PLL CONFIG");
		Display_Str(" 1:DATA BUS TEST");
		Display_Str(" 2:ADDRESS BUS TEST");
		Display_Str(" 3:DEVICE TEST");
		Display_Str(" 4: Exit from DDR menu");
		Display_Str(" Enter your option: ");

		switch((ReadC() - 0x30))
		{
		case 0x00:

			Display_Str(" Specify the PLL(in MHz) to be configured \n");
			Display_Str(" 1:100MHz 2:200MHz 3:300MHz 4:400MHz: ");
			ucPLL = (ReadC() - 0x30);

			/* PLL Calculation :- PLL = (OSC0/(N+1)) * M * 1/M2 */
			ucPLLMulVal = (uiPLLMhz[ucPLL - 1]/(10 * MHz));
			printf("PLL Val:%x \n", ucPLLMulVal);

			DC4_PLL_Peripherals(DDRPLL, 0, ucPLLMulVal, 2, ADPLLJ_CLKCRTL_HS2);
			Display_Str(" DDR PLL is configured !!!!!");

			break;


			/*DATA BUS TEST:
			 * DESCRIPTION: The test is to detect the shorted lines of the data bus
			 * Proceude: Tested the consequent 32 memory locations with the walking ones test
			 * 			which is obtained by writing and then reading the the locations
			 * 			of the consequent memory locations with 1 left shifted by the number of
			 * 			times as that of the number of the location from initial position*/

		case 0x01:

			Display_Str("DATA BUS TEST STARTED");
			Display_Str(" -----");
			Display_Str(" Select Controller[0,1]: ");
			ucCntrlSelect = (ReadC() - 0x30);

			for(Index=0;Index< DATABUSLENGTH ;Index++)
			{
				*((VUNS32 *)uiController_Addr[ucCntrlSelect] + Index) = (0x01 << Index) ;
			}

			for(Index=0; Index < DATABUSLENGTH; Index++)
			{
				if( *((VUNS32 *)uiController_Addr[ucCntrlSelect] + Index) != (0x01 << Index ))
				{
				Display_Str(" DATA BUS has problem at location :");
				HypTerWrite(uiController_Addr[ucCntrlSelect] + Index);
				/*Function for finding out shorted Lines*/
					for(i = 0; i < 32; i++)
					{
						 ErrLoc=uiController_Addr[ucCntrlSelect] +uiBank[ucBankSelect] +Index;
						if(ErrLoc && (0x01 << i) > 0 && i != Index)
						{
							Display_Str("The Data bus is shorted at line:");
							HypTerWrite(Index);
							Display_Str(" with line:");
							HypTerWrite(i);
						}
					}
				}
			}

			Display_Str(" -----");
			Display_Str(" DATABUS TEST -SUCCESSFUL");
			Display_Str("  ");
		break;

		case 0x02:
		{
			/* Description:Address bus test includes 1)Struck at high 2) Struck at low
			 * 1) write the walking 1s test to address bus for location 1,2,4, 8, 16,32...offsets from the
			 * base address write a pattern 0xAAAAAAAA. Then write 0x55555555 at location .
			 * If any address line is struck at 1 then the value writeen at the corresponding 2^ line number location */

			Display_Str(" ADDRESS BUS TEST"
					"						");
			Display_Str(" Select Controller[0,1]: ");
			ucCntrlSelect = (ReadC() - 0x30);

			Display_Str(" STRUCK AT 1 TEST-STARTED");
			Display_Str(" -----");



			for(Index=1;Index<12;Index++)
			{
				*((VUNS8 *)(uiController_Addr[ucCntrlSelect] | (0x01 << Index))) = 0xAA;
			}

			*((VUNS8 *)uiController_Addr[ucCntrlSelect]) = 0x55;

			for(Index = 1; Index < 12; Index++)
			{
				if( *((VUNS8 *)(uiController_Addr[ucCntrlSelect] | (0x01 << Index))) != 0xAA )
				{
					Display_Str(" ADDRESS BUS has Struck at 0 problem");
					HypTerWrite(uiController_Addr[ucCntrlSelect] | (0x01 << Index)); /*for reading the address in hyper terminal*/
				}

			}
			Display_Str(" STRUCT AT 1 TEST IS COMPLETED");
			Display_Str("  ");


			Display_Str(" NOW PERFORMING STRUCK AT 0 TEST");
			Display_Str(" -----");
			/*Test:		STRUCK AT 0s
			 * Description: 	1) Write the value 0xAAAAAAAA at base location
			 * 				2) Write the value 0x55555555 at locations 1,2,4,8,16,32....with walking 1s,
			 * 				3) Read back the locations 1,2,4,8,16,32
			 * 				4) if any of the address is read with 0xAAAAAAAA then the address bit corresponding to the
			 * 				address value is struck at 0 and is indicating the base location*/

			*((VUNS8 *)uiController_Addr[ucCntrlSelect]) = 0x55;
			for(Index=1;Index<12;Index++)
			{
				*((VUNS8 *)(uiController_Addr[ucCntrlSelect] | (0x01 << Index))) = 0xAA;
			}


				if (*((VUNS8 *)(uiController_Addr[ucCntrlSelect])) != 0x55)
				{
					Display_Str(" ADDRESS BUS has Struck at 0 problem Read the adress to locate the problem");
					HypTerWrite(MemAddr[(0x01 << Index)]); /*for reading the address in hyper terminal*/
				}
			Display_Str(" ADDRESSBUS STRUCK AT '0' TEST IS COMPLETE");
			Display_Str(" -----");
			Display_Str(" ADDRESS BUS TEST IS SUCCESSFUL");
			Display_Str("  ");
		}
		break;


		case 0x03:
			Display_Str(" DEVICE TEST");
			Display_Str(" -----");
			Display_Str(" Select Controller[0,1]: ");
			ucCntrlSelect = (ReadC() - 0x30);
			Display_Str(" Select Bank[0 to 7]: ");
			ucBankSelect = (ReadC() - 0x30);

			MemDiff = uiBank[ucBankSelect+1] - uiBank[ucBankSelect];


			MemAddr = ((VUNS32 *)(uiController_Addr[ucCntrlSelect] + uiBank[ucBankSelect]));
			Display_Str(" INCREMENT TEST FOR ALL THE LOCATIONS OF THE SELECTED BANK- STARTED");
			Display_Str("--------");
			Display_Str("--------");
			Display_Str(" WAIT PATIENTLY AND DONT TERMINATE AS THE TEST TAKES LONGER TIME");
			Display_Str("--------");
			for(Index = 0; Index < MemDiff; Index++)
			{
				MemAddr[Index] = Index ;
				if( MemAddr[Index] != Index )
					{
						Display_Str("Device test has problem at location :");
						HypTerWrite(uiController_Addr[ucCntrlSelect] + uiBank[ucBankSelect] +  Index);
					}
			}
			Display_Str(" INCREMENT TEST SUCCESSFUL");
			Display_Str(" INCREMENT TEST FOR ALL THE LOCATIONS OF THE SELECTED BANK- STARTED");
			Display_Str("--------");
			Display_Str(" WAIT PATIENTLY AND DONT TERMINATE AS THE TEST TAKES LONGER TIME");
			MemAddr= ((VUNS32 *)(uiController_Addr[ucCntrlSelect] + uiBank[ucBankSelect]));

						for(Index = 0; Index < MemDiff; Index++)
						{
							MemAddr[Index] = (MemDiff-Index) ;
							if( MemAddr[Index] != (MemDiff-Index) )
								{
									Display_Str("Device test has problem at location :");
									HypTerWrite(uiController_Addr[ucCntrlSelect] + uiBank[ucBankSelect] +  Index);
								}
						}
			Display_Str(" DECREMENT TEST IS SUCCESSFUL");
			Display_Str("--------");

			Display_Str(" DEVICE TEST IS SUCCESSFUL ");
		break;

		/*case 0x05:
		 * 			Display_Str("..........Pattern Test..................");
					Display_Str(" Select Controller[0,1]: ");
					ucCntrlSelect = (ReadC() - 0x30);
					Display_Str(" Select Bank[0 to 7]: ");
					ucBankSelect = (ReadC() - 0x30);
					Display_Str(" Select Word Length[0-8, 1-16, 2-32]: ");
					ucWordSelect = (ReadC() - 0x30);
					Display_Str(" Select Data Size[0-1MB, 1-4MB, 2-8MB, 3-16MB, 4-User Input]: ");
					ucSizeSelect = (ReadC() - 0x30);
					if(ucSizeSelect == 0x4)
						uiDatasize[ucSizeSelect] = GetHexData();

					Display_Str(" Select Pattern[0-0xA5A5A5A5, 1- 0x5A5A5A5A, 2-0x01234567, 3-User Input]: ");
					ucPatterSelect = (ReadC() - 0x30);
					if(ucPatterSelect == 3)
						uiPattern[ucPatterSelect] = GetHexData();

					DC4_DDR_Write(uiController_Addr[ucCntrlSelect], uiBank[ucBankSelect], uiDatasize[ucSizeSelect], uiPattern[ucPatterSelect], ucWordLength[ucWordSelect]);
					uiStatus = DC4_DDR_Read(uiController_Addr[ucCntrlSelect], uiBank[ucBankSelect], uiDatasize[ucSizeSelect], uiPattern[ucPatterSelect], ucWordLength[ucWordSelect]);

					if(0 == uiStatus)
						Display_Str(" DDR3 pattern read write successful");
					else
						Display_Str(" Read data from Bank unsuccessfully ");

		break;

			*/

		case 0x04:
			IsExit = 1;
		break;
		}
	}
}


	UNS32 HypTerWrite(UNS32 Memadd)
	{
		UNS32 i,Temp;
		UNS32 Var;
		i=8;
		Temp= Memadd;
		WriteByte(UART0_BASE_ADDRESS,'0');
		WriteByte(UART0_BASE_ADDRESS,'x');
		while(i--)
		{
			Var=Temp &(0xF0000000);
			Temp=Temp<<4;
			Var=Var>>28;
			if(Var<=0x09)
				Var+=0x30;
			else
				Var+=0x37;
			WriteByte(UART0_BASE_ADDRESS, Var);
		}
		return(Memadd);
	}

