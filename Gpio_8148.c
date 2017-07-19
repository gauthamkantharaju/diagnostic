/************************************************************************************************************
 *
 * Title			  : GPIO_8148.c
 *
 * Application		  : GPIO
 *
 * Author			  : Sailesh Myneni
 *
 * Creation Date      : May 24,2011
 *
 * Description        : Source file for the GPIO functions
 *
 *
 * Limitations(if any):
 *
 *----------------------------------------------------------------------------
 * Modification History
 *----------------------------------------------------------------------------
 * Ver 	Date       		Initials       Description
 *----------------------------------------------------------------------------
 * 0.1                                 Initial Version
 *
 *
 *---------------------------------------------------------------------------
************************************************************************************************************/

/*------------------------------------------- Include files -----------------------------------------------*/

#include"GPIO_8148.h"

/*-------------------------------- Symbolic Constants and Macros (defines) --------------------------------*/

/*------------------------------------------ Typedefs and structs -----------------------------------------*/


/*------------------------------------ Declarations (statics and globals) ---------------------------------*/

/*------------------------------------------ Function prototypes ------------------------------------------*/

/*---------------------------------------------- Functions ------------------------------------------------*/

/*************************************************************************************************************
Name            : DC4_GPIO_Init

Description     : This function initializes the GPIO clocks and executes a software reset on the
			      GPIO(0/1/2/3)module.

Parameters      : [I] uibank_baseaddr - Base address of GPIO(0/1/2/3) module.

Returns         : None
*************************************************************************************************************/

void DC4_GPIO_Init(UNS32 uibank_baseaddr)
{
	stGpioBankRegs *pstGpioRegsOvly = (stGpioBankRegs *)uibank_baseaddr;

	*((VUNS32 *)0x48181400) |= (0x01 << 1);	//CM_ALWON_L3_SLOW_CLKSTCTRL

	*((VUNS32 *)0x48181428) |= (0x01 << 8) | (0x01 << 1) ; //CM_ALWON_SYSCLK6_CLKSTCTRL
		while( (*((VUNS32 *)0x48181428) & 0x102) != 0x102);

	*((VUNS32 *)0x4818155C) |= (0x01 << 1) ; //CM_ALWON_GPIO_0_CLKCTRL
	while( (*((VUNS32 *)0x4818155C) & 0x02) != 0x2);

	*((VUNS32 *)0x48181560) |= (0x01 << 1) ; //CM_ALWON_GPIO_1_CLKCTRL
		while( (*((VUNS32 *)0x48181560) & 0x02) != 0x2);

	while( (*((VUNS32 *)0x48181400) & 0x4100) !=0x4100);

	//printf(" \n 1 \n");

	pstGpioRegsOvly->GPIO_SYSCONFIG = 0x0002;
	//printf(" \n 2 \n");
	while(pstGpioRegsOvly->GPIO_SYSSTATUS & 0x02); // Software Reset

	pstGpioRegsOvly->GPIO_SYSCONFIG = 0x08; // Idle mode settings

	//printf(" \n 3 \n");
}

/*************************************************************************************************************
Name            : DC4_GPIO_ConfigDir

Description     : This function configures Direction of the GPIO(0/1/2/3)module pins.

Parameters      : [I] uibank_baseaddr - Base address of GPIO(0/1/2/3)module.

                  [I] uipin_number - Pin number of the respective GPIO module(Please use the macros defined in
                  	  	  	  	     the GPIO_8148.h file)
				  NOTE : only one pin(MACROS specified in GPIO_8148.h)token should be passed as a parameter
				  	  	 to "uipin_number"

                  [I] uiDir- Either pin should act as  INPUT or OUTPUT(Please use the macros defined in the
                  	  	  	 GPIO_8148.h file)

Returns         : 0 - No error
				  1 - Invalid pin
*************************************************************************************************************/

UNS32 DC4_GPIO_ConfigDir(UNS32 uibank_baseaddr,UNS32 uipin_number,UNS32 uiDir)
{
	UNS32 uistatus = NO_ERR;
    stGpioBankRegs *pstGpioRegsOvly = (stGpioBankRegs *)uibank_baseaddr;

	if( uipin_number < 0 || uipin_number > 31)
		uistatus=INVALID_PIN;
	else
	{
		if(uiDir==OUTPUT)
			pstGpioRegsOvly->GPIO_OE &= ~(1<<uipin_number);
		else
			pstGpioRegsOvly->GPIO_OE |=(1<<uipin_number);			
	}
	
	return uistatus;
}

