#ifndef _SPI_H
#define	_SPI_H

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#define TRIS_SDI TRISCbits.TRISC4      // DIRECTION define SDI input
#define TRIS_SCK TRISCbits.TRISC3      // DIRECTION define clock pin as output
#define TRIS_CS  TRISAbits.TRISA5      // DIRECTION CS - RA5
#define TRIS_SDO TRISCbits.TRISC5      //DIRECTION SD0 as output
    
void init_spi( unsigned char sync_mode, unsigned char bus_mode, unsigned char smp_phase);   
unsigned char spi_write( unsigned char *buf, unsigned short len );
unsigned char spi_read( unsigned char *buf, unsigned short len );
unsigned char spi_data_ready( void );

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* _SPI_H */
