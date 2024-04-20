int rec = 0;

//char* rfid_id[]={"1900E54250EE","1900E561BC21","18003D312034"};

char* names[] = {"Yamini Pawar", "Shubham Surwade", "Karan Harale"};
char* roll_nos[] = {"42", "54", "21"};

#include<LiquidCrystal.h>
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);


#include <SPI.h>
#include <SD.h>
#include <RTClib.h>

const int chipSelect = 10;

File myFile;

RTC_DS1307 rtc;

int rled=3;
int gled=2;

void setup() {
pinMode(rled,OUTPUT);
pinMode(gled,OUTPUT);

  Serial.begin(9600);  // put your setup code here, to run once:
  lcd.begin(20, 4);
  SD.begin(10);

    Serial.println("Initializing SD card...");
  lcd.clear();
  lcd.setCursor(0 , 0);       
  lcd.print("********************");

  lcd.setCursor(0 , 1);       
  lcd.print("*   Initializing   *");
  lcd.setCursor(0, 2);
  lcd.print("*     SD Card      *");
  lcd.setCursor(0 , 3);       
  lcd.print("********************");
  delay(200);
  {
    Serial.println("Wiring is correct and a card is present.");
      lcd.clear();
      lcd.setCursor(0 , 0);       
      lcd.print("********************");
      lcd.setCursor(0 , 1);       
      lcd.print("*     SD Card      *");       
      lcd.setCursor(0, 2);
      lcd.print("*    Initiated     *");
      lcd.setCursor(0 , 3);       
      lcd.print("********************");
  delay(100);
  }
    }



void loop() {

lcd.clear();
  digitalWrite(gled,LOW);
  digitalWrite(rled,HIGH);
  
  lcd.setCursor(0 , 0);
  lcd.print("********************");
  lcd.setCursor(0, 1);
  lcd.print("*   SCAN YOU'RE    *");
  lcd.setCursor(0, 2);
  lcd.print("*    CARD HERE     *");
  lcd.setCursor(0 , 3);
  lcd.print("********************");

  delay(200);
  if (Serial.available() > 0)
  {
    rec = Serial.read();
    Serial.println(rec);

    lcd.clear();
    lcd.setCursor(2, 1);
    lcd.print("Please Wait.....");
    delay(50);

    if (rec == 'a')
    {

      digitalWrite(rled,LOW);

      digitalWrite(gled,HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("*****WELCOME********");
      lcd.setCursor(0, 1);
      lcd.print("*Roll NO:- ");
      lcd.print(roll_nos[0]);
      lcd.setCursor(19, 1);
      lcd.print("*");

      lcd.setCursor(0, 2);
      lcd.print("*");
      lcd.print(names[0]);
      lcd.setCursor(19, 2);
      lcd.print("*");
      lcdtime();
      Serial.print(roll_nos[0]); Serial.print(";"); Serial.print(names[0]);
      stime();
      
      myFile = SD.open("sheet.txt", FILE_WRITE);      // Open file
        
                                                   // If the file opened ok, write to it
      if (myFile) {
        Serial.print(" File opened ok");
        myFile.print(roll_nos[0]); myFile.print(";"); myFile.print(names[0]); filetime();
        Serial.println(" Data Saved");
      }
        delay(300);

    }
    else if (rec == 'b')
    {
      digitalWrite(rled,LOW);
      digitalWrite(gled,HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("*****WELCOME********");
      lcd.setCursor(0, 1);
      lcd.print("*Roll NO:- ");
      lcd.print(roll_nos[1]);
      lcd.setCursor(19, 1);
      lcd.print("*");

      lcd.setCursor(0, 2);
      lcd.print("*");
      lcd.print(names[1]);
      lcd.setCursor(19, 2);
      lcd.print("*");
lcdtime();
      Serial.print(roll_nos[1]); Serial.print(";"); Serial.print(names[1]);    
      stime();
      
      myFile = SD.open("sheet.txt", FILE_WRITE);      // Open file
                                                      // If the file opened ok, write to it
      if (myFile) {
        Serial.print(" File opened ok");
         myFile.print(roll_nos[1]); myFile.print(";"); myFile.print(names[1]);
        filetime();
        Serial.println(" Data Saved");
      }
      delay(300);   
 }
    else if (rec == 'c')
    {
      digitalWrite(rled,LOW);
      digitalWrite(gled,HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("*****WELCOME********");
      lcd.setCursor(0, 1);
      lcd.print("*Roll NO:- ");
      lcd.print(roll_nos[2]);
      lcd.setCursor(19, 1);
      lcd.print("*");

      lcd.setCursor(0, 2);
      lcd.print("*");
      lcd.print(names[2]);
      lcd.setCursor(19, 2);
      lcd.print("*");
       lcdtime();
      Serial.print(roll_nos[2]); Serial.print(";"); Serial.print(names[2]);   
      stime();
      
      myFile = SD.open("sheet.txt", FILE_WRITE);      // Open file
                                                     // If the file opened ok, write to it
      if (myFile) {
        Serial.print(" File opened ok");
        myFile.print(roll_nos[2]); myFile.print(";"); myFile.print(names[2]);
        filetime();
       Serial.println(" Data Saved");
      }
      delay(300);
    }
    else if (rec != 'a' && rec != 'b' && rec != 'c')
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.setCursor(0 , 0);
      lcd.print("********************");
      lcd.setCursor(0, 1);
      lcd.print("*   Invalid Card   *");
      lcd.setCursor(0, 2);
      lcd.print("*                  *");
      lcd.setCursor(0 , 3);
      lcd.print("********************");

      digitalWrite(rled,HIGH);    delay(100);    digitalWrite(rled,LOW);    delay(100);    
  digitalWrite(rled,HIGH);    delay(100);    digitalWrite(rled,LOW);    delay(100); 
   delay(100);
    }
   
    
  }
}

void lcdtime()
{
  DateTime now = rtc.now();

  lcd.setCursor(0, 3);;
  lcd.print("*");

  lcd.print(now.day(), DEC);  lcd.print('/');  lcd.print(now.month(), DEC);  lcd.print('/');  lcd.print(now.year(), DEC);
  lcd.setCursor(11, 3);

  lcd.print(now.hour(), DEC); lcd.print(':'); lcd.print(now.minute(), DEC); lcd.print(':'); lcd.print(now.second(), DEC);
  lcd.setCursor(19, 3);
  lcd.print("*");
}

void stime()
{
  DateTime now = rtc.now();

  Serial.print(" ");
  Serial.print(now.day(), DEC);  Serial.print('/');  Serial.print(now.month(), DEC);  Serial.print('/');  Serial.print(now.year(), DEC);
  Serial.print("   ");
  Serial.print(now.hour(), DEC); Serial.print(':'); Serial.print(now.minute(), DEC); Serial.print(':'); Serial.print(now.second(), DEC);

}

void filetime() {
  DateTime now = rtc.now();
  myFile.print(now.day(), DEC);  myFile.print('/');  myFile.print(now.month(), DEC);  myFile.print('/');myFile.print(now.year(), DEC);
  
  myFile.print(';');myFile.print(now.hour(), DEC);  myFile.print(':');  myFile.println(now.minute(), DEC);myFile.print(now.second(), DEC);
}
