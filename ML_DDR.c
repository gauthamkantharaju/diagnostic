/* Title: ML_DDR.c
* Application: DDR Configuration
* Copyright: HCL Technologies
* Author: Gautham.K
* Creation Date: 27/5/2011
* Requirements: ML_defineC.h
* References: 
* Description: Main C file where all the DMM, EMIF and PRCM register are configured. 
* Limitations:
* Dependencies: TMS320DM8148 compiler
* Modification: Initial Version */

#include "ML_DDRdefineC.h"

#if 0
/*
* Name: DC4_DDR2_Init
* Description: This function is used to configure all DDR related control
* 			   register.
* Parameters: None
* Returns: None
* Globals Updated: None
* Tables Updated: None
*
*/
void DC4_DDR2_Init()
{
	unsigned int iVTP0_CTRL_REG=0, iVTP1_CTRL_REG=0;

	/* PRCM related configuration registers */
	*pCM_DEFAULT_L3_FAST_CLKSTCTRL = 0x2;
	*pCM_DEFAULT_FW_CLKCTRL = 0x2;
	*pCM_DEFAULT_DDR_0_CLKCTRL = 0x2;
	*pCM_DEFAULT_DDR_1_CLKCTRL = 0x2;
	*pCM_DEFAULT_DMM_CLKCTRL = 0x2;

	/* Write 1 to Enable bit */
	iVTP0_CTRL_REG = *pVTP0_CTRL_REG;
	iVTP0_CTRL_REG |= 0x00000040;
	*pVTP0_CTRL_REG = iVTP0_CTRL_REG;

	iVTP1_CTRL_REG = *pVTP1_CTRL_REG;
	iVTP1_CTRL_REG |= 0x00000040;
	*pVTP1_CTRL_REG = iVTP1_CTRL_REG;

	/* Write 0 to CLRZ bit */
	iVTP0_CTRL_REG = *pVTP0_CTRL_REG;
	iVTP0_CTRL_REG &= 0xfffffffe;
	*pVTP0_CTRL_REG = iVTP0_CTRL_REG;

	iVTP1_CTRL_REG = *pVTP1_CTRL_REG;
	iVTP1_CTRL_REG &= 0xfffffffe;
	*pVTP1_CTRL_REG = iVTP1_CTRL_REG;

	/* Write 1 to CLRZ bit */
	iVTP0_CTRL_REG = *pVTP0_CTRL_REG;
	iVTP0_CTRL_REG |= 0x00000001;
	*pVTP0_CTRL_REG = iVTP0_CTRL_REG;

	iVTP1_CTRL_REG = *pVTP1_CTRL_REG;
	iVTP1_CTRL_REG |= 0x00000001;
	*pVTP1_CTRL_REG = iVTP1_CTRL_REG;

	/* Check for READY bit */
	while((*pVTP0_CTRL_REG & 0x20) != 0x20);

	iVTP1_CTRL_REG = *pVTP1_CTRL_REG;
	while((*pVTP1_CTRL_REG & 0x20) != 0x20);

	/* Go to idle mode */
	*pDMM_SYSCONFIG = 0x0;

	while((*pDMM_SYSCONFIG != 0x0));

	/* DDR0 controller is connected to 256MB DDR
	 * with no interleaving i.e. from
	 * 0x80000000 - 0x8FFFFFFF and DDR1 from
	 * 0x90000000 - 0x9FFFFFFF accordingly */
	*pDMM_LISA_MAP_0 = 0x80400100;
	*pDMM_LISA_MAP_2 = 0x90400210;

	/* Come back to Non IDLE mode */
	*pDMM_SYSCONFIG = 0x8;

	while((*pDMM_SYSCONFIG != 0x8));

	/* Reading back DMM registers */
	while(*pDMM_LISA_MAP_0 != 0x80400100);
	while(*pDMM_LISA_MAP_2 != 0x90400210);

	/* DMM related configuration registers */
	*pDMM_PAT_BASE_ADDR = 0x80000000;

	/* To lock the LISA registers after
	 * configuring */
	//*pDMM_LISA_LOCK = 0x1;

	/* DDR0 related configuration registers */
	*pDDR_0_DDR_PHY_CTRL_1 = 0x7;  //RL = 7
	*pDDR_0_DDR_PHY_CTRL_1_SHADOW = 0x7;
	*pDDR_0_SDRAM_TIM_1 = 0x0AAAF552;
	*pDDR_0_SDRAM_TIM_1_SHADOW = 0x0AAAF552;
	*pDDR_0_SDRAM_TIM_2 = 0x043631D2;
	*pDDR_0_SDRAM_TIM_2_SHADOW = 0x043631D2;
	*pDDR_0_SDRAM_TIM_3 = 0x00000327;
	*pDDR_0_SDRAM_TIM_3_SHADOW = 0x00000327;
	*pDDR_0_SDRAM_REF_CTRL = 0x10000C30;
	*pDDR_0_SDRAM_REF_CTRL_SHADOW = 0x10000C30;
	*pDDR_0_SDRAM_CONFIG = 0x40801AB2;  //CL = 6 and CWL =

	 /* DDR1 related configuration registers */
	*pDDR_1_DDR_PHY_CTRL_1 = 0x7;  //RL = 7
	*pDDR_1_DDR_PHY_CTRL_1_SHADOW = 0x7;
	*pDDR_1_SDRAM_TIM_1 = 0x0AAAF552;
	*pDDR_1_SDRAM_TIM_1_SHADOW = 0x0AAAF552;
	*pDDR_1_SDRAM_TIM_2 = 0x043631D2;
	*pDDR_1_SDRAM_TIM_2_SHADOW = 0x043631D2;
	*pDDR_1_SDRAM_TIM_3 = 0x00000327;
	*pDDR_1_SDRAM_TIM_3_SHADOW = 0x00000327;
	*pDDR_1_SDRAM_REF_CTRL = 0x10000C30;
	*pDDR_1_SDRAM_REF_CTRL_SHADOW = 0x10000C30;
	*pDDR_1_SDRAM_CONFIG = 0x40801AB2;  //CL = 6 and CWL =
}
#endif

