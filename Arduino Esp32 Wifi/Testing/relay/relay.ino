int in1 = 3;//in3


int in3 = 5;


int in2 = 4;//in

void setup() {
pinMode(in1, OUTPUT);
pinMode(in2, OUTPUT);// put your setup code here, to run once:
 pinMode(in3, OUTPUT);

  Serial.begin(9600);
delay(100);
}

void loop() {
digitalWrite(in1,HIGH);// put your main code here, to run repeatedly:
 delay(1000); 
 digitalWrite(in1,LOW);// put your main code here, to run repeatedly:
    delay(1000); 

digitalWrite(in2,HIGH);// put your main code here, to run repeatedly:
delay(1000);  
digitalWrite(in2,LOW);// put your main code here, to run repeatedly:
//  
  digitalWrite(in3,HIGH);// put your main code here, to run repeatedly:
delay(10000);    
  digitalWrite(in3,LOW);// put your main code here, to run repeatedly:
delay(10000);   // put your main code here, to run repeatedly:
   

 }
