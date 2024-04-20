int Fan1 = 11;
int Fan2 = 10;

void setup() {
  pinMode(Fan1, OUTPUT);
pinMode(Fan2, OUTPUT);// put your setup code here, to run once:
  Serial.begin(9600);
delay(100);
}

void loop() {
  digitalWrite(Fan1,HIGH);// put your main code here, to run repeatedly:
  digitalWrite(Fan2,LOW);
  Serial.println("FORWARD");
  delay(10000);
  digitalWrite(Fan1,LOW);// put your main code here, to run repeatedly:
     Serial.println("STOPPED");

   delay(5000);

  digitalWrite(Fan2,HIGH);
  digitalWrite(Fan1,LOW);// put your main code here, to run repeatedly:
    Serial.println("REVERSE");

  delay(10000);

}
