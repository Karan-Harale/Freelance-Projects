int buzzer=0;

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  pinMode(buzzer,OUTPUT);
 Serial.begin(9600); // put your setup code here, to run once:

lcd.init();                      // initialize the lcd 
 
  // Print a message to the LCD.
  lcd.backlight();
  


}

void loop() {
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Hello,!");
  lcd.setCursor(2,1);
  digitalWrite(buzzer,HIGH);// put your main code here, to run repeatedly:
 lcd.print("RELAY:ON");
delay(5000);

 digitalWrite(buzzer,LOW);// put your main code here, to run repeatedly:
 lcd.setCursor(2,2);
 lcd.print("RELAY:OFF");

delay(5000);
}