/*************************************************************************************************************
Name            : DC4_GPIO_Read

Description     : This function is used to read the data coming from all the GPIO module input pins.

Parameters      : [I] uibank_baseaddr - Base address of GPIO(0/1/2/3)module.

                  [I] uiPin_value - Pointer to the address where the data read from the GPIO input pins is kept.

Returns         : None
*************************************************************************************************************/

void DC4_GPIO_Read(UNS32 uibank_baseaddr, UNS32 *uiPin_value)
{
	stGpioBankRegs *pstGpioRegsOvly = (stGpioBankRegs *)uibank_baseaddr;
    *uiPin_value = pstGpioRegsOvly->GPIO_DATAIN;

}

/*************************************************************************************************************
Name            : DC4_GPIO_Set

Description     : This function is used to set the bits in GPIO_SETDATAOUT registers which is used to
        		  SET the corresponding bit in the GPIO_DATAOUT register.
                  The GPIO_DATAOUT register is used for setting the value of the GPIO output pins

Parameters      : [I] uibank_baseaddr - Base address of GPIO(0/1/2/3)module.

                  [I] uipin_number - pin number of the respective GPIO module(Please use the macros
                          	  	  	 defined in the GPIO_8148.h file)
				  NOTE : only one pin(MACROS specified in GPIO_8148.h)token should be passed as a parameter
				  	  	 to "uipin_number"

Returns         :  0 - No error
				   1 - Invalid pin
*************************************************************************************************************/

UNS32 DC4_GPIO_Set(UNS32 uibank_baseaddr, UNS32 uipin_number)
{
	UNS32 uistatus = NO_ERR;
	stGpioBankRegs *pstGpioRegsOvly=(stGpioBankRegs *) uibank_baseaddr;
	
	if(uipin_number<0 || uipin_number > 31)
		uistatus=INVALID_PIN;
	else
	{
		pstGpioRegsOvly->GPIO_SETDATAOUT = (1<<uipin_number);
	}
   return uistatus;
    
}

/*************************************************************************************************************
Name            : DC4_GPIO_Clear

Description     : This function is used to set the bits in GPIO_CLEARDATAOUT registers which is used
        		  to CLEAR the corresponding bit in the GPIO_DATAOUT register.
                  The GPIO_DATAOUT register is used for setting the value of the GPIO output pins

Parameters      : [I] uibank_baseaddr - Base address of GPIO(0/1/2/3)module.

                  [I] uipin_number - pin number of the respective GPIO module(Please use the macros
                          	  	  	 defined in the GPIO_8148.h file)
				  NOTE : only one pin(MACROS specified in GPIO_8148.h)token should be passed as a parameter
				  	  	 to "uipin_number"

Returns         : 0 - No error
				  1 - Invalid pin
*************************************************************************************************************/

UNS32 DC4_GPIO_Clear(UNS32 uibank_baseaddr, UNS32 uipin_number)
{
	UNS32 uistatus = NO_ERR;
	stGpioBankRegs *pstGpioRegsOvly=(stGpioBankRegs *) uibank_baseaddr;
	
	if(uipin_number<0 || uipin_number > 31)
		uistatus=INVALID_PIN;
	else
	{
		pstGpioRegsOvly->GPIO_CLEARDATAOUT = (1<<uipin_number);
	}
	return uistatus;
}

/*************************************************************************************************************
Name            : DC4_GPIO_Get

Description     : This function is used to read the status of an individual pin (input pin).

Parameters      : [I] uibank_baseaddr - Base address of GPIO(0/1/2/3)module.

                  [I] uipin_number - pin number of the respective GPIO module(Please use the macros
                          	  	  	 defined in the GPIO_8148.h file)
				  NOTE : only one pin(MACROS specified in GPIO_8148.h)token should be passed as a parameter
				  	  	 to "uipin_number"

                  [I] uiPin_value - Pointer to the address where the data read from the individual
                          	  	  	GPIO input pin is kept.

Returns         : 0 - No error
				  1 - Invalid pin

*************************************************************************************************************/

