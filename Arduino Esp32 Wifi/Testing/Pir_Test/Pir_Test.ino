int pirstate;
int pir=8;

void setup() {
pinMode(pir,INPUT);  
Serial.begin(9600);
delay(100);
}

void loop() {
  pirstate=digitalRead(pir);// put your main code here, to run repeatedly:

Serial.println(pirstate);
delay(1000);
}
