float value1;
float value2;
int pulse=13;
// int alcohol=A0;
// int CO2=A1;
int pulsestate=0;

void setup(){
  pinMode(pulse,INPUT);
  // pinMode(alcohol,INPUT_PULLUP);
  // pinMode(CO2,INPUT_PULLUP);
  Serial.begin(9600);
}
void loop() {
pulsestate=digitalRead(pulse);// put your main code here, to run repeatedly:

value1=analogRead(A0);
value1=value1/204.6;
value1=value1*20;
value2=analogRead(A1);
value2=value2/204.6;
value2=value2*20;

  if(pulsestate==1 & value1<20 & value2<20 )
{
  Serial.print('A');  // put your main code here, to run repeatedly:

}
 
else if (pulsestate==0  || value1>20 ||value2>20)  

{
Serial.print('a');  // put your main code here, to run repeatedly:
}
// else if (value1>20)
// {
// Serial.print('a');  // put your main code here, to run repeatedly:
 
 
// }
else if ( value2>20)
{
Serial.print('B');  // put your main code here, to run repeatedly:
}
else if ( value2<20)
{
Serial.print('b');  // put your main code here, to run repeatedly:
}
else
{
  Serial.print("System Error!");
}
}
