/*
 * Common SPI Interface: Controller-specific definitions
 *
 * (C) Copyright 2001
 * Gerald Van Baren, Custom IDEAS, vanbaren@cideas.com.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _SPI_H_
#define _SPI_H_

/* Controller-specific definitions: */

/* SPI mode flags */
#define	SPI_CPHA	0x01			/* clock phase */
#define	SPI_CPOL	0x02			/* clock polarity */
#define	SPI_MODE_0	(0|0)			/* (original MicroWire) */
#define	SPI_MODE_1	(0|SPI_CPHA)
#define	SPI_MODE_2	(SPI_CPOL|0)
#define	SPI_MODE_3	(SPI_CPOL|SPI_CPHA)
#define	SPI_CS_HIGH	0x04			/* CS active high */
#define	SPI_LSB_FIRST	0x08			/* per-word bits-on-wire */
#define	SPI_3WIRE	0x10			/* SI/SO signals shared */
#define	SPI_LOOP	0x20			/* loopback mode */
#define	SPI_SLAVE	0x40			/* slave mode */
#define	SPI_PREAMBLE	0x80			/* Skip preamble bytes */

/* SPI transfer flags */
#define SPI_XFER_BEGIN	0x01			/* Assert CS before transfer */
#define SPI_XFER_END	0x02			/* Deassert CS after transfer */

/* Header byte that marks the start of the message */
#define SPI_PREAMBLE_END_BYTE	0xec

/*-----------------------------------------------------------------------
 * Representation of a SPI slave, i.e. what we're communicating with.
 *
 * Drivers are expected to extend this with controller-specific data.
 *
 *   bus:	ID of the bus that the slave is attached to.
 *   cs:	ID of the chip select connected to the slave.
 *   max_write_size:	If non-zero, the maximum number of bytes which can
 *		be written at once, excluding command bytes.
 */
struct spi_slave {
	unsigned int	bus;
	unsigned int	cs;
	unsigned int max_write_size;
};

/*-----------------------------------------------------------------------
 * Initialization, must be called once on start up.
 *
 * TODO: I don't think we really need this.
 */
void spi_init(void);

/**
 * spi_do_alloc_slave - Allocate a new SPI slave (internal)
 *
 * Allocate and zero all fields in the spi slave, and set the bus/chip
 * select. Use the helper macro spi_alloc_slave() to call this.
 *
 * @offset: Offset of struct spi_slave within slave structure
 * @size: Size of slave structure
 * @bus: Bus ID of the slave chip.
 * @cs: Chip select ID of the slave chip on the specified bus.
 */
void *spi_do_alloc_slave(int offset, int size, unsigned int bus,
			 unsigned int cs);

/**
 * spi_alloc_slave - Allocate a new SPI slave
 *
 * Allocate and zero all fields in the spi slave, and set the bus/chip
 * select.
 *
 * @_struct: Name of structure to allocate (e.g. struct tegra_spi). This
 *	structure must contain a member 'struct spi_slave *slave'.
 * @bus: Bus ID of the slave chip.
 * @cs: Chip select ID of the slave chip on the specified bus.
 */
#define spi_alloc_slave(_struct, bus, cs) \
	spi_do_alloc_slave(offsetof(_struct, slave), \
			    sizeof(_struct), bus, cs)

/**
 * spi_alloc_slave_base - Allocate a new SPI slave with no private data
 *
 * Allocate and zero all fields in the spi slave, and set the bus/chip
 * select.
 *
 * @bus: Bus ID of the slave chip.
 * @cs: Chip select ID of the slave chip on the specified bus.
 */
#define spi_alloc_slave_base(bus, cs) \
	spi_do_alloc_slave(0, sizeof(struct spi_slave), bus, cs)

/*-----------------------------------------------------------------------
 * Set up communications parameters for a SPI slave.
 *
 * This must be called once for each slave. Note that this function
 * usually doesn't touch any actual hardware, it only initializes the
 * contents of spi_slave so that the hardware can be easily
 * initialized later.
 *
 *   bus:     Bus ID of the slave chip.
 *   cs:      Chip select ID of the slave chip on the specified bus.
 *   max_hz:  Maximum SCK rate in Hz.
 *   mode:    Clock polarity, clock phase and other parameters.
 *
 * Returns: A spi_slave reference that can be used in subsequent SPI
 * calls, or NULL if one or more of the parameters are not supported.
 */
struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int mode);

/*-----------------------------------------------------------------------
 * Free any memory associated with a SPI slave.
 *
 *   slave:	The SPI slave
 */
void spi_free_slave(struct spi_slave *slave);

