/* Title: ML_structC.h
 * Application: SPI Configuration
 * Copyright: HCL Technologies
 * Author: Gautham.K
 * Creation Date: 14/7/2011
 * Requirements: Header file to be included in ML_SPI.c and main.c
 * Description: Structure declaration for SPI module
 * Limitations: NONE
 * Dependencies: TMS470 Compiler
 * Modification: Initial Version*/

#ifndef ML_SPISTRUCTC_H
#define ML_SPISTRUCTC_H

#include "ML_SPIdefineC.h"

/* mcspi McSPI registers */
struct mcspi_channel {
	unsigned int chconf;		/* 0x2C, 0x40, 0x54, 0x68 */
	unsigned int chstat;		/* 0x30, 0x44, 0x58, 0x6C */
	unsigned int chctrl;		/* 0x34, 0x48, 0x5C, 0x70 */
	unsigned int tx;		/* 0x38, 0x4C, 0x60, 0x74 */
	unsigned int rx;		/* 0x3C, 0x50, 0x64, 0x78 */
};

struct mcspi {
	unsigned char res1[0x10];
	unsigned int sysconfig;		/* 0x10 */
	unsigned int sysstatus;		/* 0x14 */
	unsigned int irqstatus;		/* 0x18 */
	unsigned int irqenable;		/* 0x1C */
	unsigned int wakeupenable;	/* 0x20 */
	unsigned int syst;		/* 0x24 */
	unsigned int modulctrl;		/* 0x28 */
	struct mcspi_channel channel[4]; /* channel0: 0x2C - 0x3C, bus 0 & 1 & 2 & 3 */
					/* channel1: 0x40 - 0x50, bus 0 & 1 */
					/* channel2: 0x54 - 0x64, bus 0 & 1 */
					/* channel3: 0x68 - 0x78, bus 0 */
};



/*-----------------------------------------------------------------------
 * Representation of a SPI slave, i.e. what we're communicating with.
 *
 * Drivers are expected to extend this with controller-specific data.
 *
 *   bus:       ID of the bus that the slave is attached to.
 *   cs:        ID of the chip select connected to the slave.
 */
struct spi_slave {
        unsigned int    bus;
        unsigned int    cs;
};


/* SPI transfer flags */
#define SPI_XFER_BEGIN  0x01		/* Assert CS before transfer */
#define SPI_XFER_END    0x02        /* Deassert CS after transfer */

struct mcspi_spi_slave {
	struct spi_slave	slave;
	struct mcspi		*regs;
	unsigned int		freq;
	unsigned int		mode;
	unsigned int 		word_length;
};

#define offsetof(TYPE, MEMBER) ((Uint32) &((TYPE *)0)->MEMBER)
/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:        the pointer to the member.
 * @type:       the type of the container struct this is embedded in.
 * @member:     the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) \
		( (type *)( (char *)ptr - offsetof(type,member) ) )

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

static inline struct mcspi_spi_slave *to_mcspi_spi(struct spi_slave *slave)
{
	return container_of(slave, struct mcspi_spi_slave, slave);
}

int DC4_spi_claim_bus(struct spi_slave *slave);
struct spi_slave *DC4_spi_setup_slave(unsigned int bus, unsigned int cs, unsigned int max_hz, unsigned int mode, unsigned int word_lenght);
int DC4_spi_read_write(struct spi_slave *slave, unsigned int bitlen, const void *dout, void *din, unsigned long flags);
void DC4_spi_release_bus(struct spi_slave *slave);
void DC4_spi_free_slave(struct spi_slave *slave);

void DC4_spi_init();
int DC4_spi_cs_is_valid(unsigned int bus, unsigned int cs);
void DC4_spi_cs_activate(struct spi_slave *slave);
void DC4_spi_cs_deactivate(struct spi_slave *slave);

int DC4_spi_flash_cmd(struct spi_slave *spi, Uint8 cmd, void *response, Uint32 len);

#endif /* ML_STRUCTC_H */
