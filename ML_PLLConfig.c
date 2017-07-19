
#include "ML_PLLstructC.h"
#include "ML_PLLdefineC.h"

/*
 * Name: delay
 * Description: This function is used to cause delay.
 * Parameters: uiDelayCount is the delay count value.
 * Returns:    None
 * Globals Updated: None
 * Tables Updated: None
 *
 */
void delay(unsigned int uiDelayCount)
{
	unsigned int uiIndex=0;

	/* delay causing code */
	for(uiIndex=0; uiIndex < uiDelayCount; uiIndex++);
}

/*
 * Name: DC4_PLL_MMR_UNLOCK
 * Description: This function is used to unlock the MMR control
 *              and PLL sub-system registers.
 * Parameters: None
 * Returns:    None
 * Globals Updated: None
 * Tables Updated: None
 *
 */
void DC4_PLL_MMR_UNLOCK()
{
	/* to unlock the PLL Sub System registers */
	*pPLL_MMR_LOCK = 0x1EDA4C3D;

	/*unlock the Control module registers */
	*pMMR_LOCK0 = 0x2FF1AC2B;
	*pMMR_LOCK1 = 0xF757FDC0;
	*pMMR_LOCK2 = 0xE2BC3A6D;
	*pMMR_LOCK3 = 0x1EBF131D;
	*pMMR_LOCK4 = 0x6F361E05;
}

/*
 * Name: DC4_PLL_PERIPHERALS_SOURCE_SEL
 * Description: This function is used to select the clock source
 *              for the different module.
 * Parameters: uiBaseAddr is the base address of the clock source
 *             selection registers.
 * Returns:    None
 * Globals Updated: Updates _clkSRC structure
 * Tables Updated: None
 *
 */
void DC4_PLL_PERIPHERALS_SOURCE_SEL(unsigned int uiBaseAddr)
{
	stClkSrc *pstClkSrc = (stClkSrc *) uiBaseAddr;

	pstClkSrc->ARM_CLKSRC = 0x00; /*OSC0 as a source for ARM module*/
	pstClkSrc->CLKOUT_MUX = 0;
	pstClkSrc->DMTIMER_CLKSRC = 0x00; /*SYSCLK18 as source*/
	pstClkSrc->HDMI_I2S_CLKSRC = 0;
	pstClkSrc->MLB_ATL_CLKSRC = 0;

	/*PLL_AUDIO_OUT is selected as source*/
	pstClkSrc->McASP345_AUX_CLKSRC = 0x10101;

	pstClkSrc->McASP_AHCLK_CLKSRC = 0;
	pstClkSrc->McBSP_UART_CLKSRC = 0xA8;/*SYSCLK10 for UART3,4,5 PRCM O/P for McBSP*/
	pstClkSrc->OSC_SRC = 0x00; /*OSC0 as source for Video0,1,2 Audio and L3*/
	pstClkSrc->RMII_REFCLK_SRC = 0;
	pstClkSrc->SYSCLK18_CLKSRC = 0x0; /*RTCDivider as source*/
	pstClkSrc->VIDEO_PLL_CLKSRC = 0;
	pstClkSrc->WDT0_CLKSRC = 0x0; /*RTCDivider as source*/
}

/*
 * Name: DC4_PLL_MMR_LOCK
 * Description: This function is used to lock the MMR control and PLL
 *              sub-system registers.
 * Parameters: None
 * Returns:    None
 * Globals Updated: None
 * Tables Updated: None
 *
 */
void DC4_PLL_MMR_LOCK()
{
	/* lock the PLL Sub System registers */
	*pPLL_MMR_LOCK = 0x1F125B64;

	/*lock the Control module registers */
	*pMMR_LOCK0 = 0x1A1C8144;
	*pMMR_LOCK1 = 0xFDF45530;
	*pMMR_LOCK2 = 0x1AE6E320;
	*pMMR_LOCK3 = 0x2FFA927C;
	*pMMR_LOCK4 = 0x143F832C;
}

/*
 * Name: DC4_PLL_ARM
 * Description: This function is used to configure PLL for ARM 
 *              Example:- for getting clkout as 500MHz
 *			    N value is 0, M = 30 and M2 = 0
 *			    formula is:- ((OSC0 clk/(N+1)) * M)
 *			    this gives the clkout accordingly
 * Parameters: uiBaseaddr is the base address of the PLL_ARM register
 *             uiN is the pre-clock divider value
 * 			   uiM is the multiplier value
 * 			   uiM2 is the post divider value
 * Returns:    None
 * Globals Updated: Updates _pllARM structure
 * Tables Updated: None 
 * 
 */
