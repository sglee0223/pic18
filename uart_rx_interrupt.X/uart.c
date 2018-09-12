/* 
 * File:   myprintf.c
 * Author: sglee
 *
 * Created on 2018-9-12, PM 2:13
 */

#include <xc.h> 

#include "uart.h"

//void __interrupt(high_priority) isr_low (void)
void __interrupt(low_priority) isr_low (void)
{
    if (RCIE && RCIF)           
    {
        TXREG = RCREG;          
    }
 }

void init_uart(void)
{
    TRISC=0X80;
  
    TXSTA = 0x24;         // CSRC TX9 TXEN SYNC SENDB BRGH TRMT TX9D
                          // 0    0   1    0    0     1    0    0
                          // CSRC: Clock Source Select bit
                          // TX9: 9-bit Transmit Enable bit
                          // TXEN: Transmit Enable bit
                          // SYNC: EUSART Mode Select bit
                          // SENDB: Send Break Character bit
                          // BRGH: High Baud Rate Select bit
                          // TRMT: Transmit Shift Register Status bit
                          // TX9D: 9th bit of Transmit Data
    
    RCSTA = 0x90;         // SPEN RX9 SREN CREN ADDEN FERR OERR RX9D
                          // 1    0   0    0    0     0    0    0   
                          // SPEN: Serial Port Enable bit
                          // RX9: 9-bit Receive Enable bit
                          // SREN: Single Receive Enable bit
                          // CREN: Continuous Receive Enable bit
                          // ADDEN: Address Detect Enable bit
                          // FERR: Framing Error bit
                          // OERR: Overrun Error bit
                          // RX9D: 9th bit of Received Data
    
    SPBRG = 12;           // 4MHz, 19200
    
    RCIE = 0x01;                 
    GIE  = 0x01;                    
    PEIE = 0x01;
}

void putch(unsigned char byte)
{
    while(!TXIF);
 
    TXREG = byte;
}
