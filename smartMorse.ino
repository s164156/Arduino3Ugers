const unsigned int dotTime = 300;
const unsigned int dashTime = dotTime*3;
const unsigned int gapTime = dotTime;

void letterWait(){
  delay(gapTime*2);
}

void wordWait(){
  delay(gapTime*6);
}

void dot(){
  digitalWrite(LED_BUILTIN,HIGH);
  delay(dotTime);
  digitalWrite(LED_BUILTIN,LOW);
  delay(gapTime);
}

void dash(){
  digitalWrite(LED_BUILTIN,HIGH);
  delay(dashTime);
  digitalWrite(LED_BUILTIN,LOW);
  delay(gapTime);
}


void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT);
  String name = "SOS";
  Serial.println(name.length());
  for(int i = 0; i < name.length(); i++){
    Serial.println(i);
    char temp = name.charAt(i);
    Serial.println(temp);
    Serial.println("");
    switch(name.charAt(i)){
      case 'S':
        for(int j = 0; j < 3; j++){
          dot();
        } letterWait();
      break;
      
      case 'O':
        for(int j = 0; j < 3; j++){
          dash();
        } letterWait();
      break;
    }
  }
}

void loop() {
}
