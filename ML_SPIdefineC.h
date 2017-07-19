/* Title: ML_defineC.h
 * Application: SPI Configuration
 * Copyright: HCL Technologies
 * Author: Gautham.K
 * Creation Date: 14/7/2011
 * Requirements: Header file to be included in ML_SPI.c and main.c
 * Description: Defining the address of the PRCM, SPI registers base address.
 *
 * Limitations: NONE
 * Dependencies: TMS320DM8148 Compiler
 * Modification: Initial Version*/


#ifndef ML_SPIDEFINEC_H_
#define ML_SPIDEFINEC_H_

#define Uint32  unsigned int
#define Uint16  unsigned short
#define Uint8   unsigned char
#define Int32   int
#define Int16   short
#define Int8    char

#define _ARM_ //Disable this to use for DSP.

#ifdef _ARM_
	#define PRCM_BASE_ADDR					0x48180000

	#define MCSPI1_BASE		0x48030100 /* only mcspi-1 is used */
	#define MCSPI2_BASE		0x481A0000
	#define MCSPI3_BASE		0x481A2000
	#define MCSPI4_BASE		0x481A4000
	#define CFG_MOD_BASE	0x48140000
#else
	#define PRCM_BASE_ADDR					0x08180000

	#define MCSPI1_BASE		0x08030100 /* only mcspi-1 is used */
	#define MCSPI2_BASE		0x081A0000
	#define MCSPI3_BASE		0x081A2000
	#define MCSPI4_BASE		0x081A4000
	#define CFG_MOD_BASE	0x08140000
#endif
#define pCM_ALWON_L3_SLOW_CLKSTCTRL		(volatile unsigned int *)(PRCM_BASE_ADDR + 0x1400)
#define pCM_ALWON_SPI_CLKCTRL           (volatile unsigned int *)(PRCM_BASE_ADDR + 0x1590)
#define pCM_SYSCLK10_CLKSEL				(volatile unsigned int *)(PRCM_BASE_ADDR + 0x0324)

#define MCSPI_MAX_FREQ	48000000

/* per-register bitmasks */
#define MCSPI_SYSCONFIG_SMARTIDLE	(2 << 3)
#define MCSPI_SYSCONFIG_ENAWAKEUP	(1 << 2)
#define MCSPI_SYSCONFIG_AUTOIDLE	(1 << 0)
#define MCSPI_SYSCONFIG_SOFTRESET	(1 << 1)

#define MCSPI_SYSSTATUS_RESETDONE	(1 << 0)

#define MCSPI_MODULCTRL_SINGLE	(1 << 0)
#define MCSPI_MODULCTRL_MS	(1 << 2)
#define MCSPI_MODULCTRL_STEST	(1 << 3)

#define MCSPI_CHCONF_PHA		(1 << 0)
#define MCSPI_CHCONF_POL		(1 << 1)
#define MCSPI_CHCONF_CLKD_MASK	(0x0f << 2)
#define MCSPI_CHCONF_EPOL		(1 << 6)
#define MCSPI_CHCONF_WL_MASK	(0x1f << 7)
#define MCSPI_CHCONF_TRM_RX_ONLY	(0x01 << 12)
#define MCSPI_CHCONF_TRM_TX_ONLY	(0x02 << 12)
#define MCSPI_CHCONF_TRM_MASK	(0x03 << 12)
#define MCSPI_CHCONF_DMAW		(1 << 14)
#define MCSPI_CHCONF_DMAR		(1 << 15)
#define MCSPI_CHCONF_DPE0		(1 << 16)
#define MCSPI_CHCONF_DPE1		(1 << 17)
#define MCSPI_CHCONF_IS		(1 << 18)
#define MCSPI_CHCONF_TURBO	(1 << 19)
#define MCSPI_CHCONF_FORCE	(1 << 20)
#define MCSPI_CHCONF_TCS		(1 << 25)
#define MCSPI_CHCONF_FFEW		(1 << 27)
#define MCSPI_CHCONF_FFER		(1 << 28)


#define MCSPI_CHSTAT_RXS		(1 << 0)
#define MCSPI_CHSTAT_TXS		(1 << 1)
#define MCSPI_CHSTAT_EOT		(1 << 2)

#define MCSPI_CHCTRL_EN		(1 << 0)

#define MCSPI_WAKEUPENABLE_WKEN	(1 << 0)

/* Common parameters */
#define	CFG_HZ				1000
#define SPI_FLASH_PROG_TIMEOUT		((100000 * CFG_HZ) / 1000)
#define SPI_FLASH_PAGE_ERASE_TIMEOUT	((500000 * CFG_HZ) / 1000)
#define SPI_FLASH_SECTOR_ERASE_TIMEOUT	(100000 * CFG_HZ)


/* SPI mode flags */
#define SPI_CPHA    0x01            /* clock phase */
#define SPI_CPOL    0x02            /* clock polarity */
#define SPI_MODE_0  (0|0)           /* (original MicroWire) */
#define SPI_MODE_1  (0|SPI_CPHA)
#define SPI_MODE_2  (SPI_CPOL|0)
#define SPI_MODE_3  (SPI_CPOL|SPI_CPHA)
#define SPI_CS_HIGH 0x04            /* CS active high */
#define SPI_LSB_FIRST   0x08            /* per-word bits-on-wire */
#define SPI_3WIRE   0x10            /* SI/SO signals shared */
#define SPI_LOOP    0x20            /* loopback mode */

/* Common commands */
#define CMD_READ_ID				0x9f

#define CMD_READ_ARRAY_SLOW		0x03
#define CMD_READ_ARRAY_FAST		0x0b
#define CMD_READ_ARRAY_LEGACY	0xe8

#endif /*ML_DEFINEC_H_*/
