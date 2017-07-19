/* Title: ML_UART.c
 * Application: UART Configuration
 * Copyright: HCL Technologies
 * Author: Gautham.K
 * Creation Date: 27/5/2011
 * Requirements: ML_defineC.h
 * References:
 * Description: Main C file where all the UART register are configured.
 * Limitations:
 * Dependencies: TMS320DM8148 compiler
 * Modification: Initial Version */

#include "ML_UARTdefineC.h"
#include "C_Types.h"

/*
 * Name: DC4_UART_Config
 * Description: This function is used to configure all UART control register.
 * Parameters: iBaudrate is the baud-rate value used to calculate divisor value
 * 			   to be written to DLL and DLH registers.
 * Returns: None
 * Globals Updated: None
 * Tables Updated: None
 *
 */
void DC4_UART_Config(unsigned int uiUARTControllerAddr, unsigned int uiBaudRate)
{
	unsigned int uiVal=0;

	/* dividing USB PLL input to PRCM(192MHz) by 3 to get 48MHz */
	*pCM_SYSCLK10_CLKSEL = 0x3;

	/* Select SYSCLK10 for UART3,4,5 */
	*pBSP_UART_CLKSRC = 0xA8; 

	printf("UART4 clock src:%x \n", *pBSP_UART_CLKSRC);

	/* Enabling L3 Slow interconnect, UART1
	 * clock from PRCM module */
	*pCM_ALWON_L3_SLOW_CLKSTCTRL = 0x2;
	*pCM_ALWON_UART0_CLKCTRL = 0x2;
	*pCM_ALWON_UART1_CLKCTRL = 0x2;
	*pCM_ALWON_UART2_CLKCTRL = 0x2;
	*pCM_ALWON_UART3_CLKCTRL = 0x2;
	*pCM_ALWON_UART4_CLKCTRL = 0x2;	
	*pCM_ALWON_UART5_CLKCTRL = 0x2;

	while((*pCM_ALWON_L3_SLOW_CLKSTCTRL & 0x2102) != 0x2102);
	while((*pCM_ALWON_UART0_CLKCTRL & 0x2) != 0x2);
	while((*pCM_ALWON_UART1_CLKCTRL & 0x2) != 0x2);
	while((*pCM_ALWON_UART2_CLKCTRL & 0x2) != 0x2);
	while((*pCM_ALWON_UART3_CLKCTRL & 0x2) != 0x2);
	while((*pCM_ALWON_UART4_CLKCTRL & 0x2) != 0x2);
	while((*pCM_ALWON_UART5_CLKCTRL & 0x2) != 0x2);
#if 0
	printf("PRCM configuration is done \n");
#endif

	/* First configure LCR to 0xBFh
	 * IER, MCR, LCR[6] to 0 and MDR1 to 7h
	 * to disable/reset the UART module*/
	 *((volatile unsigned int*)(uiUARTControllerAddr + LCR)) = 0xBF;
/* So that we can access to IER, MCR and FCR registers */
	 *((volatile unsigned int*)(uiUARTControllerAddr + EFR)) = 0x10; 

	 /* Make LCR[7] to 0 so that we can
	  * write to IER MCR and MDR1 register*/
	 *((volatile unsigned int*)(uiUARTControllerAddr + LCR)) &= ~(1 << 7);
	 *((volatile unsigned int*)(uiUARTControllerAddr + IER)) = 0; /* Disable interrupt */
	 *((volatile unsigned int*)(uiUARTControllerAddr + MCR)) = 0; /* Disable Modem */
	 *((volatile unsigned int*)(uiUARTControllerAddr + LCR)) &= ~(1 << 6); /* Force control signal inactive */
	 *((volatile unsigned int*)(uiUARTControllerAddr + MDR1)) = 0x7; /* To be in disable state */

	/* FIFO configuration i.e configuring
	 * TCR, TLR and FCR registers */
	*((volatile unsigned int*)(uiUARTControllerAddr + MCR)) = 0x40; /* To write into TLR and TCR register */

	*((volatile unsigned int*) (uiUARTControllerAddr + SCR)) = 0x00; /* To set transmit/receive granularity */
/* Max granularity is same as FCR granularity value */
	*((volatile unsigned int*) (uiUARTControllerAddr + TLR)) = 0x00; 
/* FIFO enabled and transmit/receive granularity is set to max */
	*((volatile unsigned int*)(uiUARTControllerAddr + FCR)) = 0x0F; 
	*((volatile unsigned int*)(uiUARTControllerAddr + TCR)) = 0x00;

	/* Disabling access to TCR and TLR registers
	 * by making MCR[6] to 0 */
	*((volatile unsigned int*)(uiUARTControllerAddr + MCR)) &= ~(1 << 6);

	/* Disabling access to IER, FCR and MCR registers
	 * by making LCR=0xBFh and making EFR[4] to 0 */
	*((volatile unsigned int*)(uiUARTControllerAddr + LCR)) = 0xBF;
	*((volatile unsigned int*)(uiUARTControllerAddr + EFR)) &= ~(1 << 4);

	/* UART related configuration, LCR value should be other than 0xBFh
	 * LCR[7] should be 1 to access DLL and DLH registers */
	*((volatile unsigned int*)(uiUARTControllerAddr + LCR)) = 0x83; /* configured for 8N1 */

	if(uiUARTControllerAddr == UART4_BASE_ADDRESS)
	{
		/* Baud-rate calculation (192 * 10^6) / (16 * uiBaudRate)
		 * uiBaudrate is the baud-rate value i.e. 115200 in this case */
		uiVal = (SYSCLK8) / (16 * uiBaudRate);
		*((volatile unsigned int*)(uiUARTControllerAddr + DLL)) = (uiVal & 0xFF);
		*((volatile unsigned int*)(uiUARTControllerAddr + DLH)) = ((uiVal >> 8) & 0xFF);
	}

	/* Baud-rate calculation (48 * 10^6) / (16 * uiBaudRate)
	 * uiBaudrate is the baud-rate value i.e. 115200 in this case */
	uiVal = (SYSCLK10) / (16 * uiBaudRate);
	*((volatile unsigned int*)(uiUARTControllerAddr + DLL)) = (uiVal & 0xFF);
	*((volatile unsigned int*)(uiUARTControllerAddr + DLH)) = ((uiVal >> 8) & 0xFF);

//	printf("%d \t DLL:%x \t DLH:%x \n", uiVal, *((volatile unsigned int*)(uiUARTControllerAddr + DLL)), *((volatile unsigned int*)(uiUARTControllerAddr + DLH)));

	/* LCR[7] should be made 0
	 * To disable access to DLL and DLH registers */
	*((volatile unsigned int*)(uiUARTControllerAddr + LCR)) &= ~(1 << 7);

	/* Configure MDR1 and MDR2 registers*/
	/* First MDR2 should be configured before MDR1
	 * and MDR1 should be configured after configuring
	 * DLL, DLH and LCR registers */
	*((volatile unsigned int*)(uiUARTControllerAddr + MDR2)) = 0x00; /* For Normal UART mode */
	*((volatile unsigned int*)(uiUARTControllerAddr + MDR1)) = 0x00; /* UART 16× mode*/
}

