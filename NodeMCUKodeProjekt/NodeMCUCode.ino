//Sources:
//https://circuits4you.com/2018/02/05/esp8266-arduino-wifi-web-server-led-on-off-control/
//
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ThingSpeak.h>
#include <elapsedMillis.h>
elapsedMillis timeElapsed;

unsigned long time1;
unsigned long channelID = xx; //your channal
const char * myWriteAPIKey = "xx"; //your API
const char* serverTHINGSPEAK = "api.thingspeak.com";
const int postingInterval = 20 * 1000; // post data every 20 seconds
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
WiFiClient client;
SoftwareSerial BTserial(D6, D7); //D6 -> TXD && D7 -> RXD

char c=' ';
boolean NL = true;
int counter = 0;
//Our HTML webpage contents in program memory
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
<P ALIGN=LEFT>
<h1><p align="center">Locking System v0.1 Sebastian & Magnus</h1><br>

Visitor count: somevariable<br>
<br>
Click to open lock <a href="LockOff" target ="myIframe"><button type="button">Open Lock!</button></a><br>
<br>
Click to close lock <a href="LockOn" target ="myIframe"><button type="button">Close Lock!</button></a><br>
Current Lock State:<iframe name="myIframe" width="100" height="25" frameBorder="0"><br>


</P>

</body>
</html>
)=====";

//---------------------------------------------------------------
//On board LED Connected to GPIO2
#define LED 2  
 
//SSID and Password of your WiFi router
const char* ssid = "x";
const char* password = "x";
 
//Declare a global object variable from the ESP8266WebServer class.
ESP8266WebServer server(80); //Server on port 80

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 Serial.println("You called root page");
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Lock In Page");
 
}
 
void handleLockON() { 
 Serial.println("Lock On Page");
 digitalWrite(LED,LOW); //LED is connected in reverse
 server.send(200, "text/html", "LockOn"); //Send ADC value only to client ajax request
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Lock Status: ON");
}
 
void handleLockOFF() { 
 Serial.println("Lock OFF Page");
 digitalWrite(LED,HIGH); //LED off
 server.send(200, "text/html", "LockOff"); //Send ADC value only to client ajax request
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Lock Status: OFF");
}

void ThingSpeakUpload(){
  ThingSpeak.begin(client);
  ThingSpeak.setField(1,counter);
  ThingSpeak.writeFields(channelID, myWriteAPIKey); //get from Thingspeak
  client.stop();
}

void BLTFunc(){
  if (BTserial.available()){
        c = BTserial.read();
        Serial.write(c);
    
        //Let's assume that when the connection is first made through the app
        //it sends a single char, from that we can count up how many visitors have been there
        counter +=1;
        //Serial.println("Visitor Count: ");
        //Serial.print(counter);
        
    }
 
    // Read from the Serial Monitor and send to the Bluetooth module
    if (Serial.available()){
        c = Serial.read();
 
        // do not send line end characters to the HM-10
        if (c!=10 & c!=13 ) {  
             BTserial.write(c);
        }
        // Echo the user input to the main window. 
        // If there is a new line print the ">" character.
        if (NL) { Serial.print("\r\n>");  NL = false; }
        Serial.write(c);
        if (c==10) { NL = true; }
    }
}
//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("HELLO");
  Serial.begin(9600);
  BTserial.begin(9600);
  delay(100);
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  //Onboard LED port Direction output
  pinMode(LED,OUTPUT); 
  //Power on LED state off
  digitalWrite(LED,HIGH);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
 
  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/LockOn", handleLockON); //as Per  <a href="ledOn">, Subroutine to be called
  server.on("/LockOff", handleLockOFF); 
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}


void loop(void){
  if(timeElapsed > postingInterval){
    ThingSpeakUpload();
    timeElapsed = 0;
  }
  server.handleClient();          //Handle client requests
  BLTFunc(); 
}
