/*
 * File:   main.c
 * Author: sglee
 *
 * Created on 2018-9-18, PM 8:29
 */

#include "configuration_bits.h"

#include "system.h"
#include "LCD_BD663474.h"
#include "TouchPanel_XPT2046.h"

void init_mcu(void)
{
    //ADCON1 = 0X07;      // 
    
	TRISAbits.RA0 = 1;  // touch
    
    TRISAbits.RA1 = 0;  // lcd_cs
    TRISAbits.RA2 = 0;  // lcd_rs
    
    TRISAbits.RA4 = 0;  // touch_cs
    
  	TRISDbits.RD0 = 0;  // lcd_rst
	
    //TRISCbits.RC0 = 0;  // ?
    //TRISCbits.RC1 = 0;
    //TRISCbits.RC2 = 0;
    
	TRISB = 0x00;  // led    
    LATB = 0x00;
}

void main(void)
{
	init_mcu();

	lcd_init();

    lcd_display_test();
    __delay_ms(1000);
    
    lcd_clear_screen(RED);
	lcd_display_string( (unsigned char*)"0st Line                      ", BLACK, GREEN, 0, 0 );
	lcd_display_string( (unsigned char*)"123456789012345678901234567890", BLACK, GREEN, 0, 1 );
    lcd_display_string( (unsigned char*)"240RGB x 320-Dot              ", BLACK, GREEN, 0, 2 );
	lcd_display_string( (unsigned char*)"262,144-Color TFT-LCD Panel   ", BLACK, GREEN, 0, 3 );
	lcd_display_string( (unsigned char*)"8 x 16 dot font               ", BLACK, GREEN, 0, 4 );
	lcd_display_string( (unsigned char*)"30 strings x 20 lines         ", BLACK, GREEN, 0, 5 );
	lcd_display_string( (unsigned char*)"                              ", BLACK, GREEN, 0, 7 );
	lcd_display_string( (unsigned char*)"                              ", BLACK, GREEN, 0, 9 );
	lcd_display_string( (unsigned char*)"                              ", BLACK, GREEN, 0, 11 );
	lcd_display_string( (unsigned char*)"                              ", BLACK, GREEN, 0, 13 );
	lcd_display_string( (unsigned char*)"                              ", BLACK, GREEN, 0, 15 );
	lcd_display_string( (unsigned char*)"                              ", BLACK, GREEN, 0, 17 );
	lcd_display_string( (unsigned char*)"19st Line                     ", BLACK, GREEN, 0, 19 );
    __delay_ms(1000);
    
	TouchPanel_Calibrate(WHITE, BLUE);
    __delay_ms(1000);
    
	//TouchPanel_Calibrate2();
	GUI_Text(0, 0, (unsigned char*)"Touchpad:", WHITE, BLUE);
    
	while(1)
    {
        __delay_ms(5);
        
        if(!(TP_INT_IN))
        {
            TP_GetAdXY(&(display.x), &(display.y));
            //TP_DrawPoint(&(display.x), &(display.y));        
            Touch_Debug();            
        }
#if 0        
		getDisplayPoint(&display, Read_Ads7846(), &matrix ); 		
 	    //TP_DrawPoint(&(display.x), &(display.y));	
		Touch_Debug();
#endif        
	}
}
