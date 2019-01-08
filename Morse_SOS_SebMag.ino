const unsigned int dotTime = 300; //time of dot
const unsigned int dashTime = dotTime*3;
const unsigned int gapTime = dotTime;

void Dash(){
  digitalWrite(LED_BUILTIN,HIGH);
  delay(dashTime);
  digitalWrite(LED_BUILTIN,LOW);
  delay(gapTime);}
void Dot(){
  digitalWrite(LED_BUILTIN,HIGH);
  delay(dotTime);
  digitalWrite(LED_BUILTIN,LOW);
  delay(gapTime);}

void MorseSOS(){
  //write sos in morse
  //for first S
  for(int i = 0; i<3;i++){
    digitalWrite(LED_BUILTIN,HIGH);
    delay(dotTime);
    digitalWrite(LED_BUILTIN,LOW);
    delay(gapTime);
  }
  delay(gapTime*2);
  //for the O
  for(int i = 0; i<3;i++){
    digitalWrite(LED_BUILTIN,HIGH);
    delay(dashTime);
    digitalWrite(LED_BUILTIN,LOW);
    delay(gapTime);
    
  }
  delay(gapTime*2);
  //for second S
  for(int i = 0; i<3;i++){
    digitalWrite(LED_BUILTIN,HIGH);
    delay(dotTime);
    digitalWrite(LED_BUILTIN,LOW);
    delay(gapTime);
  }
}

void MorseMagnus(){
  //for M
  for(int i = 0; i<2;i++){
    digitalWrite(LED_BUILTIN,HIGH);
    delay(dashTime);
    digitalWrite(LED_BUILTIN,LOW);
    delay(gapTime);
    
  }
  delay(gapTime*2);
  //for A
  digitalWrite(LED_BUILTIN,HIGH);
  delay(dotTime);
  digitalWrite(LED_BUILTIN,LOW);
  delay(gapTime);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(dashTime);
  digitalWrite(LED_BUILTIN,LOW);
  delay(gapTime*3);

  //G
  for(int i = 0; i<2;i++){
    digitalWrite(LED_BUILTIN,HIGH);
    delay(dashTime);
    digitalWrite(LED_BUILTIN,LOW);
    delay(gapTime);
    
  }
  digitalWrite(LED_BUILTIN,HIGH);
  delay(dotTime);
  digitalWrite(LED_BUILTIN,LOW);
  delay(gapTime*3);

  //N
  
  digitalWrite(LED_BUILTIN,HIGH);
  delay(dashTime);
  digitalWrite(LED_BUILTIN,LOW);
  delay(gapTime);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(dotTime);
  digitalWrite(LED_BUILTIN,LOW);
  delay(gapTime*3);

  //U
  for(int i = 0; i<2;i++){
    digitalWrite(LED_BUILTIN,HIGH);
    delay(dotTime);
    digitalWrite(LED_BUILTIN,LOW);
    delay(gapTime);
    
  }
  digitalWrite(LED_BUILTIN,HIGH);
  delay(dashTime);
  digitalWrite(LED_BUILTIN,LOW);
  delay(gapTime*3);

  //S
  for(int i = 0; i<3;i++){
    digitalWrite(LED_BUILTIN,HIGH);
    delay(dotTime);
    digitalWrite(LED_BUILTIN,LOW);
    delay(gapTime);
  }
  delay(gapTime*6);
  
  
  
  
  
}

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  MorseMagnus();
}

// the loop function runs over and over again forever
void loop() {
  
}