void DC4_PLL_ARM(unsigned int uiBaseaddr, unsigned int uiN, unsigned int uiM, unsigned int uiM2)
{
	volatile unsigned int uiClkCtrl=0, uiStatus=0;

	stARMPLL *pstARMPLL = (stARMPLL *)uiBaseaddr;
	
	/* To Force IDLE mode
	uiClkCtrl = pstARMPLL->iPLL_ARM_CLKCTRL;
	pstARMPLL->iPLL_ARM_CLKCTRL = ((uiClkCtrl | 0x00800000) | 0x1);*/

	/* poll for the frequency,phase lock to occur */
	while((pstARMPLL->iPLL_ARM_STATUS & 0x00000600) != 0x00000600);

	pstARMPLL->iPLL_ARM_M2NDIV = uiN;
	pstARMPLL->iPLL_ARM_MN2DIV = uiM;
	pstARMPLL->iPLL_ARM_TENABLEDIV = 0x01; /*latching M2 & N2 values on rising edge */
	pstARMPLL->iPLL_ARM_TENABLEDIV = 0x00;
	pstARMPLL->iPLL_ARM_TENABLE = 0x01; /*latching M & N values on rising edge */
	pstARMPLL->iPLL_ARM_TENABLE = 0x00;


	uiClkCtrl = pstARMPLL->iPLL_ARM_CLKCTRL;
	pstARMPLL->iPLL_ARM_CLKCTRL = ((uiClkCtrl & 0xff7fffff) | 0x1);

	/* poll for the frequency,phase lock to occur */
	while((pstARMPLL->iPLL_ARM_STATUS & 0x00000600) != 0x00000600);

	/* Delay to stabilize the clock */
	delay(10000000);
}

/*
 * Name: DC4_PLL_Peripherals
 * Description: This function is used to configure PLL for other modules 
 *              Example:- clkout of 500MHz for DSP
 *              N value is 0, M = 100 and M2 = 4 
 *              formula is:- (((OSC0 clk/(N+1)) * M)/ M2) 
 *              this gives the clkout accordingly
 * Parameters: uiBaseaddr is the base address of the peripheral PLL register
 *             uiN is the pre-clock divider value
 * 			   uiM is the multiplier value
 * 			   uiM2 is post clock divider value
 * Returns: None
 * Globals Updated: Updates _pll structure, according to the peripherals.
 * Tables Updated: None 
 * 
 */
void DC4_PLL_Peripherals(unsigned int uiBaseaddr, unsigned int uiN, unsigned int uiM, unsigned int uiM2, unsigned int uiClkctrl_Val)
{
	volatile unsigned int uiClkCtrl=0, uiStatus=0;

	stPLL *pstPLL = (stPLL *)uiBaseaddr;
	
	/* To Force IDLE mode
	uiClkCtrl = pstPLL->iCLKCTRL;
	pstPLL->iCLKCTRL = ((uiClkCtrl | 0x00800000) | 0x1);*/

	pstPLL->iM2NDIV = (uiM2 << 16) | uiN;
	pstPLL->iMN2DIV = uiM;
	pstPLL->iTENABLEDIV = 0x01;
	pstPLL->iTENABLEDIV = 0x00;
	pstPLL->iTENABLE = 0x01;
	pstPLL->iTENABLE = 0x00;

	uiClkCtrl = pstPLL->iCLKCTRL;
	pstPLL->iCLKCTRL = ((uiClkCtrl & 0xff7fe3ff) | uiClkctrl_Val);

	/* poll for the frequency,phase lock to occur */
 	while((pstPLL->iSTATUS & 0x00000600) != 0x00000600);

	delay(10000000);
}

/*
 * Name: DC4_PRCM_CM_CLKSEL
 * Description: This function is used to select clock source for
 *              peripherals which needs clock out from PRCM module.
 * Parameters: uiBaseAddr is the base address of the clock
 *             selection registers.
 * Returns:    None
 * Globals Updated: Updates _CM_ClkSel structure
 * Tables Updated: None 
 * 
 */