/*
* Name: DC4_DDR3_Init
* Description: This function is used to configure all DDR related control
* 			   register.
* Parameters: None
* Returns: None
* Globals Updated: None
* Tables Updated: None
*
*/
void DC4_DDR3_Init()
{
	unsigned int iVTP0_CTRL_REG=0, iVTP1_CTRL_REG=0;

	/* PRCM related configuration registers */
 	*pCM_DEFAULT_L3_FAST_CLKSTCTRL = 0x2;
 	*pCM_DEFAULT_FW_CLKCTRL = 0x2;
 	*pCM_DEFAULT_DDR_0_CLKCTRL = 0x2;
 	*pCM_DEFAULT_DDR_1_CLKCTRL = 0x2;
 	*pCM_DEFAULT_DMM_CLKCTRL = 0x2;

 	/* Write 1 to Enable bit */
 	iVTP0_CTRL_REG = *pVTP0_CTRL_REG;
 	iVTP0_CTRL_REG |= 0x00000040;
 	*pVTP0_CTRL_REG = iVTP0_CTRL_REG;

 	iVTP1_CTRL_REG = *pVTP1_CTRL_REG;
	iVTP1_CTRL_REG |= 0x00000040;
	*pVTP1_CTRL_REG = iVTP1_CTRL_REG;

 	/* Write 0 to CLRZ bit */
	iVTP0_CTRL_REG = *pVTP0_CTRL_REG;
	iVTP0_CTRL_REG &= 0xfffffffe;
	*pVTP0_CTRL_REG = iVTP0_CTRL_REG;

	iVTP1_CTRL_REG = *pVTP1_CTRL_REG;
	iVTP1_CTRL_REG &= 0xfffffffe;
	*pVTP1_CTRL_REG = iVTP1_CTRL_REG;

 	/* Write 1 to CLRZ bit */
	iVTP0_CTRL_REG = *pVTP0_CTRL_REG;
	iVTP0_CTRL_REG |= 0x00000001;
	*pVTP0_CTRL_REG = iVTP0_CTRL_REG;

	iVTP1_CTRL_REG = *pVTP1_CTRL_REG;
	iVTP1_CTRL_REG |= 0x00000001;
	*pVTP1_CTRL_REG = iVTP1_CTRL_REG;

	/* Check for READY bit */
	iVTP0_CTRL_REG = *pVTP0_CTRL_REG;
	while((*pVTP0_CTRL_REG & 0x20) != 0x20);

	iVTP1_CTRL_REG = *pVTP1_CTRL_REG;
	while((*pVTP1_CTRL_REG & 0x20) != 0x20);

	/* Go to idle mode */
		*pDMM_SYSCONFIG = 0x0;

		while((*pDMM_SYSCONFIG != 0x0));

		/* DDR0 controller is connected to 256MB DDR
		 * with no interleaving i.e. from
		 * 0x80000000 - 0x8FFFFFFF and DDR1 from
		 * 0x90000000 - 0x9FFFFFFF accordingly */
		*pDMM_LISA_MAP_0 = 0x80400100;
		*pDMM_LISA_MAP_2 = 0x90400210;

		/* Come back to Non IDLE mode */
		*pDMM_SYSCONFIG = 0x8;

		while((*pDMM_SYSCONFIG != 0x8));

		/* Reading back DMM registers */
		while(*pDMM_LISA_MAP_0 != 0x80400100);
		while(*pDMM_LISA_MAP_2 != 0x90400210);

	 	/* DMM related configuration registers */
	 	*pDMM_PAT_BASE_ADDR = 0x80000000;

	 	/* To lock the LISA registers after
	 	 * configuring */
	 	//*pDMM_LISA_LOCK = 0x1;

	 	/* DDR0 related configuration registers */
		*pDDR_0_DDR_PHY_CTRL_1 = 0xA;
		*pDDR_0_DDR_PHY_CTRL_1_SHADOW = 0xA;
		*pDDR_0_SDRAM_TIM_1 = 0x1113781C;
		*pDDR_0_SDRAM_TIM_1_SHADOW = 0x1113781C;
		*pDDR_0_SDRAM_TIM_2 = 0x384F31E2;
		*pDDR_0_SDRAM_TIM_2_SHADOW = 0x384F31E2;
		*pDDR_0_SDRAM_TIM_3 = 0x00000498;
		*pDDR_0_SDRAM_TIM_3_SHADOW = 0x00000498;
		*pDDR_0_SDRAM_REF_CTRL = 0x1000144A;
		*pDDR_0_SDRAM_REF_CTRL_SHADOW = 0x1000144A;
		*pDDR_0_SDRAM_CONFIG = 0x63C22A32; /*CL = 9(prog 10),CWL = 7(prog 2) row size= 13 */

		/* DDR1 related configuration registers */
		*pDDR_1_DDR_PHY_CTRL_1 = 0xA;
		*pDDR_1_DDR_PHY_CTRL_1_SHADOW = 0xA;
		*pDDR_1_SDRAM_TIM_1 = 0x1113781C;
		*pDDR_1_SDRAM_TIM_1_SHADOW = 0x1113781C;
		*pDDR_1_SDRAM_TIM_2 = 0x384F31E2;
		*pDDR_1_SDRAM_TIM_2_SHADOW = 0x384F31E2;
		*pDDR_1_SDRAM_TIM_3 = 0x00000498;
		*pDDR_1_SDRAM_TIM_3_SHADOW = 0x00000498;
		*pDDR_1_SDRAM_REF_CTRL = 0x1000144A;
		*pDDR_1_SDRAM_REF_CTRL_SHADOW = 0x1000144A;
		*pDDR_1_SDRAM_CONFIG = 0x63C22A32; /*CL = 9(prog 10),CWL = 7(prog 2) row size= 13 */
}

