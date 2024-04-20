#include <SoftwareSerial.h>
#define RX 12
#define TX 13
SoftwareSerial esp8266(RX, TX);
int countTrueCommand;
int countTimeCommand;
boolean found = false;

String AP = "Wifi Name";       // AP NAME
String PASS = "Wifi Password";       // AP PASSWORD
String API = "Server API";   // Write API KEY
String HOST = "Server Name";
String PORT = "Host Number";

#include "DHT.h"
#define DHTPIN A0     // Analog pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);
int Fan1 = 11;
int Fan2 = 10;
int Heater = 3;//in1


int SmValue;
int WaterPump = 5;


int LdrState ;

int LDR = 7;
int AL = 4;//in4

int RainSensor = 6;
int curtains1 = 9;
int curtains2 = 8;
int sw = 0;
int  RainState;



#include <MQ135.h>
#define PIN_MQ135 A2
MQ135 mq135_sensor(PIN_MQ135);

void setup() {
  pinMode(Fan1, OUTPUT);
  pinMode(Fan2, OUTPUT);
  pinMode(Heater, OUTPUT);

  pinMode(WaterPump, OUTPUT);

  pinMode(LDR, INPUT);
  pinMode(AL, OUTPUT);//LDR Sensor

  pinMode(RainSensor, INPUT);
  pinMode(curtains1, OUTPUT);//Rain Sensor
  pinMode(curtains2, OUTPUT);

  dht.begin();
  Serial.begin(9600);  // put your setup code here, to run once:
  //  esp8266.begin(115200);
  delay(100);
  stars();
  Serial.println("Initialization Of Esp:");
  stars();  //*******************************************************************


  Serial.println("AT OK");
  Serial.println("AT+CWMODE=1 OK");
  Serial.print ("AT+CWJAP= ");
  Serial.print (AP);
  Serial.print(",");
  Serial.print(PASS);
  Serial.println(" OK");


  //  sendCommand("AT", 5, "OK");
  //  sendCommand("AT+CWMODE=1", 5, "OK");
  //  sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 20, "OK");

  //*******************************************************************
  stars();
  delay(100);
}

void loop() {
  Serial.println("\n");
  stars();//***********************************************************
  Serial.println("Gathering Sensor's Data:");
  stars();//***********************************************************

  Humidity();
  Serial.println("\n");

  Temp();
  Serial.println("\n");

  Soil_Moisture();
  Serial.println("\n");

  Ldr();
  Serial.println("\n");

  Rain_Sensor();
  Serial.println("\n");

  MQ();

  stars();
  Serial.println("\n");
  stars();
  Serial.println("Sending Data To Server:");
  stars();
  Serial.println("AT+CIPMUX=1 OK");
  Serial.print("AT+CIPSTART=0,");
  Serial.print(HOST);
  Serial.print(",");
  Serial.print(PORT);
  Serial.println(" OK");
  Serial.println("AT+CIPSEND=0");
  delay(150);

  Serial.println("AT+CIPCLOSE=0 OK");
  stars();
  delay(500);
}

String Humidity() {
  Serial.println("1. Air Temperature & Humidity Sensor(DHT11)");

  int h = dht.readHumidity();

  tab();
  Serial.print("Humidity :-");
  Serial.print(h);
  Serial.println(("%"));
  tab();
  //
  if (h < 60)
  {
    digitalWrite(Fan1, HIGH);
    digitalWrite(Fan2, LOW);
    Serial.println("Cooling Fan Started");
    delay(500);
    digitalWrite(Fan1, LOW );
    digitalWrite(Fan2, LOW);
  }
  else if (h > 80) {
    digitalWrite(Fan1, LOW );
    digitalWrite(Fan2, HIGH );
    Serial.println("Exaust  Fan Started");
    delay(500);
    digitalWrite(Fan1, LOW );
    digitalWrite(Fan2, LOW);

  }
  else
  {
    digitalWrite(Fan1, LOW );
    digitalWrite(Fan2, LOW);
    Serial.println("Stabled Humidity");
  }
  return String(h);
}

String Temp() {

  float temp = dht.readTemperature();    // Read temperature as Celsius (the default)                                       // Check if any reads failed and exit early (to try again).

  tab();
  Serial.print(("Temperature:- "));
  Serial.print(temp);
  Serial.println(("°C"));
  tab();

  if (temp < 30)
  {
    digitalWrite(Heater, HIGH);
    Serial.println("Heater Started");
  }
  else if (temp > 40)
  {
    digitalWrite(Fan1, HIGH);
    digitalWrite(Fan2, LOW);
    Serial.println("Cooling Fan Started");
    delay(500);
    digitalWrite(Fan2, LOW);

  }
  else
  {
    digitalWrite(Fan1, LOW );
    digitalWrite(Fan2, LOW);
    digitalWrite(Heater, LOW);
    Serial.println("Stabled Temperature");
  }
  return String(temp);
}


String Soil_Moisture() {
  //Soil Moisture Sensor

  Serial.println("2. Soil Moisture Sensor(YL-69):-");
  SmValue = analogRead(A1);
  //SmValue = ( 100 - ( (SmValue / 1023.00) * 100 ) );
  SmValue = SmValue / 204.6;
  SmValue = SmValue * 20;
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
  }
  else
  {
    digitalWrite(WaterPump, LOW);
    Serial.println("Water Pump Is:- OFF ");
  }
  Serial.println("\n");

  return String(SmValue);
}


String Ldr() {

  //Ldr Sensor

  Serial.println("2. LDR Sensor:-");

  LdrState = digitalRead(LDR);
  tab();
  Serial.print("Light Intensity:- ");
  Serial.println(LdrState);
  tab();

  if (LdrState == 0)
  {
    digitalWrite(AL, HIGH);
    Serial.println("Low Sunlight");
    tab();

    Serial.println("Artificial Lights Are:- ON ");
  }
  else
  {
    digitalWrite(AL, LOW);
    Serial.println("Required Amount Of Sunlight Present");
    tab();

    Serial.println("Artificial Lights Are:- OFF ");
  }
  Serial.println("\n");
  return String(not LdrState);

}

String Rain_Sensor() {

  Serial.println("4. Rain Detection Sensor(FC-37):-");
  RainState = digitalRead(RainSensor);

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

    delay(600);
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
    delay(600);
    Motor_Off();
    sw = 0;
  }
  else if (sw == 0 && RainState == 1)//
  {
    Serial.println("Curtains Are Opened");
    tab();

    Clear_Wheather();
  }
  return String(not RainState);

}

void Motor_Off() {
  digitalWrite(curtains1, LOW);
  digitalWrite(curtains2, LOW);
}
void Clear_Wheather() {
  Motor_Off();
  Serial.println("Wheather Condition :- Clear Wheather");
}

String MQ() {

  int ppm = mq135_sensor.getPPM();

  Serial.println("5. CO2 Sensor(MQ-135):-");

  tab();
  Serial.print  ("Air Quality (ppm):- ");  //The concentration of carbon dioxide in Earth's atmosphere is currently at nearly 412 parts per million (ppm)
  Serial.print(ppm);
  Serial.println("ppm");
  tab();

  if (ppm > 1000)
  {
    Serial.println("Warning Harmful Air");
  }
  else {
    Serial.println("Air Quality Is Good");
  }
  return String(ppm);
}

void stars() {

  int i = 0;
  for (i = 0; i < 50; i++)
  {
    Serial.print("*");

  }
  Serial.println("");
}

void tab() {
  Serial.print("\t\t\t\t");
}
