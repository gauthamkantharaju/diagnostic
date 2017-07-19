/*
 * EDMA.c
 *
 *  Created on: Sep 19, 2011
 *      Author: mahantesh.yk
 */
#include "C_Types.h"

#define EDMAADDR 0x49000000

/* Uses Event#/DMA Ch #N, assumes CH#1 mapped to Param #N */
void DMA(UNS32 uiSourceAddr, UNS32 uiDstAddress, UNS32 uiCount, UNS32 uiChannel)
{
	VUNS32 *pParam;
	VUNS32 *ipr;
    int i = 0;
    UNS32 tccNum = 0;

    pParam = (VUNS32*)(EDMAADDR + 0x4000 + (0x20 * uiChannel)); //param # edmaChan
    tccNum = (uiChannel << 12);
    pParam[0] = (0x0010000A | tccNum); //OPT TCC == edmachan + STATIC
    pParam[1] = uiSourceAddr;
    pParam[2] = 0x00010000 + uiCount; //Bcnt, Acnt
    pParam[3] = uiDstAddress;
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

    //Clear the interrupt
    	    *((VUNS32 *)(EDMAADDR + 0x1070)) = (0x01 << uiChannel);/* ICR */
}

/* Uses Event#/DMA Ch #N, assumes CH#1 mapped to Param #N */
void QDMA(UNS32 uiSourceAddr, UNS32 uiDstAddress, UNS32 uiCount, UNS32 uiChannel)
{
	VUNS32 *pParam;
	VUNS32 *ipr;
    int i = 0;
    UNS32 tccNum = 0;

    pParam = (VUNS32*)(EDMAADDR + 0x4000 + (0x20 * uiChannel)); //param # uiChannel
    tccNum = (uiChannel << 12);
    pParam[0] = (0x00100008 | tccNum); //OPT TCC == edmachan + STATIC
    pParam[1] = uiSourceAddr;
    pParam[2] = 0x00010000 + uiCount; //Bcnt, Acnt
    pParam[3] = uiDstAddress;
    pParam[4] = 0x0;
    pParam[5] = 0xFFFF;
    pParam[6] = 0x0;
    //pParam[7] = 0x1;

    printf("Param # %d\n", uiChannel);
    for (i = 0; i < 8; i++) {
       printf("[%d] - 0x%x\n",i ,pParam[i]);
    }
    printf("\n");

    /* SECR = 0xFFFFFFFF */
    *((unsigned int *)(EDMAADDR + 0x1040)) = 0xFFFFFFF;
    printf("SER 0x%x\n", *((unsigned int *)(EDMAADDR + 0x1038)));

    //Trigger
     *((VUNS32 *)(EDMAADDR + 0x108C)) = (0x01 << uiChannel);  //QEESR
     printf("QEER 0x%x\n", *((VUNS32 *)(EDMAADDR + 0x1084)));//QEER

    //Interrupt Enable
    // *((VUNS32 *)(EDMAADDR + 0x1060)) = 0xFFFFFFF;
    // printf("IER 0x%x\n", *((VUNS32 *)(EDMAADDR + 0x1050)));

    // ICR
    *((VUNS32 *)(EDMAADDR + 0x1070)) = (0x01 << uiChannel) ;

    // QCHMAP
    *((VUNS32 *)(EDMAADDR + 0x200 + (0x4 * uiChannel))) = (uiChannel << 5) | (0x07 << 2) ;

    ipr = (VUNS32 *)(EDMAADDR+ 0x1068);
    printf("IPR Before 0x%x\n", *ipr);

    pParam[7] = 0x1;

    /* Check for completion */
    while (((*ipr) & (0x1 << uiChannel))  != (0x01 << uiChannel));
    printf("IPR After 0x%x\n", *ipr);

    //Clear the interrupt
    	    *((VUNS32 *)(EDMAADDR + 0x1070)) = (0x01 << uiChannel);/* ICR */

}