/*
* Name: DC4_DDR_Write
* Description: This function is used to write to different DDR banks0...7
*              i.e. can be through DDR0 or DDR1 controller
* Parameters:  uiController_BaseAddr: Base address of either DDR0 or DDR1 controller
*              uiBank_Offset: Bank selection address i.e. each bank is of 16MB.
*              uiDataLen: No of bytes/short/word to be written to DDR
*              uiPattern: Pattern written to DDR i.e. either 0xA5A5A5A5 or 0x5A5A5A5A
*              ucAccessLength: Byte/Short/Word accessed at a time.
* Returns: None
* Globals Updated: None
* Tables Updated: None
*
*/
void DC4_DDR_Write(unsigned int uiController_BaseAddr, unsigned int uiBank_Offset, unsigned int uiDataLen, unsigned int uiPattern, unsigned char ucAccessLength)
{
	unsigned int uiIndex = 0;
	unsigned int pBank_Addr = (uiController_BaseAddr + uiBank_Offset);

	printf("Addr: %u \t ControllerVal: %u \t Bank: %u\n", ((unsigned int *)pBank_Addr), uiController_BaseAddr, uiBank_Offset);

	if(0 == uiPattern)
	{
		if(ucAccessLength == BYTE)
			for(uiIndex=0; uiIndex < uiDataLen; uiIndex+=1)
				*((volatile unsigned char *)pBank_Addr + uiIndex) = uiIndex;

		else if(ucAccessLength == SHORT)
			for(uiIndex=0; uiIndex < uiDataLen; uiIndex+=2)
				*((volatile unsigned short *)pBank_Addr + uiIndex) = uiIndex;

		else
			for(uiIndex=0; uiIndex < uiDataLen; uiIndex+=4)
				*((volatile unsigned int *)(pBank_Addr + uiIndex)) = uiIndex;
	}
	else
	{
		if(ucAccessLength == BYTE)
			for(uiIndex=0; uiIndex < uiDataLen; uiIndex+=1)
				*((volatile unsigned char *)pBank_Addr + uiIndex) = uiPattern;

		else if(ucAccessLength == SHORT)
			for(uiIndex=0; uiIndex < uiDataLen; uiIndex+=2)
				*((volatile unsigned short *)pBank_Addr + uiIndex) = uiPattern;

		else
			for(uiIndex=0; uiIndex < uiDataLen; uiIndex+=4)
			*((volatile unsigned int *)(pBank_Addr + uiIndex)) = uiPattern;
	}
}
 
