#include<reg51.h>
sbit rs=P2^0;
sbit rw=P2^1;
sbit en=P2^2;
void msdelay(unsigned int);
void lcdcmd(unsigned char);
void lcddata(unsigned char[],unsigned char);
void main (void)
{
	P3=00;
	rs=0;
	rw=0;
	en=0;
	lcdcmd(0x38);
	lcdcmd(0x0C);
	lcdcmd(0x06);
	while(1)
	{
		lcdcmd(0x01);
		lcdcmd(0x83);
		lcddata("MY NAME IS",10);
		lcdcmd(0xc3);
		lcddata("PREM GUDNAR",11);
		msdelay(100);
	}//end of while
}//end of main


void msdelay(unsigned int a)
{
	unsigned int x,y;
	for(x=0;x<a;x++)
	for(y=0;y<1275;y++);
}//end of msdelay


void lcdcmd(unsigned char cmd)
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