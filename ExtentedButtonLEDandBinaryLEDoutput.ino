int ledPin = 13;
int buttonPin = 8;
int x;
int x1;
int boo = 1;
int rnd;

int binarray[] = {8,4,2,1};
int pinsarray[] = {12,11,10,9};
int binresult[4] = {};

void setup() {
  pinMode(ledPin,OUTPUT);
  pinMode(buttonPin,INPUT);
  digitalWrite(ledPin,LOW);
  Serial.begin(9600);
  for(int i = 0; i<4;i++){
    pinMode(pinsarray[i],OUTPUT);
  }

}

void loop() {
  x = digitalRead(buttonPin);
  delay(100);
  x1 = digitalRead(buttonPin);
  if (x && !x1){
    rnd = (random(1,15));
    Serial.println(rnd);
    for(int i = 0; i<rnd ; i++){
      digitalWrite(ledPin,HIGH);
      delay(100);
      digitalWrite(ledPin,LOW);
      delay(100);
    }
    for(int j = 0; j<4 ; j++){
      if(rnd-binarray[j]>=0){
        rnd = rnd - binarray[j];
        binresult[j] = 1;
        
      }
      else{
        binresult[j] = 0;
      }
      
    }
    for(int j = 0; j<4 ; j++){
      digitalWrite(pinsarray[j],binresult[j]);
         }
  }
}
