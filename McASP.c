/************************************************************************************************************
 *
 * Title			: McASP.c
 *
 * Application		: Bighill project
 *
 * Author			: Mahantesh.Kumbar
 *
 * Creation Date    : 10/06/2011
 *
 * Description      : Functions to configure the McASP module
 *
 * Limitations      : Supported for Polling method only
 *
 *
 *----------------------------------------------------------------------------
 * Modification History
 *----------------------------------------------------------------------------
 * Ver 	Date       Initials       	Description
 *----------------------------------------------------------------------------
 * 0.1	10/06/2011 Mahantesh.Kumbar Initial Version
 *
 *
 *---------------------------------------------------------------------------
************************************************************************************************************/

/*------------------------------------------- Include files -----------------------------------------------*/
#include "C_Types.h"
#include "McASP_LLD.h"
#include "McASP_HLD.h"
/*-------------------------------- Symbolic Constants and Macros (defines) --------------------------------*/

/*------------------------------------------ Typedefs and structs -----------------------------------------*/


/*------------------------------------ Declarations (statics and globals) ---------------------------------*/


/*------------------------------------------ Function prototypes ------------------------------------------*/

/*---------------------------------------------- Functions ------------------------------------------------*/


/*********************************************************************************************
Function Name : DC4_McASP_EnableController
Description   :
Prototype     : void DC4_McASP_EnableController(UNS32 uiMcASP_Controller)
Parameters    : [I] uiMcASP_Controller - Base address of controller
Returns       : void
***********************************************************************************************/


void DC4_McASP_EnableController(UNS32 uiMcASP_Controller)
{
	//i2c Interface Clock
		*pL3_SLOW_CLKSTCTRL |= (0x01 << 1);	//CM_ALWON_L3_SLOW_CLKSTCTRL

		switch(uiMcASP_Controller)
		{
			case McASP_CONTROLLER_0:
				break;

			case McASP_CONTROLLER_1:
				break;

			case McASP_CONTROLLER_2:
				//PinMux
				*((VUNS32 *)0x48140898) = 0x01;//MCA[2]_ACLKX
				*((VUNS32 *)0x4814089C) = 0x01;//MCA[2]_AFSX
				*((VUNS32 *)0x481408A0) = 0x01;//MCA[2]_AXR[0]
				*((VUNS32 *)0x481408A4) = 0x01;//MCA[2]_AXR[1]

				*pCM_ALWON_McASP_2_CLKCTRL |= (0x01 << 1) ; //CM_ALWON_McASP_2_CLKCTRL
				while( (*pCM_ALWON_McASP_2_CLKCTRL & 0x02) != 0x2);

				*pCM_SYSCLK20_CLKSEL = 0x00;	//CM_SYSCLK20_CLKSEL, Selects the divider value for SYSCLK20 clock
				while( (*pCM_SYSCLK20_CLKSEL & 0x00) != 0x00);

				*pCM_AUDIOCLK_MCASP2_CLKSEL = 0x00;	//CM_AUDIOCLK_MCASP2_CLKSEL, SYSCLK20 is Selected
				while( (*pCM_AUDIOCLK_MCASP2_CLKSEL & 0x00) != 0x00);
				break;

			case McASP_CONTROLLER_3:
				break;
			case McASP_CONTROLLER_4:
				break;
			case McASP_CONTROLLER_5:
				break;
		}

		while((*pL3_SLOW_CLKSTCTRL & 0x00000900) != 0x00000900);

}

/*********************************************************************************************
Function Name : DC4_Stop_TX
Description   : This function Stops the Transmitter of McASP controller.
Prototype     : void DC4_Stop_TX(UNS32 uiMcASP_Controller)
Parameters    : [I] uiMcASP_Controller - Base address of controller
Returns       : void
***********************************************************************************************/
void DC4_Stop_TX(UNS32 uiMcASP_Controller)
{
	CSL_McASPRegs *McASP = (CSL_McASPRegs *)uiMcASP_Controller;	//pointer to the McASP controller module Registers

	McASP->McASP_XGBLCTL = 0;	//Reset/disable transmitter McASP
	McASP->McASP_XSTAT = 0x0000FFFF; //Clear(W1C)
	//McASP->McASP_XEVTCTL &= ~(TXDATADMADIS);//DMA Disable
	McASP->McASP_XINTCTL = 0x00;	//Interrupts Disabled
}

