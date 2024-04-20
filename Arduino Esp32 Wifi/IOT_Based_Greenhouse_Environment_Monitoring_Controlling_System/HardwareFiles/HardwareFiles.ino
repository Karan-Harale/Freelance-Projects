#include <SoftwareSerial.h>
#define RX 12
#define TX 13

SoftwareSerial esp8266(RX, TX);
int countTrueCommand;
int countTimeCommand;
boolean found = false;


String AP = "Wifiname";       // AP NAME
String PASS = "Password";       // AP PASSWORD


String API = "api";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";

#include "DHT.h"
#define DHTPIN A0     // Analog pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);
int Fan1 = 11;
int Fan2 = 10;
int Heater = 3;//in1

int LdrState ;
int LDR = 7;
int AL = 4;//in4

int SmValue;
int WaterPump = 5;

int RainSensor = 6;
int curtains1 = 8;
int curtains2 = 9;
int sw = 0;
int  RainState;

#include <MQ135.h>
#define PIN_MQ135 A2
MQ135 mq135_sensor(PIN_MQ135);

void setup() {
  dht.begin();
  Serial.begin(9600);

  esp8266.begin(115200);

  pinMode(Fan1, OUTPUT);//Colling + exaust fan
  pinMode(Fan2, OUTPUT);

  pinMode(Heater, OUTPUT);
  pinMode(LDR, INPUT);
  pinMode(AL, OUTPUT);//LDR Sensor

 pinMode(WaterPump, OUTPUT);


  pinMode(curtains1, OUTPUT);//Rain Sensor
  pinMode(curtains2, OUTPUT);

  stars();
  Serial.println("Initialization Of Esp:");
  stars();  //*******************************************************************


  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
  sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 20, "OK");
  //*******************************************************************
  stars();

  delay(1000);
}



void loop() {
  Serial.println("\n");
  stars();//***********************************************************
  Serial.println("Gathering Sensor's Data:");
  stars();//***********************************************************

  // GET https://api.thingspeak.com/update?api_key=PSDWLJ33CKT3JD3K&field1=0 use for test not mandatory

  String getData = "GET /update?api_key=" + API + "&field6=" + MQ()  + "&field5=" + Rain_Sensor() + "&field4=" + Ldr() + "&field3=" + Soil_Moisture()  + "&field2=" + Temp() + "&field1=" + Humidity() ;

  stars();
  Serial.println("\n");
  stars();
  Serial.println("Sending Data To Server:");
  stars();


  sendCommand("AT+CIPMUX=1", 5, "OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");
  sendCommand("AT+CIPSEND=0," + String(getData.length() + 4), 4, ">");//Try to change 4 to 6


  esp8266.println(getData);
  delay(2000);
  countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0", 5, "OK");
  stars();
  delay(2000);

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
    delay(5000);
    digitalWrite(Fan1, LOW );
    digitalWrite(Fan2, LOW);
  }
  else if (h > 90) {
    digitalWrite(Fan1, LOW );
    digitalWrite(Fan2, HIGH );
    Serial.println("Exaust  Fan Started");
    delay(5000);
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

  float temp = dht.readTemperature();    // Read temperature as Celsius (the default) // Check if any reads failed and exit early (to try again).

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
    delay(5000);
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
  Serial.println("\n");

  return String(SmValue);
}


String Ldr() {

  //Ldr Sensor

  Serial.println("2. LDR Sensor:-");

  LdrState = digitalRead(LDR);
  tab();
  Serial.print("Light Intensity:- ");
  Serial.println(not LdrState);
  tab();

  if (LdrState == 1)
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
  for (i = 0; i < 100; i++)
  {
    Serial.print("*");

  }
  Serial.println("");
}

void tab() {
  Serial.print("\t\t\t\t");
}


void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print("AT command => ");
  Serial.print(command);
  Serial.print(" ");
  while (countTimeCommand < (maxTime * 1))
  {
    esp8266.println(command);//at+cipsend
    if (esp8266.find(readReplay)) //ok
    {
      found = true;
      break;
    }

    countTimeCommand++;
  }

  if (found == true)
  {
    Serial.println("OK");
    countTrueCommand++;
    countTimeCommand = 0;
  }

  if (found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }

  found = false;
}
