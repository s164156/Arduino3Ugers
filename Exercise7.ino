int R;
int G;
int B;
int start = 1;

const int REDPIN = 9;
const int GREENPIN = 10;
const int BLUEPIN = 11;
int displayTime = 100;

void setup() {
  Serial.begin(9600);
  pinMode(REDPIN,OUTPUT);
  pinMode(GREENPIN,OUTPUT);
  pinMode(BLUEPIN,OUTPUT);
}


void loop() {
  if (Serial.available() > 0) {
    if((R == 0) || (G == 0) || (B == 0)){
      int temp = Serial.read();
      if (temp != 44){
        if(start == 1){
          R = temp;
          start = 2;
        }
        else if (start == 2){
          G = temp;
          start = 3;
        }
        else {
          B = temp;
          start = 4;
        }
      }
    }
  }
  if (start == 4){
    analogWrite(REDPIN, R);
    analogWrite(GREENPIN, G);
    analogWrite(BLUEPIN, B);
    R = 0;
    G = 0;
    B = 0;
    start = 1;
    
  }
}
