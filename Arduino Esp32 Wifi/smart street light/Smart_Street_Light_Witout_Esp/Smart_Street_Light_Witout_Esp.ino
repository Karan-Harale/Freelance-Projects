

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
  Serial.println(F("Initialize System"));
  acd1.begin(NORMAL_MODE, ON);
  acd2.begin(NORMAL_MODE, ON);
  delay(100);
}


void loop() {
  testDimmer();
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
    delay(3000);
    High2();
    delay(2000);
    Low1();
        delay(3000);
    Low2();

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
