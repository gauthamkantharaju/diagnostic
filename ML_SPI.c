/* Title: ML_SPI.c
* Application: SPI related functions definitions.
* Copyright: HCL Technologies
* Author: Gautham.K
* Creation Date: 14/7/2011
* Requirements: ML_defineC.h and ML_structC.h
* References:
* Description: SPI 'c' file where all SPI related functions
* 			   such as Reset, Write, Read, Slave setup are defined.
* Limitations: NONE
* Dependencies: TMS320DM8148 compiler
* Modification: Initial Version */

#include "ML_SPIstructC.h"
#include "C_Types.h"


inline static void writel(unsigned int val, unsigned int *ptr)
{
	*(volatile unsigned int*)ptr = val;

}

inline static unsigned int readl(unsigned int *ptr)
{
	return (*(volatile unsigned int*)ptr);

}

/*
* Name: DC4_spi_reset
* Description: This function is used to reset, configure SYSCONFIG
* 			   register of the particular SPI module/controller.
* Parameters: ds is a pointer to mcspi_spi_slave structure.
* 			  Used to write values to SYSCONFIG of MCSPI.
* Returns: None
* Globals Updated: Updates the MCSPI SYSCONFIG register in structure(mcspi).
* Tables Updated: None
*
*/
static void DC4_spi_reset(struct mcspi_spi_slave *ds)
{
	unsigned int tmp;

	writel(MCSPI_SYSCONFIG_SOFTRESET, &ds->regs->sysconfig);
	do {
		tmp = readl(&ds->regs->sysstatus);
	} while (!(tmp & MCSPI_SYSSTATUS_RESETDONE));

	writel(MCSPI_SYSCONFIG_AUTOIDLE |
	       MCSPI_SYSCONFIG_ENAWAKEUP |
	       MCSPI_SYSCONFIG_SMARTIDLE,
	       &ds->regs->sysconfig);

	writel(MCSPI_WAKEUPENABLE_WKEN, &ds->regs->wakeupenable);
}

/*
* Name: DC4_spi_setup_slave
* Description: This function only initializes the memory for the
* 			   mcspi_spi_slave structure.
* 			   Used to set the controller, chip-select
* 			   frequency and mode(POL and PHA) according to the
* 			   slave connected to the SPI controller.
* Parameters: bus is the SPI controller
* 			  cs is the chip-select i.e either 0,1,2,3 accordingly
* 			  max_hz is maximum hertz configured according to the
* 			  slave connected.
* 			  mode is about configuring POL and PHA.
* Returns: Address of the ds structure where bus, cs, max_hz and mode are
* 		   configured.
* Globals Updated: Updates the slave structure present in the
* 				   mcspi_spi_slave structure.
* Tables Updated: None
*
*/
struct spi_slave *DC4_spi_setup_slave(unsigned int bus, unsigned int cs,
				  unsigned int max_hz, unsigned int mode, unsigned int word_length)
{
	struct mcspi_spi_slave	*ds;

	ds = malloc(sizeof(struct mcspi_spi_slave));
	if (!ds) {
		printf("SPI error: malloc of SPI structure failed\n");
		return 0;
	}

	/*
	 * mcspi McSPI (MultiChannel SPI) has 4 busses (modules) with different number of
	 * chip selects (CS, channels):
	 * McSPI1 has 4 CS (bus 0, cs 0 - 3)
	 * McSPI2 has 2 CS (bus 1, cs 0 - 1)
	 * McSPI3 has 2 CS (bus 2, cs 0 - 1)
	 * McSPI4 has 1 CS (bus 3, cs 0)
	 */

	switch (bus) {
	case 0:
		ds->regs = (struct mcspi *)MCSPI1_BASE;
		break;
	case 1:
		ds->regs = (struct mcspi *)MCSPI2_BASE;
		break;
	case 2:
		ds->regs = (struct mcspi *)MCSPI3_BASE;
		break;
	case 3:
		ds->regs = (struct mcspi *)MCSPI4_BASE;
		break;
	default:
		printf("SPI error: unsupported bus %i. Supported busses 0 - 3\n", bus);
		return 0;
	}
	ds->slave.bus = bus;

	if (((bus == 0) && (cs > 3)) ||
	    ((bus == 1) && (cs > 1)) ||
	    ((bus == 2) && (cs > 1)) ||
	    ((bus == 3) && (cs > 0))) {

		printf("SPI error: unsupported chip select %i on bus %i\n", cs, bus);
		return 0;
	}
	ds->slave.cs = cs;

	if (max_hz > MCSPI_MAX_FREQ) {
		printf("SPI error: unsupported frequency %i Hz. Max frequency is 48 Mhz\n",
		       max_hz);
		return 0;
	}
	ds->freq = max_hz;

	if (mode > SPI_MODE_3) {
		printf("SPI error: unsupported SPI mode %i\n", mode);
		return 0;
	}
	ds->mode = mode;

	ds->word_length = word_length;

	return &ds->slave;
}

