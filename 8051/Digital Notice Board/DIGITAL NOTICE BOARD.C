#include<reg51.h>
sbit sw1=P2^0;
sbit sw2=P2^1;
sbit rs=P2^2;
sbit rw=P2^3;
sbit en=P2^4;
void msdelay (unsigned int);
void lcdcmd(unsigned char);
void lcddata(unsigned char[],unsigned char);
void main(void)
{
	sw1=1;
	sw2=1;
	P3=00;//as o/p
	rs=0;
	rw=0;
	en=0;
	lcdcmd(0x38);
		lcdcmd(0x0f);
		lcdcmd(0x06);
		lcdcmd(0x01);
		lcdcmd(0x80);
lcddata("DIGITAL NOTICE",14);
lcdcmd(0xc5);
	lcddata("BOARD",5);
	msdelay(500);
	while (1)
	{
		if(sw1==0)
		{
		lcdcmd(0x01);
    lcdcmd(0x83);	
lcddata("IT'S HOLIDAY",12);
lcdcmd(0xc3);
lcddata("TIME",4);
		msdelay(100);
				lcdcmd(0x01);
		}
		else if(sw2==0)
		{
				lcdcmd(0x01);
    lcdcmd(0x83);	
lcddata("HOLYDAY IS",10);
lcdcmd(0xc3);
lcddata("OVER",4);
		msdelay(100);
				lcdcmd(0x01);
		}
		else
		{
			
		}
	}//end of while
}//end of main



void msdelay(unsigned int a)
{
	unsigned int x,y;
	for(x=0;x<a;x++)
	for(y=0;y<1275;y++);
}//end of msdelay


void lcdcmd(unsigned char cmd )
{
	P3=cmd;
	rs=0;
	rw=0;
	en=1;
	msdelay(5);
	en=0;
}//end of lcdcmd

void lcddata(unsigned char a[],unsigned char len)
{
	unsigned char x;
	for(x=0;x<len;x++)
	{
		P3=a[x];
		rs=1;
		rw=0;
		en=1;
		msdelay(5);
		en=0;
	}//end of for
	}//end of lcddata