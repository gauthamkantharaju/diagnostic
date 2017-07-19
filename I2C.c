/************************************************************************************************************
 *
 * Title			: I2C.c
 *
 * Application		: Bighill project
 *
 * Author			: Mahantesh.Kumbar
 *
 * Creation Date    : 30/5/2011
 *
 * Description      	: Function to configure the I2C module
 *
 * Limitations      	: Supported for Polling method only
 *
 *
 *----------------------------------------------------------------------------
 * Modification History
 *----------------------------------------------------------------------------
 * Ver 	Date       Initials       Description
 *----------------------------------------------------------------------------
 * 0.1	30/5/2011     		      Initial Version
 *
 *
 *---------------------------------------------------------------------------
************************************************************************************************************/

/*------------------------------------------- Include files -----------------------------------------------*/
#include "C_Types.h"
#include "I2C.h"
/*-------------------------------- Symbolic Constants and Macros (defines) --------------------------------*/

/*------------------------------------------ Typedefs and structs -----------------------------------------*/


/*------------------------------------ Declarations (statics and globals) ---------------------------------*/
UNS8 ucDebug[1024 * 4];
UNS32 uiDbgIndex = 0;

/*------------------------------------------ Function prototypes ------------------------------------------*/

/*---------------------------------------------- Functions ------------------------------------------------*/


/*********************************************************************************************
Function Name : DC4_I2C_EnableController
Description   : DC4_I2C_EnableController(UNS32 uiI2C_Controller)
Prototype     : UNS32 DC4_I2C_init(UNS32 uiI2C_Controller, UNS32 i2cBusFreq, UNS16 usOwn_Address)
Parameters    : [I] uiI2C_Controller - Base address of controller
Returns       : void
***********************************************************************************************/
void DC4_I2C_EnableController()
{
	//i2c Interface Clock
	*pL3_SLOW_CLKSTCTRL |= (0x01 << 1);	//CM_ALWON_L3_SLOW_CLKSTCTRL

	//I2C_CONTROLLER_0:
	//i2c 0 Interface Clock
	*pI2C0_INTERFACE_CLK |= (0x01 << 1) ; //CM_ALWON_I2C_0_CLKCTRL
	while( (*pI2C0_INTERFACE_CLK & 0x02) != 0x2);
	//I2C PinMux
	*pI2C0_PINMUX_SCL = 0x01;//I2C[0] SCL
	*pI2C0_PINMUX_SDA = 0x01;//I2C[0] SDA

	//I2C_CONTROLLER_1:
	//i2c 1 Interface Clock
	*pI2C1_INTERFACE_CLK |= (0x01 << 1) ; //CM_ALWON_I2C_1_CLKCTRL
	while( (*pI2C1_INTERFACE_CLK & 0x02) != 0x2);
	//I2C 1 PinMux
	*pI2C1_PINMUX_SCL = 0x01;//I2C[1] SCL
	*pI2C1_PINMUX_SDA = 0x01;//I2C[1] SDA

	//I2C_CONTROLLER_2:
	//I2C 2 PinMux
	*pI2C2_PINMUX_SCL = 0x80;//I2C[2] SCL
	*pI2C2_PINMUX_SDA = 0x80;//I2C[2] SDA
	*((VUNS32 *)0x48140A18) = 0x40; //PINCNTL135
	*((VUNS32 *)0x48140A1C) = 0x40; //PINCNTL136

	//I2C_CONTROLLER_3:
	//I2C PinMux
	*pI2C3_PINMUX_SCL = 0x20;//I2C[3] SCL
	*pI2C3_PINMUX_SDA = 0x20;//I2C[3] SDA

	while((*pL3_SLOW_CLKSTCTRL & 0x00010100) != 0x00010100);

	//i2c Functional clock from SYSCLK10, PLL_USB(960Mhz)/5 => 192Mhz/4 => 48Mhz[SYSCLK10]
	*pI2C_FUNCTIONAL_CLK = 0x3 ;//CM_DPLL.CM_SYSCLK10_CLKSEL = 3[Select SYS_CLK divided by 4]
	while( (*pI2C_FUNCTIONAL_CLK & 0x03) != 0x03);
}

