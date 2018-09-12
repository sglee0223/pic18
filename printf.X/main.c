/*
 * File:   main.c
 * Author: sglee
 *
 * Created on 2018-9-12, PM 2:11
 */

#include <xc.h>

#define _XTAL_FREQ 4000000
#include <pic18.h>

#include <stdio.h>
#include <stdlib.h>
#include "configuration_bits.h"
#include "myprintf.h"

void main(void) 
{
    init_uart();

    printf("Hello, Wolrd\r\n");
    printf("character : %c\r\n", 'A');
    printf("string : %s\r\n", "Good");
    printf("decimal : %d\r\n", 888);
    printf("float : %f\r\n", 0.888);
    printf("hex : 0x%x\r\n", 0xA0);
    
    while(1)
    {
        __delay_ms(1000);
    }
    return;
}