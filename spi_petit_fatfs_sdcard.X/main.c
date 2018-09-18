/*---------------------------------------------------------------*/
/* Petit FAT file system module test program R0.03 (C)ChaN, 2014 */
/*---------------------------------------------------------------*/

#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "configuration_bits.h"
#include "main.h"

#include "uart.h"
#include "xprintf.h"
#include "diskio.h"
#include "pff.h"

/*---------------------------------------------------------*/
/* Work Area                                               */
/*---------------------------------------------------------*/

char Line[128] = {0,};		/* Console input buffer */

void put_rc (FRESULT rc)
{
	const char *p;
	FRESULT i;
	static const char str[] =
		"OK\0DISK_ERR\0NOT_READY\0NO_FILE\0NOT_OPENED\0NOT_ENABLED\0NO_FILE_SYSTEM\0";

	for (p = str, i = 0; i != rc && *p; i++) {
		while(*p++) ;
	}
	printf("rc=%u FR_%s\r\n", rc, p);
}

void put_drc (BYTE res)
{
	printf("rc=%d\r\n", res);
}

void gpio_init (void)
{
	/* Initialize GPIO ports */
    TRISB = 0x00; 
    LATB = 0x00;       

	/* Attach UART module to I/O pads */
    TRISCbits.RC7 = 1; // rx
    TRISCbits.RC6 = 0; // tx

	/* Attach SPI1 module to I/O pads */  
    TRISAbits.RA5 = 0; // CS
    LATAbits.LA5 = 1;
    
    TRISCbits.RC5 = 0; // sdo
    TRISCbits.RC4 = 1; // sdi
    TRISCbits.RC3 = 0; // sck

	LATBbits.LB0 = 0x01;		/* LED ON */
}

/*-----------------------------------------------------------------------*/
/* Main                                                                  */