/*
* Name: DC4_spi_free_slave
* Description: This function is used to free the slave.
* Parameters: slave, pointer to spi_slave structure, which contains
* 			  information about bus/controller and chip-select.*
* Returns: NONE
* Globals Updated: ds pointer to mcspi_spi_slave, which contains information
* 				   about bus/controller, chip-select,MCSPI registers, frequency
* 				   and mode is freed.
* Tables Updated: None
*
*/
void DC4_spi_free_slave(struct spi_slave *slave)
{
	struct mcspi_spi_slave *ds = to_mcspi_spi(slave);

	free(ds);
}

/*
* Name: DC4_spi_claim_bus
* Description: This function is used to reset, configuring control registers
*			   clock divider value, channel configuration, chip-select polarity
*			   mode(POL, PHA), transmit/receive mode of the SPI controller.
* Parameters: slave, pointer to spi_slave structure, which contains
* 			  information about bus/controller and chip-select.
* Returns: NONE
* Globals Updated: ds pointer to mcspi_spi_slave, which contains information
* 				   about bus/controller, chip-select,MCSPI registers configurations,
* 				   frequency and mode.
* Tables Updated: None
*
*/
int DC4_spi_claim_bus(struct spi_slave *slave)
{
	struct mcspi_spi_slave *ds = to_mcspi_spi(slave);
	unsigned int conf, div = 0;

	/* McSPI global module configuration */

	/*
	 * setup when switching from (reset default) slave mode
	 * to single-channel master mode
	 */
	DC4_spi_reset(ds);
	conf = readl(&ds->regs->modulctrl);
	conf &= ~(MCSPI_MODULCTRL_STEST | MCSPI_MODULCTRL_MS);
	conf |= MCSPI_MODULCTRL_SINGLE;
	writel(conf, &ds->regs->modulctrl);

	/* McSPI individual channel configuration */

	/* Calculate clock divisor. Valid range: 0x0 - 0xC ( /1 - /4096 ) */
	if (ds->freq) {
		while (div <= 0xC && (MCSPI_MAX_FREQ / (1 << div)) > ds->freq)
			div++;
	} else
		div = 0xC;

	conf = readl(&ds->regs->channel[ds->slave.cs].chconf);
	conf |= MCSPI_CHCONF_TCS | MCSPI_CHCONF_FFEW |
			MCSPI_CHCONF_FFER;
	/* standard 4-wire master mode:  SCK, MOSI/out, MISO/in, nCS
	 * REVISIT: this controller could support SPI_3WIRE mode.
	 */
	conf &= ~(MCSPI_CHCONF_IS|MCSPI_CHCONF_DPE1);
	conf |= MCSPI_CHCONF_DPE0;

	/* wordlength */
	conf &= ~MCSPI_CHCONF_WL_MASK;
	conf |= (ds->word_length - 1) << 7;

	/* set chipselect polarity; manage with FORCE */
	if (!(ds->mode & SPI_CS_HIGH))
		conf |= MCSPI_CHCONF_EPOL;	/* active-low; normal */
	else
		conf &= ~MCSPI_CHCONF_EPOL;

	/* set clock divisor */
	conf &= ~MCSPI_CHCONF_CLKD_MASK;
	conf |= div << 2;

	/* set SPI mode 0..3 */
	if (ds->mode & SPI_CPOL)
		conf |= MCSPI_CHCONF_POL;
	else
		conf &= ~MCSPI_CHCONF_POL;
	if (ds->mode & SPI_CPHA)
		conf |= MCSPI_CHCONF_PHA;
	else
		conf &= ~MCSPI_CHCONF_PHA;

	/* Transmit & receive mode */
	conf &= ~MCSPI_CHCONF_TRM_MASK;

	writel(conf, &ds->regs->channel[ds->slave.cs].chconf);
	return 0;
}

