/************************************************************************************************************
 *
 * Title			: McASP_LLD.h
 *
 * Application		: Bighill project
 *
 * Author			: Mahantesh.Kumbar
 *
 * Creation Date    : 10/06/2011
 *
 * Description      : Defining the address of the I2C configuration registers & Bit definition
 *
 * Limitations      : Supported for Polling method only
 *
 *
 *----------------------------------------------------------------------------
 * Modification History
 *----------------------------------------------------------------------------
 * Ver 	Date       Initials       	Description
 *----------------------------------------------------------------------------
 * 0.1	10/06/2011 Mahantesh.Kumbar Initial Version
 *
 *
 *---------------------------------------------------------------------------
************************************************************************************************************/
#ifndef _McASP_H_
#define _McASP_H_

/*------------------------------------------- Include files -----------------------------------------------*/
#include "C_Types.h"

/*-------------------------------- Symbolic Constants and Macros (defines) --------------------------------*/
#define _ARM_ //Disable this to use for DSP.

#ifdef _ARM_
	#define McASP_CONTROLLER_0	0x48038000
	#define McASP_CONTROLLER_1	0x4803C000
	#define McASP_CONTROLLER_2	0x48050000
	#define McASP_CONTROLLER_3	0x4A1A2000
	#define McASP_CONTROLLER_4	0x4A1A8000
	#define McASP_CONTROLLER_5	0x4A1AE000

	#define pL3_SLOW_CLKSTCTRL			((VUNS32 *)0x48181400)
	#define pCM_ALWON_McASP_2_CLKCTRL 	((VUNS32 *)0x48181548)
	#define pCM_SYSCLK20_CLKSEL 		((VUNS32 *)0x48180350)
	#define pCM_AUDIOCLK_MCASP2_CLKSEL 	((VUNS32 *)0x48180384)
#else
	#define McASP_CONTROLLER_0	0x08038000
	#define McASP_CONTROLLER_1	0x0803C000
	#define McASP_CONTROLLER_2	0x08050000
	#define McASP_CONTROLLER_3	0x0A1A2000
	#define McASP_CONTROLLER_4	0x0A1A8000
	#define McASP_CONTROLLER_5	0x0A1AE000

	#define pL3_SLOW_CLKSTCTRL			((VUNS32 *)0x08181400)
	#define pCM_ALWON_McASP_2_CLKCTRL 	((VUNS32 *)0x08181548)
	#define pCM_SYSCLK20_CLKSEL 		((VUNS32 *)0x08180350)
	#define pCM_AUDIOCLK_MCASP2_CLKSEL 	((VUNS32 *)0x08180384)
#endif


/*
 * DAVINCI_MCASP_PFUNC_REG - Pin Function / GPIO Enable Register Bits
 */
#define AXR(n)          (1<<n)
#define PFUNC_AMUTE     BIT(25)
#define ACLKX           BIT(26)
#define AHCLKX          BIT(27)
#define AFSX            BIT(28)
#define ACLKR           BIT(29)
#define AHCLKR          BIT(30)
#define AFSR            BIT(31)

/*
 * DAVINCI_MCASP_PDIR_REG - Pin Direction Register Bits
 */
#define AXR(n)          (1<<n)
#define PDIR_AMUTE      BIT(25)
#define ACLKX           BIT(26)
#define AHCLKX          BIT(27)
#define AFSX            BIT(28)
#define ACLKR           BIT(29)
#define AHCLKR          BIT(30)
#define AFSR            BIT(31)

/*
 * DAVINCI_MCASP_TXDITCTL_REG - Transmit DIT Control Register Bits
 */
#define DITEN   BIT(0)  /* Transmit DIT mode enable/disable */
#define VA      BIT(2)
#define VB      BIT(3)

/*
 * DAVINCI_MCASP_TXFMT_REG - Transmit Bitstream Format Register Bits
 */
#define TXROT(val)      (val)
#define TXSEL           BIT(3)
#define TXSSZ(val)      (val<<4)
#define TXPBIT(val)     (val<<8)
#define TXPAD(val)      (val<<13)
#define FSXDLY(val)     (val<<16)

/*
 * DAVINCI_MCASP_RXFMT_REG - Receive Bitstream Format Register Bits
 */
#define RXROT(val)      (val)
#define RXSEL           BIT(3)
#define RXSSZ(val)      (val<<4)
#define RXPBIT(val)     (val<<8)
#define RXPAD(val)      (val<<13)
#define RXORD           BIT(15)
#define FSRDLY(val)     (val<<16)

/*
 * DAVINCI_MCASP_TXFMCTL_REG -  Transmit Frame Control Register Bits
 */