/*********************************************************************************************
Function Name : DC4_Start_TX
Description   : This function starts the Transmitter of McASP controller.
Prototype     : void DC4_Start_TX(UNS32 uiMcASP_Controller, UNS32 uiSerializer)
Parameters    : [I] uiMcASP_Controller - Base address of controller
				[I] uiSerializer - Serializer to be used transmit
Returns       : void
***********************************************************************************************/
void DC4_Start_TX(UNS32 uiMcASP_Controller, UNS32 uiSerializer)
{
	//UNS32 uiTimeout = 0;

	CSL_McASPRegs *McASP = (CSL_McASPRegs *)uiMcASP_Controller; //pointer to the McASP controller module Registers

	McASP->McASP_WFIFOCTL |= FIFO_ENABLE ;

	//Take the internal high-frequency serial clock divider(s) out of reset
	McASP->McASP_XGBLCTL |= TXHCLKRST;
	while(!(McASP->McASP_XGBLCTL & TXHCLKRST));

	//Take the internal serial clock divider(s) out of reset
	McASP->McASP_XGBLCTL |= TXCLKRST;
	while(!(McASP->McASP_XGBLCTL & TXCLKRST));

	//Activate serializers.
	McASP->McASP_XSTAT = 0x0000FFFF;        // Clear all
	McASP->McASP_XGBLCTL |= TXSERCLR;
	while(!(McASP->McASP_XGBLCTL & TXSERCLR));
	McASP->McASP_XBUF[uiSerializer] = 0;	//Reset Buffer

	//Release state machines from reset
	McASP->McASP_XGBLCTL |= TXSMRST;
	while(!(McASP->McASP_XGBLCTL & TXSMRST));

	//Release frame sync generators from reset
	McASP->McASP_XGBLCTL |= TXFSRST;
	while(!(McASP->McASP_XGBLCTL & TXFSRST));

	while(!(McASP->McASP_SRCTL[uiSerializer] & 0x0010));
	McASP->McASP_XBUF[uiSerializer] = 0;	//Reset Buffer

	//McASP->McASP_WFIFOCTL |= FIFO_ENABLE ;		// enable FIFO
}

