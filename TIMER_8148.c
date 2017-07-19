/************************************************************************************************************
 *
 * Title			  : TIMER_8148.c
 *
 * Application		  : TIMERS
 *
 * Author			  : Sailesh Myneni
 *
 * Creation Date      : Sep 16, 2011
 *
 * Description        : Source file for the TIMER functions like starting and stopping of timers
 *
 *
 * Limitations(if any):
 *
 *----------------------------------------------------------------------------
 * Modification History
 *----------------------------------------------------------------------------
 * Ver 	Date       		Initials       Description
 *----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------
************************************************************************************************************/

#include"C_Types.h"
#include"TIMER_8148.h"
 	 	 	 	 	 	 	 	 	 /*FUNCTIONS*/

/*************************************************************************************************************

Name            : DC4_Timer_init

Description     : This function initializes the clocks required for timers in the PRCM and PLLSS modules

Prototype       : void DC4_Timer_init()

Parameters      : None

Returns         : None

*************************************************************************************************************/

void DC4_Timer_init()
{
	/*initilaization in PRCM module*/

//	*CM_ALWON_L3_SLOW_CLKSTCTRL = 0x02;           // enabling L3 slow clock control
//	 while((*(CM_ALWON_L3_SLOW_CLKSTCTRL) & 0x02) != 0x02);	 // polling for L3 clock control

	*CM_SYSCLK18_CLKSEL = 0x01 ;

	*CM_TIMER1_CLKSEL = 0x02;
	*CM_TIMER2_CLKSEL = 0x02;
	*CM_TIMER3_CLKSEL = 0x02;
	*CM_TIMER4_CLKSEL = 0x02;
	*CM_TIMER5_CLKSEL = 0x02;
	*CM_TIMER6_CLKSEL = 0x02;
	*CM_TIMER7_CLKSEL = 0x02;
	*CM_TIMER8_CLKSEL = 0x02;

	/*initialization in PLLSS registers*/

	//*PLLSS_MMR_LOCK = 0x1EDA4C3D;

	*SYSCLK18_CLKSRC=0x0;					     // selecting clock source for SYSCLK18
	*DMTIMER_CLKSRC=0x0000000;				     // selecting TIMER clock source

}

/*************************************************************************************************************

Name            : DC4_Timer_Enable

Description     : This function is used to enable the TIMERS(1/2/3/4/5/6/7/8)operation,timer runs from
				  1 to  overfow value

Prototype       : void DC4_Timer_Enable(UNS32 uiSel_Timer)

Parameters      : uiSel_Timer - This parameter is used to select the base address of the Timers1/2/3/4/5/6/7/8
								for timer operation.

Returns         : None

*************************************************************************************************************/

void DC4_Timer_Enable(UNS32 uiSel_Timer)
{
	stTimerRegs *pstTimerRegsOvly = (stTimerRegs *) uiSel_Timer;

	UNS32 uitimer_value;

	pstTimerRegsOvly->TIOCP_CFG = (1<<0);        // initiating a software reset
	while((pstTimerRegsOvly->TIOCP_CFG & 0x01)); // polling whether reset is complete or not

	pstTimerRegsOvly->TIOCP_CFG = 0x04;          //configuring the IDLEMODE and EMUFREE bits.

	pstTimerRegsOvly->TSICR = (1<<2);             //posted mode active

	//pstTimerRegsOvly->TCLR = (1<<5);              //enabling divider input

	//while(pstTimerRegsOvly->TWPS & 0x01);
	//pstTimerRegsOvly->TCLR |= (1<<3|0<<2);		  //setting the prescalar ratio (/8)

	while(pstTimerRegsOvly->TWPS & 0x01);
	pstTimerRegsOvly->TCLR |= (1<<1);			 // enabling the Auto reload mode

	while(pstTimerRegsOvly->TWPS & 0x04);
	pstTimerRegsOvly->TLDR = 0x00000001;		 // setting the load register value

	while(pstTimerRegsOvly->TWPS & 0x02);
	pstTimerRegsOvly->TCRR = 0x00000001;		 // setting the counter register value


	while(pstTimerRegsOvly->TWPS & 0x01);
	pstTimerRegsOvly->TCLR |= (1<<0);            // starting the counter

	//while(1);


}