UNS32 DC4_GPIO_Get(UNS32 uibank_baseaddr, UNS32 uipin_number, UNS32 *uiPin_value)
{
	UNS32 uistatus = NO_ERR;
	stGpioBankRegs *pstGpioRegsOvly=(stGpioBankRegs *) uibank_baseaddr;

	if(uipin_number<0 ||uipin_number > 31)
		uistatus=INVALID_PIN;
	else
	{
		if((pstGpioRegsOvly->GPIO_DATAIN) & (1<<uipin_number))
			*uiPin_value = 0x1;
		else
			*uiPin_value = 0x0;
	}
	return uistatus;
}

/*THE FINAL DATASHEETS ARE YET TO BE RELEASED AND AINTC DATASHEET IS NOT YET
 AVAILABLE. ONCE ITS AVAILABLE,THE INTERRUPTS GENERATION CAN BE COMPLETELY 
 IMPLEMENTED.THIS IS JUST A GENERIC FUNTION*/

/**************************************************************************************************************
Name            : DC4_GPIO_Irq

Description     : CONFIGURING THE INTERRUPT REQUEST GENERATION

Parameters      : [I] uibank_baseaddr - Base address of GPIO(0/1/2/3)module.

                  [I] uipin_number - pin number of the respective GPIO module(Please use the macros
                                     defined in the GPIO_8148.h file)
				  NOTE : only one pin(MACROS specified in GPIO_8148.h)token should be passed as a parameter
				  	  	 to "uipin_number"

                  [I] uiedgeorlevel_choice - The edgeorlevel_choice is used to enable either LEVEL DETECTION OR
                  	  	  	  	  	  	  	 EDGE DETECTION required for INTERRUPT REQUEST GENERATION

				  (please use the below values as parameters)

					0 – Level Detect 0
					1 – Level Detect
					2– Falling Detect
					3- Rising Detect

Returns         : 0 - No error
				  1 - Invalid pin
**************************************************************************************************************/

UNS32 DC4_GPIO_Irq(UNS32 uibank_baseaddr,UNS32 uipin_number,UNS32 uiedgeorlevel_choice)
{
	UNS32 uistatus = NO_ERR;
	stGpioBankRegs *pstGpioRegsOvly=(stGpioBankRegs *) uibank_baseaddr;
	
	if(uipin_number<0 || uipin_number > 31)
		uistatus=INVALID_PIN;
	else
	{
		if(uiedgeorlevel_choice==0)
			pstGpioRegsOvly->GPIO_LEVELDETECT0 |= (1<<uipin_number);
		if(uiedgeorlevel_choice==1)
			pstGpioRegsOvly->GPIO_LEVELDETECT1 |= (1<<uipin_number);
		if(uiedgeorlevel_choice==2)
			pstGpioRegsOvly->GPIO_FALLINGDETECT|=(1<<uipin_number);
		if(uiedgeorlevel_choice==3)
			pstGpioRegsOvly->GPIO_RISINGDETECT |= (1<<uipin_number);
	}
	return uistatus;
}
  
/*THE AINTC AND VECTOR TABLE ARE NOT YET AVAILABLE.ONCE ITS AVAILABLE,THE ISR CAN BE
  COMPLETELY IMPLEMENTED.THIS IS JUST A GENERIC FUNCTION*/

/**************************************************************************************************************
Name            : DC4_GPIO_Isr

Description     : INTERRUPT SERVICE ROUTINE

Parameters      : [I] uibank_baseaddr - Base address of GPIO(0/1/2/3)module.

                  [I] uipin_number - pin number of the respective GPIO module(Please use the macros
                          	  	  	 defined in the GPIO_8148.h file)
				  NOTE : only one pin(MACROS specified in GPIO_8148.h)token should be passed as a parameter
				  	  	 to "uipin_number"
Returns         : void
***************************************************************************************************************/

void DC4_GPIO_Isr(UNS32 uibank_baseaddr,UNS32 uipin_number)
{
	stGpioBankRegs *pstGpioRegsOvly=(stGpioBankRegs *) uibank_baseaddr;
	pstGpioRegsOvly->GPIO_IRQSTATUS_0 |= (1<<uipin_number);
}



