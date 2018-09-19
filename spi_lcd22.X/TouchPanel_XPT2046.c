#include "system.h"

#include "TouchPanel_XPT2046.h"
#include "LCD_BD663474.h"

Matrix matrix ;
Coordinate  display ;

Coordinate ScreenSample[3] = {
	{ 0, 0},
	{ 0, 0},
	{ 0, 0}
};

Coordinate DisplaySample[3] = {
	{ 120,  40},
	{  40, 200},
	{ 200, 270}	
};

#define THRESHOLD 50

void Touch_Init(void)
{
    TRISC=0X10;
    
    SSPSTAT=0X80;
    SSPCON1=0X30;
    INTCON=0X00;
    PIR1=0X00;
}

void Touch_Transmit(unsigned char send_char)
{

}

unsigned short Touch_Receive(unsigned char send_char)
{
	unsigned char tmp;
    
	SSPBUF = send_char;
	while(!SSPIF);
	SSPIF=0;

	SSPBUF = 0;
	while(!SSPIF);
	SSPIF=0;

	tmp =SSPBUF;

	SSPBUF = 0;
	while(!SSPIF);
	SSPIF=0;

	return ( ((unsigned short)tmp) << 5 | ((unsigned short)SSPBUF) >> 3);
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - Touch_Reg: address of the selected register.
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
__inline void Touch_WriteReg(unsigned short Touch_Reg)
{
	LCD_CS_H();
	LCD_RS_L();
	TOUCH_nCS_L();

	Touch_Transmit((unsigned char)(Touch_Reg>>8));
	Touch_Transmit((unsigned char)(Touch_Reg));

	TOUCH_nCS_H();
	LCD_RS_H();
}

/*******************************************************************************
* Function Name  : Touch_WriteData
* Description    : 
* Input          : - data:
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
__inline void Touch_WriteData(unsigned short data)
{
    Touch_Transmit((unsigned char)(data>>8));
    Touch_Transmit((unsigned char)(data));
}

/*******************************************************************************
* Function Name  : Touch_Write_Command
* Description    : 
* Input          : - Touch_Reg: address of the selected register.
*                  - Touch_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
__inline void Touch_Write_Command(unsigned short Touch_Reg,unsigned short data)
{
	LCD_CS_H();
	LCD_RS_L();
	TOUCH_nCS_L();

	Touch_Transmit((unsigned char)(Touch_Reg>>8));
	Touch_Transmit((unsigned char)(Touch_Reg));
	LCD_RS_H();
	Touch_Transmit((unsigned char)(data>>8));
	Touch_Transmit((unsigned char)(data));
	TOUCH_nCS_H();
}

//****************************************** 
unsigned short GetTouchADC (unsigned char data)
{	  
	unsigned short Num;
	
	LCD_CS_H();
	TOUCH_nCS_L();
          
	Num = Touch_Receive(data);
    
	TOUCH_nCS_H();

	return Num;   
}

//****************************************** 
void TP_GetAdXY(unsigned short *x, unsigned short *y)  
{  
	unsigned short adx, ady; 
    adx=GetTouchADC(CHX);  
    ady=GetTouchADC(CHY);
	*x=adx; 
    *y=ady; 	
} 

/*******************************************************************************
* Function Name  : TP_DrawPoint
* Description    : 
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TP_DrawPoint(unsigned short Xpos,unsigned short Ypos)
{
  LCD_SetPoint(Xpos,Ypos,BLACK);     /* Center point */
  LCD_SetPoint(Xpos+1,Ypos,BLACK);
  LCD_SetPoint(Xpos,Ypos+1,BLACK);
  LCD_SetPoint(Xpos+1,Ypos+1,BLACK);	
}	