/*
 * Name: DC4_UART_DATA_COMPARE
 * Description: This function is used to compare destination and source buffer.
 * Parameters: ucDestBuffer is the destination buffer value which is read from UART
 * 			   ucSrcBuffer is the source buffer value which is written UART
 *			   uiLength is the length of the buffer
 * Returns: ucStatus is the status i.e.
 * 			0 for success
 *			uiIndex representing at which index value the mismatch occurred.
 * Globals Updated: None
 * Tables Updated: None
 *
 */
unsigned char DC4_UART_DATA_COMPARE(unsigned int uiUARTControllerAddr, unsigned char* ucDestBuffer, unsigned char* ucSrcBuffer, unsigned int uiLength)
{
	unsigned int uiIndex=0, ucStatus=0;

	for(uiIndex=0; uiIndex < uiLength; uiIndex++)
	{
		if(ucDestBuffer[uiIndex] != ucSrcBuffer[uiIndex])
			break;
	}

	if(uiIndex != uiLength)
		ucStatus = uiIndex;

	return ucStatus;
}

/*
 * Name: DC4_UART_Read
 * Description: This function is used to read byte data from UART RBR registers.
 * Parameters: pucRead is a pointer to the read buffer where read data is stored
 * 			   iNoOfBytes is the no of bytes to be read.
 * Returns: None
 * Globals Updated: None
 * Tables Updated: None
 *
 */
void DC4_UART_Read(unsigned int uiUARTControllerAddr, unsigned char *pucRead, unsigned int uiNoOfBytes)
{
	unsigned int uiIndex=0;

	for(uiIndex=0; uiIndex < uiNoOfBytes; uiIndex++)
	{
		/* If there is 1 byte or in case FIFO is
		 * full read the data from RBR register */
		while((*((volatile unsigned int*)(uiUARTControllerAddr + LSR)) & 0x1)!= 0x1);
		pucRead[uiIndex] = *((volatile unsigned int*)(uiUARTControllerAddr + RBR));
		//printf("Data Read: %d \t %c \n", uiIndex, pucRead[uiIndex]);
	}
}

/*
 * Name: DC4_UART_Write
 * Description: This function is used to write byte data to UART THR registers.
 * Parameters: pucWrite is a pointer to the write buffer where data is to be transmitted
 * 			   iNoOfBytes is the no of bytes to be transmitted.
 * Returns: None
 * Globals Updated: None
 * Tables Updated: None
 *
 */
