const int ledPinR =  13;// red Pin
const int ledPinY =  10;// Yellow Pin
const int ledPinG =  8;// Green Pin
//pin 13 = Red
//pin 10 = Yellow
//pin 8 = Green
const unsigned int waitTime = 2000;

void TurnOnOffDe(const int somePin, const int wait, char x, char stage){
  if( stage == '1'){
  digitalWrite(somePin,HIGH);
  if( x == 'R'){
    Serial.println("THE LIGHT IS: RED");
  }
  if( x == 'Y'){
    Serial.println("THE LIGHT IS: YELLOW");
  }
  if( x == 'G'){
    Serial.println("THE LIGHT IS: GREEN");
  }
  delay(waitTime);
  digitalWrite(somePin,LOW);
  }
  else{
    
    digitalWrite(ledPinR,HIGH);
    digitalWrite(ledPinY,HIGH);
    Serial.println("THE LIGHT IS RED AND YELLOW");
    delay(wait);
    digitalWrite(ledPinR,LOW);
    digitalWrite(ledPinY,LOW);
  }
}

void RedStage(){
  TurnOnOffDe(ledPinR,waitTime,'R','1'); 
}

void RedYellow(){
  TurnOnOffDe(ledPinR,waitTime,'X','0');
  
}

void YellowStage(){
  TurnOnOffDe(ledPinY,waitTime-1000,'Y','1'); 
}
void GreenStage(){
  TurnOnOffDe(ledPinG,waitTime,'G','1'); 
}

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(ledPinR, OUTPUT);
  pinMode(ledPinG, OUTPUT);
  pinMode(ledPinY, OUTPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  RedStage();
  RedYellow();
  GreenStage();
  YellowStage();
}
