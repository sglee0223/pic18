
#include "system.h"

#include "LCD_BD663474.h"
#include "LCD_font.h"
#include "TouchPanel_XPT2046.h"

//****************************************** 
void LCD_Transmit(unsigned char send_char)
{
    //while(!SSPIF);
	SSPBUF=(unsigned char)(send_char);
	while(!SSPIF);
	SSPIF=0;
	//tmp=SSPBUF;
}

//****************************************** 
void LCD_WRITE_REG(unsigned int index)
{
	unsigned short value_index;
	
	LCD_RS_L();
	LCD_CS_L();
    value_index=index;
    value_index =value_index>>8;
    LCD_Transmit((unsigned char)(value_index));

    value_index=index;
    value_index &=0x00ff;
    LCD_Transmit((unsigned char)(index));
	LCD_CS_H();
	LCD_RS_H();
}

//****************************************** 
void LCD_SEND_COMMAND(unsigned int index,unsigned int data)
{
	//select command register
	LCD_RS_L();
	LCD_CS_L();
    LCD_Transmit((unsigned char)(index>>8));
    LCD_Transmit((unsigned char)(index));
	LCD_CS_H();
	//send data
	LCD_RS_H();
	LCD_CS_L();
    LCD_Transmit((unsigned char)(data>>8));
    LCD_Transmit((unsigned char)(data));
	LCD_CS_H();
}

//****************************************** 
void LCD_WRITE_COMMAND(unsigned int index,unsigned int data)
{
	//select command register
	LCD_RS_L();
	LCD_CS_L();
    LCD_Transmit((unsigned char)(index>>8));
    LCD_Transmit((unsigned char)(index));
	LCD_CS_H();
	//send data
	LCD_RS_H();

	LCD_CS_L();
    LCD_Transmit((unsigned char)(data>>8));
    LCD_Transmit((unsigned char)(data));
	LCD_CS_H();
}

//****************************************** 
void LCD_WRITE_DATA(unsigned int data)
{
	LCD_Transmit((unsigned char)(data>>8));
	LCD_Transmit((unsigned char)(data));
}

//****************************************** 
void lcd_ctrl_port_init()
{
    TRISCbits.RC3 = 0; // clock pin as output    
    TRISCbits.RC4 = 1; // SDI input
    TRISCbits.RC5 = 0; // SD0 as output
    
    SSPSTAT = 0X80; // SSPSTAT: MSSP STATUS REGISTER (SPI MODE)
                    // SMP CKE D/A P S R/W UA BF
                        // SMP: Sample bit
                            // SPI Master mode:
                            // 1 = Input data sampled at end of data output time
                            // 0 = Input data sampled at middle of data output time
    SSPCON1 = 0X30; // SSPCON1: MSSP CONTROL REGISTER 1 (SPI MODE)
                    // WCOL SSPOV SSPEN CKP SSPM3 SSPM2 SSPM1 SSPM0
                        // SSPEN: Synchronous Serial Port Enable bit
                            // 1 = Enables serial port and configures SCK, SDO, SDI and SS as serial port pins
                            // 0 = Disables serial port and configures these pins as I/O port pins
                        // CKP: Clock Polarity Select bit
                            // 1 = Idle state for clock is a high level
                            // 0 = Idle state for clock is a low level
    INTCON = 0X00;  // INTCON REGISTER
                    // GIE/GIEH PEIE/GIEL TMR0IE INT0IE RBIE TMR0IF INT0IF RBIF
    PIR1 = 0X00;    // PIR1: PERIPHERAL INTERRUPT REQUEST (FLAG) REGISTER 1
                    // PSPIF ADIF RCIF TXIF SSPIF CCP1IF TMR2IF TMR1IF
}

