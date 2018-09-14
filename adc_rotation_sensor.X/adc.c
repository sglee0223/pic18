/* 
 * File:   adc.c
 * Author: sglee
 *
 * Created on 2018-9-14, AM 0:21
 */

#include <xc.h> 

#include "adc.h"

void init_AD_PORT(void)               
{
    PORTA  = 0xFF;               
    TRISA  = 0xFF;    
    
    ADCON2 = 0x14;  // ADFM ? ACQT2 ACQT1 ACQT0 ADCS2 ADCS1 ADCS0
                    // 4 TAD, FOSC/4
    ADCON1 = 0x0C;  // ? ? VCFG1 VCFG0 PCFG3 PCFG2 PCFG1 PCFG0
                    // AN0 ~ 2, ADC port
    ADCON0 = 0x01;  // ? ? CHS3 CHS2 CHS1 CHS0 GO/DONE ADON
                    // ADC on              
 } 

unsigned long getADC(void)
{
    unsigned long  result;
    
    ADCON0bits.GO_DONE = 0x1;   // GO/DONE: A/D Conversion Status bit
                                // When ADON = 1:
                                    // 1 = A/D conversion in progress
                                    // 0 = A/D Idle
        
    while(ADCON0bits.GO_DONE);          

    result = ADRESH;           
    result = result << 8;       
    result = result + ADRESL;    
    result = result >> 6;   
    
    return result;
}