/*-----------------------------------------------------------------------
 * Claim the bus and prepare it for communication with a given slave.
 *
 * This must be called before doing any transfers with a SPI slave. It
 * will enable and initialize any SPI hardware as necessary, and make
 * sure that the SCK line is in the correct idle state. It is not
 * allowed to claim the same bus for several slaves without releasing
 * the bus in between.
 *
 *   slave:	The SPI slave
 *
 * Returns: 0 if the bus was claimed successfully, or a negative value
 * if it wasn't.
 */
int spi_claim_bus(struct spi_slave *slave);

/*-----------------------------------------------------------------------
 * Release the SPI bus
 *
 * This must be called once for every call to spi_claim_bus() after
 * all transfers have finished. It may disable any SPI hardware as
 * appropriate.
 *
 *   slave:	The SPI slave
 */
void spi_release_bus(struct spi_slave *slave);

/*-----------------------------------------------------------------------
 * SPI transfer
 *
 * This writes "bitlen" bits out the SPI MOSI port and simultaneously clocks
 * "bitlen" bits in the SPI MISO port.  That's just the way SPI works.
 *
 * The source of the outgoing bits is the "dout" parameter and the
 * destination of the input bits is the "din" parameter.  Note that "dout"
 * and "din" can point to the same memory location, in which case the
 * input data overwrites the output data (since both are buffered by
 * temporary variables, this is OK).
 *
 * spi_xfer() interface:
 *   slave:	The SPI slave which will be sending/receiving the data.
 *   bitlen:	How many bits to write and read.
 *   dout:	Pointer to a string of bits to send out.  The bits are
 *		held in a byte array and are sent MSB first.
 *   din:	Pointer to a string of bits that will be filled in.
 *   flags:	A bitwise combination of SPI_XFER_* flags.
 *
 *   Returns: 0 on success, not 0 on failure
 */
int  spi_xfer(struct spi_slave *slave, unsigned int bitlen, const void *dout,
		void *din, unsigned long flags);

/*-----------------------------------------------------------------------
 * Determine if a SPI chipselect is valid.
 * This function is provided by the board if the low-level SPI driver
 * needs it to determine if a given chipselect is actually valid.
 *
 * Returns: 1 if bus:cs identifies a valid chip on this board, 0
 * otherwise.
 */
int  spi_cs_is_valid(unsigned int bus, unsigned int cs);

/*-----------------------------------------------------------------------
 * Activate a SPI chipselect.
 * This function is provided by the board code when using a driver
 * that can't control its chipselects automatically (e.g.
 * common/soft_spi.c). When called, it should activate the chip select
 * to the device identified by "slave".
 */
void spi_cs_activate(struct spi_slave *slave);

/*-----------------------------------------------------------------------
 * Deactivate a SPI chipselect.
 * This function is provided by the board code when using a driver
 * that can't control its chipselects automatically (e.g.
 * common/soft_spi.c). When called, it should deactivate the chip
 * select to the device identified by "slave".
 */
void spi_cs_deactivate(struct spi_slave *slave);

/*-----------------------------------------------------------------------
 * Set transfer speed.
 * This sets a new speed to be applied for next spi_xfer().
 *   slave:	The SPI slave
 *   hz:	The transfer speed
 */
void spi_set_speed(struct spi_slave *slave, uint hz);

/*-----------------------------------------------------------------------
 * Write 8 bits, then read 8 bits.
 *   slave:	The SPI slave we're communicating with
 *   byte:	Byte to be written
 *
 * Returns: The value that was read, or a negative value on error.
 *
 * TODO: This function probably shouldn't be inlined.
 */
static inline int spi_w8r8(struct spi_slave *slave, unsigned char byte)
{
	unsigned char dout[2];
	unsigned char din[2];
	int ret;

	dout[0] = byte;
	dout[1] = 0;

	ret = spi_xfer(slave, 16, dout, din, SPI_XFER_BEGIN | SPI_XFER_END);
	return ret < 0 ? ret : din[1];
}

/**
 * Set up a SPI slave for a particular device tree node
 *
 * This calls spi_setup_slave() with the correct bus number. Call
 * spi_free_slave() to free it later.
 *
 * @param blob		Device tree blob
 * @param node		SPI peripheral node to use
 * @param cs		Chip select to use
 * @param max_hz	Maximum SCK rate in Hz (0 for default)
 * @param mode		Clock polarity, clock phase and other parameters
 * @return pointer to new spi_slave structure
 */
struct spi_slave *spi_setup_slave_fdt(const void *blob, int node,
		unsigned int cs, unsigned int max_hz, unsigned int mode);

#endif	/* _SPI_H_ */
