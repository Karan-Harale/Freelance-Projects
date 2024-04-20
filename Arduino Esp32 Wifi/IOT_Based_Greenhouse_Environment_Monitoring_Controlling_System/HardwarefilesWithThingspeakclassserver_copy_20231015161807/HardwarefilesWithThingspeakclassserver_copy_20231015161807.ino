#include "WiFiEsp.h"
#include "ThingSpeak.h"

// #include "pinConfig.h"

char ssid[] = "Connecting...";  // your network SSID (name)
char pass[] = "makeiteasy";     // your network password

WiFiEspClient client;


#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial esp8266(13, 12);  // RX, TX
#define ESP_BAUDRATE 19200
#else
#define ESP_BAUDRATE 115200
#endif

unsigned long myChannelNumber = "1898529";
const char* myWriteAPIKey = "59W7WMAZELCT5BH0";





#include "DHT.h"
#define DHTPIN A0      // Analog pin connected to the DHT sensor
#define DHTTYPE DHT11  // DHT 11
// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);


const int Fan1 = 9;
const int Fan2 = 8;

const int Heater = 2;  //this pin is used for artificial lights relay ip


// int LdrState;
const int LDR = 6;
const int AL = 3;  //in4

// int soil_Moisture_Value;
const int WaterPump = 4;


// int RainState;
const int RainSensor = 7;
const int curtains1 = 10;
const int curtains2 = 11;
int sw = 0;



// #include <MQ135.h>
// #define PIN_MQ135 A2
// MQ135 mq135_sensor(PIN_MQ135);


  int humidity_value;

  int temp_value;


 
 
int RainState;

void setup() {
  pinMode(Fan1, OUTPUT);  //Colling + exaust fan
  pinMode(Fan2, OUTPUT);
  pinMode(Heater, OUTPUT);

  pinMode(LDR, INPUT);  //LDR Sensor
  pinMode(AL, OUTPUT);

  pinMode(WaterPump, OUTPUT);  //waterpump  as this pins are connected to relay our relay is active low type so when we want to turn on relay we need to give LOW signal to it so it works fine if relay is active high then we gi e high pulse to turn on it.

  pinMode(curtains1, OUTPUT);  //Rain Sensor
  pinMode(curtains2, OUTPUT);


  dht.begin();

  Serial.begin(9600);

// Serial.println("\n");
//   stars();
//   Serial.println("Initialization Of Esp:");
//   stars();  //*******************************************************************


  // initialize serial for ESP module
  setEspBaudRate(ESP_BAUDRATE);



  // initialize ESP module
  WiFi.init(&esp8266);

  Serial.println("Searching for Wireless Network...");

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.print("\t..Wireless Network not Available");
    // don't continue
    while (true)
      ;
  }
  Serial.println("Wireless Network found ");



  ThingSpeak.begin(client);  // Initialize ThingSpeak

  delay(1500);
}


void loop() {
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting  to SSID: ");
    Serial.print(ssid);
    Serial.print("\t");

    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network.
      delay(5000);
    }
    // Serial.println("Connected Successfully.");
  }
  stars();

  stars();  //***********************************************************
  Serial.println("Uplinking data on Server:");
  stars();  //***********************************************************

  humidity_temp_data();

  // air_Quality();

 
  ThingSpeak.setField(1, humidity_value);

  ThingSpeak.setField(2, temp_value);
  ThingSpeak.setField(3,   soil_Moisture());

 ThingSpeak.setField(4,   light_intensity());

    ThingSpeak.setField(5,  weather_condition());


  int statusCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);


    if (statusCode == 200) {
      Serial.println("Channel update successful.");
    } else {
      Serial.println("Problem updating channel. HTTP error code " + String(statusCode));
    }
  //   stars();
  delay(15000);  // Wait 15 seconds to update the channel again
}






int humidity_temp_data() {
   // Serial.println("1. Air Temperature & Humidity Sensor(DHT11)");

    humidity_value = dht.readHumidity();

    temp_value = dht.readTemperature();  // Read temperature as Celsius (the default) // Check if any reads failed and exit early (to try again).

  tab();
  Serial.print("Humidity :- ");
  Serial.print(humidity_value);
  Serial.println(("%"));

  tab();
  Serial.print(("Temperature :- "));
  Serial.print(temp_value);
  Serial.println(("°C"));
  tab();

  if (temp_value < 30) {
    digitalWrite(Heater, LOW);  //as relay is active low type so we are giving LOW pulse to turn on the relay dont confuse here
    // Serial.println("Heater Started");
  }

  else if (temp_value > 40 || humidity_value < 60) {
    digitalWrite(Fan1, HIGH);
    digitalWrite(Fan2, LOW);
    // Serial.println("Cooling Fan Started");
  } else if (humidity_value > 90) {
    digitalWrite(Fan1, LOW);
    digitalWrite(Fan2, HIGH);
    // Serial.println("Exaust  Fan Started");

  } else {
    digitalWrite(Heater, HIGH);  //as relay is active low type so we are giving HIGH pulse to turn off the relay dont confuse here

    digitalWrite(Fan1, LOW);
    digitalWrite(Fan2, LOW);
    // Serial.println("Stabled Humidity :- Fans off");
    // tab();
    // Serial.println("Stabled Temperature :- Heaters off");
  }
    Serial.println("\n");

}