/*
* Name: DC4_DDR_Read
* Description: This function is used to read from different DDR banks0...7
*              i.e. can be through DDR0 or DDR1 controller
* Parameters:  uiController_BaseAddr: Base address of either DDR0 or DDR1 controller
*              uiBank_Offset: Bank selection address i.e. each bank is of 16MB.
*              uiDataLen: No of bytes/short/word to be read from DDR
*              uiPattern: Pattern written to DDR i.e. either 0xA5A5A5A5 or 0x5A5A5A5A
*              ucAccessLength: Byte/Short/Word wise accessed at a time.
* Returns: None
* Globals Updated: None
* Tables Updated: None
*
*/
unsigned int DC4_DDR_Read(unsigned int uiController_BaseAddr, unsigned int uiBank_Offset, unsigned int uiDataLen, unsigned int uiPattern, unsigned char ucAccessLength)
{
	unsigned int uiIndex = 0, uiStatus = 0;
	unsigned int pBank_Addr = (uiController_BaseAddr + uiBank_Offset);

	if(0 == uiPattern)
	{
		if(ucAccessLength == BYTE)
		{
			for(uiIndex=0; uiIndex < uiDataLen; uiIndex+=1)
			if((uiIndex & 0xFF) != *((volatile unsigned char *)pBank_Addr + uiIndex))
				break;
		}
		else if(ucAccessLength == SHORT)
		{
			for(uiIndex=0; uiIndex < uiDataLen; uiIndex+=2)
			if((uiIndex & 0xFFFF) != *((volatile unsigned short *)pBank_Addr + uiIndex))
				break;
		}
		else
		{
			for(uiIndex=0; uiIndex < uiDataLen; uiIndex+=4)
			if(uiIndex != *((volatile unsigned int *)pBank_Addr + uiIndex))
				break;
		}
	}

	else
	{
		if(ucAccessLength == BYTE)
		{
			for(uiIndex=0; uiIndex < uiDataLen; uiIndex+=1)
			if((uiPattern & 0xFF) != *((volatile unsigned char *)pBank_Addr + uiIndex))
				break;
		}
		else if(ucAccessLength == SHORT)
		{
			for(uiIndex=0; uiIndex < uiDataLen; uiIndex+=2)
			if((uiPattern & 0xFFFF) != *((volatile unsigned short *)pBank_Addr + uiIndex))
				break;
		}
		else
		{
			for(uiIndex=0; uiIndex < uiDataLen; uiIndex+=4)
			if(uiPattern != (*((volatile unsigned int *)(pBank_Addr + uiIndex))))
				break;
		}
	}

	if(uiIndex != uiDataLen)
		uiStatus = ((unsigned int)pBank_Addr + uiIndex); /* Sending out the failed addr */

	return uiStatus;
}