//****************************************** 
void lcd_init(void)
{
    lcd_ctrl_port_init();
	
	LCD_RST_L();
	__delay_us(100);
	LCD_RST_H();
	__delay_us(100);

	LCD_WRITE_COMMAND( 0x000, 0x0001 ); /* oschilliation start */
	__delay_us(100);
	/* Power settings */  	
	LCD_WRITE_COMMAND( 0x100, 0x0000 ); /*power supply setup*/	
	LCD_WRITE_COMMAND( 0x101, 0x0000 ); 
	LCD_WRITE_COMMAND( 0x102, 0x3110 ); 
	LCD_WRITE_COMMAND( 0x103, 0xe200 ); 
	LCD_WRITE_COMMAND( 0x110, 0x009d ); 
	LCD_WRITE_COMMAND( 0x111, 0x0022 ); 
	LCD_WRITE_COMMAND( 0x100, 0x0120 ); 
	__delay_us(100);

	LCD_WRITE_COMMAND( 0x100, 0x3120 );
	__delay_us(100);
	/* Display control */   
	LCD_WRITE_COMMAND( 0x001, 0x0100 );
	LCD_WRITE_COMMAND( 0x002, 0x0000 );
	LCD_WRITE_COMMAND( 0x003, 0x1230 );
	LCD_WRITE_COMMAND( 0x006, 0x0000 );
	LCD_WRITE_COMMAND( 0x007, 0x0101 );
	LCD_WRITE_COMMAND( 0x008, 0x0808 );
	LCD_WRITE_COMMAND( 0x009, 0x0000 );
	LCD_WRITE_COMMAND( 0x00b, 0x0000 );
	LCD_WRITE_COMMAND( 0x00c, 0x0000 );
	LCD_WRITE_COMMAND( 0x00d, 0x0018 );
	/* LTPS control settings */   
	LCD_WRITE_COMMAND( 0x012, 0x0000 );
	LCD_WRITE_COMMAND( 0x013, 0x0000 );
	LCD_WRITE_COMMAND( 0x018, 0x0000 );
	LCD_WRITE_COMMAND( 0x019, 0x0000 );

	LCD_WRITE_COMMAND( 0x203, 0x0000 );
	LCD_WRITE_COMMAND( 0x204, 0x0000 );

	LCD_WRITE_COMMAND( 0x210, 0x0000 );
	LCD_WRITE_COMMAND( 0x211, 0x00ef );
	LCD_WRITE_COMMAND( 0x212, 0x0000 );
	LCD_WRITE_COMMAND( 0x213, 0x013f );
	LCD_WRITE_COMMAND( 0x214, 0x0000 );
	LCD_WRITE_COMMAND( 0x215, 0x0000 );
	LCD_WRITE_COMMAND( 0x216, 0x0000 );
	LCD_WRITE_COMMAND( 0x217, 0x0000 );

	// Gray scale settings
	LCD_WRITE_COMMAND( 0x300, 0x5343);
	LCD_WRITE_COMMAND( 0x301, 0x1021);
	LCD_WRITE_COMMAND( 0x302, 0x0003);
	LCD_WRITE_COMMAND( 0x303, 0x0011);
	LCD_WRITE_COMMAND( 0x304, 0x050a);
	LCD_WRITE_COMMAND( 0x305, 0x4342);
	LCD_WRITE_COMMAND( 0x306, 0x1100);
	LCD_WRITE_COMMAND( 0x307, 0x0003);
	LCD_WRITE_COMMAND( 0x308, 0x1201);
	LCD_WRITE_COMMAND( 0x309, 0x050a);

	/* RAM access settings */ 
	LCD_WRITE_COMMAND( 0x400, 0x4027 );
	LCD_WRITE_COMMAND( 0x401, 0x0000 );
	LCD_WRITE_COMMAND( 0x402, 0x0000 );	/* First screen drive position (1) */   	
	LCD_WRITE_COMMAND( 0x403, 0x013f );	/* First screen drive position (2) */   	
	LCD_WRITE_COMMAND( 0x404, 0x0000 );

	LCD_WRITE_COMMAND( 0x200, 0x0000 );
	LCD_WRITE_COMMAND( 0x201, 0x0000 );
	
	LCD_WRITE_COMMAND( 0x100, 0x7120 );
	LCD_WRITE_COMMAND( 0x007, 0x0103 );
	__delay_us(100);
	LCD_WRITE_COMMAND( 0x007, 0x0113 );

	__delay_us(20);
	
	lcd_clear_screen(RED);
}

