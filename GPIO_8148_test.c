/*
 * GPIO_8148_test.c
 *
 *  Created on: Aug 24, 2011
 *      Author: sailesh.myneni
 */

#include"GPIO_8148.h"


void DC4_Delay(UNS32 uiDelay)
{
	uiDelay = uiDelay * 2000;
	while(uiDelay--);
}

void GPIO_Test_Terminal()
{
	UNS32 uiIndex,uitoggle=5, uiPinVal=0;
	UNS32 uiBaseAddr[4] = {GPIO0_BASE, GPIO1_BASE, GPIO2_BASE, GPIO3_BASE};
	UNS32 uipin_num[32] = {GPP0,GPP1,GPP2,GPP3,GPP4,GPP5,GPP6,GPP7,GPP8,
						   GPP9,GPP10,GPP11,GPP12,GPP13,GPP14,GPP15,GPP16,
						   GPP17,GPP18,GPP19,GPP20,GPP21,GPP22,GPP23,GPP24,
						   GPP25,GPP26,GPP27,GPP28,GPP29,GPP30,GPP31};
	UNS8 ucAddr=0, ucPinNo=0, ucState=0;
	UNS32 uiExit=0, uiStatus=0;

	while(!uiExit)
	{
		Display_Str(" ");
		Display_Str(" 0: GPIO pin configuration as Output");
		Display_Str(" 1: GPIO pin configuration as Input");
		Display_Str(" 2: Toggle GPIO pin");
		Display_Str(" 3: Read the value of a GPIO Input pin");
		Display_Str(" 4: Exit from GPIO menu");
		Display_Str(" Enter your option: ");

		switch((ReadC() - 0x30))
		{
		case 0x00:
			Display_Str(" Select GPIO Controller");
			Display_Str(" 0-GPIO, 1-GPIO1, 2-GPIO2, 3-GPIO3: ");
			ucAddr = (ReadC() - 0x30);

			Display_Str(" Select GPIO pin number[0 to 1F(31)]: ");
			ucPinNo = GetHexData();
			
			if((uiStatus = DC4_GPIO_ConfigDir(uiBaseAddr[ucAddr], uipin_num[ucPinNo], OUTPUT)))
			{
				printf("Please enter valid GPIO pin number \n");
				uiExit = 0;
			}
			else
			{	
				printf("\n Selected GPIO pin is configured as Output \n");
				Display_Str(" Select 1-Set, 0-Clear: ");
				ucState = (ReadC() - 0x30);
		              
				if(1 == ucState)
				{
					    DC4_GPIO_Set(uiBaseAddr[ucAddr],uipin_num[ucPinNo]);
					    printf("Selected GPIO pin:%d is SET \n", uipin_num[ucPinNo]);
				}
				else
				{
					    DC4_GPIO_Clear(uiBaseAddr[ucAddr],uipin_num[ucPinNo]);
	    				    printf("Selected GPIO pin:%d is CLEARED \n", uipin_num[ucPinNo]);	
				}
			}

			break;

		case 0x01:
			Display_Str(" Select GPIO Controller");
			Display_Str(" 0-GPIO, 1-GPIO1, 2-GPIO2, 3-GPIO3: ");
			ucAddr = (ReadC() - 0x30);
			Display_Str(" Select GPIO pin number[0 to 1F(31)]: ");
			ucPinNo = GetHexData();

			if((uiStatus = DC4_GPIO_ConfigDir(uiBaseAddr[ucAddr], uipin_num[ucPinNo], INPUT)))
			{
				printf("Please enter valid GPIO pin number \n");
				uiExit = 0;
			}
			else
				printf("Selected GPIO pin:%d is configured as Input \n", uipin_num[ucPinNo]);

			break;

		case 0x02:
			Display_Str(" Select GPIO Controller");
			Display_Str(" 0-GPIO, 1-GPIO1, 2-GPIO2, 3-GPIO3: ");
			ucAddr = (ReadC() - 0x30);

			Display_Str(" Select GPIO pin number[0 to 1F(31)]: ");
			ucPinNo = GetHexData();

			for(uiIndex=0;uiIndex<=uitoggle;uiIndex++)
			{
				DC4_GPIO_Set(uiBaseAddr[ucAddr], uipin_num[ucPinNo]);
				DC4_Delay(10);
				DC4_GPIO_Clear(uiBaseAddr[ucAddr], uipin_num[ucPinNo]);
				DC4_Delay(10);				
			}
			break;

		case 0x03:
				Display_Str(" Select GPIO Controller");
				Display_Str(" 0-GPIO, 1-GPIO1, 2-GPIO2, 3-GPIO3: ");
				ucAddr = (ReadC() - 0x30);

				Display_Str(" Select GPIO pin number[0 to 1F(31)]: ");
				ucPinNo = GetHexData();

				if((uiStatus = DC4_GPIO_ConfigDir(uiBaseAddr[ucAddr], uipin_num[ucPinNo], INPUT)))
				{
					printf("Please enter valid GPIO pin number \n");
					uiExit = 0;
				}
				else
				{
					DC4_GPIO_Get(uiBaseAddr[ucAddr], uipin_num[ucPinNo], &uiPinVal);
					printf("Value of the PinNo:%d = 0x%x \n", ucPinNo, (uiPinVal & (uipin_num[ucPinNo])));
				}
				break;

		case 0x04:	
				uiExit = 1;
				break;
		}
	}
}
