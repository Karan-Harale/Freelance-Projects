#include<reg51.h>
sbit rs=P3^0;
sbit rw=P3^1;
sbit en=P3^2;
sbit frwsw=P0^0;
sbit revsw=P0^1;
sbit leftsw=P0^2;
sbit rightsw=P0^3;
sbit stopsw=P0^4;
sbit in1=P3^3;
sbit in2=P3^4;
sbit in3=P3^5;
sbit in4=P3^6;
void msdelay(unsigned int);
void lcdcmd(unsigned char);
void lcddata(unsigned char [],unsigned char);
void main(void)
{
	P2=00;//act as o/p
	P3=00;//
	rs=0;//
	rw=0;//
	en=0;//
	P3=00;//
	in1=0;//
	in2=0;//
	in3=0;//
	in4=0;//
	
	frwsw=1;//
	revsw=1;//ACT AS I/P
	leftsw=1;
	rightsw=1;
	stopsw=1;//
	
	lcdcmd(0x38);//8 bit
	lcdcmd(0x0f);//cur blinking
	lcdcmd(0x06);//increment
	lcdcmd(0x01);//clear screen
		  lcdcmd(0x80);
			lcddata("HELLO",5);
			lcdcmd(0xc0);
			lcddata("WORLD",5);
	
	while(1)
	{
		if(frwsw==0)
		{
		in1=1;
	  in2=0;
		in3=1;
		in4=0;
			lcdcmd(0x01);//clear screen
			msdelay(5);
			lcdcmd(0x80);
			lcddata("FORWARD",7);
			lcdcmd(0xc0);
			lcddata("SWITCH",6);
		}
			else if(revsw==0)
			{
		in1=0;
	  in2=1;
		in3=0;
		in4=1;
			lcdcmd(0x01);//clear screen
			msdelay(5);
			lcdcmd(0x80);
			lcddata("REVERSE",8);
			lcdcmd(0xc0);
			lcddata("SWITCH",6);
			}
			else if(leftsw==0)
			{
		in1=0;
	  in2=1;
		in3=1;
		in4=1;
			lcdcmd(0x01);//clear screen
			msdelay(5);
			lcdcmd(0x80);
			lcddata("LEFT",4);
			lcdcmd(0xc0);
			lcddata("SWITCH",6);
			}
			else if(rightsw==0)
			{
		in1=1;
	  in2=0;
		in3=0;
		in4=0;
			lcdcmd(0x01);//clear screen
			msdelay(5);
			lcdcmd(0x80);
			lcddata("RIGHT",5);
			lcdcmd(0xc0);
			lcddata("SWITCH",6);
			}
			else if(stopsw==0)
			{
		in1=1;
	  in2=1;
		in3=1;
		in4=1;
			lcdcmd(0x01);//clear screen
			msdelay(5);
			lcdcmd(0x80);
			lcddata("STOPED",6);
			lcdcmd(0xc0);
			lcddata("SWITCH",6);
			}
			else
			{
		in1=0;
	  in2=0;
		in3=0;
		in4=0;
		}//END OF IF
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
	P2=cmd;
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
		P2=a[x];
		rs=1;
		rw=0;
		en=1;
		msdelay(5);
		en=0;
	}//end of for
	}//end of lcddata