//****************************************** 
void lcd_clear_screen(unsigned int color_background)
{
	unsigned int i,j;

	LCD_WRITE_COMMAND(0x210,0x00);
	LCD_WRITE_COMMAND(0x212,0x0000);
	LCD_WRITE_COMMAND(0x211,0xEF);
	LCD_WRITE_COMMAND(0x213,0x013F);

	LCD_WRITE_COMMAND(0x200,0x0000);
	LCD_WRITE_COMMAND(0x201,0x0000);
	
	LCD_RS_L();
	LCD_WRITE_REG(0x202);	//RAM Write index
	LCD_CS_L();

	for(i=0;i<320;i++)
	{
		for(j=0;j<240;j++)
		{
		  LCD_WRITE_DATA( color_background );
		}
	}
	LCD_RS_L();
	LCD_CS_H();
}

//****************************************** 
void lcd_clear_area(unsigned int color_front, 
                    unsigned char x, 
                    unsigned int y, 
                    unsigned int width, 
                    unsigned height)
{
	unsigned int i, j;

	LCD_WRITE_COMMAND( 0x210, x ); 	//x start point
	LCD_WRITE_COMMAND( 0x212, y ); 	//y start point
	LCD_WRITE_COMMAND( 0x211, x + width - 1 );	//x end point
	LCD_WRITE_COMMAND( 0x213, y + height - 1 );	//y end point
		
	LCD_WRITE_COMMAND( 0x200, x );
	LCD_WRITE_COMMAND( 0x201, y );

	LCD_WRITE_REG(0x202);	//RAM Write index

	for( i = 0; i < height; i++ )
	{
		for( j = 0; j < width; j++ )
		{
			LCD_WRITE_DATA( color_front );

		}
	}
}

//****************************************** 
void lcd_clear_Rect(unsigned int color_front, 
                    unsigned int x0, 
                    unsigned int y0, 
                    unsigned int x1, 
                    unsigned int y1)
{
	unsigned int i, j;

	LCD_WRITE_COMMAND( 0x210, x0 ); 	//x start point
	LCD_WRITE_COMMAND( 0x212, y0 ); 	//y start point
	LCD_WRITE_COMMAND( 0x211, x1);	//x end point
	LCD_WRITE_COMMAND( 0x213, y1);	//y end point
		
	LCD_WRITE_COMMAND( 0x200, x0 );
	LCD_WRITE_COMMAND( 0x201, y0 );

	LCD_RS_L();
	LCD_WRITE_REG(0x202);	//RAM Write index
	LCD_CS_L();

	for( i = 0; i <=x1-x0; i++ )//;
	{
		for( j = 0; j <= y1-y0; j++ )
		{
			LCD_WRITE_DATA( color_front );
		}
	}
}

//****************************************** 
void lcd_set_cursor(unsigned int x, unsigned int y)
{
  if( (x > 320) || (y > 240) )
	{
		return;
	}
	LCD_WRITE_COMMAND( 0x200, x );
	LCD_WRITE_COMMAND( 0x201, y );
}

//****************************************** 
void lcd_display_char(  unsigned char ch_asc, 
                        unsigned int color_front,
                        unsigned int color_background, 
                        unsigned char postion_x, 
                        unsigned char postion_y)
{
	unsigned char i, j, b;
	const unsigned char *p = 0;
	
	LCD_WRITE_COMMAND(0x210,postion_x*8); 	//x start point
	LCD_WRITE_COMMAND(0x212,postion_y*16); 	//y start point
	LCD_WRITE_COMMAND(0x211,postion_x*8+7);	//x end point
	LCD_WRITE_COMMAND(0x213,postion_y*16+15);	//y end point

	LCD_WRITE_COMMAND(0x200,postion_x*8);	//RAM X address(0 ~ FF, actually 0 ~ EF)	
	LCD_WRITE_COMMAND(0x201,postion_y*16);	//RAM Y address(0 ~ 1FF, actually 0 ~ 13F)
	
	LCD_RS_L();
	LCD_WRITE_REG(0x202);	//RAM Write index
	LCD_CS_L();

	p = ascii;
	p += ch_asc*16;
	for(j=0;j<16;j++)
	{
		b=*(p+j);
		for(i=0;i<8;i++)
		{
			if( b & 0x80 )
			{
				LCD_WRITE_DATA(color_front);
			}
			else
			{
				LCD_WRITE_DATA(color_background);
			}
			b=b<<1;			
		}	
	}

	LCD_CS_H();
}

