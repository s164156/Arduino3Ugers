int ledPin = 13;
int buttonPin = 8;
int x;
int x1;
int boo = 1;

void setup() {
  pinMode(ledPin,OUTPUT);
  pinMode(buttonPin,INPUT);
  digitalWrite(ledPin,LOW);
  Serial.begin(9600);

}

void loop() {
  x = digitalRead(buttonPin);
  delay(100);
  x1 = digitalRead(buttonPin);
  if (x && !x1){
    if (boo == 1){
      digitalWrite(ledPin,HIGH);
      boo = 0;
    } else {
      digitalWrite(ledPin,LOW);
      boo = 1;
    }
  }
}
