/*
 * File:   main.c
 * Author: sglee
 *
 * Created on 2018-9-12, AM 2:03
 */

// PIC18F4520 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = OFF       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#define _XTAL_FREQ 1000000

#include <pic18.h>

//#define _16BIT_TIMER
#define _8BIT_TIMER

/*    
 https://www.exploreembedded.com/wiki/PIC18F4520_Timer

RegValue = TimerMax-((Delay * Fosc)/(Prescalar*4))
delay = ((MaxValue-REG_val)*(Prescal*4))/Fosc

regvalue = 256 - ((0.02s * 1000000)/(32*4))
	= 256 - ((20000)/128))
	= 256 - 156.25
	= 99.75

delay = (256 - 101) * (32 * 4)/1000000 
	= 150 * 128 / 1000000
	= 19200 / 1000000
	= 0.0192s 
	= 19.2ms
	= 20ms
 
regvalue = 65536 - ((0.02s * 1000000)/(32*4))
	= 65536 - ((20000)/128))
	= 65536 - 156.25
	= 65379.75
    = 0xFF63

 
delay = (65536 - 65379) * (32 * 4)/1000000 
        = (157) * (128)/1000000
        = 0.020096s
*/

//void __interrupt(high_priority) isr_low (void)
void __interrupt(low_priority) isr_low (void)
{      
#ifdef _16BIT_TIMER    
    TMR0H = 0xFF;
    TMR0L = 0x63;
#endif
#ifdef _8BIT_TIMER
    TMR0 = 100;
#endif
    
    TMR0IF = 0;
    
    PORTB = ~PORTB;
}

void main(void) 
{
    TRISB = 0X00; // output
    PORTB = 0;
    
    TMR0IP = 0; // TMR0 Overflow Interrupt Priority bit
                // 0 Low, 1 High
    INTCON = 0xA0; // GIE PEIE TMR0IE INT0IE RBIE TMR0IF INT0IF RBIF
                   // 1   0    1      0      0    0       0
                   // GIE/GIEH: Global Interrupt Enable bit
                   // PEIE/GIEL: Peripheral Interrupt Enable bit
                   // TMR0IE: TMR0 Overflow Interrupt Enable bit
                   // INT0IE: INT0 External Interrupt Enable bit
                   // RBIE: RB Port Change Interrupt Enable bit
                   // TMR0IF: TMR0 Overflow Interrupt Flag bit
                   // INT0IF: INT0 External Interrupt Flag bit
                   // RBIF: RB Port Change Interrupt Flag bit
#ifdef _16BIT_TIMER    
    TMR0H = 0xFF;
    TMR0L = 0x63;
#endif
#ifdef _8BIT_TIMER 
    TMR0 = 100;
#endif    
#ifdef _16BIT_TIMER    
    T0CON = 0x84; // 1:32 prescale value
                  // TMR0ON T08BIT T0CS T0SE PSA T0PS2 T0PS1 T0PS0
                  // 1       0      0    0    0  1      0     0
                  // TMR0ON: Timer0 On/Off Control bit
                  // T08BIT: Timer0 8-bit/16-bit Control bit
                  // T0CS: Timer0 Clock Source Select bit
                  // T0SE: Timer0 Source Edge Select bit
                  // PSA: Timer0 Prescaler Assignment bit
                  // T0PS2:T0PS0: Timer0 Prescaler Select bits
#endif    
#ifdef _8BIT_TIMER
    T0CON = 0xC4;
#endif
    while(1)
    {    
        __delay_ms(100);
    }
}