void DC4_PRCM_CM_CLKSEL(unsigned int uiBaseAddr)
{
	stCM_CLKSEL *pstClkSel = (stCM_CLKSEL *) uiBaseAddr;

	pstClkSel->CM_APA_CLKSEL = 0;

	/* Functional clock for McASP0,1,2
	 * SYSCLK20,21,22 i.e 192MHZ
	 * using SYSCLK21 */
	pstClkSel->CM_AUDIOCLK_MCASP0_CLKSEL = 0x1;
	pstClkSel->CM_AUDIOCLK_MCASP1_CLKSEL = 0x1;
	pstClkSel->CM_AUDIOCLK_MCASP2_CLKSEL = 0x1;

	pstClkSel->CM_HDMI_CLKSEL = 0x0; /* dividing by 1 to get 200MHz */

	/* Functional clock for SPI0,1,2,3 I2C0,1,2,3
	 * UART0,1,2 and HDMI CEC clock
	 * suppose to be 48MHz
	 * USB_PLL is 960MHz is dividing by 5
	 * then dividing by 4 to get 48MHz */
	pstClkSel->CM_SYSCLK10_CLKSEL = 0x3;

	pstClkSel->CM_SYSCLK14_CLKSEL = 0;
	pstClkSel->CM_SYSCLK16_CLKSEL = 0;
	pstClkSel->CM_SYSCLK18_CLKSEL = 0x1; /* Audio PLL generated clock 32KHz */
	pstClkSel->CM_SYSCLK19_CLKSEL = 0;

	/* SYSCLK20,21,22 is used for McASP0,1,2 HDMI I2S, McBSP */
	pstClkSel->CM_SYSCLK20_CLKSEL = 0;
	pstClkSel->CM_SYSCLK21_CLKSEL = 0;
	pstClkSel->CM_SYSCLK22_CLKSEL = 0;

	/* SGX related */
	pstClkSel->CM_SYSCLK23_CLKSEL = 0; /* dividing by 1 to get 200MHz */

	/* IVA functional clock i.e 266MHz so dividing it by 0 */
	pstClkSel->CM_SYSCLK3_CLKSEL = 0x00;

	/* SYSCLK18 is been used for timer */
	pstClkSel->CM_TIMER1_CLKSEL = 0;
	pstClkSel->CM_TIMER2_CLKSEL = 0;
	pstClkSel->CM_TIMER3_CLKSEL = 0;
	pstClkSel->CM_TIMER4_CLKSEL = 0;
	pstClkSel->CM_TIMER5_CLKSEL = 0;
	pstClkSel->CM_TIMER6_CLKSEL = 0;
	pstClkSel->CM_TIMER7_CLKSEL = 0;

	/* divider value for Video PLL B3 divider */
	pstClkSel->CM_VPB3_CLKSEL = 0;

	/* divider value for Video PLL C1 divider */
	pstClkSel->CM_VPC1_CLKSEL = 0;

	/* divider value for Video PLL D1 divider */
	pstClkSel->CM_VPD1_CLKSEL = 0;
}

/*
 * Name: DC4_PRCM_CM_DEFAULT
 * Description: This function is used to configure the default clock
 *              control for different PRCM dependent modules.
 * Parameters: uiBaseAddr is the base address of the PRCM
 *             DEFAULT registers.
 * Returns:    None
 * Globals Updated: Updates _CM_Default structure
 * Tables Updated: None
 *
 */
void DC4_PRCM_CM_DEFAULT(unsigned int uiBaseAddr)
{
	stCM_DEFAULT *pstDefault = (stCM_DEFAULT *) uiBaseAddr;

/******************DMM related************************/
	pstDefault->CM_D_DMM_CLKCTRL = 0x2;
	while((pstDefault->CM_D_DMM_CLKCTRL & 0x2) != 0x2);
/******************************************************/

/*****************Ducati related**********************/
	/* Software force transition is enabled */
	pstDefault->CM_D_DUCATI_CLKSTCTRL = 0x2;
	pstDefault->CM_D_DUCATI_CLKCTRL = 0x2;

	while((pstDefault->CM_D_DUCATI_CLKSTCTRL & 0x302) != 0x302);
	while((pstDefault->CM_D_DUCATI_CLKCTRL & 0x2) != 0x2);
/******************************************************/

/*******************EMIF related**********************/
	pstDefault->CM_D_EMIF_0_CLKCTRL = 0x2;
	while((pstDefault->CM_D_EMIF_0_CLKCTRL & 0x2) != 0x2);

	pstDefault->CM_D_EMIF_1_CLKCTRL = 0x2;
	while((pstDefault->CM_D_EMIF_1_CLKCTRL & 0x2) != 0x2);
/******************************************************/

/************************FW**************************/
	pstDefault->CM_D_FW_CLKCTRL = 0x2;
	while((pstDefault->CM_D_FW_CLKCTRL & 0x2) != 0x2);
/*****************************************************/

/*******************PCI related***********************/
	/* Software force transition is enabled */
	/* PCI functional clock is not generated have to check */
	pstDefault->CM_D_PCI_CLKSTCTRL = 0x2;
	pstDefault->CM_D_PCI_CLKCTRL = 0x2;

	while((pstDefault->CM_D_PCI_CLKSTCTRL & 0x102)!= 0x102);
	while((pstDefault->CM_D_PCI_CLKCTRL & 0x2) != 0x2);
/******************************************************/

/*********************SATA related*********************/
	pstDefault->CM_D_SATA_CLKCTRL = 0x2;
	while((pstDefault->CM_D_SATA_CLKCTRL & 0x2) != 0x2);
/*******************************************************/

/*********************TPSS related*********************/
	/* Software force transition is enabled */
	pstDefault->CM_D_TPPSS_CLKSTCTRL = 0x2;
	pstDefault->CM_D_TPPSS_CLKCTRL = 0x2;

	while((pstDefault->CM_D_TPPSS_CLKSTCTRL & 0x7F02) != 0x7F02);
	while((pstDefault->CM_D_TPPSS_CLKCTRL & 0x2) != 0x2);
/********************************************************/

/***********************USB related**********************/
	pstDefault->CM_D_USB_CLKCTRL = 0x2;
	while((pstDefault->CM_D_USB_CLKCTRL & 0x2) != 0x2);
/*********************************************************/
}

