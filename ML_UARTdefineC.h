/* Title: ML_defineC.h
 * Application: UART Configuration
 * Copyright: HCL Technologies
 * Author: Gautham.K
 * Creation Date: 27/5/2011
 * Requirements: Header file to be included in ML_UART.c
 * Description: Defining the address of the UART configuration registers
 * Limitations: NONE
 * Dependencies: TMS320DM8148 Compiler  
 * Modification: Initial Version*/

#ifndef ML_UARTDEFINEC_H_
#define ML_UARTDEFINEC_H_

#include "C_Types.h"

#define SUCCESS					0

#define _ARM_ //Disable this to use for DSP.

#ifdef _ARM_
#define UART0_BASE_ADDRESS 			0x48020000
#define UART1_BASE_ADDRESS 			0x48022000
#define UART2_BASE_ADDRESS 			0x48024000
#define UART3_BASE_ADDRESS 			0x481A6000
#define UART4_BASE_ADDRESS 			0x481A8000
#define UART5_BASE_ADDRESS 			0x481AA000
#else
#define UART0_BASE_ADDRESS 			0x08020000
#define UART1_BASE_ADDRESS 			0x08022000
#define UART2_BASE_ADDRESS 			0x08024000
#define UART3_BASE_ADDRESS 			0x081A6000
#define UART4_BASE_ADDRESS 			0x081A8000
#define UART5_BASE_ADDRESS 			0x081AA000
#endif

#define RBR					0x0
#define THR					0x0
#define DLL					0x0
#define DLH					0x4
#define IER					0x4
#define FCR					0x8
#define EFR					0x8
#define LCR					0xC
#define MCR					0x10
#define LSR					0x14
#define TCR					0x18
#define TLR					0x1C
#define MDR1					0x20
#define MDR2					0x24
#define SCR					0x40


#define PRCM_BASE_ADDR				0x48180000
#define pCM_ALWON_L3_SLOW_CLKSTCTRL		(volatile unsigned int *)(PRCM_BASE_ADDR + 0x1400)
#define pCM_ALWON_UART0_CLKCTRL  		(volatile unsigned int *)(PRCM_BASE_ADDR + 0x1550)
#define pCM_ALWON_UART1_CLKCTRL  		(volatile unsigned int *)(PRCM_BASE_ADDR + 0x1554)
#define pCM_ALWON_UART2_CLKCTRL  		(volatile unsigned int *)(PRCM_BASE_ADDR + 0x1558)
#define pCM_ALWON_UART3_CLKCTRL  		(volatile unsigned int *)(PRCM_BASE_ADDR + 0x1580)
#define pCM_ALWON_UART4_CLKCTRL  		(volatile unsigned int *)(PRCM_BASE_ADDR + 0x1584)
#define pCM_ALWON_UART5_CLKCTRL  		(volatile unsigned int *)(PRCM_BASE_ADDR + 0x1588)
#define pCM_SYSCLK10_CLKSEL			(volatile unsigned int *)(PRCM_BASE_ADDR + 0x0324)
#define pCM_DEFAULT_USB_CLKCTRL         	(volatile unsigned int *)(PRCM_BASE_ADDR + 0x0558)
#define pBSP_UART_CLKSRC			(volatile unsigned int *)(PRCM_BASE_ADDR + 0x02D8)

#define SYSCLK8					192000000
#define SYSCLK10				48000000

void DC4_UART_Config(unsigned int uiUartControllerAddr, unsigned int uiBaudRate);
void DC4_UART_Read(unsigned int uiUartControllerAddr, unsigned char *pucRead, unsigned int uiNoOfBytes);
void DC4_UART_Write(unsigned int uiUartControllerAddr, unsigned char *pucWrite, unsigned int uiNoOfBytes);
void DC4_Baudrate_Config(unsigned int uiBaudrate);

UNS16 ReadByte(UNS32 uiCntrlAddr);

#endif /*ML_DEFINEC_H_*/