/*********************************************************************************************
Function Name : DC4_Config_TX
Description   : This function configure the Transmitter of McASP controller.
Prototype     : void DC4_Start_TX(UNS32 uiMcASP_Controller, UNS32 uiSerializer)
Parameters    : [I] uiMcASP_Controller - Base address of controller
				[I] dev - Device structure which configuration data
Returns       : void
***********************************************************************************************/
UNS32 DC4_Config_TX(UNS32 uiMcASP_Controller, McASP_dev *dev)
{
	//UNS32 uiIndex = 0;
	UNS32 uiClkCTrl = 0, uiHClkCtrl = 0;

	UNS32 uiClkSpeed = (dev->uiSpeed * 1000);

	CSL_McASPRegs *McASP = (CSL_McASPRegs *)uiMcASP_Controller;//pointer to the McASP controller module Registers

	DC4_Stop_TX(uiMcASP_Controller);

	//Word size
	if(dev->uiBitorder == TXORD_MSB)
		McASP->McASP_XMASK = (0xFFFFFFFF << (32 - dev->uiWordSize));
	else
		McASP->McASP_XMASK = (0xFFFFFFFF >> (32 - dev->uiWordSize));

	//Bitstream is MSB first. Bit reversal is performed in transmit format bit reverse unit.
	McASP->McASP_XFMT = dev->uiBitorder | TXSSZ(dev->uiSlotSize) | TXROT(dev->uiRotate) | FSXDLY(dev->uiSlotDelay) | TXSEL;//TXORD;

	//Transmit frame sync mode select bits & Transmit frame sync width
	McASP->McASP_AFSXCTL = FSXMOD(dev->uiSlots) | FSXDUR | AFSXE;

	//Reset the Clock controller
	McASP->McASP_ACLKXCTL = McASP->McASP_AHCLKXCTL = 0;
	if(dev->uiClockSource)
	{
		//if Clock Speed is lesser than 49MHz, need to use CLKXDIV(1 to 32) of ACLKXCTL register
		if(uiClkSpeed < 49000)
		{
			//Maximum divide-down ratio from AHCLKX to ACLKX
			uiClkCTrl = 32;
			// (200MHz/32) = 6.250MHz/uiClkSpeed = HCLKXDIV value
			uiHClkCtrl = ((FUNC_CLOCK)/uiClkCTrl) / uiClkSpeed;
			uiClkCTrl -= 1; //AHCLKX value, 0 = 1 & 31 = 32.
		}
		else
		{
			//value to assign to the HCLKXDIV of AHCLKXCTL to get required speed
			uiHClkCtrl = (FUNC_CLOCK)/uiClkSpeed;
			//Clock Speed is greater than 49MHz so make CLKXDIV = 0,
			uiClkCTrl = 0;
			//AHCLKX is inverted before programmable bit clock divider,
			uiHClkCtrl |= AHCLKXPOL;
		}
		uiClkCTrl |= ACLKXE; //Internal transmit clock source from output of programmable bit clock divider
		uiHClkCtrl |= AHCLKXE; //Internal transmit high-frequency clock
	}

	//Asynchronous. Separate clock and frame sync used by transmit and receive sections
	McASP->McASP_ACLKXCTL |= TX_ASYNC | uiClkCTrl;

	//Internal transmit high-frequency clock source from output of programmable high clock divider
	McASP->McASP_AHCLKXCTL |= uiHClkCtrl;

	McASP->McASP_XTDM = (0xFFFFFFFF >> (32 - dev->uiSlots)) ; //Number of slots in a frame *****

	//Transmit clock maximum boundary(0xFF)& minimum boundary(0x00), Transmit clock check prescaler value(0x8=>McASP system clock divided by 256)
	McASP->McASP_XCLKCHK = 0x00FF0008;

	McASP->McASP_PFUNC = 0x00000000;	// All PINS as McASP

	/*for (uiIndex = 0; uiIndex < 5; uiIndex++)
	{
		//Check which Serializer as transmitter
		//if(dev->uiSerializer & (1 << uiIndex))
		if((dev->uiSerializer & uiIndex) == uiIndex)
		{
			McASP->McASP_SRCTL[uiIndex] = DISMOD(LOGIC_HIGH) | MODE(TRANSMITTER);//Configure Serializer as transmitter
			McASP->McASP_PDIR |= AXR(uiIndex); //Make Pin as output because it is transmitter
		}
	}*/

	McASP->McASP_SRCTL[dev->uiSerializer] = DISMOD(LOGIC_HIGH) | MODE(TRANSMITTER);//Configure Serializer as transmitter
	McASP->McASP_PDIR |= AXR(dev->uiSerializer); //Make Pin as output because it is transmitter

	if(dev->uiClockSource)
		//Make Clock & frame sync pin as output because internal clock is used.
		McASP->McASP_PDIR |= AHCLKX | ACLKX | AFSX;// | AXR(uiSerializer);
	else
		McASP->McASP_PDIR &= ~(AHCLKX | ACLKX | AFSX);// | AXR(uiSerializer);

	//Disable Loopback
	McASP->McASP_DITCTL = 0x00000000;
	McASP->McASP_DLBCTL = 0x00000000;
	McASP->McASP_AMUTE  = 0x00000000;

	//Start_TX(uiMcASP_Controller, uiSerializer);
	return 1;
}

