// RFID Based Attendance System Using 8051 Microcontroller

//library description
#include<reg51.h>					//Header file for generic 80C51 and 80C31 microcontroller.
#include<string.h>
#include<stdio.h>
#define LCDPORT P1


//The sbit type defines a bit within a special function register (SFR)

sbit rs=P1^0;
sbit rw=P1^1;
sbit en=P1^2;
sbit led1=P2^3;
sbit led2=P2^4;

char i,rx_data[50];
char rfid[13],ch=0;
unsigned char result[1];


void delay(int itime)
{
	int i,j;
	for(i=0;i<itime;i++)
	for(j=0;j<1275;j++);
}

void daten()
{
	rs=1;
	rw=0;
	en=1;
	delay(5);
	en=0;
}

void lcddata(unsigned char ch)
{
	LCDPORT=ch & 0xf0;
	daten();
	LCDPORT=(ch<<4) & 0xf0;
	daten();
}

void cmden(void)
{
	rs=0;
	en=1;
	delay(5);
	en=0;
}

void lcdcmd(unsigned char ch)
{
	LCDPORT=ch & 0xf0;
	cmden();
	LCDPORT=(ch<<4) & 0xf0;
	cmden();
}

void lcdstring(char *str)
{
	while(*str)
	{
		lcddata(*str);
		str++;
	}
}

void lcd_init(void)
{
	lcdcmd(0x02);
	lcdcmd(0x28);
	lcdcmd(0x0e);
	lcdcmd(0x01);
}


//uart configuration
void uart_init()
{
	TMOD=0x20;		
	SCON=0x50;
	TH1=0xfd;
	TR1=1;
}

char rxdata()
{
	while(!RI);
		ch=SBUF;    
		RI=0;
		return ch;
}

void main()
{
	led1=0;
	led2=0;
	
	uart_init();
	lcd_init();
	lcdstring("RFID Security");

	lcd_init();
	lcdstring("MRITYUNJAY");
	lcdcmd(0xc0);
	lcdstring("BHUVAN");

	lcd_init();
	lcdstring("MRITYUNJAYA");	
	lcdcmd(0xc0);
	lcdstring("SAKSHAM");
	
	while(1)
	{
		lcd_init();
		lcdstring("Scan Your Card:");
	
		i=0;
		for(i=0;i<12;i++)		
		rfid[i]=rxdata();
		rfid[i]='\0';		
		
		lcd_init();
		lcdstring("Rfid No. is:");
		lcdcmd(0xc0);
		for(i=0;i<12;i++)		
		lcddata(rfid[i]);		
		
		if(strncmp(rfid,"4D0098E62516",12)==0)
		{
			lcdcmd(0x01);             
			lcdstring("Authorised");
			
			led1=1;
			led2=0;
			delay(300);
		}
		
		else 
		{
			lcdcmd(0x01);
			lcdstring("Unauthorised");
			led1=0;
			led2=1;
		}
  }
}