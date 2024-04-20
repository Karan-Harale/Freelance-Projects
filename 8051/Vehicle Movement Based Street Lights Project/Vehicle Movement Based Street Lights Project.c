#include <reg51.h>
#define stlight P2
#define sense P0
void main()
{
sense=0x0FF;
stlight=0x00;
while(1)
{
if (sense^0) //if 1st ir is high
stlight=0x3; //make 3 light on
if (sense^1)// if 2nd ir is high
stlight=0x7; 		//make 3 light on
if (sense^2) 		//if 3rd ir is high
stlight=0x0e;		// make 3 light on
if (sense^3)		// if 4th ir is high
stlight=0x1C;		// make 3 light on
if (sense^4) 		//i		//f 5th ir is high
stlight=0x38; 		//make 3 light on
if (sense^5) 		//if 6th ir is high
stlight=0x70; 		//make 3 light on
if (sense^6) 		//if 7th ir is high
stlight=0xE0; 		//make last 3 light on
if (sense^7) 		//if 8th ir is high
stlight=0xC0; 		//make last 2 light on
}
}