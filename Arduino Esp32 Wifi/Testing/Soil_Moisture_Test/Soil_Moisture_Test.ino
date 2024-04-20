int SmValue;
int WaterPump = 5;

void setup() {
 pinMode(WaterPump, OUTPUT);

 Serial.begin(9600); // put your setup code here, to run once:
delay(100);
}

void loop() {
  Serial.println("2. Soil Moisture Sensor(YL-69):-");
  SmValue = analogRead(A1);
  SmValue = ( 100 - ( (SmValue / 1023.00) * 100 ) );

  tab();
  Serial.print  ("Moisture In Soil(%):- ");
  Serial.print(SmValue);
  Serial.println("%");
  tab();

  if (SmValue < 20)
  {

    Serial.println("Need Watering");
   digitalWrite(WaterPump, HIGH);
    tab();
    Serial.println("Water Pump Is:- ON ");
    delay(5000);
    digitalWrite(WaterPump, LOW);
    
  }
  else
  {
    digitalWrite(WaterPump, LOW);
    Serial.println("Water Pump Is:- OFF ");
  }
}
