String AP = "lol";       // AP NAME
String PASS = "KKKKKKKK"; // AP PASSWORD
String API = "9BM2EP40ZL17SRNX";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";

#include "DHT.h"
#define DHTPIN A0     // Analog pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
Serial.begin(115200);  // put your setup code here, to run once:
delay(1500);
dht.begin();
Serial.println("AT+CWQAP");

delay(5000);


Serial.println("AT");
delay(5000);

Serial.println("AT+CWMODE=1");
delay(5000);

Serial.println("AT+CWLAP");
delay(5000);

Serial.println("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"");
delay(5000);
}

void loop() {
delay(5000);

  int h = dht.readHumidity();

  int temp = dht.readTemperature(); 


    Serial.print(("Temperature:- "));
    Serial.print(temp);
    Serial.println(("°C"));  


    Serial.print("Humidity :-");
    Serial.print(h);
    Serial.println(("%"));
delay(5000);

  String getData = "GET /update?api_key=" + API + "&field1=" + temp + "&field2=" + h;

Serial.println("AT+CIPMUX=1");
delay(5000);

Serial.println("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT);
delay(5000);

Serial.println("AT+CIPSEND=0," + String(getData.length() + 4));
delay(5000);

Serial.println(getData);
delay(15000); 

Serial.println("AT+CIPCLOSE=0");
delay(50000);
}