int main (void)
{
	char *ptr;
	long p1, p2;
	BYTE res;
	UINT s1, s2, s3, ofs, cnt, w;
	FATFS fs;			/* File system object */
	DIR dir;			/* Directory object */
	FILINFO fno;		/* File information */

    char dirName[] = "/";
    char fileName[] = "a.txt";
    char text[] = "12345";
    
	gpio_init();

    //ei();
	uart_init();	/* Initialize UART driver */
    
#if _USE_XFUNC_OUT
	xdev_out(uart_put);
#endif
#if _USE_XFUNC_IN
	xdev_in(uart_get);
#endif    

    printf("\r\n\r\nPetit FatFs test monitor\r\n");

#if 0
    printf("disk_initialize\r\n");
    if (disk_initialize() == STA_NOINIT)
        printf("disk_initialize error\r\n");
    else
        printf("disk_initialize good\r\n");

    printf("pf_mount\r\n");
    put_rc(pf_mount(&fs));  /* fi - Mount the volume */

    printf("pf_opendir\r\n");
    put_rc(pf_opendir(&dir, dirName));
    
    printf("pf_open\r\n");
    put_rc(pf_open(fileName));   /* fo <file> - Open a file */

    cnt = strlen(text); 
        
    printf("pf_write [%d]\r\n", cnt);   
    p2 = 0;
    res = pf_write(text, cnt, &w);	/* Write data to the file */
    p2 += w;
    
    if (res != FR_OK)
        put_rc(res);
    if (cnt != w) 
        printf("pf_write error w[%d]\r\n", w); 

    printf("pf_write\r\n");
    res = pf_write(0, 0, &w);		/* Finalize the write process */
    put_rc(res);
    if (res == FR_OK)
        printf("%lu bytes written.\r\n", p2);
    
    printf("pf_lseek\r\n");
    res = pf_lseek(0);
    put_rc(res);
    if (res == FR_OK)
        printf("fptr = %lu(0x%lX)\r\n", fs.fptr, fs.fptr);  
    
    printf("pf_read\r\n");
    ofs = fs.fptr;                
    res = pf_read(Line, sizeof(Line), &s1);
    if (res != FR_OK) 
    { 
        put_rc(res);
    }
    ptr = Line;
    while (s1) 
    {
        s2 = (s1 >= 16) ? 16 : s1;
        s1 -= s2;
        put_dump((BYTE*)ptr, ofs, s2, DW_CHAR);
        ptr += 16; 
        ofs += 16;
    }
                
#endif
    
/*
    Test Sequence
 
    > di - Initialize physical drive
    > fi - Mount the volum
    > fl [<path>] - Directory listing
    > fo <file> - Open a file
    > fw <len> <val> - Write data to the file
    > fe <pos> - Move file pointer of the file
    > fd - Read the file 128 bytes and dump it
    > 
 */    
	for (;;) {
        LATBbits.LB0 = PORTBbits.RB0 ^ 0x01;

#if _USE_XFUNC_OUT
        xputc('>');
#endif
#if _USE_XFUNC_IN
        xgets(Line, sizeof Line);
#endif
        printf(">");
        uart_gets(Line, sizeof(Line));
        printf("\r\n");
        
        ptr = Line;
        
		switch (*ptr++) 
        {
		case 'd' :
			switch (*ptr++) 
            {
			case 'i' :	/* di - Initialize physical drive */
                printf("\tInitialize physical drive\r\n\t");
                
				res = disk_initialize();
				put_drc(res);
				break;

			case 'd' :	/* dd <sector> <ofs> - Dump partial secrtor 128 bytes */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2)) 
                    break;
				s2 = p2;
				res = disk_readp((BYTE*)Line, p1, s2, 128);
				if (res) { 
                    put_drc(res); 
                    break; 
                }
				s3 = s2 + 128;
				for (ptr = Line; s2 < s3; s2 += 16, ptr += 16, ofs += 16) 
                {
					s1 = (s3 - s2 >= 16) ? 16 : s3 - s2;
					put_dump((BYTE*)ptr, s2, s1, 16);
				}
				break;
			}
			break;

		case 'f' :
			switch (*ptr++) 
            {

			case 'i' :	/* fi - Mount the volume */
                printf("\tMount the volume\r\n\t");
                
				put_rc(pf_mount(&fs));
				break;

			case 'o' :	/* fo <file> - Open a file */
                printf("\tOpen a file\r\n\t");
                
				while (*ptr == ' ') 
                    ptr++;
				put_rc(pf_open(ptr));
				break;
#if _USE_READ
			case 'd' :	/* fd - Read the file 128 bytes and dump it */
                printf("\tRead the file 128 bytes and dump it\r\n");
                
				ofs = fs.fptr;                
				res = pf_read(Line, sizeof(Line), &s1);
				if (res != FR_OK) 
                { 
                    printf("\t");
                    put_rc(res); 
                    break; 
                }
				ptr = Line;
				while (s1) 
                {
					s2 = (s1 >= 16) ? 16 : s1;
					s1 -= s2;
                    printf("\t");
					put_dump((BYTE*)ptr, ofs, s2, DW_CHAR);
					ptr += 16; 
                    ofs += 16;
				}
				break;

			case 't' :	/* ft - Type the file data via dreadp function */
				do {
					res = pf_read(0, 32768, &s1);
					if (res != FR_OK) 
                    { 
                        put_rc(res); 
                        break; 
                    }
				} while (s1 == 32768);
				break;
#endif
#if _USE_WRITE
			case 'w' :	/* fw <len> <val> - Write data to the file */
                printf("\tWrite data to the file\r\n");
                
				//if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2)) 
                if (!xatoi(&ptr, &p1)) 
                {
                    printf("len invalid\r\n");
                    break;
                }
                if (!xatoi(&ptr, &p2)) 
                {
                    printf("val invalid\r\n");
                    break;
                }              
                
				for (s1 = 0; s1 < sizeof(Line); Line[s1++] = (BYTE)p2) 
                    ;
				p2 = 0;

				while (p1) 
                {
					if ((UINT)p1 >= sizeof(Line)) 
                    {
						cnt = sizeof(Line); 
                        p1 -= sizeof(Line);
					} 
                    else 
                    {
						cnt = (UINT)p1; 
                        p1 = 0;
					}
                                        
					res = pf_write(Line, cnt, &w);	/* Write data to the file */
					p2 += w;
					if (res != FR_OK) 
                    { 
                        printf("\t");
                        put_rc(res); 
                        break; 
                    }
					if (cnt != w) 
                    {
                        printf("\tcnt != w\r\n");
                        break;
                    }
				}
				res = pf_write(0, 0, &w);		/* Finalize the write process */
                printf("\t");
				put_rc(res);
				if (res == FR_OK)
					printf("\t%lu bytes written.\r\n", p2);
				break;

			case 'p' :	/* fp - Write console input to the file */
				printf("Enter lines to write. A blank line finalize the write operation.\r\n");
                
				for (;;) 
                {
#if _USE_XFUNC_IN                    
					xgets(Line, sizeof Line);
#endif
                    uart_gets(Line, sizeof(Line));
                    printf("\r\n");
        
					if (!Line[0]) 
                        break;
					strcat(Line, "\r\n");
					res = pf_write(Line, strlen(Line), &w);	/* Write a line to the file */
					if (res) 
                        break;
				}
				res = pf_write(0, 0, &w);		/* Finalize the write process */
				put_rc(res);
				break;
#endif
#if _USE_LSEEK
			case 'e' :	/* fe - Move file pointer of the file */
				while (*ptr == ' ') 
                    ptr++;
                
				if (!xatoi(&ptr, &p1))
                {
                    printf("seek pointer is invalid\r\n");
                    break;
                }

				res = pf_lseek(p1);
                printf("\t");
				put_rc(res);
				if (res == FR_OK)
					printf("\tfptr = %lu(0x%lX)\r\n", fs.fptr, fs.fptr);
				break;
#endif
#if _USE_DIR
			case 'l' :	/* fl [<path>] - Directory listing */
                printf("\tDirectory listing\r\n");
                
				while (*ptr == ' ') 
                    ptr++;
				res = pf_opendir(&dir, ptr);
				if (res) 
                { 
                    put_rc(res); 
                    break; 
                }
				s1 = 0;
				for(;;) 
                {
					res = pf_readdir(&dir, &fno);
					if (res != FR_OK) 
                    { 
                        put_rc(res); 
                        break; 
                    }
					if (!fno.fname[0]) 
                        break;
					if (fno.fattrib & AM_DIR)
						printf("\t   <DIR>   %s\r\n", fno.fname);
					else
						printf("\t%9lu  %s\r\n", fno.fsize, fno.fname);
					s1++;
				}
				printf("\t%u item(s)\r\n", s1);
				break;
#endif
			}
			break;
		}
	}

}


