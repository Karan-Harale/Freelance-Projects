int LdrState ;
int LDR = 7;
int AL = 4;//in4  4

void setup() {
 pinMode(LDR, INPUT);
  pinMode(AL, OUTPUT);//LDR Sensor
 // put your setup code here, to run once:
  Serial.begin(9600);
  delay(100);

}

void loop() {
 
  Serial.println("3. LDR Sensor:-");

  LdrState = digitalRead(LDR);
  Serial.print("Light Intensity:- ");
  Serial.println(not LdrState);

  if (LdrState == 1)
  {
    digitalWrite(AL, HIGH);
    Serial.println("Low Sunlight");

    Serial.println("Artificial Lights Are:- ON ");
  }
  else
  {
    digitalWrite(AL, LOW);
    Serial.println("Required Amount Of Sunlight Present");

    Serial.println("Artificial Lights Are:- OFF ");
  }
  delay(1000);
}