//****************************************** 
void lcd_display_string(unsigned char *str, 
						unsigned int color_front, 
						unsigned int color_background, 
						unsigned char x, 
						unsigned char y )
{
	while (*str) 
	{ 
		lcd_display_char( *str, color_front, color_background, x, y);
		if(++x>=30)
		{
			x=0;
			if(++y>=20)
			{
				y=0;
			}
		}
		str++;
    }
}

//****************************************** 
void lcd_display_GB2312( unsigned char gb, 
						unsigned int color_front, 
						unsigned int color_background, 
						unsigned char postion_x, 
						unsigned char postion_y )
{
	unsigned char i,j,b;
	static unsigned char *p;
	
	LCD_WRITE_COMMAND(0x210,postion_x*16); 	//x start point
	LCD_WRITE_COMMAND(0x212,postion_y*16); 	//y start point
	LCD_WRITE_COMMAND(0x211,postion_x*16+15);	//x end point
	LCD_WRITE_COMMAND(0x213,postion_y*16+15);	//y end point
	LCD_WRITE_COMMAND(0x200,postion_x*16);	
	LCD_WRITE_COMMAND(0x201,postion_y*16);

	LCD_RS_L();
	LCD_WRITE_REG(0x202);	//RAM Write index
	LCD_CS_L();
	
	//p = (unsigned char *)GB2312;
	p += gb*32;
	for(j=0;j<32;j++)
	{
		b=*(p+j);
		for(i=0;i<8;i++)
		{
			if(b&0x80)
			{
				LCD_WRITE_DATA(color_front);
			}
			else
			{
				LCD_WRITE_DATA(color_background);
			}
			b=b<<1;
		}	
	}
	LCD_CS_H();
}

//****************************************** 
void lcd_display_image( const unsigned char *img, 
					unsigned char x, 
					unsigned int y, 
					unsigned int width, 
					unsigned height )
{
	unsigned int i, j;
	unsigned int data16;

	LCD_WRITE_COMMAND( 0x210, x ); 	//x start point
	LCD_WRITE_COMMAND( 0x212, y ); 	//y start point
	LCD_WRITE_COMMAND( 0x211, x + width - 1 );	//x end point
	LCD_WRITE_COMMAND( 0x213, y + height - 1 );	//y end point
		
	LCD_WRITE_COMMAND( 0x200, x );
	LCD_WRITE_COMMAND( 0x201, y );

	LCD_RS_L();
	LCD_WRITE_REG(0x202);	//RAM Write index
	LCD_CS_L();

	for( i = 0; i < height; i++ )
	{
		for( j = 0; j < width; j++ )
		{
			//Be carful of MCU type, big endian or little endian
			//little endian
			data16 = ( *(img + 1) << 8 ) | (*img);
			LCD_WRITE_DATA( data16 );
			img += 2;
		}
	}
	LCD_CS_H();
}

//****************************************** 
const unsigned int color[] =
{
	// 0 ~ 262143, 0x00000 ~ 0x3FFFF
	0xf800,	//red
	0x07e0,	//green
	0x001f,	//blue
	0xffe0,	//yellow
	0x0000,	//black
	0xffff,	//white
	0x07ff,	//light blue
	0xf81f	//purple
};

