//rain condition
  
     int indicator=13;
     int ir=12;               //proximity sensor
     int rain=11;             //rain sensor
     int in1=10;             //Rain MOtor
     int in2=9;
     int sw=8;
     int swstate=0;           // Switch
     int irstate=0;
     int rainstate=1;


//lights condition

     int AL=7;                //Artificial Lights
     int ldr=6;               //LDR Sensor
     int ldrstate=0;


// DHT11 humidity and temperature Sensor

      #include<dht.h>
      #define dht_apin A0
       dht DHT;

      int in3 =5;
      int in4 =4;
      int heaterbulb=3;

//Soil moisture condition

     int sm=A1;                   //Soil moisture sensor connected to analog pin A1
     float thresholdvalue=78.20;  //THRESHOLD SET AT 78.20% i.e. 800
     int motor = 2;              // Water pump

void setup() {
     // Rain Sensor:
           { 
               pinMode(indicator,OUTPUT);
               pinMode(in1,OUTPUT);         
               pinMode(in2,OUTPUT);

               pinMode(rain,INPUT);
               pinMode(sw,INPUT_PULLUP);
               pinMode(ir,INPUT);

               digitalWrite(indicator,LOW);
               digitalWrite(in1,LOW);
               digitalWrite(in2,LOW);
            }

     //LDR Sensor
            {
               pinMode(ldr,INPUT);            
               pinMode(AL,OUTPUT);
               digitalWrite(AL,LOW);
            }  

    // Soil moisture Sensor
            {
               pinMode(sm, INPUT);
               pinMode(motor, OUTPUT);        
               digitalWrite(motor, LOW);
            }

   //DHT11
            {
               pinMode(A0,INPUT);
               pinMode(in3,OUTPUT);           
               pinMode(in4,OUTPUT);
               pinMode(heaterbulb,OUTPUT);

               digitalWrite(in3,LOW);
               digitalWrite(in4,LOW);
               digitalWrite(heaterbulb,LOW);
            }
          
 //Serial Initialization         
{          
Serial.begin(9600);         
delay(500);   
}

///Option Bar

{
  
Serial.println("   Welcome to Greenhouse Automation System  ");
Serial.println("*********************************************");
}
}

void loop() {

       {

               Serial.println();
               Serial.println("a.Rain Condition");

               swstate=digitalRead(sw);
               irstate=digitalRead(ir);
               rainstate=digitalRead(rain);


               if (swstate==1 && rainstate==0 && irstate==0 )
        {
               digitalWrite(in1,HIGH);
               digitalWrite(in2,LOW); 
               digitalWrite(indicator,HIGH);

               Serial.println("    ->Rain starts ");
               Serial.println("      ==>Motors ON");
         }
    
              else if(swstate==1 && rainstate==0 && irstate==1 )
        {
               digitalWrite(in1,LOW);
               digitalWrite(in2,LOW);
               digitalWrite(indicator,HIGH);

               Serial.println("    ->Rain starts & side Curtains are closed");
               Serial.println("      ==>Motors OFF");
        }

             else if(swstate==1 && rainstate==1 && irstate==1 )
       {
             digitalWrite(in1,LOW);
             digitalWrite(in2,HIGH);
             digitalWrite(indicator,LOW);
     
             Serial.println("    ->Clear whether & side Curtains Are opened");
             Serial.println("      ==>Motors OFF");
       }
            else
       {
             digitalWrite(in1,LOW);
             digitalWrite(in2,LOW);
             digitalWrite(indicator,LOW);

             Serial.println("    ->Clear whether");
             Serial.println("      ==>Motors OFF");
       }
     }
 
// Rain sensor ends here

        { 
        
        
              Serial.println();  
              Serial.println("b.Artificial Light Condition");

              ldrstate=digitalRead(ldr);
    
              if(ldrstate==1)
           {
              digitalWrite(AL,HIGH);

              Serial.println("    ->Low Sunlight");
              Serial.println("      ==>Artificial lights are  ON");
           }
            else 
           {
             digitalWrite(AL,LOW);

             Serial.println("    ->Required Sunlight Present");
             Serial.println("      ==>Artificial lights are  OFF");
           }   
  
       }           //  LDR ends here

       { 
                   
            Serial.println();
            Serial.println("C.Humidity & Temperature ");

            DHT.read11(dht_apin);

            Serial.print  ("    ->Current Humidity    =  ");
            Serial.print(DHT.humidity);
            Serial.println("%  ");
 
            Serial.print  ("    ->Current Temperature =  ");
            Serial.print(DHT.temperature); 
            Serial.println("C  ");
       {
            if(DHT.humidity<50)
        {
            Serial.println("      ->Less Humidity Need Cooling");       
            Serial.println("        ==> Cooling Fan Started");
            digitalWrite(in3,HIGH); 
            digitalWrite(in4,LOW); 
        }
           else if(DHT.humidity>70)
       {
           Serial.println("      ->More Humidity Need Exaust");  
           Serial.println("        ==> Exaust Fan Started");
           digitalWrite(in3,LOW); 
           digitalWrite(in4,HIGH);
        }
           else
           Serial.println("  ->Humidity Stabled");
       {

           digitalWrite(in3,LOW); 
           digitalWrite(in4,LOW);
       }
}

//Dht11 temperature

           if(DHT.temperature<25)
       {
           Serial.println("      ->Less Temperature Need Heating");  
           Serial.println("        ==> Heater Started");
           digitalWrite(heaterbulb,HIGH);
                                       
        }
           else if(DHT.temperature>38)

       {
           Serial.println("      ->More Temperature Need Cooling");  
           Serial.println("        ==> Cooling Fan Started");
           digitalWrite(in3,HIGH);
       }
           else
       {
           digitalWrite(heaterbulb,LOW);
           Serial.println  ("  ->Temperature Stabled");
       }
       }
       {
        
              Serial.println();
              Serial.println("D.Soil Moisture");

              float smvalue=analogRead (sm);
              smvalue=smvalue/204.6;
              smvalue=smvalue*20;

              Serial.print( "    ->Water Content in soil = ");
              Serial.println( smvalue);

              if(smvalue < thresholdvalue)
       {
              Serial.println("      ->Doesn't need watering");
              digitalWrite(motor, LOW);
              Serial.println("        ==> Water Pump OFF");
       }
              else 
       {
             Serial.println("      -> Time to water your plant");
             digitalWrite(motor, HIGH);

             Serial.println("        ==> Water Pump Started");
       }
       }
}
