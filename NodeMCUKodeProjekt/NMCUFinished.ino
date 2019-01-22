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
char DBuser [] = "xxxxx"; //MySQL user (normally root)
char DBpass [] = "xxxxxx"; //MySQL password to said user. 


elapsedMillis timeElapsed;
elapsedMillis timeElapsed2;
elapsedMillis timeElapsed3;

char INSERT_SQL[] = "UPDATE UserIDS.Telekom SET OpenLock = 102 WHERE id = 1"; //MySQl command sample
WiFiClient client;
MySQL_Connection conn((Client *)&client); //connection for MySQL

void DataFunctionMySQL(char statement[]){ //datafunction for MYSQL
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  cur_mem->execute(statement); 
  delete cur_mem;
}

//Global variable declarations
char c = ' ';
boolean NL = true;
int opencounter = 0;
int closecounter = 0;
int doorlock = D5; //pin for doorlock

//thingspeak variables
unsigned long time1;
unsigned long channelID = 677039;
const char * myWriteAPIKey = "xxxxxxx"; 
const char* serverTHINGSPEAK = "api.thingspeak.com";

const int postingInterval = 20 * 1000; // posting interval 20s.
const int MySQLCheck = 5 * 1000; //5 second check
const int postingInterval2 = 3.5 * 1000;


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
 closecounter += 1;
}
//LockOFF 'page'
void handleLockOFF() { 
 Serial.println("Lock OFF Page");
 server.send(200, "text/html", "LockOff");
 
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Lock Status: OFF");
 digitalWrite(doorlock,LOW);
 opencounter += 1;
}

//Function to do the thingspeak upload
void ThingSpeakUpload(){
  ThingSpeak.begin(client);
  ThingSpeak.setField(1,opencounter);
  ThingSpeak.setField(2,closecounter);
  ThingSpeak.writeFields(channelID, myWriteAPIKey); //get from Thingspeak
  client.stop();
}
//Bluetooth function
int checkuser = 0;
void BLTFunc(){
  if (BTserial.available()){
    delay(50);
        c = BTserial.read();
        if(c == 'M'){
          checkuser = 1;
          
        }
        if(c == 'X'){
          checkuser = 2;
          
        }
        if(c == '0'){
          digitalWrite(doorlock,LOW);
          //lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Lock Status: OFF");
          opencounter += 1;
          if(checkuser == 1){
            lcd.setCursor(0,1);
            lcd.print("Magnus entered");
            
          }
          if(checkuser == 2){
            lcd.setCursor(0,1);
            lcd.print("Sebastian ENT");
          }
          

        }
        if(c == '1'){
          digitalWrite(doorlock,HIGH);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Lock Status: ON");
          closecounter += 1;
          checkuser = 0;

        }
        
        Serial.write(c);
        
        

        
    }

    //AT COMMAND CODE, UNCOMMENT IF NEEDED. 
    /*
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
    */
    
}


//   _____  ______  _______  _    _  _____  
//  / ____||  ____||__   __|| |  | ||  __ \ 
// | (___  | |__      | |   | |  | || |__) |
//  \___ \ |  __|     | |   | |  | ||  ___/ 
//  ____) || |____    | |   | |__| || |     
// |_____/ |______|   |_|    \____/ |_| 

void setup(void){
  //generel LCD setup
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Booting System");
  
  //Serials
  Serial.begin(9600);
  BTserial.begin(9600);
  delay(100);
  
  WiFi.begin(ssid, password);     //CONNECTING TO WIFI
  Serial.println("");
 
  //Doorlock pin set to output
  pinMode(doorlock,OUTPUT);
  digitalWrite(doorlock,HIGH); //we always start with a locked door. 
  
  
  while (WiFi.status() != WL_CONNECTED) { //testing if we can actually connect to the specified wifi
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //Print the assigned IP 

  boolean checksql = false;
  while (conn.connect(server_addr, 3306, DBuser, DBpass) != true) {
    delay(50);
    Serial.print ( "." );
    if(timeElapsed2>MySQLCheck){ //break if we can't connect within the interval specified
      checksql = true;
      
      break;
    }
    
  }
  if(checksql == true){Serial.println("Could not connect to MySQL Google server, please check your settings");} 
  else{
  Serial.println("");  
  Serial.println("You are now connected to the MySQL Google server.");
  }
  
 
  server.on("/", handleRoot);      //We display root page, and define the two other pages.
  
  server.on("/LockOn", handleLockON); //This originates from <a href="LockOn">, Subroutine to be called
  
  server.on("/LockOff", handleLockOFF); 
 
  server.begin();
  Serial.println("HTTP server started");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Lock Status: ON");

}


// _       ____    ____   _____  
//| |     / __ \  / __ \ |  __ \ 
//| |    | |  | || |  | || |__) |
//| |    | |  | || |  | ||  ___/ 
//| |____| |__| || |__| || |      
//|______|\____/  \____/ |_|
 
void loop(){

  if(timeElapsed > postingInterval){ //if 20s has passed, we wanna do a upload. We do this to have everything running and not doing a delay();
    ThingSpeakUpload(); 
    timeElapsed = 0;
    
  }
  
  /*if(timeElapsed3 > postingInterval2){ //if 10s has passed, we wanna do a upload to the MySQL server
    DataFunctionMySQL(INSERT_SQL); 
    timeElapsed3 = 0;
    
  }*/
  server.handleClient();          //This handles client requests. 
  BLTFunc();                      // call to our bluetooth function.
  
  
  
  
}