/*
* Name: DC4_spi_release_bus
* Description: This function is used to release the bus/controller.
* Parameters: slave, pointer to spi_slave structure, which contains
* 			  information about bus/controller and chip-select.
* Returns: NONE
* Globals Updated:
* Tables Updated: None
*
*/
void DC4_spi_release_bus(struct spi_slave *slave)
{
	struct mcspi_spi_slave *ds = to_mcspi_spi(slave);

	/* Reset the SPI hardware */
	DC4_spi_reset(ds);
}

/*
* Name: DC4_spi_read_write
* Description: This function is used to write and read from the slave
* 			   device.
* Parameters: slave, pointer to spi_slave structure, which contains
* 			  information about bus/controller and chip-select.
* 			  bitlen is the no of bit to be transfered
* 			  dout is the pointer to the buffer where the transmitted data/command is present.
* 			  din is the pointer to the buffer where the read data is present.
* 			  flags is used chip select assertion/De-assertion.
* Returns: NONE
* Globals Updated: slave, pointer to spi_slave structure, which contains
* 			       information about bus/controller and chip-select.
* Tables Updated: None
*
*/
int DC4_spi_read_write(struct spi_slave *slave, unsigned int bitlen,
		const void *dout, void *din, unsigned long flags)
{
	struct mcspi_spi_slave *ds = to_mcspi_spi(slave);
	unsigned int	len, i;
	const UNS16	*txp = (UNS16 *)dout;
	UNS16		*rxp = (UNS16 *)din;
	UNS8 temp;

	if (bitlen == 0)
		/* Finish any previously submitted transfers */
		goto out;

	/*
	 * It's not clear how non-8-bit-aligned transfers are supposed to be
	 * represented as a stream of bytes...this is a limitation of
	 * the current SPI interface - here we terminate on receiving such a
	 * transfer request.
	 */
	if (bitlen % 16) {
		/* Errors always terminate an ongoing transfer */
		flags |= SPI_XFER_END;
		goto out;
	}

	len = bitlen / 16;

	if (flags & SPI_XFER_BEGIN) {
		/* enable McSPI channel */
		writel(MCSPI_CHCTRL_EN, &ds->regs->channel[ds->slave.cs].chctrl);

		writel((readl(&ds->regs->channel[ds->slave.cs].chconf) | (0x00100000)),
				&ds->regs->channel[ds->slave.cs].chconf);
	}

	/* Keep writing and reading 1 byte until done */
	for (i = 0; i < len; i++) {

		/* wait till TX register is empty (TXS == 1) */
		while ((readl(&ds->regs->channel[ds->slave.cs].chstat) & MCSPI_CHSTAT_TXS) == 0);

		/* Write the data */
		if (txp) {
			writel(*txp, &ds->regs->channel[ds->slave.cs].tx);
			txp++;
		}
		else
			writel(0, &ds->regs->channel[ds->slave.cs].tx);

		/* Wait till RX register contains data (RXS == 1) */
		while ((readl(&ds->regs->channel[ds->slave.cs].chstat) & MCSPI_CHSTAT_RXS) == 0);

		/* Read the data */
		if  (rxp) {
			*rxp = readl(&ds->regs->channel[ds->slave.cs].rx);
			rxp++;
		}
		else
			temp = readl(&ds->regs->channel[ds->slave.cs].rx);

	}

out:
	if (flags & SPI_XFER_END) {
		writel((readl(&ds->regs->channel[ds->slave.cs].chconf) & (~0x00100000)),
				&ds->regs->channel[ds->slave.cs].chconf);

		/* Disable McSPI channel */
		writel(0, &ds->regs->channel[ds->slave.cs].chctrl);

	}

	return 0;
}

int DC4_spi_flash_cmd(struct spi_slave *spi, UNS8 cmd, void *response, UNS32 len)
{
	unsigned long flags = SPI_XFER_BEGIN;
	int ret;

	if (len == 0)
		flags |= SPI_XFER_END;

	ret = DC4_spi_read_write(spi, 8, &cmd, 0, flags);
	if (ret) {
		printf("SF: Failed to send command %02x: %d\n", cmd, ret);
		return ret;
	}

	if (len) {
		ret = DC4_spi_read_write(spi, len * 8, 0, response, SPI_XFER_END);
		if (ret)
			printf("SF: Failed to read response (%zu bytes): %d\n",
					len, ret);
	}

	return ret;
}

/*int DC4_spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
	return 1;
}

void DC4_spi_cs_activate(struct spi_slave *slave)
{
	 nothing to do
}

void DC4_spi_cs_deactivate(struct spi_slave *slave)
{
	 nothing to do
}

void DC4_spi_init()
{
	 do nothing
}*/


