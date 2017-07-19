/* Title: ML_structC.h
 * Application: PLL Configuration
 * Copyright: HCL Technologies
 * Author: Gautham.K
 * Creation Date: 18/5/2011
 * Requirements: Header file to be included in ML_PLLConfig.c
 * Description: Structure declaration for PLL module
 * Limitations: NONE
 * Dependencies: TMS470 Compiler
 * Modification: Initial Version*/

#ifndef ML_PLLSTRUCTC_H_
#define ML_PLLSTRUCTC_H_

typedef struct _pllARM
{
	volatile unsigned int iPLL_ARM_PWRCTRL;
	volatile unsigned int iPLL_ARM_CLKCTRL;
	volatile unsigned int iPLL_ARM_TENABLE;
	volatile unsigned int iPLL_ARM_TENABLEDIV;
	volatile unsigned int iPLL_ARM_M2NDIV;
	volatile unsigned int iPLL_ARM_MN2DIV;
	volatile unsigned int iPLL_ARM_FRACDIV;
	volatile unsigned int iPLL_ARM_BWCTRL;
	volatile unsigned int iPLL_ARM_FRACCTRL;
	volatile unsigned int iPLL_ARM_STATUS;
	volatile unsigned int iPLL_ARM_M3DIV;
	volatile unsigned int iPLL_ARM_RAMPCTRL;
}stARMPLL;

typedef struct _pll
{
	volatile unsigned int iPWRCTRL;
	volatile unsigned int iCLKCTRL;
	volatile unsigned int iTENABLE;
	volatile unsigned int iTENABLEDIV;
	volatile unsigned int iM2NDIV;
	volatile unsigned int iMN2DIV;
	volatile unsigned int iFRACDIV;
	volatile unsigned int iBWCTRL;
	volatile unsigned int iFRACCTRL;
	volatile unsigned int iSTATUS;
}stPLL;

typedef struct _CM_ClkSel
{
	volatile unsigned char reserved[8];
	volatile unsigned int CM_SYSCLK3_CLKSEL;
	volatile unsigned char reserved1[20];
	volatile unsigned int CM_SYSCLK10_CLKSEL;
	volatile unsigned char reserved2[16];
	volatile unsigned int CM_VPB3_CLKSEL;
	volatile unsigned int CM_VPC1_CLKSEL;
	volatile unsigned int CM_VPD1_CLKSEL;
	volatile unsigned int CM_SYSCLK19_CLKSEL;
	volatile unsigned int CM_SYSCLK20_CLKSEL;
	volatile unsigned int CM_SYSCLK21_CLKSEL;
	volatile unsigned int CM_SYSCLK22_CLKSEL;
	volatile unsigned int CM_APA_CLKSEL;
	volatile unsigned int CM_SYSCLK14_CLKSEL;
	volatile unsigned int CM_SYSCLK16_CLKSEL;
	volatile unsigned int CM_SYSCLK18_CLKSEL;
	volatile unsigned int CM_AUDIOCLK_MCASP0_CLKSEL;
	volatile unsigned int CM_AUDIOCLK_MCASP1_CLKSEL;
	volatile unsigned int CM_AUDIOCLK_MCASP2_CLKSEL;
	volatile unsigned int CM_AUDIOCLK_MCBSP_CLKSEL;
	volatile unsigned int CM_TIMER0_CLKSEL; /*0x038C*/
	volatile unsigned int CM_TIMER1_CLKSEL;
	volatile unsigned int CM_TIMER2_CLKSEL;
	volatile unsigned int CM_TIMER3_CLKSEL;
	volatile unsigned int CM_TIMER4_CLKSEL;
	volatile unsigned int CM_TIMER5_CLKSEL;
	volatile unsigned int CM_TIMER6_CLKSEL;
	volatile unsigned int CM_TIMER7_CLKSEL;
	volatile unsigned int CM_HDMI_CLKSEL;
	volatile unsigned int CM_SYSCLK23_CLKSEL;
}stCM_CLKSEL;

typedef struct _CM_Default
{
	//volatile unsigned char reserved[8];
	volatile unsigned int CM_D_TPPSS_CLKSTCTRL;
	volatile unsigned int CM_D_PCI_CLKSTCTRL;
	volatile unsigned char reserved1[4];
	volatile unsigned int CM_D_DUCATI_CLKSTCTRL;
	volatile unsigned char reserved2[4];
	volatile unsigned int CM_D_EMIF_0_CLKCTRL;
	volatile unsigned int CM_D_EMIF_1_CLKCTRL;
	volatile unsigned int CM_D_DMM_CLKCTRL;
	volatile unsigned int CM_D_FW_CLKCTRL;
	volatile unsigned char reserved3[36];
	volatile unsigned int CM_D_TPPSS_CLKCTRL;
	volatile unsigned int CM_D_USB_CLKCTRL;
	volatile unsigned char reserved4[4];
	volatile unsigned int CM_D_SATA_CLKCTRL;
	volatile unsigned char reserved5[16];
	volatile unsigned int CM_D_DUCATI_CLKCTRL;
	volatile unsigned int CM_D_PCI_CLKCTRL;
}stCM_DEFAULT;

