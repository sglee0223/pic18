/*
 * File:   main.c
 * Author: sglee
 *
 * Created on 2018-9-13, PM 7:29
 */

#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
#include "myprintf.h"

#include "configuration_bits.h"
#include "main.h"

#include "at45dbxx.h"
#include "spi.h"

void main(void) 
{
    init_uart();

    printf("spi_at45dbxx\r\n");
    
    init_spi(0, 0, 0);  // SPI Master mode, clock = FOSC/4
                        // data transmitted on rising edge, clock idle state low
                        // Slew rate control enabled for high-speed mode (400 kHz)
    
    at45db_initialize();
    at45db_test();

    while(1)
    {
        __delay_ms(999);
    }
    
    return;
}
