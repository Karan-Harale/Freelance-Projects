#include <Servo.h>


const int trigPin = 2;  // Trig pin of the ultrasonic sensor

const int echoPin = 3;  // Echo pin of the ultrasonic sensor

const int irSensorPin = 4;   // IR sensor pin

const int ledPin = 7;   // LED pin 

const int lightPin = 6; // Light pin


Servo servo1;

Servo servo2;


bool lidOpen = false;  // Variable to track the lid status

unsigned long lastObjectTime = 0;  // Timestamp to track when object was last detected



void setup() {

  servo1.attach(9);

  servo2.attach(5);

  pinMode(trigPin, OUTPUT);

  pinMode(echoPin, INPUT);

  pinMode(irSensorPin, INPUT);

  pinMode(ledPin, OUTPUT);


// Initialize the servo positions to keep the lid closed

  servo1.write(90); //right

  servo2.write(91); //left

   // Delay for 1 second

  delay(1000);

   // Initialize the serial communication for debugging

  Serial.begin(9600);

}


void loop() {

  // Check if an object is within a certain range using the ultrasonic sensor

  long duration;

  int distance;

  

  digitalWrite(trigPin, LOW);

  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);

  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  

  duration = pulseIn(echoPin, HIGH);

  distance = (duration / 2) / 29.1;  // Calculate distance in centimeters

  Serial.println(distance);


  // Check if the IR sensor is activated

  int irSensorValue = digitalRead(irSensorPin);

 Serial.println(irSensorValue);


    // If distance is less than 40 cm or the IR sensor is activated

  if (distance < 40 || irSensorValue == LOW ) {  // Change the threshold value as needed

     // If the lid is not open, open it and set the status

   if (!lidOpen) {

      lidOpen = true;

      digitalWrite(ledPin, HIGH);

      digitalWrite(lightPin, HIGH);

      Serial.println("Led On");

    for (int angle = 90; angle <= 170; angle++) {

      servo1.write(angle);

      servo2.write(170 - angle);

      delay(10);

    }

   }

   lastObjectTime = millis(); // Record the time when an object was detected

  } else {

    // If the lid was open and it's been more than 2 seconds since an object was detected, close it

    if (lidOpen && millis() - lastObjectTime >= 2000) {

    for (int angle = 170; angle >= 90; angle--) {

      servo1.write(angle);

      servo2.write(181 - angle);

      delay(20);

    }

    lidOpen = false;

    // Turn off the LED

    digitalWrite(ledPin, LOW);

    digitalWrite(lightPin,LOW);

    Serial.println("Led Off");

  }

  }

  }

