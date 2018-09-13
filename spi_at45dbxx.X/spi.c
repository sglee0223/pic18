/* 
 * File:   spi.c
 * Author: sglee
 *
 * Created on 2018-9-13, PM 7:30
 */

#include <xc.h> 

#include "spi.h"
#include "at45dbxx.h"

void init_spi( unsigned char sync_mode, unsigned char bus_mode, unsigned char smp_phase)
{
    TRIS_SDI = 1;               // SDI input
    TRIS_SDO = 0;               // SD0 output   
    
    SSPSTAT &= 0x3F;        // power on state 
    
    SSPCON1 = 0x00;         // power on state
    SSPCON1 |= sync_mode;   // select serial mode       
                                // 0000 = SPI Master mode, clock = FOSC/4
   
    SSPSTAT |= smp_phase;   // select data input sample phase

    switch( bus_mode )
    {
        case 0:     // SPI bus mode 0,0
            SSPSTATbits.CKE = 1;    // data transmitted on rising edge
            break;    
        case 2:     // SPI bus mode 1,0
            SSPSTATbits.CKE = 1;    // data transmitted on falling edge
            SSPCON1bits.CKP = 1;    // clock idle state high
            break;
        case 3:     // SPI bus mode 1,1
            SSPCON1bits.CKP = 1;    // clock idle state high
            break;
        default:    // default SPI bus mode 0,1
            break;
   }
    
    switch( sync_mode )
    {
        case 4:             // slave mode w /SS enable
            TRIS_SCK = 1;   // define clock pin as input    
            TRIS_CS  = 1;   // define /SS1 pin as input
            break;
        case 5:             // slave mode w/o /SS enable
            TRIS_SCK = 1;   // define clock pin as input    
            break;        
        default:            // master mode, define clock pin as output
            TRIS_SCK = 0;   // define clock pin as output    
            break;
    }

    SSPCON1 |= 0x20;            // enable synchronous serial port,  0b00100000  Enable serial port and configures SCK, SDO, SDI
}

unsigned char spi_write( unsigned char *buf, unsigned short len )
{
    unsigned short i;
    unsigned char temp;
    
    for(i=0; i<len; i++)
    {
        temp = SSPBUF;           // Clears BF
        PIR1bits.SSPIF = 0;      // Clear interrupt flag
        SSPCON1bits.WCOL = 0;    //Clear any previous write collision
        SSPBUF = buf[i];         // write byte to SSPBUF register
        if ( SSPCON1 & 0x80 )    // test if write collision occurred
            return 0;       // if WCOL bit is set return negative #
        else
            while( !PIR1bits.SSPIF );  // wait until bus cycle complete
    }
    return 1;                // if WCOL bit is not set return non-negative#
}

unsigned char spi_read( unsigned char *buf, unsigned short len )
{
    unsigned short i;
    unsigned char temp;
  
    for(i=0; i<len; i++)
    {
        temp = SSPBUF;          // Clear BF
        PIR1bits.SSPIF = 0;     // Clear interrupt flag
        SSPBUF = 0x00;          // initiate bus cycle
        while(!PIR1bits.SSPIF); // wait until cycle complete

        buf[i] = SSPBUF;
    }
    return 1;
}

unsigned char spi_data_ready( void )
{
    if ( SSPSTATbits.BF )
        return 1;                // data in SSPBUF register
    else
        return 0;                 // no data in SSPBUF register
}


