/************************************************************************************************************
 *
 * Title			: TIMER_8148.h
 *
 * Application		: TIMERS
 *
 * Author			: Sailesh Myneni
 *
 * Creation Date    : Sep 16, 2011
 *
 * Description      : Hader file to be included along with TIMER_8148.c
 *
 * Limitations      :
 *
 *
 *----------------------------------------------------------------------------
 * Modification History
 *----------------------------------------------------------------------------
 * Ver 	Date       Initials       Description
 *----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------
************************************************************************************************************/

#ifndef TIMER_8148_H_
#define TIMER_8148_H_

#include"C_Types.h"

/*PLSS*/
#define PLLSS_BASE_ADDR 0x481C5000

#define PLLSS_MMR_LOCK (volatile int *)(PLLSS_BASE_ADDR + 0x0040)
#define SYSCLK18_CLKSRC (volatile int *)(PLLSS_BASE_ADDR + 0x02F0)
#define DMTIMER_CLKSRC  (volatile int *)(PLLSS_BASE_ADDR + 0x02E0)

/*PRCM*/
#define PRCM_BASE_ADDR 0x48180000

#define CM_ALWON_L3_SLOW_CLKSTCTRL (volatile int *)(PRCM_BASE_ADDR + 0x1400)

#define CM_SYSCLK18_CLKSEL (volatile int *)(PRCM_BASE_ADDR + 0x0378)

#define CM_TIMER1_CLKSEL (volatile int *)(PRCM_BASE_ADDR + 0x0390)
#define CM_TIMER2_CLKSEL (volatile int *)(PRCM_BASE_ADDR + 0x0394)
#define CM_TIMER3_CLKSEL (volatile int *)(PRCM_BASE_ADDR + 0x0398)
#define CM_TIMER4_CLKSEL (volatile int *)(PRCM_BASE_ADDR + 0x039C)
#define CM_TIMER5_CLKSEL (volatile int *)(PRCM_BASE_ADDR + 0x03A0)
#define CM_TIMER6_CLKSEL (volatile int *)(PRCM_BASE_ADDR + 0x03A4)
#define CM_TIMER7_CLKSEL (volatile int *)(PRCM_BASE_ADDR + 0x03A8)
#define CM_TIMER8_CLKSEL (volatile int *)(PRCM_BASE_ADDR + 0x03AC)


/*TIMERS(1-8) BASE ADDRESSES*/
#define TIMER1_BASE_ADDR 0x4802E000
#define TIMER2_BASE_ADDR 0x48040000
#define TIMER3_BASE_ADDR 0x48042000
#define TIMER4_BASE_ADDR 0x48044000
#define TIMER5_BASE_ADDR 0x48046000
#define TIMER6_BASE_ADDR 0x48048000
#define TIMER7_BASE_ADDR 0x4804A000
#define TIMER8_BASE_ADDR 0x481C1000

#define AUTO_RELOAD_MODE (1<<1)
#define ONE_SHOT_MODE (0<<1)


/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/


typedef struct  {

    VUNS32 TIDR;
    VUNS8  RSVD0[12];
    VUNS32 TIOCP_CFG;
    VUNS8  RSVD1[12];
    VUNS32 IRQ_EOI;
    VUNS32 IRQSTATUS_RAW;
    VUNS32 IRQSTATUS;
    VUNS32 IRQSTATUS_SET;
    VUNS32 IRQSTATUS_CLR;
    VUNS32 IRQWAKEEN;
    VUNS32 TCLR;
    VUNS32 TCRR;
    VUNS32 TLDR;
    VUNS32 TTGR;
    VUNS32 TWPS;
    VUNS32 TMAR;
    VUNS32 TCAR1;
    VUNS32 TSICR;
    VUNS32 TCAR2;

} stTimerRegs;

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile stTimerRegs         *pstTimerRegsOvly;


/*FUNCTION PROTOTYPES*/
void DC4_Timer_init();
void DC4_Timer_Enable(UNS32 uiSel_Timer);
void DC4_Timer_Rate_Sel(UNS32 uiSel_Timer,UNS32 uitimer_rate_msec,UNS32 uiAuto_oneshot_modesel);
void DC4_Timer_Stop(UNS32 uiuiSel_Timer);


#endif /* TIMER_8148_H_ */
