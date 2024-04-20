#include <SoftwareSerial.h>
#include <dht11.h>
#define RX 2
#define TX 3


String AP = "wifiname";       // AP NAME
String PASS = "wifipassword"; // AP PASSWORD
String API = "RZFA29U6FOVK5REQ";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
  
SoftwareSerial esp8266(RX,TX); 


#include "DHT.h"
#define DHTPIN A0     //  DHT sensor connected to the Analog pin A0
#define DHTTYPE DHT11   // Type Of DHT Sensor

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);
 
int h;
int temp;

void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}

void loop() {
 
delay(2000);

//DHT11
{
  Serial.println("1. Air Temperature & Humidity Sensor(DHT11)");

  int h = dht.readHumidity();

  int temp = dht.readTemperature();                            // Read temperature as Celsius (the default)

                                                              // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(temp) )
  {
    Serial.println(("Failed to read from DHT sensor!"));
    return;
  }
  else
  {
    Serial.print(("Temperature:- "));
    Serial.print(temp);
    Serial.println(("°C"));
    delay(100);  


    Serial.print("Humidity :-");
    Serial.print(h);
    Serial.print(("%"));
    delay(100);  

  }
}

 String getData = "GET /update?api_key="+ API +"&field1="+temp+"&field2="+h;
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
}





void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". AT command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OK");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
