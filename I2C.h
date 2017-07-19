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
 * Description      : Defining the address of the I2C configuration registers & Bit definition
 *
 * Limitations      : NONE
 *
 *
 *----------------------------------------------------------------------------
 * Modification History
 *----------------------------------------------------------------------------
 * Ver 	Date       Initials       Description
 *----------------------------------------------------------------------------
 * 0.1	30/5/2011            	  Initial Version
 *
 *
 *---------------------------------------------------------------------------
************************************************************************************************************/

#ifndef _I2C_H_
#define _I2C_H_

/*------------------------------------------- Include files -----------------------------------------------*/
#include "C_Types.h"

/*-------------------------------- Symbolic Constants and Macros (defines) --------------------------------*/

#define _ARM_ //Disable this to use for DSP.

#ifdef _ARM_
	#define I2C_CONTROLLER_0	0x48028000
	#define I2C_CONTROLLER_1	0x4802A000
	#define I2C_CONTROLLER_2	0x4819C000
	#define I2C_CONTROLLER_3	0x4819E000

	#define pL3_SLOW_CLKSTCTRL	((VUNS32 *)0x48181400)
	#define pI2C0_INTERFACE_CLK	((VUNS32 *)0x48181564)
	#define pI2C1_INTERFACE_CLK	((VUNS32 *)0x48181568)

	#define pI2C_FUNCTIONAL_CLK	((VUNS32 *)0x48180324)
#else
	#define I2C_CONTROLLER_0 	0x08028000
	#define I2C_CONTROLLER_1	0x0802A000
	#define I2C_CONTROLLER_2	0x0819C000
	#define I2C_CONTROLLER_3	0x0819E000

	#define pL3_SLOW_CLKSTCTRL	((VUNS32 *)0x08181400)
	#define pI2C0_INTERFACE_CLK	((VUNS32 *)0x08181564)
	#define pI2C1_INTERFACE_CLK	((VUNS32 *)0x08181568)

	#define pI2C_FUNCTIONAL_CLK	((VUNS32 *)0x08180324)
#endif

#define pI2C0_PINMUX_SCL		((VUNS32 *)0x48140C18)
#define pI2C0_PINMUX_SDA		((VUNS32 *)0x48140C1C)

#define pI2C1_PINMUX_SCL		((VUNS32 *)0x48140934)
#define pI2C1_PINMUX_SDA		((VUNS32 *)0x48140938)

#define pI2C2_PINMUX_SCL		((VUNS32 *)0x48140924)
#define pI2C2_PINMUX_SDA		((VUNS32 *)0x48140928)

#define pI2C3_PINMUX_SCL		((VUNS32 *)0x48140910)
#define pI2C3_PINMUX_SDA		((VUNS32 *)0x4814090C)

/* I2C functional clock is 48MHz(SYSCLK10) */
#define I2C_FUNC_CLOCK  (48000000)

/* I2C Interrupt Enable */
#define I2C_IE_XDR     (1u << 14)   /* TX Buffer drain int enable */
#define I2C_IE_RDR     (1u << 13)   /* RX Buffer drain int enable */
#define I2C_IE_XRDY    (1u << 4)    /* TX data ready int enable */
#define I2C_IE_RRDY    (1u << 3)    /* RX data ready int enable */
#define I2C_IE_ARDY    (1u << 2)    /* Access ready int enable */
#define I2C_IE_NACK    (1u << 1)    /* No ack interrupt enable */
#define I2C_IE_AL      (1u << 0)    /* Arbitration lost int ena */

/* I2C Status Register 	*/
#define I2C_STAT_XDR   (1u << 14)   // TX Buffer draining
#define I2C_STAT_RDR   (1u << 13)   // RX Buffer draining
#define I2C_STAT_BB    (1u << 12)   // Bus busy
#define I2C_STAT_ROVR  (1u << 11)   // Receive overrun
#define I2C_STAT_XUDF  (1u << 10)   // Transmit underflow
#define I2C_STAT_AAS   (1u << 9)    // Address as slave
#define I2C_STAT_AD0   (1u << 8)    // Address zero
#define I2C_STAT_XRDY  (1u << 4)    // Transmit data ready
#define I2C_STAT_RRDY  (1u << 3)    // Receive data ready
#define I2C_STAT_ARDY  (1u << 2)    // Register access ready
#define I2C_STAT_NACK  (1u << 1)    // No ack interrupt enable
#define I2C_STAT_AL    (1u << 0)    // Arbitration lost int ena

// I2C WE wakeup enable register
#define I2C_WE_XDR_WE  (1u << 14)   // TX drain wakup
#define I2C_WE_RDR_WE  (1u << 13)   // RX drain wakeup
#define I2C_WE_AAS_WE  (1u << 9)    // Address as slave wakeup
#define I2C_WE_BF_WE   (1u << 8)    // Bus free wakeup
#define I2C_WE_STC_WE  (1u << 6)    // Start condition wakeup
#define I2C_WE_GC_WE   (1u << 5)    // General call wakeup
#define I2C_WE_DRDY_WE (1u << 3)    // TX/RX data ready wakeup
#define I2C_WE_ARDY_WE (1u << 2)    // Reg access ready wakeup
#define I2C_WE_NACK_WE (1u << 1)    // No acknowledgment wakeup
#define I2C_WE_AL_WE   (1u << 0)    // Arbitration lost wakeup

