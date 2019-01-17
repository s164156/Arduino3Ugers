//Library includes

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ThingSpeak.h>
#include <elapsedMillis.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
IPAddress server_addr (35 ,228 ,121 ,250); //35.228.121.250
char DBuser [] = "root";
char DBpass [] = "Freezepolice1";
//char SQLCOMMAND[] = "INSERT INTO UserIDS.Telekom (UUID) VALUES ('Test'); //format is DBName.TABLE
elapsedMillis timeElapsed;
WiFiClient client;

MySQL_Connection conn((Client *)&client);

//Global variable declarations
char c= ' ';
boolean NL = true;
int counter = 0;
int doorlock = D5; //pin for doorlock

//thingspeak variables
unsigned long time1;
unsigned long channelID = 677039;
const char * myWriteAPIKey = "G2GJ90ZGZQJCDNWC"; 
const char* serverTHINGSPEAK = "api.thingspeak.com";

const int postingInterval = 20 * 1000; // posting interval 20s.

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial BTserial(D6, D7); //D6 -> TXD && D7 -> RXD


//HTML webpage stored in progmem, basically stores data in flash memory of the nodemcu
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
<P ALIGN=LEFT>
<h1><p align="center">Locking System v0.1 Sebastian & Magnus</h1><br>
<br>
Click to open lock <a href="LockOff" target ="myIframe"><button type="button">Open Lock!</button></a><br>
<br>
Click to close lock <a href="LockOn" target ="myIframe"><button type="button">Close Lock!</button></a><br>

Current Lock State:<iframe name="myIframe" width="100" height="25" frameBorder="0"></iframe><br>

</P>
<P ALIGN="center">
<pre>
 .----------------.   .----------------.   .----------------.   .----------------.   .----------------.   .----------------.  .----------------. 
| .--------------. | | .--------------. | | .--------------. | | .--------------. | | .--------------. | | .--------------. || .--------------. |
| | _____  _____ | | | |  _________   | | | |   _____      | | | |     ______   | | | |     ____     | | | | ____    ____ | || |  _________   | |
| ||_   _||_   _|| | | | |_   ___  |  | | | |  |_   _|     | | | |   .' ___  |  | | | |   .'    `.   | | | ||_   \  /   _|| || | |_   ___  |  | |
| |  | | /\ | |  | | | |   | |_  \_|  | | | |    | |       | | | |  / .'   \_|  | | | |  /  .--.  \  | | | |  |   \/   |  | || |   | |_  \_|  | |
| |  | |/  \| |  | | | |   |  _|  _   | | | |    | |   _   | | | |  | |         | | | |  | |    | |  | | | |  | |\  /| |  | || |   |  _|  _   | |
| |  |   /\   |  | | | |  _| |___/ |  | | | |   _| |__/ |  | | | |  \ `.___.'\  | | | |  \  `--'  /  | | | | _| |_\/_| |_ | || |  _| |___/ |  | |
| |  |__/  \__|  | | | | |_________|  | | | |  |________|  | | | |   `._____.'  | | | |   `.____.'   | | | ||_____||_____|| || | |_________|  | |
| |              | | | |              | | | |              | | | |              | | | |              | | | |              | || |              | |
| '--------------' | | '--------------' | | '--------------' | | '--------------' | | '--------------' | | '--------------' || '--------------' |
 '----------------'   '----------------'   '----------------'   '----------------'   '----------------'   '----------------'  '----------------'
 </pre>
</P>

</body>
</html>
)=====";


//SSID & PASS for the dtu network thing
const char* ssid = ".";
const char* password = "pingpongok5";
 
//Let's use the 8266WebServer class which is a lot more convient
ESP8266WebServer server(80); //Server on port 80

//Root page 
void handleRoot() {
 Serial.println("You called root page");
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
 
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Lock In Page");
 
}
//LockON 'page'
void handleLockON() { 
 Serial.println("Lock On Page");
 server.send(200, "text/html", "LockOn"); 
 
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Lock Status: ON");
 digitalWrite(doorlock,HIGH);
}
//LockOFF 'page'
void handleLockOFF() { 
 Serial.println("Lock OFF Page");
 server.send(200, "text/html", "LockOff");
 
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Lock Status: OFF");
 digitalWrite(doorlock,LOW);
}

//Function to do the thingspeak upload
void ThingSpeakUpload(){
  ThingSpeak.begin(client);
  ThingSpeak.setField(1,counter);
  ThingSpeak.writeFields(channelID, myWriteAPIKey); //get from Thingspeak
  client.stop();
}

//Bluetooth function
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

void UploadToMYSQL(char statement[]){
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  cur_mem -> execute(statement);
  delete cur_mem;
}
//   _____  ______  _______  _    _  _____  
//  / ____||  ____||__   __|| |  | ||  __ \ 
// | (___  | |__      | |   | |  | || |__) |
//  \___ \ |  __|     | |   | |  | ||  ___/ 
//  ____) || |____    | |   | |__| || |     
// |_____/ |______|   |_|    \____/ |_| 

void setup(void){
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("HELLO");
  Serial.begin(9600);
  BTserial.begin(9600);
  delay(100);
  
  WiFi.begin(ssid, password);     //CONNECTIN TO WIFI
  Serial.println("");
 
  //Onboard LED port Direction output
  pinMode(doorlock,OUTPUT); 
  //Power on LED state off
  //digitalWrite(LED,HIGH);
  
  
  while (WiFi.status() != WL_CONNECTED) { //testing if we can actually connect
    delay(500);
    Serial.print(".");
  }
  //while (conn.connect(server_addr, 3306, DBuser, DBpass) != true){
    //delay(500);
  //}
  //Serial.println("You are now connected to the MySQL Google server.");
 
  //If connection is accepted we print the ssid and its local IP 
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
 
  server.on("/", handleRoot);      //We displa root page, and define the two other pages.
  server.on("/LockOn", handleLockON); //This originates from <a href="LockOn">, Subroutine to be called
  server.on("/LockOff", handleLockOFF); 
 
  server.begin();
  Serial.println("HTTP server started");
}


// _       ____    ____   _____  
//| |     / __ \  / __ \ |  __ \ 
//| |    | |  | || |  | || |__) |
//| |    | |  | || |  | ||  ___/ 
//| |____| |__| || |__| || |     
//|______|\____/  \____/ |_|
 
void loop(void){
  if(timeElapsed > postingInterval){ //if 20s has passed, we wanna do a upload. We do this to have everything running and not doing a delay();
    ThingSpeakUpload();
    timeElapsed = 0;
  }
  server.handleClient();          //This handles client requests. 
  BLTFunc();                      // call to our bluetooth function. 
 
}
