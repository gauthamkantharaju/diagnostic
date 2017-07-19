#ifndef _CSLR_GPIO_H_
#define _CSLR_GPIO_H_

#include "C_Types.h"

#define _ARM_ //Disable this to use for DSP.

#ifdef _ARM_
	#define GPIO0_BASE 0x48032000 /*GPIO 0 MODULE BASE ADDRESS*/
	#define GPIO1_BASE 0x4804C000 /*GPIO 1 MODULE BASE ADDRESS*/
	#define GPIO2_BASE 0x481AC000 /*GPIO 2 MODULE BASE ADDRESS*/
	#define GPIO3_BASE 0x481AE000 /*GPIO 3 MODULE BASE ADDRESS*/
#else
	#define GPIO0_BASE 0x08032000 /*GPIO 0 MODULE BASE ADDRESS*/
	#define GPIO1_BASE 0x0804C000 /*GPIO 1 MODULE BASE ADDRESS*/
	#define GPIO2_BASE 0x081AC000 /*GPIO 2 MODULE BASE ADDRESS*/
	#define GPIO3_BASE 0x081AE000 /*GPIO 3 MODULE BASE ADDRESS*/
#endif

/*Macros used to configure GPIO channel as input or output*/
#define OUTPUT 0
#define INPUT 1

/*FUNCTION PROTOTYPES*/
void   DC4_GPIO_Init(UNS32 uibank_baseaddr);
UNS32 DC4_GPIO_ConfigDir(UNS32 uibank_baseaddr,UNS32 uipin_number,UNS32 Dir);
void   DC4_GPIO_Read(UNS32 uibank_baseaddr,UNS32*uiGPIO_value);
UNS32 DC4_GPIO_Set(UNS32 uibank_baseaddr,UNS32 uipin_number);
UNS32 DC4_GPIO_Clear(UNS32 uibank_baseaddr,UNS32 uipin_number);
UNS32 DC4_GPIO_Get(UNS32 uibank_baseaddr,UNS32 pin_number,UNS32*uiGPIO_value);
UNS32 DC4_GPIO_Irq(UNS32 uibank_baseaddr,UNS32 uipin_number,UNS32 uiedge_level_choice);
void DC4_Gpio_Isr(UNS32 uibank_baseaddr,UNS32 uipin_number);

/* return values used in the GPIO functions (for debug puposes)*/

#define  NO_ERR		0
#define  INVALID_PIN 	1


/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/

/**************************************************************************\
* Register Overlay Structure for BANK
\**************************************************************************/

typedef struct  {

    VUNS32 GPIO_REVISION;
    VUNS8  RSVD0[12];
    VUNS32 GPIO_SYSCONFIG;
    VUNS8  RSVD1[12];
    VUNS32 GPIO_EOI;
    VUNS32 GPIO_IRQSTATUS_RAW_0;
    VUNS32 GPIO_IRQSTATUS_RAW_1;
    VUNS32 GPIO_IRQSTATUS_0;
    VUNS32 GPIO_IRQSTATUS_1;
    VUNS32 GPIO_IRQSTATUS_SET_0;
    VUNS32 GPIO_IRQSTATUS_SET_1;
    VUNS32 GPIO_IRQSTATUS_CLR_0;
    VUNS32 GPIO_IRQSTATUS_CLR_1;
    VUNS32 GPIO_IRQWAKEN_0;
    VUNS32 GPIO_IRQWAKEN_1;
    VUNS8  RSVD2[200];
    VUNS32 GPIO_SYSSTATUS;
    VUNS8  RSVD3[24];
    VUNS32 GPIO_CTRL;
    VUNS32 GPIO_OE;
    VUNS32 GPIO_DATAIN;
    VUNS32 GPIO_DATAOUT;
    VUNS32 GPIO_LEVELDETECT0;
    VUNS32 GPIO_LEVELDETECT1;
    VUNS32 GPIO_RISINGDETECT;
    VUNS32 GPIO_FALLINGDETECT;
    VUNS32 GPIO_DEBOUNCENABLE;
    VUNS32 GPIO_DEBOUNCINGTIME;
    VUNS8  RSVD4[56];
    VUNS32 GPIO_CLEARDATAOUT;
    VUNS32 GPIO_SETDATAOUT;

} stGpioBankRegs;


/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile stGpioBankRegs         *pstGpioRegsOvly;

/**************************************************************************\
* Bank and Pin Tokens
\**************************************************************************/

#define GPP0  0
#define GPP1  1
#define GPP2  2
#define GPP3  3
#define GPP4  4
#define GPP5  5
#define GPP6  6
#define GPP7  7
#define GPP8  8
#define GPP9  9
#define GPP10 10
#define GPP11 11
#define GPP12 12
#define GPP13 13
#define GPP14 14
#define GPP15 15
#define GPP16 16
#define GPP17 17
#define GPP18 18
#define GPP19 19
#define GPP20 20
#define GPP21 21
#define GPP22 22
#define GPP23 23
#define GPP24 24
#define GPP25 25
#define GPP26 26
#define GPP27 27
#define GPP28 28
#define GPP29 29
#define GPP30 30
#define GPP31 31

void GPIO_Test_Terminal();

#endif

