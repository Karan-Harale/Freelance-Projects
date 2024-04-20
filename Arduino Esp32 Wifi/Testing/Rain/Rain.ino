int RainState;

int RainSensor = 6;

void setup() {
   pinMode(RainSensor, INPUT);
 // put your setup code here, to run once:
  Serial.begin(9600);
  delay(100);

}

void loop() {
  RainState = digitalRead(RainSensor);

  Serial.print("RainState = ");

  Serial.println(not RainState);// put your main code here, to run repeatedly:

}