/*******************************************************************************
* Function Name  : DrawCross
* Description    : 
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void DrawCross(unsigned short Xpos,unsigned short Ypos,int color)
{
    LCD_DrawLine(Xpos-15,Ypos,Xpos-2,Ypos,color);
    LCD_DrawLine(Xpos+2,Ypos,Xpos+15,Ypos,color);
    LCD_DrawLine(Xpos,Ypos-15,Xpos,Ypos-2,color);
    LCD_DrawLine(Xpos,Ypos+2,Xpos,Ypos+15,color);

    LCD_DrawLine(Xpos-15,Ypos+15,Xpos-7,Ypos+15,color);
    LCD_DrawLine(Xpos-15,Ypos+7,Xpos-15,Ypos+15,color);

    LCD_DrawLine(Xpos-15,Ypos-15,Xpos-7,Ypos-15,color);
    LCD_DrawLine(Xpos-15,Ypos-7,Xpos-15,Ypos-15,color);

    LCD_DrawLine(Xpos+7,Ypos+15,Xpos+15,Ypos+15,color);
    LCD_DrawLine(Xpos+15,Ypos+7,Xpos+15,Ypos+15,color);

    LCD_DrawLine(Xpos+7,Ypos-15,Xpos+15,Ypos-15,color);
    LCD_DrawLine(Xpos+15,Ypos-15,Xpos+15,Ypos-7,color);	  	
}	
	
//****************************************** 
Coordinate *Read_Ads7846(void)
{
    static Coordinate  screen;
    unsigned short m0,m1,m2,TP_X[1],TP_Y[1];
    unsigned int temp[3];
    unsigned char count=0;
    int buffer[2][9]={{0},{0}};
  
    do
    {		   
		if (TP_INT_IN == 0)
		{
			TP_GetAdXY(TP_X, TP_Y);  
			buffer[0][count]=TP_X[0];  
			buffer[1][count]=TP_Y[0];
			count++;  
			
			//printf("x: %d, y: %d\r\n", TP_X[0], TP_Y[0]);			
		}
    }
    while(!TP_INT_IN && (count < 9));  /* TP_INT_IN  */
	
    if(count == 9)   /* Average X Y  */ 
    {
        /* Average X  */
        temp[0]=(buffer[0][0]+buffer[0][1]+buffer[0][2])/3;
		temp[1]=(buffer[0][3]+buffer[0][4]+buffer[0][5])/3;
		temp[2]=(buffer[0][6]+buffer[0][7]+buffer[0][8])/3;

		m0=temp[0]-temp[1];
		m1=temp[1]-temp[2];
		m2=temp[2]-temp[0];

		m0=m0>0?m0:(-m0);
		m1=m1>0?m1:(-m1);
		m2=m2>0?m2:(-m2);

		if( m0>THRESHOLD  &&  m1>THRESHOLD  &&  m2>THRESHOLD ) return 0;

		if(m0<m1)
		{
			if(m2<m0) 
				screen.x=(temp[0]+temp[2])/2;
			else 
				screen.x=(temp[0]+temp[1])/2;	
		}
		else if(m2<m1) 
			screen.x=(temp[0]+temp[2])/2;
		else 
			screen.x=(temp[1]+temp[2])/2;

		/* Average Y  */
		temp[0]=(buffer[1][0]+buffer[1][1]+buffer[1][2])/3;
		temp[1]=(buffer[1][3]+buffer[1][4]+buffer[1][5])/3;
		temp[2]=(buffer[1][6]+buffer[1][7]+buffer[1][8])/3;
		m0=temp[0]-temp[1];
		m1=temp[1]-temp[2];
		m2=temp[2]-temp[0];
		m0=m0>0?m0:(-m0);
		m1=m1>0?m1:(-m1);
		m2=m2>0?m2:(-m2);
		if(m0>THRESHOLD&&m1>THRESHOLD&&m2>THRESHOLD) 
			return 0;

		if(m0<m1)
		{
			if(m2<m0) 
				screen.y=(temp[0]+temp[2])/2;
			else 
				screen.y=(temp[0]+temp[1])/2;	
		}
		else if(m2<m1) 
			 screen.y=(temp[0]+temp[2])/2;
		else
			 screen.y=(temp[1]+temp[2])/2;

		return &screen;
    }  
    return 0; 
}
	 

/*******************************************************************************
* Function Name  : setCalibrationMatrix
* Description    : Calculate K A B C D E F
* Input          : None
* Output         : None
* Return         : 
* Attention		 : None
*******************************************************************************/
unsigned char setCalibrationMatrix( Coordinate * displayPtr,
                          Coordinate * screenPtr,
                          Matrix * matrixPtr)
{

  unsigned char retTHRESHOLD = 1;
  /* K£½(X0£­X2) (Y1£­Y2)£­(X1£­X2) (Y0£­Y2) */
  matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                       ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
  if( matrixPtr->Divider == 0 )
  {
    retTHRESHOLD = 0;
  }
  else
  {
    /* A£½((XD0£­XD2) (Y1£­Y2)£­(XD1£­XD2) (Y0£­Y2))£¯K	*/
    matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
	/* B£½((X0£­X2) (XD1£­XD2)£­(XD0£­XD2) (X1£­X2))£¯K	*/
    matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) - 
                    ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* C£½(Y0(X2XD1£­X1XD2)+Y1(X0XD2£­X2XD0)+Y2(X1XD0£­X0XD1))£¯K */
    matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y ;
    /* D£½((YD0£­YD2) (Y1£­Y2)£­(YD1£­YD2) (Y0£­Y2))£¯K	*/
    matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y)) ;
    /* E£½((X0£­X2) (YD1£­YD2)£­(YD0£­YD2) (X1£­X2))£¯K	*/
    matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) - 
                    ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* F£½(Y0(X2YD1£­X1YD2)+Y1(X0YD2£­X2YD0)+Y2(X1YD0£­X0YD1))£¯K */
    matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y ;
  }
  return( retTHRESHOLD ) ;
}