/*********************************************************************************************
Function Name : DC4_McASP_Transmit
Description   : This function transmit the data on McASP controller.
Prototype     : void DC4_McASP_Transmit(UNS32 uiMcASP_Controller, McASP_dev *dev, void *buffer, UNS32 uiLength)
Parameters    : [I] uiMcASP_Controller - Base address of controller
				[I] dev - Device structure which holds configuration data
				[I] *buffer - Data buffer pointer which holds data
				[I] uiLength - Number of words to transmit
Returns       : void
***********************************************************************************************/
void DC4_McASP_Transmit(UNS32 uiMcASP_Controller, McASP_dev *dev, void *buffer, UNS32 uiLength)
{
	UNS32 uiIndex = 0;
	UNS32 bitpos = 0;
	UNS32 uiSerializer = dev->uiSerializer;

	CSL_McASPRegs *McASP = (CSL_McASPRegs *)uiMcASP_Controller;//pointer to the McASP controller module Registers
	if(dev->uiBitorder == TXORD_MSB)
		bitpos = (32 - dev->uiWordSize);

	if(dev->uiWordSize <= 8)	//For word size lesser than or equal to 8-bit
	{
		for(uiIndex = 0; uiIndex < uiLength; uiIndex++)
		{
			while(!(McASP->McASP_SRCTL[uiSerializer] & 0x0010)); //wait for transmit Free
			McASP->McASP_XBUF[uiSerializer] = (*(((UNS8 *)buffer) + uiIndex) << bitpos); //put data in Transmit Buffer of controller based on MSB or LSB
			while(!(McASP->McASP_XSTAT & 0x0020)); //wait for transmit complete
		}
	}
	else if(dev->uiWordSize <= 16)	//For word size lesser than or equal to 16-bit, but greater than 8-bit
	{
		for(uiIndex = 0; uiIndex < uiLength; uiIndex++)
		{
			while(!(McASP->McASP_SRCTL[uiSerializer] & 0x0010)); //wait for transmit Free
			McASP->McASP_XBUF[uiSerializer] = (*(((UNS16 *)buffer) + uiIndex) << bitpos);//put data in Transmit Buffer of controller based on MSB or LSB
			while(!(McASP->McASP_XSTAT & 0x0020)); //wait for transmit complete
		}
	}
	else	//For word size lesser than or equal to 32-bit, but greater than 16-bit
	{
		for(uiIndex = 0; uiIndex < uiLength; uiIndex++)
		{
			while(!(McASP->McASP_SRCTL[uiSerializer] & 0x0010)); //wait for transmit Free
			McASP->McASP_XBUF[uiSerializer] = (*(((UNS32 *)buffer) + uiIndex) << bitpos);//put data in Transmit Buffer of controller based on MSB or LSB
			while(!(McASP->McASP_XSTAT & 0x0020)); //wait for transmit complete
		}
	}
}

/*********************************************************************************************
Function Name : DC4_Stop_RX
Description   : This function Stops the Receiver of McASP controller.
Prototype     : void DC4_Stop_RX(UNS32 uiMcASP_Controller)
Parameters    : [I] uiMcASP_Controller - Base address of controller
Returns       : void
***********************************************************************************************/
void DC4_Stop_RX(UNS32 uiMcASP_Controller)
{
	CSL_McASPRegs *McASP = (CSL_McASPRegs *)uiMcASP_Controller;

	McASP->McASP_RGBLCTL = 0x0;	//Reset/disable Receiver McASP
	McASP->McASP_RSTAT = 0xFFFFFFFF;	//Clear the status(W1C)
	McASP->McASP_REVTCTL &= ~(RXDATADMADIS);//DMA Disable
}

/*********************************************************************************************
Function Name : DC4_Start_RX
Description   : This function starts the Receiver of McASP controller.
Prototype     : void DC4_Start_RX(UNS32 uiMcASP_Controller, UNS32 uiSerializer)
Parameters    : [I] uiMcASP_Controller - Base address of controller
Returns       : void
***********************************************************************************************/
void DC4_Start_RX(UNS32 uiMcASP_Controller, UNS32 uiSerializer)
{
	CSL_McASPRegs *McASP = (CSL_McASPRegs *)uiMcASP_Controller;

	McASP->McASP_RFIFOCTL |= FIFO_ENABLE ;      // enable FIFO

	//Take the internal high-frequency serial clock divider(s) out of reset
	McASP->McASP_RGBLCTL |= RXHCLKRST;
	while(!(McASP->McASP_RGBLCTL & RXHCLKRST));

	//Take the internal serial clock divider(s) out of reset
	McASP->McASP_RGBLCTL |= RXCLKRST;
	while(!(McASP->McASP_RGBLCTL & RXCLKRST));

	//Activate serializers.
	McASP->McASP_RSTAT = 0x0000FFFF;        // Clear all
	McASP->McASP_RGBLCTL |= RXSERCLR;
	while(!(McASP->McASP_RGBLCTL & RXSERCLR));
	//McASP->McASP_XBUF[uiSerializer] = 0;	//Reset Buffer

	//Release state machines from reset
	McASP->McASP_RGBLCTL |= RXSMRST;
	while(!(McASP->McASP_RGBLCTL & RXSMRST));

	//Release frame sync generators from reset
	McASP->McASP_RGBLCTL |= RXFSRST;
	while(!(McASP->McASP_RGBLCTL & RXFSRST));

	//while(!(McASP->McASP_SRCTL[uiSerializer] & 0x0020));
}