#define I2C_WE_ALL     (I2C_WE_XDR_WE | I2C_WE_RDR_WE | \
                I2C_WE_AAS_WE | I2C_WE_BF_WE | \
                I2C_WE_STC_WE | I2C_WE_GC_WE | \
                I2C_WE_DRDY_WE | I2C_WE_ARDY_WE | \
                I2C_WE_NACK_WE | I2C_WE_AL_WE)

/* I2C Buffer Configuration Register (I2C_BUF): */
#define I2C_BUF_RDMA_EN    (1u << 15)   // RX DMA channel enable
#define I2C_BUF_RXFIF_CLR  (1u << 14)   // RX FIFO Clear
#define I2C_BUF_XDMA_EN    (1u << 7)    // TX DMA channel enable
#define I2C_BUF_TXFIF_CLR  (1u << 6)    // TX FIFO Clear

// I2C Configuration Register (I2C_CON):
#define I2C_CON_EN          (1u << 15)   // I2C module enable
#define I2C_CON_BE          (1u << 14)   // Big endian mode
#define I2C_CON_OPMODE		(1u << 12)   // High Speed support
#define I2C_CON_STB         (1u << 11)   // Start byte mode (master)
#define I2C_CON_MST         (1u << 10)   // Master/slave mode
#define I2C_CON_TRX         (1u << 9)    // TX/RX mode (master only)
#define I2C_CON_XA          (1u << 8)    // Expand address
#define I2C_CON_RM          (1u << 2)    // Repeat mode (master only)
#define I2C_CON_STP         (1u << 1)    // Stop cond (master only)
#define I2C_CON_STT         (1u << 0)    // Start condition (master)

/* I2C SCL time value when Master */
#define I2C_SCLL_HSSCLL    (8u)
#define I2C_SCLH_HSSCLH    (8u)

/* OCP_SYSSTATUS bit definitions */
#define SYSS_RESETDONE_MASK         (1u << 0)

/* OCP_SYSCONFIG bit definitions */
#define SYSC_CLOCKACTIVITY_MASK     (0x3u << 8)
#define SYSC_CLOCKACTIVITY_SHIFT    (8u)
#define SYSC_SIDLEMODE_MASK         (0x3u << 3)
#define SYSC_IDLEMODE_SHIFT         (3u)
#define SYSC_ENAWAKEUP_MASK         (1u << 2)
#define SYSC_SOFTRESET_MASK         (1u << 1)
#define SYSC_AUTOIDLE_MASK          (1u << 0)

#define SYSC_IDLEMODE_SMART         (0x2u)
#define SYSC_CLOCKACTIVITY_FCLK     (0x2u)

#define I2C_MIN_BUS_FREQ            (1u)		//Min I2C bus frequency - 1 KHz
#define I2C_MAX_BUS_FREQ			(400u) 		//Max I2C bus frequency - 400 KHz
#define I2C_BUFFER_MAXLENGTH		(64 * 1024) //Maximum buffer length which could be transferred


//I2c Flags
#define I2C_READ                (0x0001u)
#define I2C_WRITE               (0x0002u)
#define I2C_MASTER              (0x0004u)
#define I2C_SLAVE               (0x0008u)
#define I2C_START               (0x0010u)
#define I2C_STOP                (0x0020u)
#define I2C_RESTART             (0x0040u)
#define I2C_REPEAT              (0x0080u)
#define I2C_IGNORE_BUS_BUSY     (0x0100u)
#define I2C_M_IGNORE_NAK        (0x0200u)

/* Use these flags for simple MASTER read/write transactions on the I2C bus */
/** \brief  Default read flag */
#define I2C_DEFAULT_READ \
               (I2C_READ | I2C_MASTER | I2C_START | I2C_STOP)
/** \brief  Default write flag */
#define I2C_DEFAULT_WRITE \
               (I2C_WRITE | I2C_MASTER | I2C_START | I2C_STOP)

//I2C Driver Error codes
#define NO_ERR 						0
#define TIMEOUT_ERR					1
#define INVALID_BUFFLEN 			2
#define I2C_BUS_BUSY_ERR			3	//I2C bus find that the bus is busy
#define I2C_ARBITRATION_LOSS_ERR    4	//I2C driver lost the bus arbitration
#define I2C_NACK_ERR                5	//I2C slave did not acknowledgement
#define I2C_TRANSMIT_UNDERFLOW_ERR  6	//Transmit underflow error
#define I2C_RECEIVE_OVERFLOW_ERR	7	//Receive overflow error

//Delay value
#define I2C_DELAY_BIG   (5000)
#define I2C_DELAY_MED   (2500)
#define I2C_DELAY_SMALL (1000)