void lcd_display_test(void)
{
	unsigned int temp, num;
	unsigned int i;
	unsigned char n;

	lcd_clear_screen(RED);
    __delay_us(600);
    
	LCD_WRITE_COMMAND(0x210,0x00);
	LCD_WRITE_COMMAND(0x212,0x0000);
	LCD_WRITE_COMMAND(0x211,0xEF);
	LCD_WRITE_COMMAND(0x213,0x013F);
	
	LCD_WRITE_COMMAND(0x200,0x0000);
	LCD_WRITE_COMMAND(0x201,0x0000);
	
	LCD_RS_L();
	LCD_WRITE_REG(0x202);	//RAM Write index
	LCD_CS_L();
	for(n=0;n<8;n++)
	{
		LCD_CS_L();
		LCD_RS_H();
		temp=color[n];
		for(num=40*240;num>0;num--)
		{
			LCD_WRITE_DATA(temp);
		}
	}

	__delay_us(600);
    
#if 0
	//display pure color
	for(n=0;n<8;n++)
	{
		LCD_WRITE_COMMAND(0x210,0x00);
		LCD_WRITE_COMMAND(0x212,0x0000);
		LCD_WRITE_COMMAND(0x211,0xEF);
		LCD_WRITE_COMMAND(0x213,0x013F);
		
		LCD_WRITE_COMMAND(0x200,0x0000);
		LCD_WRITE_COMMAND(0x201,0x0000);
		
		LCD_RS_L();
		LCD_WRITE_REG(0x202);	//RAM Write index
		LCD_CS_L();

		LCD_CS_L();
		LCD_RS_H();

		temp=color[n];
		for(i=0;i<240;i++)
		{
			for(num=0;num<320;num++)
			{
				LCD_WRITE_DATA(temp);
			}
		}
		__delay_us(60);
		__delay_us(60);
		__delay_us(60);
	}
#endif	
	
	LCD_CS_H();
	__delay_us(60);

	//lcd_clear_screen(GREEN);
}

//****************************************** 
void Swap(unsigned int *a , unsigned int *b)	//for BresenhamLine
{
	unsigned int tmp;
	tmp = *a ;
	*a = *b ;
	*b = tmp ;
} 

//****************************************** 
void lcd_draw_dot(
                  unsigned int x,
                  unsigned int y,
                  unsigned int color_front)
{
//	unsigned int i;

//	y=239-y;
//	x=319-x;	
// 	i=x;
// 	x=y;
// 	y=i;
	LCD_WRITE_COMMAND(0x210,x);
	LCD_WRITE_COMMAND(0x212,y);
	LCD_WRITE_COMMAND(0x211,x+1);
	LCD_WRITE_COMMAND(0x213,y+1);
	
 	LCD_SEND_COMMAND( 0x200, x );
 	LCD_SEND_COMMAND( 0x201, y );

// 	LCD_SEND_COMMAND( 0x202, color_front );
// 	LCD_SEND_COMMAND( 0x202, color_front );
	
	LCD_RS_L();
	LCD_WRITE_REG(0x202);	//RAM Write index
	LCD_CS_L();
	LCD_WRITE_DATA(color_front);
	LCD_WRITE_DATA(color_front);
}

//****************************************** 
void lcd_draw_bigdot(unsigned int   color_front,
                     unsigned int    x,
                     unsigned int    y )
{
	lcd_draw_dot(color_front,x,y);
	lcd_draw_dot(color_front,x,y+1);
	lcd_draw_dot(color_front,x,y-1);

	lcd_draw_dot(color_front,x+1,y);
	lcd_draw_dot(color_front,x+1,y+1);
	lcd_draw_dot(color_front,x+1,y-1);
	
	lcd_draw_dot(color_front,x-1,y);    
	lcd_draw_dot(color_front,x-1,y+1);
	lcd_draw_dot(color_front,x-1,y-1);
}