/*********************************************************************************************
Function Name : I2C_init
Description   : This function Reset & configure the I2C..
Prototype     : UNS32 DC4_I2C_init(UNS32 uiI2C_Controller, UNS32 i2cBusFreq, UNS16 usOwn_Address)
Parameters    : [I] uiI2C_Controller - Base address of controller
				[I] i2cBusFreq - Bus speed upto 400KHz(Ex : For "100KHz" just pass as "100")
				[I] usOwn_Address - Own address of I2C
Returns       : NO_ERR - No error
				TIMEOUT_ERR - Reset Timeout
***********************************************************************************************/
UNS32 DC4_I2C_init(UNS32 uiI2C_Controller, UNS32 i2cBusFreq, UNS16 usOwn_Address)
{
	volatile UNS32 uiTimeout = 1000;
	UNS32 uiStatus = NO_ERR;
	UNS32 uiIclk = 0;
	UNS32 uiClk = 0;

	//point to the I2c controller module Reg
	CSL_I2cRegs *i2c = (CSL_I2cRegs *)uiI2C_Controller;

	//DC4_I2C_EnableController();

	i2cBusFreq = (i2cBusFreq * 1000); //Convert to KHz

	//Reset the I2c Module
	i2c->I2C_SYSC = SYSC_SOFTRESET_MASK;
	i2c->I2C_CON = I2C_CON_EN;
	//wait for the reset to complete
	while( !(i2c->I2C_SYSS & SYSS_RESETDONE_MASK) && (uiTimeout--) );

	//Configure I2C
	if(uiTimeout != 0)
	{
		// Disable it again
		i2c->I2C_CON &= ~(I2C_CON_EN) ;

		//SYSC register is cleared by the reset, rewrite it
		i2c->I2C_SYSC = SYSC_AUTOIDLE_MASK | SYSC_ENAWAKEUP_MASK |(0x01 << 0x03)| (0x03 << 0x08);

    	//(iclk/(PSC+1)) => 4800000/(3+1) => 12MHZ
    	i2c->I2C_PSC = 3;
    	uiIclk = I2C_FUNC_CLOCK /(3 + 1);

		// Standard and fast speed (upto 400Khz)
    	uiClk = ( uiIclk / (2 * i2cBusFreq) ); //50% duty cycle
		i2c->I2C_SCLH = uiClk - 5;	//tHIGH = (SCLH + 5) * ICLK time period
		i2c->I2C_SCLL = uiClk - 7;	//tLOW = (SCLL + 7) * ICLK time period,

		//Own address
		i2c->I2C_OA = (usOwn_Address & 0x03FF);

		//Interrupt Enable
		//i2c->I2C_IRQENABLE_SET = I2C_IE_XRDY | I2C_IE_RRDY | I2C_IE_ARDY | I2C_IE_NACK | I2C_IE_AL;

    	// Take the I2C module out of reset
		i2c->I2C_CON |= I2C_CON_EN;
	}
	else
		uiStatus = TIMEOUT_ERR; //Err, Reset is not happened properly

	return uiStatus;
}

/*********************************************************************************************
Function Name : DC4_I2C_BufferConfig
Description   : This function configure Buffer & FIFO of the I2C controller.
Prototype     : UNS32 DC4_I2C_BufferConfig(UNS32 uiI2C_Controller, UNS32 uiFlags, UNS32 Tx_fifo_threshold, UNS32 Rx_fifo_threshold)
Parameters    : [I] uiI2C_Controller - Base address of controller
				[I] uiFlags - Buffer related configuration(X/R DMA Enable, X/R FIFO Clear)
				[I] Tx_fifo_threshold - Value for the TX FIFO Threshold(Max - 64)
				[I] Rx_fifo_threshold - Value for the RX FIFO Threshold(Max - 64)
Returns       : NO_ERR - No error
				INVALID_BUFFLEN - Tx_fifo_threshold/Rx_fifo_threshold not in limit
***********************************************************************************************/
UNS32 DC4_I2C_BufferConfig(UNS32 uiI2C_Controller, UNS32 uiFlags, UNS32 Tx_fifo_threshold, UNS32 Rx_fifo_threshold)
{
	UNS32 uiStatus = NO_ERR;

	//point to the I2c controller module Registers
	CSL_I2cRegs *i2c = (CSL_I2cRegs *)uiI2C_Controller;

	//Configure Buffer (X/R DMA Enable, X/R FIFO Clear)
	i2c->I2C_BUF |= uiFlags;

	//Configure TX FIFO threshold
	if(Tx_fifo_threshold > 0 && Tx_fifo_threshold <= 64)
	{
		i2c->I2C_BUF |= (Tx_fifo_threshold & 0x0000003F);
	}
	else
		uiStatus = INVALID_BUFFLEN;

	//Configure RX FIFO threshold
	if( (Rx_fifo_threshold > 0) && (Rx_fifo_threshold <= 64))
	{
		i2c->I2C_BUF |= ((Rx_fifo_threshold << 8) & 0x00003F00);
	}
	else
		uiStatus = INVALID_BUFFLEN;

	return uiStatus;
}