/*************************************************************************************************************

Name            : DC4_Timer_Rate_Sel

Description     : This function is used to enable the TIMERS(1/2/3/4/5/6/7/8),which will run the timer for
				  a particular time(msec) as user wants

Prototype       : void DC4_Timer_Rate_Sel(UNS32 uiSel_Timer,UNS32 uitimer_rate_msec)

Parameters      : uiSel_Timer - This parameter is used to select the base address of the Timers1/2/3/4/5/6/7/8
 	 	 	 	 	 	 	 	for timer operation

				  uitimer_rate_msec - This parameter is used to run the counter for the time slected by
				  	  	  	  	  	  the user

				  uiAuto_oneshot_modesel - this parameter is to select auto reload mode aor one shot mode for
				  	  	  	  	  	  	   the time(uitimer_rate_msec) chosen by user.

Returns         : None

*************************************************************************************************************/

void DC4_Timer_Rate_Sel(UNS32 uiSel_Timer,UNS32 uitimer_rate_msec,UNS32 uiAuto_oneshot_modesel)
{

	stTimerRegs *pstTimerRegsOvly = (stTimerRegs *) uiSel_Timer;

	pstTimerRegsOvly->TIOCP_CFG = (1<<0);        // initiating a software reset
	while((pstTimerRegsOvly->TIOCP_CFG & 0x01)); // polling whether reset is complete or not

	pstTimerRegsOvly->TIOCP_CFG = 0x04;          //configuring the IDLEMODE and EMUFREE bits.

	pstTimerRegsOvly->TSICR = (1<<2);             //posted mode active

	//pstTimerRegsOvly->TCLR = (1<<5);              //enabling divider input

	//while(pstTimerRegsOvly->TWPS & 0x01);
	//pstTimerRegsOvly->TCLR |= (1<<3|0<<2);		  //setting the prescalar ratio (/8)

	while(pstTimerRegsOvly->TWPS & 0x01);
	pstTimerRegsOvly->TCLR |= uiAuto_oneshot_modesel;			 // enabling the Auto reload or one shot mode

	while(pstTimerRegsOvly->TWPS & 0x04);
	pstTimerRegsOvly->TLDR = ((0xFFFFFFFF+1)-(uitimer_rate_msec/0.03125));		 // setting the load register value

	while(pstTimerRegsOvly->TWPS & 0x02);
	pstTimerRegsOvly->TCRR =((0xFFFFFFFF+1)-(uitimer_rate_msec/0.03125)); // setting the counter register value

	while(pstTimerRegsOvly->TWPS & 0x01);
	pstTimerRegsOvly->TCLR |= (1<<0);            // starting the counter

	while(!(pstTimerRegsOvly->IRQSTATUS_RAW & 0x02));
}

/*************************************************************************************************************

Name            : DC4_Timer_Stop

Description     : This function is used to stop the TIMERS(1/2/3/4/5/6/7/8) operation

Prototype       : void DC4_Timer_Stop(UNS32 uiSel_Timer)

Parameters      : uiSel_Timer - This parameter is used to select the base address of the
								Timers1/2/3/4/5/6/7/8 to stop operation,this freezes only the counter value.

Returns         : None

*************************************************************************************************************/

void DC4_Timer_Stop(UNS32 uiSel_Timer)
{
	stTimerRegs *pstTimerRegsOvly = (stTimerRegs *) uiSel_Timer;

	while(pstTimerRegsOvly->TWPS & 0x01);
	pstTimerRegsOvly->TCLR = (0<<0);           // stopping the counter

	//while(1);
}
