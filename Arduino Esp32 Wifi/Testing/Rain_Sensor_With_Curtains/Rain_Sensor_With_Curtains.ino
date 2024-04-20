int RainSensor = 6;
int curtains1 = 8;
int curtains2 = 9;
int sw=0;
void setup() {
pinMode(curtains1, OUTPUT);//Rain Sensor
  pinMode(curtains2, OUTPUT);  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(100);

}

void loop() {
 Serial.println("5. Rain Detection Sensor(FC-37):-");
int  RainState = digitalRead(RainSensor);

  tab();
  Serial.print("RainState = ");

  Serial.println(not RainState);

  tab();

  if (RainState == 0 && sw == 0)    //Raining curtains starts upward for duration
  {
    digitalWrite(curtains1, HIGH);
    digitalWrite(curtains2, LOW);
    Serial.println("Wheather Condition :- Raining");
    tab();
    Serial.println("Curtains Are Closing.....");

    delay(6000);
    Motor_Off();

    sw = 1;
  }

  else if (sw == 1 && RainState == 0) //raining and curtains are fully closed so motors are off in this state
  {

    Serial.println("Wheather Condition :- Raining");
    tab();
    Serial.println("Curtains Are Closed");

    Motor_Off();

  }

  else if (sw == 1 && RainState == 1) // when curtains are closed and wheather is cleared then motor rotates anticlockwise for some duration and open the curtains
  {
    digitalWrite(curtains1, LOW);
    digitalWrite(curtains2, HIGH);

    Serial.println("Curtains Are Opening.....");
    delay(6000);
    Motor_Off();
    sw = 0;
  }
  else if (sw == 0 && RainState == 1)//
  {
    Serial.println("Curtains Are Opened");
    tab();

    Clear_Wheather();
  }
    delay(1000);

}


void Motor_Off() {
  digitalWrite(curtains1, LOW);
  digitalWrite(curtains2, LOW);
}
void Clear_Wheather() {
  Motor_Off();
  Serial.println("Wheather Condition :- Clear Wheather");
}






void stars() {

  int i = 0;
  for (i = 0; i < 100; i++)
  {
    Serial.print("*");

  }
  Serial.println("");
}

void tab() {
  Serial.print("\t\t\t\t");
}