void DC4_UART_Write(unsigned int uiUARTControllerAddr, unsigned char *pucWrite, unsigned int uiNoOfBytes)
{
	unsigned int uiIndex=0;

	for(uiIndex=0; uiIndex < uiNoOfBytes; uiIndex++)
	{
		*((volatile unsigned int*)(uiUARTControllerAddr + THR)) = pucWrite[uiIndex];

		/* FIFO empty write data into the FIFO
		 * else poll for shift register till transmit is completed */
		/* Check whether Transmit FIFO register is empty */
		while((*((volatile unsigned int*)(uiUARTControllerAddr + LSR)) & 0x20) != 0x20);

		/* Checking whether Transmit FIFO and shift
		 * register are empty, so that transmit
		 * is completed */
		while((*((volatile unsigned int*)(uiUARTControllerAddr + LSR)) & 0x40) != 0x40);
	}
}

UNS16 ReadByte(UNS32 uiCntrlAddr)
{
	while((*((VUNS32*)(uiCntrlAddr + LSR)) & 0x1)!= 0x1);
	return *((VUNS32*)(uiCntrlAddr + RBR));
}

void WriteByte(UNS32 uiCntrlAddr, UNS8 Data)
{
	*((VUNS32*)(uiCntrlAddr + THR)) = Data;
	while((*((VUNS32*)(uiCntrlAddr + LSR)) & 0x20) != 0x20);
	while((*((VUNS32*)(uiCntrlAddr + LSR)) & 0x40) != 0x40);
}


#define EDMAADDR 0x49000000
void UART_TX_DMA(UNS8 *ucBuffer, UNS32 uiCount, UNS8 ucFlag)
{
	VUNS32 *pParam;
	VUNS32 *ipr;
    int i = 0;

    UNS32 tccNum = 0;
    UNS32 uiChannel = 0;

    uiChannel = (ucFlag) ? 26 : 27;
    tccNum = (uiChannel << 12);

    pParam = (VUNS32*)(EDMAADDR + 0x4000 + (0x20 * uiChannel)); //param # edmaChan
    if(ucFlag)
    {
		pParam[0] = (0x0010000A | tccNum); //OPT TCC == edmachan + STATIC
		pParam[1] = (UNS32)ucBuffer; //Source
		pParam[2] = 0x00010000 + uiCount; //Bcnt, Acnt
		pParam[3] = UART0_BASE_ADDRESS;//Destination
    }
    else
    {

    	pParam[0] = (0x0010000A | tccNum); //OPT TCC == edmachan + STATIC
		pParam[1] = UART0_BASE_ADDRESS;//Source
		pParam[2] = 0x00010000 + uiCount; //Bcnt, Acnt
		pParam[3] = (UNS32)ucBuffer; //Destination
    }
    pParam[4] = 0x0;
    pParam[5] = 0xFFFF;
    pParam[6] = 0x0;
    pParam[7] = 0x1;

    printf("Param # %d\n", uiChannel);
    for (i = 0; i < 8; i++) {
       printf("[%d] - 0x%x\n",i ,pParam[i]);
    }
    printf("\n");

    /* SECR = 0xFFFFFFFF */
    *((unsigned int *)(EDMAADDR + 0x1040)) = 0xFFFFFFF;
    printf("SER 0x%x\n", *((unsigned int *)(EDMAADDR + 0x1038)));

    /* EESR  = 0x2
    *((VUNS32 *)(EDMAADDR + 0x1030)) = (0x01 << edmaChan);
    printf("EER 0x%x\n", *((VUNS32 *)(EDMAADDR + 0x1020)));
    */

    /* IESR = 0x02 */
    /*
    *((VUNS32 *)(EDMAADDR + 0x1060)) = 0xFFFFFFF;
    printf("IER 0x%x\n", *((VUNS32 *)(EDMAADDR + 0x1050)));
    */

    /* ICR */
    *((VUNS32 *)(EDMAADDR + 0x1070)) = (0x01 << uiChannel);

    /* DCHMAP */
    *((VUNS32 *)(EDMAADDR + 0x100 + (0x4 * uiChannel))) = (uiChannel << 5) ;

    ipr = (VUNS32 *)(EDMAADDR+ 0x1068);
    printf("IPR Before 0x%x\n", *ipr);

    /* ESR = 0x02 */
    *((VUNS32 *)(EDMAADDR + 0x1010)) = (0x01 << uiChannel);

    printf("EER 0x%x\n", *((VUNS32 *)(EDMAADDR + 0x1020)));

    /* Check for completion */
    while (((*ipr) & (0x1 << uiChannel))  != (0x01 << uiChannel));
    printf("IPR After 0x%x\n", *ipr);

}