//****************************************** 
unsigned char lcd_draw_line(
                        unsigned int x0,
                        unsigned int y0,
                        unsigned int x1,
                        unsigned int y1,
												unsigned int color)
{
  short dx,dy;
  short temp;

  if( x0 > x1 )
  {
    temp = x1;
    x1 = x0;
    x0 = temp;   
  }
  if( y0 > y1 )
  {
    temp = y1;
    y1 = y0;
    y0 = temp;   
  }

  dx = x1-x0;
  dy = y1-y0;

  if( dx == 0 )
  {
    do
    { 
      lcd_draw_dot(x0, y0, color);
      y0++;
    }while( y1 >= y0 ); 
    return 0; 
  }
  if( dy == 0 )
  {
    do
    {
      lcd_draw_dot(x0, y0, color);
      x0++;
    }
    while( x1 >= x0 ); 
		return 0;
  }

	/* Bresenham's line algorithm  */
  if( dx > dy )
  {
    temp = 2 * dy - dx;
    while( x0 != x1 )
    {
	    lcd_draw_dot(x0,y0,color);
	    x0++;
	    if( temp > 0 )
	    {
	      y0++;
	      temp += 2 * dy - 2 * dx; 
	 	  }
      else         
      {
			  temp += 2 * dy;
			}       
    }
    lcd_draw_dot(x0,y0,color);
  }  
  else
  {
    temp = 2 * dx - dy;
    while( y0 != y1 )
    {
	 	  lcd_draw_dot(x0,y0,color);     
      y0++;                 
      if( temp > 0 )           
      {
        x0++;               
        temp+=2*dy-2*dx; 
      }
      else
			{
        temp += 2 * dy;
			}
    } 
    lcd_draw_dot(x0,y0,color);
	}		
	return 0;
}

//****************************************** 
unsigned long mypow(unsigned char m,unsigned char n)
{
	unsigned long result=1;	 
	while(n--)result*=m;    
	return result;
}

//****************************************** 
void lcd_display_number(unsigned int x,
                        unsigned int y,
                        unsigned long num,
                        unsigned char num_len )
{         	
	unsigned char t,temp;
	unsigned char enshow=0;
				   
	for(t=0;t<num_len;t++)
	{
		temp=(num/mypow(10,num_len-t-1))%10;
		if(enshow==0&&t<(num_len-1))
		{
			if(temp==0)
			{
				lcd_display_char(' ',BLACK,WHITE,x+t,y);
				continue;
			}else enshow=1; 
		 	 
		}
	 	lcd_display_char(temp+'0',BLACK,WHITE,x+t,y); 
	}
} 

//****************************************** 
__inline unsigned short LCD_ReadData(void)
{
	return 0;
}

//****************************************** 
__inline unsigned short LCD_ReadReg(unsigned short LCD_Reg)
{
	return 0;
}

//****************************************** 
static void LCD_SetCursor( unsigned short Xpos, unsigned short Ypos )
{
	LCD_SEND_COMMAND(0x210,Xpos);
	LCD_SEND_COMMAND(0x212,Ypos);
	LCD_SEND_COMMAND(0x211,Xpos);
	LCD_SEND_COMMAND(0x213,Ypos);
}


//****************************************** 
void LCD_Clear(unsigned short Color)
{
	unsigned int i, j;
    
	TOUCH_nCS_H();
	LCD_SEND_COMMAND( 0x003, 0x1230 );

	LCD_SEND_COMMAND(0x210,0x0000);
	LCD_SEND_COMMAND(0x212,0x0000);
	LCD_SEND_COMMAND(0x211,0xEF);
	LCD_SEND_COMMAND(0x213,0x013F);
	
	LCD_SEND_COMMAND(0x200,0x0000);
	LCD_SEND_COMMAND(0x201,0x0000);
	
	LCD_WRITE_REG(0x202);	//RAM Write index
	LCD_CS_L();
	LCD_RS_H();
	
	for( i = 0; i < MAX_X; i++ )
	{
        for( j = 0; j < MAX_Y; j++ )
            LCD_WRITE_DATA(Color);
	}
	
	LCD_CS_H();
	LCD_SEND_COMMAND( 0x003, 0x1030 );
}

/******************************************************************************
* Function Name  : LCD_BGR2RGB
* Description    : RRRRRGGGGGGBBBBB convert to BBBBBGGGGGGRRRRR
* Input          : RGB color
* Output         : None
* Return         : RGB color
* Attention		 :
*******************************************************************************/
static unsigned short LCD_BGR2RGB(unsigned short color)
{
	unsigned short  r, g, b, rgb;
	
	b = ( color>>0 )  & 0x1f;
	g = ( color>>5 )  & 0x3f;
	r = ( color>>11 ) & 0x1f;
	
	rgb =  (b<<11) + (g<<5) + (r<<0);
	
	return( rgb );
}

//****************************************** 
unsigned short LCD_GetPoint(unsigned short Xpos,unsigned short Ypos)
{
	return 0;
}