/*------------------------------------------ Typedefs and structs -----------------------------------------*/
/**************************************************************************\
* 					Register Overlay Structure                             *
\**************************************************************************/
typedef struct  {
    VUNS32 I2C_REVNB_LO;               /* 0x00 */
    VUNS32 I2C_REVNB_HI;               /* 0x04 */
    VUNS32 RSVD0[2];
    VUNS32 I2C_SYSC;                   /* 0x10 */
    VUNS32 RSVD1[3];
    VUNS32 I2C_EOI;                    /* 0x20 */
    VUNS32 I2C_IRQSTATUS_RAW;          /* 0x24 */
    VUNS32 I2C_IRQSTATUS;              /* 0x28 */
    VUNS32 I2C_IRQENABLE_SET;          /* 0x2C */
    VUNS32 I2C_IRQENABLE_CLR;          /* 0x30 */
    VUNS32 I2C_WE;                     /* 0x34 */
    VUNS32 I2C_DMARXENABLE_SET;        /* 0x38 */
    VUNS32 I2C_DMATXENABLE_SET;        /* 0x3C */
    VUNS32 I2C_DMARXENABLE_CLR;        /* 0x40 */
    VUNS32 I2C_DMATXENABLE_CLR;        /* 0x44 */
    VUNS32 I2C_DMARXWAKE_EN;           /* 0x48 */
    VUNS32 I2C_DMATXWAKE_EN;           /* 0x4C */
    VUNS32 RSVD2[16];
    VUNS32 I2C_SYSS;                   /* 0x90 */
    VUNS32 I2C_BUF;                    /* 0x94 */
    VUNS32 I2C_CNT;                    /* 0x98 */
    VUNS32 I2C_DATA;                   /* 0x9C */
    VUNS32 RSVD4;                      /* 0xA0 */
    VUNS32 I2C_CON;                    /* 0xA4 */
    VUNS32 I2C_OA;                     /* 0xA8 */
    VUNS32 I2C_SA;                     /* 0xAC */
    VUNS32 I2C_PSC;                    /* 0xB0 */
    VUNS32 I2C_SCLL;                   /* 0xB4 */
    VUNS32 I2C_SCLH;                   /* 0xB8 */
    VUNS32 I2C_SYSTEST;                /* 0xBC */
    VUNS32 I2C_BUFSTAT;                /* 0xC0 */
    VUNS32 I2C_OA1;                    /* 0xC4 */
    VUNS32 I2C_OA2;                    /* 0xC8 */
    VUNS32 I2C_OA3;                    /* 0xCC */
    VUNS32 I2C_ACTOA;                  /* 0xD0 */
    VUNS32 I2C_SBLOCK;                 /* 0xD4 */
} CSL_I2cRegs;

/*------------------------------------ Declarations (statics and globals) ---------------------------------*/

/**************************************************************************\
* 					Overlay structure typedef definition				   *
\**************************************************************************/
typedef volatile CSL_I2cRegs             * CSL_I2cRegsOvly;

/*---------------------------------------- Functions  Prototype ---------------------------------------------*/

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
UNS32 DC4_I2C_init(UNS32 uiI2C_Controller, UNS32 i2cBusFreq, UNS16 usOwn_Address);

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
UNS32 DC4_I2C_BufferConfig(UNS32 uiI2C_Controller, UNS32 uiFlags, UNS32 Tx_fifo_threshold, UNS32 Rx_fifo_threshold);


/*********************************************************************************************
Function Name : DC4_I2C_IRQEnable
Description   : This function configure Interrupt of the I2C controller.
Prototype     : void DC4_I2C_IRQEnable(UNS32 uiI2C_Controller, UNS32 uiIntrFlags)
Parameters    : [I] uiI2C_Controller - Base address of controller
				[I] uiIntrFlags - Interrupt flags for which Interrupts has to be Enabled
Returns       : void
***********************************************************************************************/
void DC4_I2C_IRQEnable(UNS32 uiI2C_Controller, UNS32 uiIntrFlags);

/*********************************************************************************************
Function Name : DC4_I2C_IRQDisable
Description   : This fucntion configure Interrupt of the I2C controller.
Prototype     : void DC4_I2C_IRQDisable(UNS32 uiI2C_Controller, UNS32 uiIntrFlags)
Parameters    : [uiI2C_Controller - Base address of controller
				uiIntrFlags - Interrupt flags for which Interrupts has to be disabled
Returns       : void
***********************************************************************************************/
void DC4_I2C_IRQDisable(UNS32 uiI2C_Controller, UNS32 uiIntrFlags);

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
UNS32 DC4_I2C_ReadrWrite(UNS32 uiI2C_Controller, UNS16 slave_addr, UNS8 *src_buffer, UNS16 *Data_length, UNS16 usFlags);

void DC4_I2C_EnableController();

void I2C_Test_Terminal();
void DC4_I2C_ISR(UNS32 uiI2C_Controller);

#endif /*_I2C_H_*/

