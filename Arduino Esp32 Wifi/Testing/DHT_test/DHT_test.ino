
#include "DHT.h"
#define DHTPIN A0     // Analog pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

int in0 = 11;
int in1 = 10;

int heater = 3;


void setup() {
  dht.begin();
  Serial.begin(9600);

  pinMode(in0, OUTPUT);
  pinMode(in1, OUTPUT);

  pinMode(heater, OUTPUT);
  delay(100);
}

void loop() {
  float h = dht.readHumidity();

  float temp = dht.readTemperature();                            // Read temperature as Celsius (the default)

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


    Serial.print("Humidity :-");
    Serial.print(h);
    Serial.println(("%"));


    if (temp < 30)
    {
      digitalWrite(heater, HIGH);
    }
    
    else if (h < 20 || temp>40)
    {
      digitalWrite(in0,HIGH );
      digitalWrite(in1, LOW);
    }
    else if (h > 60) {
      digitalWrite(in0,LOW  );
      digitalWrite(in1, HIGH );
    }
    else
    { digitalWrite(in0, LOW );
      digitalWrite(in1, LOW);
      digitalWrite(heater, HIGH);
    }
  }
  delay(100);
}