//****************************************** 
void LCD_SetPoint(unsigned short Xpos,unsigned short Ypos,unsigned short point)
{
	if( Xpos >= MAX_X || Ypos >= MAX_Y )
	{
		return;
	}
	
	TOUCH_nCS_H();
	LCD_SEND_COMMAND(0x210,0x00);
	LCD_SEND_COMMAND(0x212,0x00);
	LCD_SEND_COMMAND(0x211,MAX_X);
	LCD_SEND_COMMAND(0x213,MAX_Y);
	LCD_SEND_COMMAND(0x200,Xpos);	
	LCD_SEND_COMMAND(0x201,Ypos);	
	LCD_SEND_COMMAND(0x202,point);	
}

//****************************************** 
void LCD_DrawLine( unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1 , unsigned short color )
{
  short dx,dy;
  short temp;

  if( x0 > x1 )
  {
    temp = x1;
    x1 = x0;
    x0 = temp;   
  }
  if( y0 > y1 )
  {
    temp = y1;
    y1 = y0;
    y0 = temp;   
  }

  dx = x1-x0;
  dy = y1-y0;

  if( dx == 0 )
  {
    do
    { 
      LCD_SetPoint(x0, y0, color);
      y0++;
    }
    while( y1 >= y0 ); 
    return; 
  }
  if( dy == 0 )
  {
    do
    {
      LCD_SetPoint(x0, y0, color);
      x0++;
    }
    while( x1 >= x0 ); 
		return;
  }

	/* Bresenham's line algorithm  */
  if( dx > dy )
  {
    temp = 2 * dy - dx;
    while( x0 != x1 )
    {
	    LCD_SetPoint(x0,y0,color);
	    x0++;
	    if( temp > 0 )
	    {
	      y0++;
	      temp += 2 * dy - 2 * dx; 
	 	  }
      else         
      {
			  temp += 2 * dy;
			}       
    }
    LCD_SetPoint(x0,y0,color);
  }  
  else
  {
    temp = 2 * dx - dy;
    while( y0 != y1 )
    {
	 	  LCD_SetPoint(x0,y0,color);     
      y0++;                 
      if( temp > 0 )           
      {
        x0++;               
        temp+=2*dy-2*dx; 
      }
      else
			{
        temp += 2 * dy;
			}
    } 
    LCD_SetPoint(x0,y0,color);
	}
} 

