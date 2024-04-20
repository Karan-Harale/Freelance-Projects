#define BLYNK_TEMPLATE_ID "TMPL9iiBHsD5"
#define BLYNK_DEVICE_NAME "Digital Notice Board"
#define BLYNK_AUTH_TOKEN  "49fjlRtJkK-X2R-nXlJ4lyhhwm0ky9QE"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include "DHT.h"
#include <MQ135.h>


char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Harshal";
char pass[] = "87654321";

#define DHTPIN 2          // What digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
#define PIN_MQ135 A0

//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
MQ135 mq135_sensor(PIN_MQ135);


BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.

#include <LiquidCrystal_I2C.h>

//define I2C address......
LiquidCrystal_I2C lcd(0x27, 20, 4);



String i = "";
String j = "";

int Buzzer = 0;
void setup()
{
  lcd.init();
  lcd.clear();
  lcd.backlight();
  pinMode(Buzzer, OUTPUT);
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  
  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}


BLYNK_WRITE(V5)
{
  // You can also use:
  j = param.asStr();
  // double d = param.asDouble();
  Serial.println(j);
  Blynk.virtualWrite(V5, "OK MSG Received..");

  if (j.equalsIgnoreCase(i)) {
    digitalWrite(Buzzer, LOW);
    Serial.println("Relay OFF ");
  }
  else
  {
    i = j;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(j);

    Serial.print("i=");
    Serial.println(i);
    digitalWrite(Buzzer, HIGH);
    Serial.println("Buzzer ON ");
    delay(10000);
    digitalWrite(Buzzer, LOW);
    Serial.println("Buzzer OFF ");

  }

}


void sendSensor()
{
  int h = dht.readHumidity();
  int t = dht.readTemperature(); // 
  int Co2_PPM = mq135_sensor.getPPM();

  Blynk.virtualWrite(V2, h);
  Blynk.virtualWrite(V3, t);
  Blynk.virtualWrite(V4, Co2_PPM);

  lcd.setCursor(0, 2);

  lcd.print("Humid:");
  lcd.print(h);
  lcd.print("%");
  lcd.print(" ");
  lcd.print("Temp:");
  lcd.print(t);
  lcd.print("C");
  
 lcd.setCursor(0, 3);

  lcd.print("Air Quality: ");
  lcd.print(Co2_PPM);
  lcd.print("PPM");
  
  if (Co2_PPM > 1000)
  {

    lcd.clear();
    lcd.setCursor(0, 0);
    digitalWrite(Buzzer, HIGH);

    lcd.print("WARNING..");
    lcd.setCursor(0, 2);

    lcd.print("HARMFUL AIR");
    delay(10000);
    digitalWrite(Buzzer, LOW);
    lcd.clear();
    lcd.setCursor(0, 2);

  lcd.print("Humid:");
  lcd.print(h);
  lcd.print("%");
  lcd.print(" ");
  lcd.print("Temp:");
  lcd.print(t);
  lcd.print("C");
  
 lcd.setCursor(0, 3);

  lcd.print("Air Quality: ");
  lcd.print(Co2_PPM);
   lcd.setCursor(17, 3);

  lcd.print("PPM");
  }
  else {
    digitalWrite(Buzzer, LOW);

  }
}


void loop()
{
  Blynk.run();
  timer.run();
}