#define FSXPOL          BIT(0)
#define AFSXE           BIT(1)
#define FSXDUR          BIT(4)
#define FSXMOD(val)     (val<<7)

/*
 * DAVINCI_MCASP_RXFMCTL_REG - Receive Frame Control Register Bits
 */
#define FSRPOL          BIT(0)
#define AFSRE           BIT(1)
#define FSRDUR          BIT(4)
#define FSRMOD(val)     (val<<7)

/*
 * DAVINCI_MCASP_ACLKXCTL_REG - Transmit Clock Control Register Bits
 */
#define ACLKXDIV(val)   (val)	//Clock Divider
#define ACLKXE          BIT(5)	//External or Internal Clock
#define TX_ASYNC        BIT(6)
#define ACLKXPOL        BIT(7)	//Clock Polarity

/*
 * DAVINCI_MCASP_ACLKRCTL_REG Receive Clock Control Register Bits
 */
#define ACLKRDIV(val)   (val)	//Clock Divider
#define ACLKRE          BIT(5)	//External or Internal Clock
#define RX_ASYNC        BIT(6)
#define ACLKRPOL        BIT(7)	//Clock Polarity

/*
 * DAVINCI_MCASP_AHCLKXCTL_REG - High Frequency Transmit Clock Control
 *     Register Bits
 */
#define AHCLKXDIV(val)  (val)
#define AHCLKXPOL       BIT(14)
#define AHCLKXE         BIT(15)

/*
 * DAVINCI_MCASP_AHCLKRCTL_REG - High Frequency Receive Clock Control
 *     Register Bits
 */
#define AHCLKRDIV(val)  (val)
#define AHCLKRPOL       BIT(14)
#define AHCLKRE         BIT(15)

/*
 * DAVINCI_MCASP_XRSRCTL_BASE_REG -  Serializer Control Register Bits
 */
#define MODE(val)       (val)
#define DISMOD(val)		(val<<2)
#define TXSTATE         BIT(4)
#define RXSTATE         BIT(5)

#define INACTIVE	0x00	//0 Serializer is inactive.
#define TRANSMITTER 0x01	//1h Serializer is transmitter.
#define RECEIVER	0x02	//2h Serializer is receiver.

#define TRI_STATE	0x00 	//0 Drive on pin is 3-state.
#define	LOGIC_LOW	0x02	//2h Drive on pin is logic low.
#define	LOGIC_HIGH	0x03	//3h Drive on pin is logic high.

/*
 * DAVINCI_MCASP_LBCTL_REG - Loop Back Control Register Bits
 */
#define LBEN            BIT(0)
#define LBORD           BIT(1)
#define LBGENMODE(val)  (val<<2)

/*
 * DAVINCI_MCASP_TXTDMSLOT_REG - Transmit TDM Slot Register configuration
 */
#define TXTDMS(n)       (1<<n)

/*
 * DAVINCI_MCASP_RXTDMSLOT_REG - Receive TDM Slot Register configuration
 */
#define RXTDMS(n)       (1<<n)

/*
 * DAVINCI_MCASP_GBLCTL_REG -  Global Control Register Bits
 */
#define RXCLKRST        BIT(0)  /* Receiver Clock Divider Reset */
#define RXHCLKRST       BIT(1)  /* Receiver High Frequency Clock Divider */
#define RXSERCLR        BIT(2)  /* Receiver Serializer Clear */
#define RXSMRST         BIT(3)  /* Receiver State Machine Reset */
#define RXFSRST         BIT(4)  /* Frame Sync Generator Reset */
#define TXCLKRST        BIT(8)  /* Transmitter Clock Divider Reset */
#define TXHCLKRST       BIT(9)  /* Transmitter High Frequency Clock Divider*/
#define TXSERCLR        BIT(10) /* Transmit Serializer Clear */
#define TXSMRST         BIT(11) /* Transmitter State Machine Reset */
#define TXFSRST         BIT(12) /* Frame Sync Generator Reset */

/*
 * DAVINCI_MCASP_AMUTE_REG -  Mute Control Register Bits
 */
#define MUTENA(val)     (val)
#define MUTEINPOL       BIT(2)
#define MUTEINENA       BIT(3)
#define MUTEIN          BIT(4)
#define MUTER           BIT(5)
#define MUTEX           BIT(6)
#define MUTEFSR         BIT(7)
#define MUTEFSX         BIT(8)
#define MUTEBADCLKR     BIT(9)
#define MUTEBADCLKX     BIT(10)
#define MUTERXDMAERR    BIT(11)
#define MUTETXDMAERR    BIT(12)

/*
 * DAVINCI_MCASP_REVTCTL_REG - Receiver DMA Event Control Register bits
 */
#define RXDATADMADIS    BIT(0)