/*
 * Name: DC4_PRCM_GEM
 * Description: This function is used to configure the clock control
 *              for GEM module.
 * Parameters: None
 * Returns:    None
 * Globals Updated: None
 * Tables Updated: None
 *
 */
void DC4_PRCM_GEM()
{
	/* Software force transition is enabled */
	*pCM_GEM_CLKSTCTRL = 0x2;
	*pCM_ACTIVE_GEM_CLKCTRL = 0x2;

	while((*pCM_GEM_CLKSTCTRL & 0x702)!= 0x702);
	while((*pCM_ACTIVE_GEM_CLKCTRL & 0x2) != 0x2);
}

/*
 * Name: DC4_PRCM_PM_RM_IVA
 * Description: This function is used to configure the
 *              power and reset management for IVA module.
 * Parameters: None
 * Returns:    None
 * Globals Updated: None
 * Tables Updated: None
 *
 */
void DC4_PRCM_PM_RM_IVA()
{
	/* Software force transition is enabled */
	*pCM_IVAHD0_CLKSTCTRL = 0x2;
	*pCM_IVAHD0_IVAHD_CLKCTRL = 0x2;

	while((*pCM_IVAHD0_CLKSTCTRL & 0x102) != 0x102);
	while((*pCM_IVAHD0_IVAHD_CLKCTRL & 0x2) != 0x2);

	*pCM_IVAHD0_SL2_CLKCTRL = 0x2;
	while((*pCM_IVAHD0_SL2_CLKCTRL &0x2) != 0x2);

	/* Power and reset management */
	*pPM_IVAHD0_PWRSTCTRL = 0x3;
	while((*pPM_IVAHD0_PWRSTCTRL & 0x30003)!= 0x30003);

	while((*pPM_IVAHD0_PWRSTST & 0x37) != 0x37);

	*pRM_IVAHD0_RSTCTRL = 0x7;
}

/*
 * Name: DC4_PRCM_PM_RM_ISP
 * Description: This function is used to configure the
 *              power and reset management for ISP module.
 * Parameters: None
 * Returns:    None
 * Globals Updated: None
 * Tables Updated: None
 *
 */
void DC4_PRCM_PM_RM_ISP()
{
	/* Software force transition is enabled */
	*pCM_ISP_CLKSTCTRL = 0x2;
	*pCM_ISP_ISP_CLKCTRL = 0x2;

	while((*pCM_ISP_CLKSTCTRL & 0x102) != 0x102);
	while((*pCM_ISP_ISP_CLKCTRL & 0x40002)!= 0x40002);

	*pCM_ISP_FDIF_CLKCTRL = 0x2;
	while((*pCM_ISP_FDIF_CLKCTRL & 0x2) != 0x2);

	/* Power and reset management */
	*pPM_ISP_PWRSTCTRL = 0x3;
	while((*pPM_ISP_PWRSTCTRL & 0x30003) != 0x30003);

	while((*pPM_ISP_PWRSTST & 0x37) != 0x37);

	*pRM_ISP_RSTCTRL = 0x4;
}

/*
 * Name: DC4_PRCM_PM_RM_DSS
 * Description: This function is used to configure the
 *              power and reset management for DSS module.
 * Parameters: None
 * Returns:    None
 * Globals Updated: None
 * Tables Updated: None
 *
 */
void DC4_PRCM_PM_RM_DSS()
{
	/* Software force transition is enabled */
	*pCM_DSS_CLKSTCTRL = 0x2;
	*pCM_DSS_DSS_CLKCTRL = 0x2;

	while((*pCM_DSS_CLKSTCTRL & 0x102) != 0x102);
	while((*pCM_DSS_DSS_CLKCTRL & 0x2) != 0x2);

	*pCM_DSS_HDMI_CLKCTRL = 0x2;
	while((*pCM_DSS_HDMI_CLKCTRL & 0x2) != 0x2);

	/* Power and reset management */
	*pPM_DSS_PWRSTCTRL = 0x3;
	while((*pPM_DSS_PWRSTCTRL & 0x30003) != 0x30003);

	while((*pPM_DSS_PWRSTST & 0x37) != 0x37);

	*pRM_DSS_RSTCTRL = 0x4;
}

/*
 * Name: DC4_PRCM_PM_RM_SGX
 * Description: This function is used to configure the
 *              power and reset management for SGX module.
 * Parameters: None
 * Returns:    None
 * Globals Updated: None
 * Tables Updated: None
 *
 */
