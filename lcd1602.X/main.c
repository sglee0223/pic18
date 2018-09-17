/*
 * File:   main.c
 * Author: sglee
 *
 * Created on 2018-9-17, AM 1:21
 */

#include <xc.h>

#include <stdio.h>

#include "main.h"
#include "configuration_bits.h"

#define rs LATCbits.LATC4 // Register Select, High-Data, Low-Instruction
#define rw LATCbits.LATC5 // High-Read / Low-Write
#define e  LATCbits.LATC6 // Enable Signal

void write(char x)
 {
    PORTD = x;   
    
    rs = 1;                     
    rw = 0; 
    
    e = 1;         
    __delay_ms(3); // 3ms or up
    e = 0;                      
} 

void lcd_write(unsigned char *str, unsigned char len)
{
    int i;
    
    for(i = 0;i < len; i++)       
    {
        write(str[i]);       
    }
}

void lcd_enable()
 {
    rs = 0;                    
    rw = 0;  
    
    e = 1;                                   
    __delay_ms(3); // 3ms or up
    e = 0;                      
 }

void lcd_init()
{
    PORTD = 0x01; // Clear Display
    lcd_enable();
    
    PORTD = 0x38; // 8-bit mode, 2 lines, 5x7 format
    lcd_enable();
    
    PORTD = 0x0E; // Display On. Cursor appear.             
    lcd_enable();
    
    PORTD = 0x06; // Increment cursor
    lcd_enable();
    
    PORTD = 0x80; // Set cursor position to 1st line, 1st column
    lcd_enable();
}

void lcd_port_init()
 {
    TRISC = 0x00;             
    TRISD = 0x00;              
 }

void main(void) 
{
    unsigned char str[32] = {0,};
    
    lcd_port_init(); 
    lcd_init(); 

    sprintf(str, "%s", "..Using 1602 LCD");
    lcd_write(str, 16);     

    PORTD = 0XC0; // move to 2nd line
    lcd_enable(); 

    sprintf(str, "%s", ".......over 8bit");
    lcd_write(str, 16);  
        
    while(1)
    {
        __delay_ms(500);
    }
}