void DevTx_DMA(UNS32 uiBufferAddr, UNS32 uiTxRxRegAddr, UNS32 uiCount, UNS32 uiChannel)
{
	VUNS32 *pParam;
	VUNS32 *ipr;
    int i = 0;
    UNS32 tccNum = 0;
    tccNum = (uiChannel << 12);

    pParam = (VUNS32*)(EDMAADDR + 0x4000 + (0x20 * uiChannel)); //param # edmaChan

	pParam[0] = (0x0010000A | tccNum); //OPT TCC == edmachan + STATIC
	pParam[1] = uiBufferAddr; 			//Source
	pParam[3] = uiTxRxRegAddr;			//Destination
	pParam[2] = (uiCount << 0) + (0x0001 << 16); //Bcnt, Acnt
	pParam[4] = (0x0 << 0);				//BDstIndx,BsrcIndx
	pParam[5] = 0xFFFF;					//BCntRld, Link
	pParam[6] = 0x0;					//CDstIndx,CsrcIndx
	pParam[7] = 0x1;					//Ccnt

    printf("Param # %d\n", uiChannel);
    for (i = 0; i < 8; i++) {
       printf("[%d] - 0x%x\n",i ,pParam[i]);
    }
    printf("\n");

    /* SECR = 0xFFFFFFFF */
    *((unsigned int *)(EDMAADDR + 0x1040)) = 0xFFFFFFF;
    printf("SER 0x%x\n", *((unsigned int *)(EDMAADDR + 0x1038)));

     //EESR  = 0x2
    //*((VUNS32 *)(EDMAADDR + 0x1030)) = (0x01 << uiChannel);
    //printf("EER 0x%x\n", *((VUNS32 *)(EDMAADDR + 0x1020)));


    /* IESR = 0x02 */
    /*
    *((VUNS32 *)(EDMAADDR + 0x1060)) = 0xFFFFFFF;
    printf("IER 0x%x\n", *((VUNS32 *)(EDMAADDR + 0x1050)));
    */

    //Clear the interrupt
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

    //Clear the interrupt
    *((VUNS32 *)(EDMAADDR + 0x1070)) = (0x01 << uiChannel);/* ICR */

}
void DevRecvDMA(UNS32 uiBufferAddr, UNS32 uiTxRxRegAddr, UNS32 uiCount,  UNS32 uiChannel)
{
	VUNS32 *pParam;
		VUNS32 *ipr;
	    int i = 0;
	    UNS32 tccNum = 0;
	    tccNum = (uiChannel << 12);

	    pParam = (VUNS32*)(EDMAADDR + 0x4000 + (0x20 * uiChannel)); //param # edmaChan
		pParam[0] = (0x00100009 | tccNum); 	//OPT TCC == edmachan + STATIC
		pParam[1] = uiTxRxRegAddr; 			//Source
		pParam[3] = uiBufferAddr;			//Destination
		pParam[2] = (uiCount << 0) + (0x0001 << 16); //Bcnt, Acnt
		pParam[4] = (0x0 << 0);				//BDstIndx,BsrcIndx
	    pParam[5] = 0xFFFF;					//BCntRld, Link
	    pParam[6] = 0x0;					//CDstIndx,CsrcIndx
	    pParam[7] = 0x0;					//Ccnt

	    printf("Param # %d\n", uiChannel);
	    for (i = 0; i < 8; i++) {
	       printf("[%d] - 0x%x\n",i ,pParam[i]);
	    }
	    printf("\n");

	    /* SECR = 0xFFFFFFFF */
	    *((unsigned int *)(EDMAADDR + 0x1040)) = 0xFFFFFFF;
	    printf("SER 0x%x\n", *((unsigned int *)(EDMAADDR + 0x1038)));

	     //EESR  = 0x2
	    //*((VUNS32 *)(EDMAADDR + 0x1030)) = (0x01 << uiChannel);
	    //printf("EER 0x%x\n", *((VUNS32 *)(EDMAADDR + 0x1020)));


	    /* IESR = 0x02 */
	    /*
	    *((VUNS32 *)(EDMAADDR + 0x1060)) = 0xFFFFFFF;
	    printf("IER 0x%x\n", *((VUNS32 *)(EDMAADDR + 0x1050)));
	    */

	    //Clear the interrupt
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

	    //Clear the interrupt
	    *((VUNS32 *)(EDMAADDR + 0x1070)) = (0x01 << uiChannel);/* ICR */
}

