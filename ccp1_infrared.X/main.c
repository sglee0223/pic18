/*
 * File:   main.c
 * Author: sglee
 *
 * Created on 2018-9-12, AM 2:03
 */

#include <xc.h>

#include "main.h"
#include "configuration_bits.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memset
#include "myprintf.h"


unsigned int ir_cnt = 0;
unsigned char ir_arr[9] = {0x00, };
unsigned char ir_start = 0, ir_end = 0, ir_result = 0;

/* 
http://ww1.microchip.com/downloads/en/DeviceDoc/31014a.pdf
   
https://www.exploreembedded.com/wiki/PIC18F4520_Timer

RegValue = TimerMax-((Delay * Fosc)/(Prescalar*4))
delay = ((MaxValue-REG_val)*(Prescal*4))/Fosc
 
regvalue = 65536 - ...

delay = (65536 - 0) * (1 * 4)/4000000 
        = (65536) * (4)/4000000
        = 0.065536 s
*/

//void __interrupt(high_priority) isr_low (void)
void __interrupt(low_priority) isr_low (void)
{      
    unsigned char i;
    
    if( PIE1bits.TMR1IE && PIR1bits.TMR1IF )
    {
        PIR1bits.TMR1IF = 0;
        TMR1L = 0;
        TMR1H = 0;
        
        LATB0 = (PORTBbits.RB0) ^ 0x01; // 7.6Hz, 0.065s
    }
    
    if( PIR1bits.CCP1IF )
    {  
        LATB1 = (PORTBbits.RB1) ^ 0x01;
        
        ir_cnt++;
        
        PIE1bits.CCP1IE = 0; 
        PIR1bits.CCP1IF = 0;
        
        if( ir_cnt > 0) // first falling edge
        {
            ir_start = 1;
        }   
        
        if( (20 < ir_cnt ) &&  (ir_cnt <= 28) ) // 20 < cnt <= 28
        { 
            ir_arr[ir_cnt - 21] = CCPR1H; // Capture/Compare/PWM Register 1, High Byte
        }
        
        if( ir_cnt == 36 ) // 36 falling edge
        {
            ir_cnt = 0; 
            for(i = 0;i < 8;i++)  
            {
                ir_result >>= 1;
                if( ir_arr[i] > 6 ) // maybe 0.065536 * 6 = 0.393 time over
                    ir_result |= 0x80;
            }
            ir_end = 1;
        }
        
        PIE1bits.CCP1IE = 1;
        
        TMR1L = 0;
        TMR1H = 0;        
    } 
}

void ccp_interrupt_init()
{
    TRISC2 = 1; // RC2 input

    INTCONbits.GIE = 1;    // Global Interrupt Enable bit
    INTCONbits.PEIE = 1;   // Peripheral Interrupt Enable bit

    PIE1bits.CCP1IE = 1;     // CCP1 Interrupt Enable bit
    
    CCP1CONbits.CCP1M = 0x04;   // CCP1CON REGISTER
                                // P1M1 P1M0 DC1B1 DC1B0 CCP1M3 CCP1M2 CCP1M1 CCP1M0
                                // P1M1:P1M0: Enhanced PWM Output Configuration bits
                                // DC1B1:DC1B0: PWM Duty Cycle bit 1 and bit 0
                                // CCP1M3:CCP1M0: Enhanced CCP Mode Select bits
                                    // 0100 = Capture mode, every falling edge
    PIR1bits.CCP1IF = 0;    // CCP1 Interrupt Flag bit

    T1CONbits.T1CKPS0 = 0;      // T1CKPS1:T1CKPS0: Timer1 Input Clock Prescale Select bits                        
    T1CONbits.T1CKPS1 = 0;      // 11 = 1:8 Prescale value
                                // 10 = 1:4 Prescale value
                                // 01 = 1:2 Prescale value
                                // 00 = 1:1 Prescale value
    T1CONbits.TMR1CS  = 0;  // TMR1CS: Timer1 Clock Source Select bit           
                                // 1 = External clock from pin RC0/T1OSO/T13CKI (on the rising edge)
                                // 0 = Internal clock (FOSC/4)
        
    // Timer 1 Interrupt on
    PIE1bits.TMR1IE = 1;      
    TMR1L  = 0;     
    TMR1H  = 0;
    T1CONbits.TMR1ON = 1;  
}

void main(void) 
{
    init_uart();
    ccp_interrupt_init();

    TRISB = 0X00;
    
    printf("ccp1_infrared\r\n");
            
    while(1)
    {
        if ( ir_start )
        {
            __delay_ms(500);
            
            //PORTB = result;
            printf("result[0x%x]\r\n", ir_result);
            
            ir_start = 0;            
            ir_end = 0;
            ir_cnt = 0;
            ir_result = 0;
            memset(ir_arr, 0x00, sizeof(ir_arr));
        }
    }
}