/*********************************************************************************************
Function Name : DC4_I2C_IRQEnable
Description   : This function configure Interrupt of the I2C controller.
Prototype     : void DC4_I2C_IRQEnable(UNS32 uiI2C_Controller, UNS32 uiIntrFlags)
Parameters    : [I] uiI2C_Controller - Base address of controller
				[I] uiIntrFlags - Interrupt flags for which Interrupts has to be Enabled
Returns       : void
***********************************************************************************************/
void DC4_I2C_IRQEnable(UNS32 uiI2C_Controller, UNS32 uiIntrFlags)
{
	CSL_I2cRegs *i2c = (CSL_I2cRegs *)uiI2C_Controller;

	//Enable Interrupts
	i2c->I2C_IRQENABLE_SET |= uiIntrFlags;
}

/*********************************************************************************************
Function Name : DC4_I2C_IRQDisable
Description   : This fucntion configure Interrupt of the I2C controller.
Prototype     : void DC4_I2C_IRQDisable(UNS32 uiI2C_Controller, UNS32 uiIntrFlags)
Parameters    : [uiI2C_Controller - Base address of controller
				uiIntrFlags - Interrupt flags for which Interrupts has to be disabled
Returns       : void
***********************************************************************************************/
void DC4_I2C_IRQDisable(UNS32 uiI2C_Controller, UNS32 uiIntrFlags)
{
	CSL_I2cRegs *i2c = (CSL_I2cRegs *)uiI2C_Controller;

	//Disable Interrupts
	i2c->I2C_IRQENABLE_CLR |= (uiIntrFlags);
}

/*********************************************************************************************
Function Name : Delay
Description   : This function adds delay for I2C.
Prototype     : static void DC4_Delay(UNS32 uiDelay)
Parameters    : [I] uiDelay - delay value
Returns       : void
***********************************************************************************************/
static void DC4_Delay(UNS32 uiDelay)
{
    	while(uiDelay--);
}

