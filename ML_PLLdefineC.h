/* Title: ML_defineC.h
 * Application: PLL Configuration
 * Copyright: HCL Technologies
 * Author: Gautham.K
 * Creation Date: 18/5/2011
 * Requirements: Header file to be included in ML_PLLConfig.c
 * Description: Defining the address of the PLL configuration registers
 * Limitations: NONE
 * Dependencies: TMS470 Compiler
 * Modification: Initial Version*/
 
#ifndef ML_PLLDEFINEC_H_
#define ML_PLLDEFINEC_H_

#define ADPLLJ_CLKCRTL_HS2       0x00000801 //HS2 Mode,TINTZ =1  --used by all PLL's except HDMI
#define ADPLLJ_CLKCRTL_HS1       0x00001001 //HS1 Mode,TINTZ =1  --used only for HDMI
#define ADPLLJ_CLKCRTL_CLKDCO    0x200a0000 //Enable CLKDCOEN,CLKLDOEN,CLKDCOPWDNZ -- used for HDMI,USB

#define PRCM_BASE_ADDR			   0x48180000

#define CM_CLKSEL				   0x48180300
#define CM_DEFAULT				   0x4818050C
#define CM_ALWON				   0x48181400

#define ARMPLL                     0x481C5048
#define DSPPLL                     0x481C5080
#define L3PLL                      0x481C5110
#define DDRPLL 			           0x481C5290
#define USBPLL					   0x481C5260
#define AUDIO                      0x481C5230
#define VIDEO0                     0x481C51A0
#define VIDEO1                     0x481C51D0
#define HDMI                       0x481C5200
#define SGX						   0x481C50B0
#define IVA                        0x481C50E0 //HDVICP
#define ISS                        0x481C5140 //MEDIA CONTROLLER
#define DSS						   0x481C5170 //HDVPSS

#define CLK_SRC					   0x481C52C0

#define pPRM_RSTCTRL               ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x00A0))
#define pPRM_RSTTIME               ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x00A4))
#define pPRM_RSTST                 ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x00A8))

#define pCM_CLKOUT_CTRL            ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0100))

#define pCM_GEM_CLKSTCTRL          ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0400))
#define pCM_ACTIVE_GEM_CLKCTRL     ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0420))

#define pCM_IVAHD0_CLKSTCTRL       ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0600))
#define pCM_IVAHD0_IVAHD_CLKCTRL   ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0620))
#define pCM_IVAHD0_SL2_CLKCTRL     ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0624))

#define pCM_ISP_CLKSTCTRL          ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0700))
#define pCM_ISP_ISP_CLKCTRL        ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0720))
#define pCM_ISP_FDIF_CLKCTRL       ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0724))

#define pCM_DSS_CLKSTCTRL          ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0800))
#define pCM_DSS_DSS_CLKCTRL        ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0820))
#define pCM_DSS_HDMI_CLKCTRL       ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0824))

#define pCM_SGX_CLKSTCTRL          ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0900))
#define pCM_SGX_SGX_CLKCTRL        ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0920))

#define pPM_ACTIVE_PWRSTCTRL       ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0A00))
#define pPM_ACTIVE_PWRSTST         ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0A04))
#define pRM_ACTIVE_RSTCTRL         ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0A10))
#define pRM_ACTIVE_RSTST           ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0A14))

#define pRM_DEFAULT_RSTCTRL        ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0B10))
#define pRM_DEFAULT_RSTST          ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0B14))

#define pPM_IVAHD0_PWRSTCTRL       ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0C00))
#define pPM_IVAHD0_PWRSTST         ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0C04))
#define pRM_IVAHD0_RSTCTRL         ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0C10))
#define pRM_IVAHD0_RSTST           ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0C14))

#define pPM_ISP_PWRSTCTRL          ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0D00))
#define pPM_ISP_PWRSTST            ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0D04))
#define pRM_ISP_RSTCTRL            ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0D10))
#define pRM_ISP_RSTST              ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0D14))

#define pPM_DSS_PWRSTCTRL          ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0E00))
#define pPM_DSS_PWRSTST            ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0E04))
#define pRM_DSS_RSTCTRL            ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0E10))
#define pRM_DSS_RSTST              ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0E14))

#define pPM_SGX_PWRSTCTRL          ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0F00))
#define pPM_SGX_PWRSTST            ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0F10))
#define pRM_SGX_RSTCTRL            ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0F04))
#define pRM_SGX_RSTST              ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x0F14))

#define pPM_EFUSE_PWRSTST          ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x1804))
#define pRM_ALWON_RSTCTRL          ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x1810))
#define pRM_ALWON_RSTST            ((volatile unsigned int *)(PRCM_BASE_ADDR + 0x1814))


#define	pCONTROl_SYSCONFIG         ((volatile unsigned int *) 0x481C5010)
#define pPLL_MMR_LOCK              ((volatile unsigned int *) 0x481C5040)
#define pCLKOUT_MUX				   ((volatile unsigned int *) 0x481C52E4)


#define pMMR_LOCK0                ((volatile unsigned int *) 0x48140060)
#define pMMR_LOCK1                ((volatile unsigned int *) 0x48140064)
#define pMMR_LOCK2                ((volatile unsigned int *) 0x48140068)
#define pMMR_LOCK3                ((volatile unsigned int *) 0x4814006C)
#define pMMR_LOCK4                ((volatile unsigned int *) 0x48140070)

void DC4_PLL_MMR_UNLOCK();
void DC4_PLL_PERIPHERALS_SOURCE_SEL(unsigned int uiBaseAddr);
void DC4_PLL_MMR_LOCK();
void DC4_PLL_ARM(unsigned int uiBaseaddr, unsigned int uiN, unsigned int uiM, unsigned int uiM2);
void DC4_PLL_Peripherals(unsigned int uiBaseaddr, unsigned int uiN, unsigned int uiM, unsigned int uiM2, unsigned int uiClkctrl_Val);
void DC4_PRCM_CM_CLKSEL(unsigned int uiBaseAddr);
void DC4_PRCM_CM_DEFAULT(unsigned int uiBaseAddr);
void DC4_PRCM_CM_ALWON(unsigned int uiBaseAddr);

#endif /*ML_DEFINEC_H_*/