/*********************************************************************************************
Function Name : DC4_Config_RX
Description   : This function configure the Receiver of McASP controller.
Prototype     : UNS32 DC4_Config_RX(UNS32 uiMcASP_Controller, McASP_dev *dev)
Parameters    : [I] uiMcASP_Controller - Base address of controller
				[I] dev - Device structure which configuration data
Returns       : void
***********************************************************************************************/
UNS32 DC4_Config_RX(UNS32 uiMcASP_Controller, McASP_dev *dev)
{
	//UNS32 uiIndex = 0;
	CSL_McASPRegs *McASP = (CSL_McASPRegs *)uiMcASP_Controller;
	UNS32 uiClkCTrl = 0, uiHClkCtrl = 0;

	UNS32 uiClkSpeed = (dev->uiSpeed * 1000);

	//DC4_Stop_RX(uiMcASP_Controller);
	if(dev->uiBitorder == RXORD)
		McASP->McASP_RMASK = (0xFFFFFFFF << (32 - dev->uiWordSize));//Word size
	else
		McASP->McASP_RMASK = (0xFFFFFFFF >> (32 - dev->uiWordSize));//Word size

	//TXORD;//Bitstream is MSB first. Bit reversal is performed in transmit format bit reverse unit.
	McASP->McASP_RFMT =  dev->uiBitorder | RXSSZ(dev->uiSlotSize) | RXROT(dev->uiRotate) | FSRDLY(dev->uiSlotDelay);

	//receive frame sync mode select bits & Transmit frame sync width
	McASP->McASP_AFSRCTL = FSRMOD(dev->uiSlots) | FSRDUR | AFSRE;

	//Reset the Clock controller
	McASP->McASP_ACLKXCTL = McASP->McASP_AHCLKXCTL = 0;
	if(dev->uiClockSource)
	{
		//if Clock Speed is lesser than 49MHz, need to use CLKXDIV(1 to 32) of ACLKXCTL register
		if(uiClkSpeed < 49000)
		{
			//Maximum divide-down ratio from AHCLKX to ACLKX
			uiClkCTrl = 32;
			// (200MHz/32) = 6.250MHz/uiClkSpeed = HCLKXDIV value
			uiHClkCtrl = ((FUNC_CLOCK)/uiClkCTrl) / uiClkSpeed;
			uiClkCTrl -= 1;
		}
		else
		{
			//value to assign to the HCLKXDIV of AHCLKXCTL to get required speed
			uiHClkCtrl = (FUNC_CLOCK)/uiClkSpeed;
			//Clock Speed is greater than 49MHz so make CLKXDIV = 0,
			uiClkCTrl = 0;
			//AHCLKX is inverted before programmable bit clock divider,
			uiHClkCtrl |= AHCLKRPOL;
		}
		uiClkCTrl |= ACLKRE; //Internal Clock
		uiHClkCtrl |= AHCLKRE; //Internal Clock
	}

	McASP->McASP_ACLKRCTL |= uiClkCTrl; //Asynchronous. Separate clock and frame sync used by transmit and receive sections
	McASP->McASP_AHCLKRCTL |= uiHClkCtrl; //Internal receive high-frequency clock source from output of programmable high clock divider

	McASP->McASP_RTDM = (0xFFFFFFFF >> (32 - dev->uiSlots)) ; //Number of slots in a frame *****

	McASP->McASP_PFUNC = 0x00000000;	// All PINS as McASP
	//Check which Serializer as Receiver
	/*for (uiIndex = 0; uiIndex < 5; uiIndex++)
	{
		if(dev->uiSerializer & (1 << uiIndex))
		{
			McASP->McASP_SRCTL[uiIndex] |= DISMOD(LOGIC_HIGH) | MODE(RECEIVER);//Configure Serializer as Receiver
			McASP->McASP_PDIR &= ~(AXR(uiIndex));//Make Pin as input because it is Receiver
		}
	}*/

	McASP->McASP_SRCTL[dev->uiSerializer] = DISMOD(LOGIC_HIGH) | MODE(TRANSMITTER);//Configure Serializer as transmitter
	McASP->McASP_PDIR |= AXR(dev->uiSerializer); //Make Pin as output because it is transmitter

	if(dev->uiClockSource)
		//Make Clock & frame sync pin as output because internal clock is used.
		McASP->McASP_PDIR |= AHCLKR | ACLKR | AFSR;
	else
		McASP->McASP_PDIR &= ~(AHCLKR | ACLKR | AFSR);// | AXR(uiSerializer);

	//Disable Loopback
	McASP->McASP_DITCTL = 0x00000000;
	McASP->McASP_DLBCTL = 0x00000000;
	McASP->McASP_AMUTE  = 0x00000000;

	//Start_RX(uiMcASP_Controller, uiSerializer);
	return 0;
}