void DC4_PRCM_PM_RM_SGX()
{
	/* Software force transition is enabled */
	*pCM_SGX_CLKSTCTRL = 0x2;
	*pCM_SGX_SGX_CLKCTRL = 0x2;

	while((*pCM_SGX_CLKSTCTRL & 0x102) != 0x102);
	while((*pCM_SGX_SGX_CLKCTRL & 0x2) != 0x2);

	/* Power and reset management */
	*pPM_SGX_PWRSTCTRL = 0x3;
	while((*pPM_SGX_PWRSTCTRL & 0x30003) != 0x30003);

	while((*pPM_SGX_PWRSTST & 0x37) != 0x37);

	*pRM_SGX_RSTCTRL = 0x1;
}

void DC4_PM_RM_ACTIVE()
{
	/* GEM WARM reset */
	*pRM_ACTIVE_RSTCTRL = 0x3;

	*pPM_ACTIVE_PWRSTCTRL = 0x3;
	while((*pPM_ACTIVE_PWRSTCTRL & 0x30003) != 0x30003);
}

void DC4_RM_DEFAULT()
{
	*pRM_DEFAULT_RSTCTRL = 0xFF;
}

/*
 * Name: DC4_PRCM_CM_ALWON
 * Description: This function is used to configure the clock control,
 *              and check and poll for the same for different modules.
 * Parameters: uiBaseAddr is the base address of the PRCM
 *             ALWON registers.
 * Returns:    None
 * Globals Updated: Updates _CM_Alwon structure
 * Tables Updated: None
 *
 */