/*********************************************************************************************
Function Name : I2C_ReadrWrite
Description   : This function used for Transmit or Receive of the data on I2C bus.
Prototype     : UNS32 DC4_I2C_ReadrWrite(UNS32 uiI2C_Controller, UNS16 usSlave_addr, UNS8 *ucbuffer, UNS16 *usData_length, UNS16 usFlags)
Parameters    : [I] uiI2C_Controller - Base address of controller
				[I] usSlave_addr - Slave address of the I2C
				[IO] *ucbuffer - pointer to the data Buffer for Transmit/Receive
				[IO] *usData_length - Pointer to Data length for Transmit/Receive,and also
				holds the Numbers of data sent or Received if error occurs
				[I] usFlags - Holds the info as flags like write/read operation, Start/Stop Condition

Returns       : NO_ERR - No error
				INVALID_BUFFLEN - Data length is not in the watermark
				I2C_BUS_BUSY_ERR - Bus is not free
				I2C_NACK_ERR - Ack Error
				TIMEOUT_ERR - Timeout Error for Read/Write Operation
***********************************************************************************************/
UNS32 DC4_I2C_ReadrWrite(UNS32 uiI2C_Controller, UNS16 usSlave_addr, UNS8 *ucbuffer, UNS16 *usData_length, UNS16 usFlags)
{
	volatile UNS32 uiTimeout = 0;
	UNS32 uiStatus = NO_ERR;
	UNS16 usI2C_Config = 0;
	UNS16 usIndex = 0;

	CSL_I2cRegs *i2c = (CSL_I2cRegs *)uiI2C_Controller;

	// Disable it
	//i2c->I2C_CON &= ~(I2C_CON_EN) ;

	i2c->I2C_SA = (usSlave_addr & 0x7F);		//Configure Slave address

	if(*usData_length <= I2C_BUFFER_MAXLENGTH)
	{
		i2c->I2C_CNT = *usData_length;				//Assign the Data Length

		// Enable I2C controller, speed configuration,  Master
		usI2C_Config = I2C_CON_EN | I2C_CON_MST;

		//Enable for write operation else it will be for read operation
		if (usFlags & I2C_WRITE)
			usI2C_Config |= I2C_CON_TRX;

		//Enable Start Condition
		if (usFlags & I2C_START)
			usI2C_Config |= I2C_CON_STT;

		//Enable Stop Condition
		if (usFlags & I2C_STOP)
			usI2C_Config |= I2C_CON_STP;
		//Check For Bus Busy
		uiTimeout = 10000;
		while( (i2c->I2C_IRQSTATUS_RAW & I2C_STAT_BB) && (uiTimeout--) );

		if(uiTimeout != 0)
		{
			//Initiate a Transfer (Configure I2C)
			i2c->I2C_CON = usI2C_Config;

			//Wait for the data transmit completion
			uiTimeout = 10000;
			while(!(i2c->I2C_IRQSTATUS_RAW & I2C_STAT_XRDY) && (uiTimeout--));

			if ((uiTimeout != 0) && (usFlags & I2C_WRITE))
			{
				//ucDebug[uiDbgIndex++] = i2c->I2C_CNT;
				// transmit.
				for(usIndex = 0; usIndex < *usData_length; usIndex++)
				{
					i2c->I2C_DATA = ucbuffer[usIndex];	//Put data
					ucDebug[uiDbgIndex++] = i2c->I2C_CNT;
					//printf(" i2c->I2C_CNT = %x \n", i2c->I2C_CNT);
					//Wait for the data transmit completion
					uiTimeout = 5000;
					do
					{
						DC4_Delay(I2C_DELAY_SMALL );
					}
					while(!(i2c->I2C_IRQSTATUS_RAW & I2C_STAT_XRDY) && (uiTimeout--));
					if(uiTimeout == 0)
					{
						*usData_length = usIndex;
						uiStatus = TIMEOUT_ERR;
						break; //to Come out from the for loop
					}
				}

				//Check for ACK from slave
				DC4_Delay(I2C_DELAY_BIG);
				if( i2c->I2C_IRQSTATUS_RAW & I2C_STAT_NACK )
					uiStatus = I2C_NACK_ERR;

			}
			else if(uiTimeout != 0)
			{
				// Receive
				for(usIndex = 0; usIndex < *usData_length; usIndex++)
				{
					//Wait for the data Receive
					uiTimeout = 5000;
					do
					{
						DC4_Delay(I2C_DELAY_SMALL );
					}
					while(!(i2c->I2C_IRQSTATUS_RAW & I2C_STAT_RRDY) && (uiTimeout--));
					if(uiTimeout == 0)
					{
						*usData_length = usIndex;
						uiStatus = TIMEOUT_ERR;
						break; //to Come out from the for loop
					}
					ucbuffer[usIndex] = i2c->I2C_DATA; //Read data;
				}

				DC4_Delay(I2C_DELAY_BIG);
				if( i2c->I2C_IRQSTATUS_RAW & I2C_STAT_NACK )
					uiStatus = I2C_NACK_ERR;
			}
			else
				uiStatus = TIMEOUT_ERR;
		}
		else
			uiStatus = I2C_BUS_BUSY_ERR;
	}
	else
		uiStatus = INVALID_BUFFLEN;

	return uiStatus;
}


//This function is not yet completed, it just clearing the Interrupt.
void DC4_I2C_ISR(UNS32 uiI2C_Controller)
{
	CSL_I2cRegs *i2c = (CSL_I2cRegs *)uiI2C_Controller;

	i2c->I2C_IRQSTATUS |= i2c->I2C_IRQSTATUS; //Clear the Interrupts
}