/*******************************************************************************
* Function Name  : getDisplayPoint
* Description    : Touch panel X Y to display X Y
* Input          : None
* Output         : None
* Return         : 
* Attention		 : None
*******************************************************************************/
unsigned char getDisplayPoint(Coordinate * displayPtr,
                     Coordinate * screenPtr,
                     Matrix * matrixPtr )
{
  unsigned char retTHRESHOLD = 1;

  if( matrixPtr->Divider != 0 )
  {
    /* XD = AX+BY+C */        
    displayPtr->x = ( (matrixPtr->An * screenPtr->x) + 
                      (matrixPtr->Bn * screenPtr->y) + 
                       matrixPtr->Cn 
                    ) / matrixPtr->Divider ;
	/* YD = DX+EY+F */        
    displayPtr->y = ( (matrixPtr->Dn * screenPtr->x) + 
                      (matrixPtr->En * screenPtr->y) + 
                       matrixPtr->Fn 
                    ) / matrixPtr->Divider ;
		
		if (displayPtr->x > 240)
			displayPtr->x = 0;
		if (displayPtr->y > 320)
			displayPtr->y = 0;
  }
  else
  {
    retTHRESHOLD = 0;
  }
  return(retTHRESHOLD);
} 

//****************************************** 
void TouchPanel_Calibrate(unsigned short charColor, unsigned short bkColor )
{
    unsigned char i;
    Coordinate * Ptr;
	
	LCD_Clear(bkColor);
	
    for(i=0;i<3;i++)
    { 
		GUI_Text(10,100,(unsigned char *)"Touch crosshair to calibrate",charColor,bkColor);
		__delay_ms(500);
		DrawCross(DisplaySample[i].x,DisplaySample[i].y,charColor);
#if 1		
		do                                       
		{
			Ptr=Read_Ads7846();
			__delay_ms(100);
		}
		while( Ptr == (void*)0 );
#endif        
		
		ScreenSample[i].x= Ptr->x; 
		ScreenSample[i].y= Ptr->y;	 
		DrawCross(DisplaySample[i].x,DisplaySample[i].y,bkColor);        
        __delay_ms(500);
    }
    setCalibrationMatrix( &DisplaySample[0], &ScreenSample[0], &matrix );
	GUI_Text(10,100,(unsigned char *)"Touch crosshair to calibrate",bkColor,bkColor);
} 

//****************************************** 
void TouchPanel_Calibrate2(void)
{
  ScreenSample[0].x = 2418; ScreenSample[0].y= 3024;
  ScreenSample[1].x = 3264; ScreenSample[1].y= 2496;
  ScreenSample[2].x = 3688; ScreenSample[2].y= 3664;   	
  setCalibrationMatrix( &DisplaySample[0],&ScreenSample[0],&matrix );
} 

//****************************************** 
void Touch_Debug(void)
{
	unsigned int XAD;

	GUI_Text(0,0,(unsigned char *)"Coordinate:", RED, GRAY);
	GUI_Text(0,16,(unsigned char *)"XAD=", RED, GRAY);
	GUI_Text(0,32,(unsigned char *)"yAD=", RED, GRAY); 

	XAD=32;	
	PutChar(XAD,16,48+display.x/1000,BLACK,GRAY); 			
	
	XAD=XAD+8;
	PutChar(XAD,16,48+display.x%1000/100,BLACK,GRAY);  
	
	XAD=XAD+8; 
	PutChar(XAD,16,48+display.x%100/10,BLACK,GRAY); 	  
	
	XAD=XAD+8;
	PutChar(XAD,16,48+display.x%10,BLACK,GRAY);     		

	XAD=32;
	PutChar(XAD,32,48+display.y/1000,BLACK,GRAY); 
	
	XAD=XAD+8;
	PutChar(XAD,32,48+display.y%1000/100,BLACK,GRAY); 
  
	XAD=XAD+8;
	PutChar(XAD,32,48+display.y%100/10,BLACK,GRAY);  
	
	XAD=XAD+8;
	PutChar(XAD,32,48+display.y%10,BLACK,GRAY);         
}