void DC4_PRCM_CM_ALWON(unsigned int uiBaseAddr)
{
	stCM_ALWON *pstCM_ALWON = (stCM_ALWON *) uiBaseAddr;

/***********************ATL****************************/
	pstCM_ALWON->CM_ATL_CLKCTRL = 0x2;
	/* POLL for ATL */
	while((pstCM_ALWON->CM_ATL_CLKCTRL & 0x2) != 0x2);
/********************END OF ATL*************************/

/********************CONTROL_CLKCTRL**********************/
	pstCM_ALWON->CM_CONTROL_CLKCTRL = 0x2;
	/* POLL for Control CLKCTRL */
	while((pstCM_ALWON->CM_CONTROL_CLKCTRL & 0x2) != 0x2);
/****************END OF CONTROL_CLKCTRL********************/

/****************************CUST_EFUSE**********************/
	pstCM_ALWON->CM_EFUSE_CLKCTRL = 0x2;
	/* POLL for CUST_EFUSE */
	while((pstCM_ALWON->CM_EFUSE_CLKCTRL & 0x2) != 0x2);
/************************************************************/

/***********************DCAN_0_1**************************/
	pstCM_ALWON->CM_DCAN01_CLKCTRL = 0x2;
	/* POLL for DCAN_0_1 */
	while((pstCM_ALWON->CM_DCAN01_CLKCTRL & 0x2) != 0x2);
/************************************************************/

	//pstCM_ALWON->CM_DEBUGSS_CLKCTRL = 0;

/******************ETHERNET*********************************/
	pstCM_ALWON->CM_ETHERNET_CLKSTCTRL = 0x2;
	pstCM_ALWON->CM_ETHER_0_CLKCTRL = 0x2;
	pstCM_ALWON->CM_ETHER_1_CLKCTRL = 0x2;

	/* POLL for Ethernet i.e. are active
	 * RFT and Ethernet GCLK */
	while((pstCM_ALWON->CM_ETHERNET_CLKSTCTRL & 0x302) != 0x302);

	/* POLL for Ethernet0 */
	while((pstCM_ALWON->CM_ETHER_0_CLKCTRL & 0x2) != 0x2);

	/* POLL for Ethernet1 */
	while((pstCM_ALWON->CM_ETHER_1_CLKCTRL & 0x2) != 0x2);
/***************END OF ETHERNET*******************************/

/***********************GPIO******************************/
	pstCM_ALWON->CM_GPIO0_CLKCTRL = 0x102;
	/* POLL for GPIO0 */
	while((pstCM_ALWON->CM_GPIO0_CLKCTRL & 0x102) != 0x102);

	pstCM_ALWON->CM_GPIO1_CLKCTRL = 0x102;
	/* POLL for GPIO1 */
	while((pstCM_ALWON->CM_GPIO1_CLKCTRL & 0x102) != 0x102);
/********************END OF GPIO**************************/

/************************GPMC****************************/
	pstCM_ALWON->CM_GPMC_CLKCTRL = 0x2;
	/* POLL for GPMC GCLK */
	while((pstCM_ALWON->CM_GPMC_CLKCTRL & 0x2) != 0x2);
/********************************************************/

/***********************I2C******************************/
	pstCM_ALWON->CM_I2C_0_CLKCTRL = 0x2;
	/* POLL for I2C0 GCLK */
	while((pstCM_ALWON->CM_I2C_0_CLKCTRL & 0x2)!= 0x2);

	pstCM_ALWON->CM_I2C_1_CLKCTRL = 0x2;
	/* POLL for I2C1 GCLK */
	while((pstCM_ALWON->CM_I2C_1_CLKCTRL & 0x2) != 0x2);
/*********************END OF I2C**************************/

/*********************L3**********************************/
	/* Clock control for L3 Slow interface */
	pstCM_ALWON->CM_L3SLOW_CLKSTCTRL = 0x2;

	/* L3 Fast */
	pstCM_ALWON->CM_L3FAST_CLKSTCTRL = 0x2;

	/* Clock control for L3 Medium unsigned interface
		 * SECSS */
	pstCM_ALWON->CM_L3MED_CLKSTCTRL = 0x2;

	pstCM_ALWON->CM_L3_CLKCTRL = 0x2;

	/* POLL for L3 Fast GCLK */
	while((pstCM_ALWON->CM_L3FAST_CLKSTCTRL & 0x102)!= 0x102);


	/* POLL for SECSS GCLK */
	while((pstCM_ALWON->CM_L3MED_CLKSTCTRL & 0x102) != 0x102);

	/* POLL for L3 GCLK */
	while((pstCM_ALWON->CM_L3_CLKCTRL & 0x2) != 0x2);


/******************END OF L3*******************************/

/************************L4******************************/
	while((pstCM_ALWON->CM_L4HS_CLKCTRL & 0x2) != 0x2);

	while((pstCM_ALWON->CM_L4LS_CLKCTRL & 0x2) != 0x2);
/********************END OF L4***************************/

/***********************MAILBOX**************************/
	pstCM_ALWON->CM_MAILBOX_CLKCTRL = 0x2;
	/* POLL for MAILBOX GCLK */
	while((pstCM_ALWON->CM_MAILBOX_CLKCTRL & 0x2) != 0x2);
/*********************************************************/

/**********************McASP********************************/
	pstCM_ALWON->CM_MCASP0_CLKCTRL = 0x2;
	/* POLL for McASP0 GCLK */
	while((pstCM_ALWON->CM_MCASP0_CLKCTRL & 0x2) != 0x2);

	pstCM_ALWON->CM_MCASP1_CLKCTRL = 0x2;
	/* POLL for McASP1 GCLK */
	while((pstCM_ALWON->CM_MCASP1_CLKCTRL & 0x2) != 0x2);

	pstCM_ALWON->CM_MCASP2_CLKCTRL = 0x2;
	/* POLL for McASP2 GCLK */
	while((pstCM_ALWON->CM_MCASP2_CLKCTRL & 0x2) != 0x2);

	pstCM_ALWON->CM_MCASP345_CLKCTRL = 0x2;
	/* POLL for McASP3,4,5 GCLK */
	while((pstCM_ALWON->CM_MCASP345_CLKCTRL & 0x2) != 0x2);
/******************END OF McASP***************************/

/***********************McBSP*****************************/
	pstCM_ALWON->CM_MCBSP_CLKCTRL = 0x2;
	/* POLL for McBSP GCLK */
	while((pstCM_ALWON->CM_MCBSP_CLKCTRL & 0x2) != 0x2);
/**********************************************************/

/****************************MLB**************************/
	pstCM_ALWON->CM_MLB_CLKCTRL = 0x2;
	/* POLL for MLB GCLK */
	while((pstCM_ALWON->CM_MLB_CLKCTRL & 0x2) != 0x2);
/**********************************************************/

/**********************MMCHS***************************/
	pstCM_ALWON->CM_MMCHS_0_CLKCTRL = 0x2;
	/* POLL for MMCHS0 GCLK */
	while((pstCM_ALWON->CM_MMCHS_0_CLKCTRL & 0x2) != 0x2);

	pstCM_ALWON->CM_MMCHS_1_CLKCTRL = 0x2;
	/* POLL for MMCHS1 GCLK */
	while((pstCM_ALWON->CM_MMCHS_1_CLKCTRL & 0x2) != 0x2);

	pstCM_ALWON->CM_MMCHS_2_CLKCTRL = 0x2;
	/* POLL for MMCHS2 GCLK */
	while((pstCM_ALWON->CM_MMCHS_2_CLKCTRL & 0x2) != 0x2);
/**************END OF MMCHS*****************************/

/****************************MMU*************************/
	pstCM_ALWON->CM_MMUCLKSTCTRL = 0x2;
	pstCM_ALWON->CM_MMUCFGCLKSTCTRL = 0x2;

	pstCM_ALWON->CM_MMUCFG_CLKCTRL = 0x2;

	pstCM_ALWON->CM_MMUDATA_CLKCTRL = 0x2;

	/* POLL for MMU CFG GCLK */
	while((pstCM_ALWON->CM_MMUCFGCLKSTCTRL & 0x102) != 0x102);

	/* POLL for MMU GCLK */
	while((pstCM_ALWON->CM_MMUCLKSTCTRL & 0x102) != 0x102);

	/* POLL for MMUCFG GCLK */
	while((pstCM_ALWON->CM_MMUCFG_CLKCTRL & 0x2) != 0x2);

	/* POLL for MMUDATA GCLK */
	while((pstCM_ALWON->CM_MMUDATA_CLKCTRL & 0x2) != 0x2);
/**********************************************************/

/********************** MPU*****************************/
	pstCM_ALWON->CM_MPU_CLKSTCTRL = 0x2;
	pstCM_ALWON->CM_MPU_CLKCTRL = 0x2;

	/* POLL for MPU CFG GCLK */
	while((pstCM_ALWON->CM_MPU_CLKSTCTRL & 0x102) != 0x102);
	/* POLL for MPU CFG GCLK */
	while((pstCM_ALWON->CM_MPU_CLKCTRL & 0x2) != 0x2);
/******************END OF MPU*****************************/

/***********************OCMC****************************/
	pstCM_ALWON->CM_OCMC_0_CLKSTCTRL = 0x2;
	pstCM_ALWON->CM_OCMC_0_CLKCTRL = 0x2;

	/* POLL for OCMC0 CFG GCLK */
	while((pstCM_ALWON->CM_OCMC_0_CLKSTCTRL & 0x102) != 0x102);
	/* POLL for OCMC0  GCLK */
	while((pstCM_ALWON->CM_OCMC_0_CLKCTRL & 0x2) != 0x2);
/***************END OF OCMC********************************/

	pstCM_ALWON->CM_PATA_CLKCTRL = 0x2;
	/* POLL for PATA GCLK */
	while((pstCM_ALWON->CM_PATA_CLKCTRL & 0x2) != 0x2);

/************************************RTC***********************************/
	pstCM_ALWON->CM_RTC_CLKSTCTRL = 0x2;
	pstCM_ALWON->CM_RTC_CLKCTRL = 0x2;

	/* POLL for RTC CFG GCLK */
	while((pstCM_ALWON->CM_RTC_CLKSTCTRL & 0x102) != 0x102);
/****************************************************************************/

	pstCM_ALWON->CM_SDIO_CLKCTRL = 0x2;
	/* POLL for SDIO GCLK */
	while((pstCM_ALWON->CM_SDIO_CLKCTRL & 0x2) != 0x2);

	pstCM_ALWON->CM_SECSS_CLKCTRL = 0x2;
	/* POLL for SECSS GCLK */
	while((pstCM_ALWON->CM_SECSS_CLKCTRL & 0x2) != 0x2);

/********************SMARTCARD************************/
	pstCM_ALWON->CM_SMARTCARD0_CLKCTRL = 0x2;
	/* POLL for SMARTCARD0 GCLK */
	while((pstCM_ALWON->CM_SMARTCARD0_CLKCTRL & 0x2) != 0x2);

	pstCM_ALWON->CM_SMARTCARD1_CLKCTRL = 0x2;
	/* POLL for SMARTCARD1 GCLK */
	while((pstCM_ALWON->CM_SMARTCARD1_CLKCTRL & 0x2) != 0x2);
/***********END OF SMARTCARD***************************/

	pstCM_ALWON->CM_SPINBOX_CLKCTRL = 0x2;
	/* POLL for SPINBOX GCLK */
	while((pstCM_ALWON->CM_SPINBOX_CLKCTRL & 0x2) != 0x2);


	pstCM_ALWON->CM_SPI_CLKCTRL = 0x2;
	/* POLL for SPI */
	while((pstCM_ALWON->CM_SPI_CLKCTRL & 0x2) != 0x2);

/**************************SR****************************/
	pstCM_ALWON->CM_SR_0_CLKCTRL = 0x2;
	/* POLL for SR0 */
	while((pstCM_ALWON->CM_SR_0_CLKCTRL & 0x2) != 0x2);

	pstCM_ALWON->CM_SR_1_CLKCTRL = 0x2;
	/* POLL for SR1 */
	while((pstCM_ALWON->CM_SR_1_CLKCTRL & 0x2) != 0x2);

	pstCM_ALWON->CM_SR_2_CLKCTRL = 0x2;
	/* POLL for SR2 */
	while((pstCM_ALWON->CM_SR_2_CLKCTRL & 0x2) != 0x2);

	pstCM_ALWON->CM_SR_3_CLKCTRL = 0x2;
	/* POLL for SR3 */
	while((pstCM_ALWON->CM_SR_3_CLKCTRL & 0x2) != 0x2);
/********************END OF SR****************************/

/*********************SYSCLK4,5,6**************************/
	pstCM_ALWON->CM_SYSCLK4_CLKSTCTRL = 0x2;
	/* POLL for SYSCLK4 CFG GCLK i.e.
	 * L3F L3S L3M and SYSCLK4 */
	while((pstCM_ALWON->CM_SYSCLK4_CLKSTCTRL & 0xF02) != 0xF02);

	/* SYSCLK5 */
	pstCM_ALWON->CM_SYSCLK5_CLKSTCTRL = 0x2;
	/* POLL for SYSCLK5 CFG GCLK i.e.
	 * Debug and SYSCLK5 */
	while((pstCM_ALWON->CM_SYSCLK5_CLKSTCTRL & 0x302) != 0x302);

	/* SYSCLK6 */
	pstCM_ALWON->CM_SYSCLK6_CLKSTCTRL = 0x2;
	/* POLL for SYSCLK6 CFG GCLK i.e.
	 * SYSCLK6 */
	while((pstCM_ALWON->CM_SYSCLK6_CLKSTCTRL & 0x102) != 0x102);
/**************END OF SYSCLK4,5,6***************************/

/***********************TPCC******************************/
	pstCM_ALWON->CM_TPCC_CLKCTRL = 0x2;
	/* POLL for TPCC */
	while((pstCM_ALWON->CM_TPCC_CLKCTRL & 0x2) != 0x2);
/*********************END OF TPCC*************************/

/***********************TPTC******************************/
	pstCM_ALWON->CM_TPTC0_CLKCTRL = 0x2;
	/* POLL for TPTC0 */
	while((pstCM_ALWON->CM_TPTC0_CLKCTRL & 0x2) != 0x2);


	pstCM_ALWON->CM_TPTC1_CLKCTRL = 0x2;
	/* POLL for TPTC1 */
	while((pstCM_ALWON->CM_TPTC1_CLKCTRL & 0x2) != 0x2);

	pstCM_ALWON->CM_TPTC2_CLKCTRL = 0x2;
	/* POLL for TPTC2 */
	while((pstCM_ALWON->CM_TPTC2_CLKCTRL & 0x2) != 0x2);

	pstCM_ALWON->CM_TPTC3_CLKCTRL = 0x2;
	/* POLL for TPTC3 */
	while((pstCM_ALWON->CM_TPTC3_CLKCTRL & 0x2) != 0x2);
/*********************END OF TPTC*************************/

/***********************UART******************************/
	pstCM_ALWON->CM_UART0_CLKCTRL = 0x2;
	/* POLL for UART0 */
	while((pstCM_ALWON->CM_UART0_CLKCTRL & 0x2) != 0x2);

	pstCM_ALWON->CM_UART1_CLKCTRL = 0x2;
	/* POLL for UART1 */
	while((pstCM_ALWON->CM_UART1_CLKCTRL & 0x2) != 0x2);

	pstCM_ALWON->CM_UART2_CLKCTRL = 0x2;
	/* POLL for UART2 */
	while((pstCM_ALWON->CM_UART2_CLKCTRL & 0x2) != 0x2);

	pstCM_ALWON->CM_UART3_CLKCTRL = 0x2;
	/* POLL for UART3 */
	while((pstCM_ALWON->CM_UART3_CLKCTRL & 0x2) != 0x2);

	pstCM_ALWON->CM_UART4_CLKCTRL = 0x2;
	/* POLL for UART4 */
	while((pstCM_ALWON->CM_UART4_CLKCTRL & 0x2) != 0x2);

	pstCM_ALWON->CM_UART5_CLKCTRL = 0x2;
	/* POLL for UART5 */
	while((pstCM_ALWON->CM_UART5_CLKCTRL & 0x2) != 0x2);
/****************END OF UART*******************************/

/***********************VCP******************************/
	pstCM_ALWON->CM_VCP_CLKSTCTRL = 0x2;
	pstCM_ALWON->CM_VCP_CLKCTRL = 0x2;

	/* POLL for VCP CFG GCLK */
	while((pstCM_ALWON->CM_VCP_CLKSTCTRL & 0x102) != 0x102);
	/* POLL for VCP GCLK */
	while((pstCM_ALWON->CM_VCP_CLKCTRL & 0x2) != 0x2);
/******************END OF VCP*****************************/

/**************************WDT***************************/
	pstCM_ALWON->CM_WDTIMER_CLKCTRL = 0x2;
	/* POLL for WDT */
	while((pstCM_ALWON->CM_WDTIMER_CLKCTRL & 0x2) != 0x2);
/*********************************************************/

	/* L3 Slow */
	/* POLL for L3 Slow interface i.e. active
	 * L3 Slow, McASP0,1,2 McBSP UART, GPIO0,1
	 * I2C SPI TIMER0,1,2,3,4,5,6,7 SDIO */
	/* Since TIMER clkctrl registers are not given in CM_ALWON so
	 * I am comparing it with 0xF73FF02 instead of 0xFFFFF02 */
	while((pstCM_ALWON->CM_L3SLOW_CLKSTCTRL & 0xF73FF02) != 0xF73FF02);
}

