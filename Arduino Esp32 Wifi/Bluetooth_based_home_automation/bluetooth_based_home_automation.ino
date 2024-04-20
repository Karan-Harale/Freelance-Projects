int fan=13;
int light =12;

int rec =0 ;



void setup() {
pinMode(fan,OUTPUT);
pinMode(light,OUTPUT);

Serial.begin(9600);
}

void loop() {

if (Serial.available()>0)
{
  rec=Serial.read();
  Serial.println(rec);
delay(100);
  if(rec== 'A')
  {
    digitalWrite(fan,HIGH);
  Serial.println("fan on");
  }
else if(rec== 'a')
  {
digitalWrite(fan,LOW);
  Serial.println("fan OFF");
  }
  
 if(rec== 'B')
  {
    digitalWrite(light,HIGH);
  Serial.println("light on");
}
else if(rec== 'b')
{
  digitalWrite(light,LOW);
  Serial.println("light  OFF");
}
}
}
