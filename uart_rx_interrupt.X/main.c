/*
 * File:   main.c
 * Author: sglee
 *
 * Created on 2018-9-12, PM 10:40
 */

#include <xc.h>

#define _XTAL_FREQ 4000000
#include <pic18.h>

#include <stdio.h>
#include <stdlib.h>
#include "configuration_bits.h"
#include "uart.h"

void main(void) 
{
    init_uart();
    
    while(1)
    {
        __delay_ms(1000);
    }
    return;
}