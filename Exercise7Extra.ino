int buttonPin = 8;
int x;
int x1;
int rnd, rnd1, rnd2;

const int REDPIN = 9;
const int GREENPIN = 10;
const int BLUEPIN = 11;

void setup() {
  Serial.begin(9600);
  pinMode(REDPIN,OUTPUT);
  pinMode(GREENPIN,OUTPUT);
  pinMode(BLUEPIN,OUTPUT);
  pinMode(buttonPin,INPUT);
}


void loop() {
  x = digitalRead(buttonPin);
  delay(100);
  x1 = digitalRead(buttonPin);
  if (x && !x1){
    rnd = (random(0,255));
    rnd1 = (random(0,255));
    rnd2 = (random(0,255));
    Serial.println(rnd);
    Serial.println(rnd1);
    Serial.println(rnd2);
    analogWrite(REDPIN,rnd);
    analogWrite(GREENPIN,rnd1);
    analogWrite(BLUEPIN,rnd2);
  }
}