//****************************************** 
void LCD_Drawcircle(int x0, int y0, int r, int color)
{
	int  draw_x0, draw_y0;                        
	int  draw_x1, draw_y1;        
	int  draw_x2, draw_y2;        
	int  draw_x3, draw_y3;        
	int  draw_x4, draw_y4;        
	int  draw_x5, draw_y5;        
	int  draw_x6, draw_y6;        
	int  draw_x7, draw_y7;        
	int  xx, yy;                                  

	int  di;                                           
 
	if(0==r) 
		return;

	draw_x0 = draw_x1 = x0;
	draw_y0 = draw_y1 = y0 + r;
	if(draw_y0<MAX_Y) 
		LCD_SetPoint(draw_x0, draw_y0, color);
			
	draw_x2 = draw_x3 = x0;
	draw_y2 = draw_y3 = y0 - r;
	if(draw_y2>=0) 
		LCD_SetPoint(draw_x2, draw_y2, color);

	draw_x4 = draw_x6 = x0 + r;
	draw_y4 = draw_y6 = y0;
	if(draw_x4<MAX_X) 
		LCD_SetPoint(draw_x4, draw_y4, color);

	draw_x5 = draw_x7 = x0 - r;
	draw_y5 = draw_y7 = y0;
	if(draw_x5>=0) 
		LCD_SetPoint(draw_x5, draw_y5, color); 
	if(1==r) 
		return;
 
	di = 3 - 2*r;

	xx = 0;
	yy = r;        
	while(xx<yy)
	{  
		if(di<0)
		{  
			di += 4*xx + 6;              
		}
		else
		{  
			di += 4*(xx - yy) + 10;

			yy--;          
			draw_y0--;
			draw_y1--;
			draw_y2++;
			draw_y3++;
			draw_x4--;
			draw_x5++;
			draw_x6--;
			draw_x7++;                 
		}
				
		xx++;   
		draw_x0++;
		draw_x1--;
		draw_x2++;
		draw_x3--;
		draw_y4++;
		draw_y5++;
		draw_y6--;
		draw_y7--;
							
		if( (draw_x0<=MAX_X)&&(draw_y0>=0) )        
		{  
			LCD_SetPoint(draw_x0, draw_y0, color);
		}            
		if( (draw_x1>=0)&&(draw_y1>=0) )        
		{  
			LCD_SetPoint(draw_x1, draw_y1, color);
		}
		if( (draw_x2<=MAX_X)&&(draw_y2<=MAX_Y) )        
		{  
			LCD_SetPoint(draw_x2, draw_y2, color);   
		}
		if( (draw_x3>=0)&&(draw_y3<=MAX_Y) )        
		{  
			LCD_SetPoint(draw_x3, draw_y3, color);
		}
		if( (draw_x4<=MAX_X)&&(draw_y4>=0) )        
		{  
			LCD_SetPoint(draw_x4, draw_y4, color);
		}
		if( (draw_x5>=0)&&(draw_y5>=0) )        
		{  
			LCD_SetPoint(draw_x5, draw_y5, color);
		}
		if( (draw_x6<=MAX_X)&&(draw_y6<=MAX_Y) )        
		{  
			LCD_SetPoint(draw_x6, draw_y6, color);
		}
		if( (draw_x7>=0)&&(draw_y7<=MAX_Y) )        
		{  
			LCD_SetPoint(draw_x7, draw_y7, color);
		}
	}
}

//****************************************** 
void PutChar( unsigned short Xpos, unsigned short Ypos, unsigned char ASCI, unsigned short charColor, unsigned short bkColor )
{
	unsigned short i, j;
	unsigned char buffer[16], tmp_char;
	
	LCD_SEND_COMMAND(0x210,Xpos); 	//x start point
	LCD_SEND_COMMAND(0x212,Ypos); 	//y start point
	LCD_SEND_COMMAND(0x211,Xpos+7);	//x end point
	LCD_SEND_COMMAND(0x213,Ypos+15);	//y end point

	LCD_SEND_COMMAND(0x200,Xpos);	//RAM X address(0 ~ FF, actually 0 ~ EF)	
	LCD_SEND_COMMAND(0x201,Ypos);	//RAM Y address(0 ~ 1FF, actually 0 ~ 13F)
	
    LCD_RS_L();
	LCD_WRITE_REG(0x202);	//RAM Write index	
	
	LCD_CS_L();
	GetASCIICode(buffer,ASCI);  
	for(i=0;i<16;i++)
	{
		tmp_char = buffer[i];
		for(j=0;j<8;j++)
		{
            if(tmp_char & 0x80)
				LCD_WRITE_DATA(charColor);
			else
				LCD_WRITE_DATA(bkColor);
            tmp_char = tmp_char << 1;
		}	
	}

	LCD_CS_H();
}

//****************************************** 
void GUI_Text(unsigned short Xpos, unsigned short Ypos, unsigned char *str,unsigned short Color, unsigned short bkColor)
{
	unsigned char TempChar;
	   
	do
	{
		TempChar = *str++;  
		PutChar( Xpos, Ypos, TempChar, Color, bkColor );    
		if( Xpos < MAX_X )
		{
            Xpos += 8;;
		} 
		else if ( Ypos < MAX_Y)
		{
            Xpos = 0;
            Ypos += 16;
		}   
		else
		{
            Xpos = 0;
            Ypos = 0;
		}   
	}
	while ( *str != 0 );
}

//****************************************** 
void DrawGrids(void)
{
	unsigned short i;
	
	LCD_Clear(GRAY);	
	for(i=0;i<MAX_Y;i=i+10)
		LCD_DrawLine(0,i,MAX_X,i,BLACK);

	for(i=0;i<MAX_X;i=i+10)
		LCD_DrawLine(i,0,i,MAX_Y,BLACK);
}
