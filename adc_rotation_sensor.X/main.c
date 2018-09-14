/*
 * File:   main.c
 * Author: sglee
 *
 * Created on 2018-9-14, AM 0:20
 */

#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
#include "myprintf.h"

#include "configuration_bits.h"
#include "main.h"

#include "adc.h"

void main(void) 
{
    unsigned long adcValue;
    
    init_uart();

    printf("adc_rotation_sensor\r\n");

    init_AD_PORT();

    while(1)
    {
        adcValue = getADC();
        printf("adc : %d\r\n", adcValue); 
        
        __delay_ms(999);
    }
    
    return;
}
