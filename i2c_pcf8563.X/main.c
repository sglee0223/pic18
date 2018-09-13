/*
 * File:   main.c
 * Author: sglee
 *
 * Created on 2018-9-12, PM 2:11
 */

#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
#include "myprintf.h"

#include "configuration_bits.h"
#include "main.h"

#include "pcf8563.h"
#include "i2c.h"

void main(void) 
{
    init_uart();

    printf("i2c_pcf8563\r\n");
    
    init_i2c(_XTAL_FREQ, 100000);   // Initialize I2C Master with 100KHz clock
    init_pcf8563();
    
    PCF8563_setDate(1999, 12, 31);
    PCF8563_setTime(23, 59, 55);
    
    while(1)
    {
        PCF8563_getDate();
        PCF8563_getTime();
        __delay_ms(999);
    }
    
    return;
}