/*
 * DAVINCI_MCASP_XEVTCTL_REG - Transmitter DMA Event Control Register bits
 */
#define TXDATADMADIS    BIT(0)

/*
 * DAVINCI_MCASP_W[R]FIFOCTL - Write/Read FIFO Control Register bits
 */
#define FIFO_ENABLE     BIT(16)
#define NUMEVT_MASK     (0xFF << 8)
#define NUMDMA_MASK     (0xFF)

#define DAVINCI_MCASP_NUM_SERIALIZER    6

/*------------------------------------------ Typedefs and structs -----------------------------------------*/
typedef struct  {

	volatile UNS32		McASP_REV;			/* 0h  */
	volatile UNS32		RSVD0[3];
	volatile UNS32		McASP_PFUNC;		/* 10h */
	volatile UNS32		McASP_PDIR;		   	/* 14h */
 	volatile UNS32		McASP_PDOUT;		/* 18h */
	volatile UNS32		McASP_PD_IN_SET;	/* 1Ch */
	volatile UNS32		McASP_PDCLR;		/* 20h */
	volatile UNS32		RSVD1[0x8];
	volatile UNS32		McASP_GBLCT;		/* 44h */
	volatile UNS32		McASP_AMUTE;		/* 48h */
	volatile UNS32		McASP_DLBCTL;		/* 4Ch */
	volatile UNS32		McASP_DITCTL;		/* 50h */
	volatile UNS32		RSVD2[0x3];
	volatile UNS32		McASP_RGBLCTL;		/* 60h */
	volatile UNS32		McASP_RMASK;		/* 64h */
	volatile UNS32		McASP_RFMT;			/* 68h */
	volatile UNS32		McASP_AFSRCTL;		/* 6Ch */
	volatile UNS32		McASP_ACLKRCTL;		/* 70h */
	volatile UNS32		McASP_AHCLKRCTL;	/* 74h */
	volatile UNS32		McASP_RTDM;			/* 78h */
	volatile UNS32		McASP_RINTCTL;		/* 7Ch */
	volatile UNS32		McASP_RSTAT;		/* 80h */
	volatile UNS32		McASP_RSLOT;		/* 84h */
	volatile UNS32		McASP_RCLKCHK;		/* 88h */
	volatile UNS32		McASP_REVTCTL;		/* 8Ch */
	volatile UNS32		RSVD3[0x4];
	volatile UNS32		McASP_XGBLCTL;		/* A0h */
	volatile UNS32		McASP_XMASK;		/* A4h */
	volatile UNS32		McASP_XFMT;			/* A8h */
	volatile UNS32		McASP_AFSXCTL;		/* ACh */
	volatile UNS32		McASP_ACLKXCTL;		/* B0h */
	volatile UNS32		McASP_AHCLKXCTL;	/* B4h */
	volatile UNS32		McASP_XTDM;			/* B8h */
	volatile UNS32		McASP_XINTCTL;		/* BCh */
	volatile UNS32		McASP_XSTAT;		/* C0h */
	volatile UNS32		McASP_XSLOT;		/* C4h */
	volatile UNS32		McASP_XCLKCHK;		/* C8h */
	volatile UNS32		McASP_XEVTCTL;		/* CCh */
	volatile UNS32		RSVD4[0xC];
	volatile UNS32		McASP_DITCSRA[6];	/* 100h - 114h */
	volatile UNS32		McASP_DITCSRB[6];	/* 118h - 12Ch */
	volatile UNS32		McASP_DITUDRA[6];	/* 130h - 144h */
	volatile UNS32	    McASP_DITUDRB[6];	/* 148h - 15Ch */
	volatile UNS32		RSVD5[8];
	volatile UNS32		McASP_SRCTL[6];		/* 180h - 194h*/
	volatile UNS32		RSVD6[0x1A];
	volatile UNS32		McASP_XBUF[6];		/* 200h - 214h*/
	volatile UNS32		RSVD7[0x1A];
	volatile UNS32		McASP_RBUF[6];		/* 280h - 294h */
	volatile UNS32		RSVD8[0x35A];
	volatile UNS32		McASP_WFIFOCTL;		/* 1000h */
	volatile UNS32		McASP_WFIFOSTS;		/* 1004h */
	volatile UNS32		McASP_RFIFOCTL;		/* 1008h */
	volatile UNS32		McASP_RFIFOSTS;		/* 100Ch */
}CSL_McASPRegs;

/*------------------------------------------ Function prototypes ------------------------------------------*/

/*------------------------------------ Declarations (statics and globals) ---------------------------------*/

/*---------------------------------------- Functions  Prototype ---------------------------------------------*/

#endif /*_McASP_H_*/
