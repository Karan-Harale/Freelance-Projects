String AP = "wifi name";       // AP NAME
String PASS = "wifi password "; // AP PASSWORD
String API = "C5DV4NFIKXXE0VWZ";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";


void setup() {
Serial.begin(115200);  // put your setup code here, to run once:
delay(1500);

}

void loop() {
Serial.println("AT+CWQAP");
delay(10000);

Serial.println("AT");
delay(10000);

Serial.println("AT+CWMODE=1");
delay(10000);

Serial.println("AT+CWLAP");
delay(10000);

Serial.println("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"");
delay(10000);
}
