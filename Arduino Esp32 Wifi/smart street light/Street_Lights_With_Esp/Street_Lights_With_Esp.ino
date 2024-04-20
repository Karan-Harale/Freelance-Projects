#include <SoftwareSerial.h>
#define RX 6
#define TX 7

String AP = "vivo";       // AP NAME
String PASS = "12345678"; // AP PASSWORD
String API = "E3IW578G1KBMFAIK";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
int countTrueCommand;
int countTimeCommand;
boolean found = false;

SoftwareSerial esp8266(RX, TX);




//Libraries
#include <RBDdimmer.h>//https://github.com/RobotDynOfficial/RBDDimmer
//Parameters
int zeroCrossPin  = 2;
int acd1Pin  = 3;
int acd2Pin  = 10;


int pirstate;
int pir = 8;

int ldrstate;

int ldr = 13;

dimmerLamp acd1(acd1Pin);
dimmerLamp acd2(acd2Pin);


void setup() {

  pinMode(pir, INPUT);
  pinMode(ldr, INPUT);
  //Init Serial USB
  Serial.begin(9600);
    esp8266.begin(115200);


 Serial.println("Initialization Of Esp:");
  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
  sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 20, "OK");
  
  acd1.begin(NORMAL_MODE, ON);
  acd2.begin(NORMAL_MODE, ON);

 delay(500);
}


void loop() {

 String getData = "GET /update?api_key=" + API + "&field1=" + ldrstate;

sendCommand("AT+CIPMUX=1", 5, "OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");
  sendCommand("AT+CIPSEND=0," + String(getData.length() + 4), 4, ">");
  esp8266.println(getData);
  delay(500);
  countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0", 5, "OK");

    testDimmer();
s
  delay(1000);

}

void testDimmer() { /* function testDimmer */

  ////Sweep light power to test dimmer

  ldrstate = digitalRead(ldr);
  pirstate = digitalRead(pir);
  Serial.print("ldrstate:");
  Serial.println(ldrstate);
  Serial.print("pirstate:");
  Serial.println(pirstate);

  if (ldrstate == 1 && pirstate == 1)
  {
    High1();
    Low2();

    delay(3000);
    High2();
    Low1();
  }
  else if (ldrstate == 1 && pirstate == 0)
  {
    Low1();
    Low2();
  }
  else
  {
   acd1.setPower(0); // setPower(0-100%);
  Serial.print("Street Light 1 Intensity -> ");
  Serial.println(acd1.getPower()); 
  
  acd2.setPower(0); // setPower(0-100%);
  Serial.print("Street Light 2 Intensity -> -> ");
  Serial.print(acd2.getPower());
  Serial.println("%");
  Serial.println("Day Time...");
  }
}

void High1() {
  acd1.setPower(80); // setPower(0-100%);
  Serial.print("Street Light 1 Intensity  -> ");
  Serial.print(acd1.getPower());
  Serial.println("%");

  Serial.println("High Intensity: ");
  Serial.println("\t\t Vehicle  Detected");
}

void Low1() {
  acd1.setPower(35); // setPower(0-100%);
  Serial.print("Street Light 1 Intensity ->  ");
  Serial.print(acd1.getPower());
  Serial.println("%");

  Serial.println("Low Intensity: ");
  Serial.println("\t\t Vehicle Not Detected");

}

void High2() {
  acd2.setPower(80); // setPower(0-100%);
  Serial.print("Street Light 2 Intensity -> ");
  Serial.print(acd2.getPower());
  Serial.println("%");

  Serial.println("High Intensity: ");
  Serial.println("\t\t Vehicle  Detected");
}

void Low2() {
  acd2.setPower(35); // setPower(0-100%);
  Serial.print("Street Light 2 Intensity -> ");
  Serial.print(acd2.getPower());
  Serial.println("%");

  Serial.println("Low Intensity: ");
  Serial.println("\t\t Vehicle Not Detected");

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
