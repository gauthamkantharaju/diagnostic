/* Title: ML_defineC.h
 * Application: DDR Configuration
 * Copyright: HCL Technologies
 * Author: Gautham.K
 * Creation Date: 27/5/2011
 * Requirements: Header file to be included in ML_DDR.c
 * Description: Defining the address of the DMM, EMIF and PRCM configuration registers
 * Limitations: NONE
 * Dependencies: TMS320DM8148 Compiler  
 * Modification: Initial Version*/

#ifndef ML_DDRDEFINEC_H_
#define ML_DDRDEFINEC_H_

#define PRCM_BASE_ADDR					0x48180000

#define DDR0_BASE_ADDR					0x80000000
#define DDR1_BASE_ADDR					0x90000000

#define COUNTER_PATTERN                 0
#define PATTERN_1						0xA5A5A5A5
#define PATTERN_2						0x5A5A5A5A
#define PATTERN_3						0x01234567

#define BYTE							8
#define SHORT							16
#define WORD							32

#define pVTP0_CTRL_REG       			((volatile unsigned int *)0x48140E0C)
#define pVTP1_CTRL_REG       			((volatile unsigned int *)0x48140E10)
#define pCM_DEFAULT_L3_FAST_CLKSTCTRL	(volatile unsigned int *)(PRCM_BASE_ADDR + 0x0508)
#define pCM_DEFAULT_DDR_0_CLKCTRL		(volatile unsigned int *)(PRCM_BASE_ADDR + 0x0520)
#define pCM_DEFAULT_DDR_1_CLKCTRL    	(volatile unsigned int *)(PRCM_BASE_ADDR + 0x0524)
#define pCM_DEFAULT_DMM_CLKCTRL 		(volatile unsigned int *)(PRCM_BASE_ADDR + 0x0528)
#define pCM_DEFAULT_FW_CLKCTRL 			(volatile unsigned int *)(PRCM_BASE_ADDR + 0x052C)

#define DMM_BASE_ADDR					0x4E000000
#define pDMM_SYSCONFIG					(volatile unsigned int *)(DMM_BASE_ADDR + 0x10)
#define pDMM_LISA_LOCK					(volatile unsigned int *)(DMM_BASE_ADDR + 0x1C)
#define pDMM_LISA_MAP_0				    (volatile unsigned int *)(DMM_BASE_ADDR + 0x40)
#define pDMM_LISA_MAP_1				    (volatile unsigned int *)(DMM_BASE_ADDR + 0x44)
#define pDMM_LISA_MAP_2				    (volatile unsigned int *)(DMM_BASE_ADDR + 0x48)
#define pDMM_LISA_MAP_3				    (volatile unsigned int *)(DMM_BASE_ADDR + 0x4C)
#define pDMM_PAT_BASE_ADDR 				(volatile unsigned int *)(DMM_BASE_ADDR + 0x460)

#define DDR_0_CFG_BASE_ADDR				0x4C000000
#define pDDR_0_SDRAM_CONFIG				(volatile unsigned int *)(DDR_0_CFG_BASE_ADDR + 0x08)
#define pDDR_0_SDRAM_CONFIG2			(volatile unsigned int *)(DDR_0_CFG_BASE_ADDR + 0x0C)
#define pDDR_0_SDRAM_REF_CTRL			(volatile unsigned int *)(DDR_0_CFG_BASE_ADDR + 0x10)
#define pDDR_0_SDRAM_REF_CTRL_SHADOW	(volatile unsigned int *)(DDR_0_CFG_BASE_ADDR + 0x14)
#define pDDR_0_SDRAM_TIM_1				(volatile unsigned int *)(DDR_0_CFG_BASE_ADDR + 0x18)
#define pDDR_0_SDRAM_TIM_1_SHADOW		(volatile unsigned int *)(DDR_0_CFG_BASE_ADDR + 0x1C)
#define pDDR_0_SDRAM_TIM_2				(volatile unsigned int *)(DDR_0_CFG_BASE_ADDR + 0x20)
#define pDDR_0_SDRAM_TIM_2_SHADOW		(volatile unsigned int *)(DDR_0_CFG_BASE_ADDR + 0x24)
#define pDDR_0_SDRAM_TIM_3				(volatile unsigned int *)(DDR_0_CFG_BASE_ADDR + 0x28)
#define pDDR_0_SDRAM_TIM_3_SHADOW		(volatile unsigned int *)(DDR_0_CFG_BASE_ADDR + 0x2C)
#define pDDR_0_DDR_PHY_CTRL_1			(volatile unsigned int *)(DDR_0_CFG_BASE_ADDR + 0xE4)
#define pDDR_0_DDR_PHY_CTRL_1_SHADOW	(volatile unsigned int *)(DDR_0_CFG_BASE_ADDR + 0xE8)
#define pDDR_0_IODFT_TLGC			    (volatile unsigned int *)(DDR_0_CFG_BASE_ADDR + 0x60)

#define DDR_1_CFG_BASE_ADDR				0x4D000000
#define pDDR_1_SDRAM_CONFIG				(volatile unsigned int *)(DDR_1_CFG_BASE_ADDR + 0x08)
#define pDDR_1_SDRAM_CONFIG2			(volatile unsigned int *)(DDR_1_CFG_BASE_ADDR + 0x0C)
#define pDDR_1_SDRAM_REF_CTRL			(volatile unsigned int *)(DDR_1_CFG_BASE_ADDR + 0x10)
#define pDDR_1_SDRAM_REF_CTRL_SHADOW	(volatile unsigned int *)(DDR_1_CFG_BASE_ADDR + 0x14)
#define pDDR_1_SDRAM_TIM_1				(volatile unsigned int *)(DDR_1_CFG_BASE_ADDR + 0x18)
#define pDDR_1_SDRAM_TIM_1_SHADOW		(volatile unsigned int *)(DDR_1_CFG_BASE_ADDR + 0x1C)
#define pDDR_1_SDRAM_TIM_2				(volatile unsigned int *)(DDR_1_CFG_BASE_ADDR + 0x20)
#define pDDR_1_SDRAM_TIM_2_SHADOW		(volatile unsigned int *)(DDR_1_CFG_BASE_ADDR + 0x24)
#define pDDR_1_SDRAM_TIM_3				(volatile unsigned int *)(DDR_1_CFG_BASE_ADDR + 0x28)
#define pDDR_1_SDRAM_TIM_3_SHADOW		(volatile unsigned int *)(DDR_1_CFG_BASE_ADDR + 0x2C)
#define pDDR_1_DDR_PHY_CTRL_1			(volatile unsigned int *)(DDR_1_CFG_BASE_ADDR + 0xE4)
#define pDDR_1_DDR_PHY_CTRL_1_SHADOW	(volatile unsigned int *)(DDR_1_CFG_BASE_ADDR + 0xE8)

#define BANK0							0x00010630 //Since some part of sections are used from BANK0
#define BANK1							0x01000000
#define BANK2							0x02000000
#define BANK3							0x03000000
#define BANK4							0x04000000
#define BANK5							0x05000000
#define BANK6							0x06000000
#define BANK7							0x07000000

void DC4_DDR3_Init();
void DC4_DDR2_Init();
void DC4_DDR_Write(unsigned int uiController_BaseAddr, unsigned int uiBank_Offset, unsigned int uiDataLen, unsigned int uiPattern, unsigned char ucAccessLength);
unsigned int DC4_DDR_Read(unsigned int uiController_BaseAddr, unsigned int uiBank_Offset, unsigned int uiDataLen, unsigned int uiPattern, unsigned char ucAccessLength);

#endif /*ML_DEFINEC_H_*/