int soil_Moisture() {
  //Soil Moisture Sensor

  // Serial.println("2. Soil Moisture Sensor(YL-69):-");

  int soil_Moisture_Value = analogRead(A1);
  soil_Moisture_Value = (100 - ((soil_Moisture_Value / 1023.00) * 100));




  tab();
  Serial.print("Moisture In Soil(%):- ");
  Serial.print(soil_Moisture_Value);
  Serial.println("%");
  tab();

  if (soil_Moisture_Value < 20) {

    Serial.println("Need Watering");
    digitalWrite(WaterPump, LOW);  //as relay is active low type so we are giving LOW pulse to turn on the relay dont confuse here
    // tab();
    // Serial.println("Water Pump Is:- ON ");


  } else {
    digitalWrite(WaterPump, HIGH);  //as relay is active low type so we are giving LOW pulse to turn off the relay dont confuse here
    // Serial.println("Water Pump Is:- OFF ");
  }
  Serial.println("\n");
  return soil_Moisture_Value;
}

int light_intensity() {

  //Ldr Sensor
  // Serial.println("2. LDR Sensor:-");

    int LdrState = digitalRead(LDR);



  tab();
  Serial.print("Light Intensity:- ");
  Serial.println(not LdrState);
  tab();

  if (LdrState == 1) {
    digitalWrite(AL, LOW);  //as relay is active low type so we are giving LOW pulse to turn on the relay dont confuse here
    Serial.println("Low Sunlight");
    // tab();

    // Serial.println("Artificial Lights Are:- ON ");
  } else {
    digitalWrite(AL, HIGH);  //as relay is active low type so we are giving LOW pulse to turn off the relay dont confuse here
    Serial.println("Required Amount Of Sunlight Present");
    // tab();

    // Serial.println("Artificial Lights Are:- OFF ");
  }

  Serial.println("\n");

  return not(LdrState);
}


int weather_condition() {

  // Serial.println("4. Rain Detection Sensor(FC-37):-");
  RainState = digitalRead(RainSensor);
     

  tab();
  Serial.print("RainState = ");

  Serial.println(not RainState);

  tab();

  if (RainState == 0 && sw == 0)  //Raining curtains starts upward for duration
  {
    digitalWrite(curtains1, HIGH);
    digitalWrite(curtains2, LOW);
    // Serial.println("Wheather Condition :- Raining");
    // tab();
    Serial.println("Curtains Are Closing.....");

    delay(6000);
    Motor_Off();

    sw = 1;
  }

  else if (sw == 1 && RainState == 0)  //raining and curtains are fully closed so motors are off in this state
  {

    // Serial.println("Wheather Condition :- Raining");
    // tab();
    Serial.println("Curtains Are Closed");

    Motor_Off();

  }

  else if (sw == 1 && RainState == 1)  // when curtains are closed and wheather is cleared then motor rotates anticlockwise for some duration and open the curtains
  {
    digitalWrite(curtains1, LOW);
    digitalWrite(curtains2, HIGH);

    Serial.println("Curtains Are Opening.....");
    delay(6000);
    Motor_Off();
    sw = 0;
  } else if (sw == 0 && RainState == 1)  //
  {
    Serial.println("Curtains Are Opened");
    tab();

    Clear_Wheather();
  }

  return (not RainState);
}


// int air_Quality() {

//   int ppm = mq135_sensor.getPPM();

//   Serial.println("5. CO2 Sensor(MQ-135):-");

//   tab();
//   Serial.print("Air Quality (ppm):- ");  //The concentration of carbon dioxide in Earth's atmosphere is currently at nearly 412 parts per million (ppm)
//   Serial.print(ppm);
//   Serial.println("ppm");
//   tab();

//   if (ppm > 18) {
//     Serial.println("Warning Harmful Air");
//   } else {
//     Serial.println("Air Quality Is Good");
//   }
// }









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
  for (i = 0; i < 100; i++) {
    Serial.print("*");
  }
  Serial.println("");
}

void tab() {
  Serial.print("\t\t\t\t");
}


void setEspBaudRate(unsigned long baudrate) {
  long rates[6] = { 115200, 74880, 57600, 38400, 19200, 9600 };


  Serial.print("Setting ESP8266 baudrate to ");
  Serial.print(baudrate);
  Serial.println("...");



  for (int i = 0; i < 6; i++) {
    esp8266.begin(rates[i]);
    delay(100);
    esp8266.print("AT+UART_DEF=");
    esp8266.print(baudrate);
    esp8266.print(",8,1,0,0\r\n");
    delay(100);
  }

  esp8266.begin(baudrate);
}