typedef struct _CM_Alwon
{
	unsigned int CM_L3SLOW_CLKSTCTRL;
	unsigned int CM_ETHERNET_CLKSTCTRL;
	unsigned int CM_L3MED_CLKSTCTRL;
	unsigned int CM_MMUCLKSTCTRL;
	unsigned int CM_MMUCFGCLKSTCTRL;
	unsigned int CM_OCMC_0_CLKSTCTRL;
	unsigned int CM_VCP_CLKSTCTRL;
	unsigned int CM_MPU_CLKSTCTRL;
	unsigned int CM_SYSCLK4_CLKSTCTRL;
	unsigned int CM_SYSCLK5_CLKSTCTRL;
	unsigned int CM_SYSCLK6_CLKSTCTRL;
	unsigned int CM_RTC_CLKSTCTRL;
	unsigned int CM_L3FAST_CLKSTCTRL;
	unsigned char resv[268];
	unsigned int CM_MCASP0_CLKCTRL;
	unsigned int CM_MCASP1_CLKCTRL;
	unsigned int CM_MCASP2_CLKCTRL;
	unsigned int CM_MCBSP_CLKCTRL;
	unsigned int CM_UART0_CLKCTRL;
	unsigned int CM_UART1_CLKCTRL;
	unsigned int CM_UART2_CLKCTRL;
	unsigned int CM_GPIO0_CLKCTRL;
	unsigned int CM_GPIO1_CLKCTRL;
	unsigned int CM_I2C_0_CLKCTRL;
	unsigned int CM_I2C_1_CLKCTRL;
	unsigned int CM_MCASP345_CLKCTRL;
	unsigned int CM_ATL_CLKCTRL;
	unsigned int CM_MLB_CLKCTRL;
	unsigned int CM_PATA_CLKCTRL;
	unsigned char reserved[4];
	unsigned int CM_UART3_CLKCTRL;
	unsigned int CM_UART4_CLKCTRL;
	unsigned int CM_UART5_CLKCTRL;
	unsigned int CM_WDTIMER_CLKCTRL;
	unsigned int CM_SPI_CLKCTRL;
	unsigned int CM_MAILBOX_CLKCTRL;
	unsigned int CM_SPINBOX_CLKCTRL;
	unsigned int CM_MMUDATA_CLKCTRL;
	unsigned char reserved1[8];
	unsigned int CM_MMUCFG_CLKCTRL;
	unsigned char reserved2[4];
	unsigned int CM_SDIO_CLKCTRL;
	unsigned int CM_OCMC_0_CLKCTRL;
	unsigned int CM_VCP_CLKCTRL;
	unsigned int CM_SMARTCARD0_CLKCTRL;
	unsigned int CM_SMARTCARD1_CLKCTRL;
	unsigned int CM_CONTROL_CLKCTRL;
	unsigned int CM_SECSS_CLKCTRL;
	unsigned char reserved3[4];
	unsigned int CM_GPMC_CLKCTRL;
	unsigned int CM_ETHER_0_CLKCTRL;
	unsigned int CM_ETHER_1_CLKCTRL;
	unsigned int CM_MPU_CLKCTRL;
	unsigned int CM_DEBUGSS_CLKCTRL;
	unsigned int CM_L3_CLKCTRL;
	unsigned int CM_L4HS_CLKCTRL;
	unsigned int CM_L4LS_CLKCTRL;
	unsigned int CM_RTC_CLKCTRL;
	unsigned int CM_TPCC_CLKCTRL;
	unsigned int CM_TPTC0_CLKCTRL;
	unsigned int CM_TPTC1_CLKCTRL;
	unsigned int CM_TPTC2_CLKCTRL;
	unsigned int CM_TPTC3_CLKCTRL;
	unsigned int CM_SR_0_CLKCTRL;
	unsigned int CM_SR_1_CLKCTRL;
	unsigned int CM_SR_2_CLKCTRL;
	unsigned int CM_SR_3_CLKCTRL;
	unsigned int CM_DCAN01_CLKCTRL;
	unsigned int CM_MMCHS_0_CLKCTRL;
	unsigned int CM_MMCHS_1_CLKCTRL;
	unsigned int CM_MMCHS_2_CLKCTRL;
	unsigned int CM_EFUSE_CLKCTRL;
}stCM_ALWON;

typedef struct _clkSRC
{
	unsigned int OSC_SRC;
	unsigned int ARM_CLKSRC;
	unsigned int VIDEO_PLL_CLKSRC;
	unsigned int MLB_ATL_CLKSRC;
	unsigned int McASP345_AUX_CLKSRC;
	unsigned int McASP_AHCLK_CLKSRC;
	unsigned int McBSP_UART_CLKSRC;
	unsigned int HDMI_I2S_CLKSRC;
	unsigned int DMTIMER_CLKSRC;
	unsigned int CLKOUT_MUX;
	unsigned int RMII_REFCLK_SRC;
	unsigned char reserved[4];
	unsigned int SYSCLK18_CLKSRC;
	unsigned int WDT0_CLKSRC;
}stClkSrc;

#endif /*ML_STRUCTC_H_*/
