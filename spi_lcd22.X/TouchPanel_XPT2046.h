#ifndef __TOUCHPANEL_H__
#define __TOUCHPANEL_H__

#include "LCD_BD663474.h"

#define TOUCH_nCS_H()    {LATAbits.LA4=1;}
#define TOUCH_nCS_L()    {LATAbits.LA4=0;}

#define TP_INT_IN       PORTAbits.RA0

typedef	struct POINT 
{
   unsigned short x;
   unsigned short y;
}Coordinate;

typedef struct Matrix 
{						
	long double An,  
							Bn,     
							Cn,   
							Dn,    
							En,    
							Fn,     
							Divider ;
} Matrix ;

extern Coordinate ScreenSample[3];
extern Coordinate DisplaySample[3];
extern Matrix matrix ;
extern Coordinate  display ;

#define	CHX 	0x90
#define	CHY 	0xd0

void Touch_Init(void);
void TP_GetAdXY(unsigned short *x, unsigned short *y);
Coordinate *Read_Ads7846(void);
void TouchPanel_Calibrate(unsigned short charColor, unsigned short bkColor );
void DrawCross(unsigned short Xpos,unsigned short Ypos,int color);
void TP_DrawPoint(unsigned short Xpos,unsigned short Ypos);
unsigned char setCalibrationMatrix( Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr);
unsigned char getDisplayPoint(Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr );
void TouchPanel_Calibrate2(void);
void Touch_Debug(void);
#endif 
