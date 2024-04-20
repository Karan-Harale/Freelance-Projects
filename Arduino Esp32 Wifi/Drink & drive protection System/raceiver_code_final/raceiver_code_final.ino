int state=0;
int relay=13;
int led= 12;
void setup(){
  Serial.begin(9600);
  pinMode(relay,OUTPUT);
  pinMode(led,OUTPUT); 
}
void loop(){
  if(Serial.available()>0) 
{
state=Serial.read();
// Serial.println(state);

if(state=='A')
{
 digitalWrite(relay,HIGH);
 Serial.println("BIKE:ON");
}
else if (state== 'B')
{
 Serial.println("Co2 Level: High");
digitalWrite(led,HIGH);
delay(100);
    digitalWrite(led,LOW);
    delay(100);

}
else if(state== 'b'){
    digitalWrite(led,LOW);
 Serial.println("Co2 Level: Balanced");

}
else 
{
    digitalWrite(relay,LOW);
  Serial.println("BIKE:OFF");
    digitalWrite(led,LOW);
}
}
}