/*********************************************************************************************
Function Name : DC4_McASP_Receive
Description   : This function transmit the data on McASP controller.
Prototype     : UNS32 DC4_Config_RX(UNS32 uiMcASP_Controller, McASP_dev *dev)
Parameters    : [I] uiMcASP_Controller - Base address of controller
				[I] dev - Device structure which configuration data
				[I] *buffer - Data buffer pointer which holds data
				[I] uiLength - Number of words to transmit
Returns       : void
***********************************************************************************************/
void DC4_McASP_Receive(UNS32 uiMcASP_Controller, McASP_dev *dev, void *buffer, UNS32 uiLength)
{
	UNS32 uiIndex = 0;
		UNS32 bitpos = 0;
		UNS32 uiSerializer = dev->uiSerializer;

		CSL_McASPRegs *McASP = (CSL_McASPRegs *)uiMcASP_Controller;//pointer to the McASP controller module Registers
		if(dev->uiBitorder == TXORD_MSB)
			bitpos = (32 - dev->uiWordSize);

		if(dev->uiWordSize <= 8)	//For word size lesser than or equal to 8-bit
		{
			for(uiIndex = 0; uiIndex < uiLength; uiIndex++)
			{
				while(!(McASP->McASP_SRCTL[uiSerializer] & 0x0020)); //wait for transmit Free
				*(((UNS8 *)buffer) + uiIndex) = McASP->McASP_RBUF[uiSerializer]; //put data in Transmit Buffer of controller based on MSB or LSB
				while(!(McASP->McASP_RSTAT & 0x0020)); //wait for transmit complete
			}
		}
		else if(dev->uiWordSize <= 16)	//For word size lesser than or equal to 16-bit, but greater than 8-bit
		{
			for(uiIndex = 0; uiIndex < uiLength; uiIndex++)
			{
				while(!(McASP->McASP_SRCTL[uiSerializer] & 0x0020)); //wait for transmit Free
				*(((UNS16 *)buffer) + uiIndex) = McASP->McASP_RBUF[uiSerializer];//put data in Transmit Buffer of controller based on MSB or LSB
				while(!(McASP->McASP_RSTAT & 0x0020)); //wait for transmit complete
			}
		}
		else	//For word size lesser than or equal to 32-bit, but greater than 16-bit
		{
			for(uiIndex = 0; uiIndex < uiLength; uiIndex++)
			{
				while(!(McASP->McASP_SRCTL[uiSerializer] & 0x0020)); //wait for transmit Free
				*(((UNS32 *)buffer) + uiIndex) = McASP->McASP_RBUF[uiSerializer];//put data in Transmit Buffer of controller based on MSB or LSB
				while(!(McASP->McASP_RSTAT & 0x0020)); //wait for transmit complete
			}
		}
}

void DC4_McASP_Loopback(UNS32 uiMcASP_Controller)
{
	CSL_McASPRegs *McASP = (CSL_McASPRegs *)uiMcASP_Controller;

	McASP->McASP_AFSRCTL &= ~(AFSRE);
	McASP->McASP_PDIR &= ~(AHCLKR | ACLKR | AFSR);

	McASP->McASP_DLBCTL = LBEN | LBGENMODE(0x01) | LBORD;
	McASP->McASP_ACLKXCTL &= ~(TX_ASYNC);

}
//0549F11166000035
