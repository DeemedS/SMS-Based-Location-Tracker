#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <Wire.h>

int RXPin = 13;
int TXPin = 12;
int GPSRXPin = 10;
int GPSTXPin = 11;
int but = 0;
int GPSBaud = 9600;
int SIMBaud = 9600;

const int buttonPin1 = 9;
const int buttonPin2 = 8;
const int buttonPin3 = 7;

const int ledPin1 =  6;
const int ledPin2 =  5;
const int ledPin3 =  4;
int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;

TinyGPSPlus gps;
SoftwareSerial gpsSerial(GPSRXPin, GPSTXPin);
SoftwareSerial simSerial(RXPin, TXPin);
char Received_SMS;
short M1_OK=-1,M2_OK=-1;
String Data_SMS;

void setup()
{
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  
  Serial.begin(9600);
  gpsSerial.begin(GPSBaud);
  ReceiveMode();
  
  }

void loop()
{
  String RSMS; 
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  // This sketch displays information every time a new sentence is correctly encoded.
      gps.encode(gpsSerial.read());
  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while(true);
  }

  while(gpsSerial.available()>0){       //When SIM800L sends something to the Arduino... problably the SMS received... if something else it's not a problem
        
        Received_SMS=gpsSerial.read();  //"char Received_SMS" is now containing the full SMS received
        Serial.print(Received_SMS);   //Show it on the serial monitor (optional)     
        RSMS.concat(Received_SMS);    //concatenate "char received_SMS" to RSMS which is "empty"
        M1_OK=RSMS.indexOf("M1");   //And this is why we changed from char to String, it's to be able to use this function "indexOf"
        M2_OK=RSMS.indexOf("M2");   //"indexOf function looks for the substring "x" within the String (here RSMS) and gives us its index or position
                                      //For example if found at the beginning it will give "0" after 1 character it will be "1"
                                      //If it's not found it will give "-1", so the variables are integers
        
    }

  if(M1_OK!=-1){                         //If "DHT" word is found within the SMS, it means that DHT_OK have other value than -1 so we can proceed
    Serial.println("found M1");          //Shows on the serial monitor "found DHT" (optional)

    Data_SMS = "M1";       //Prepare the SMS to send, it contains some strings
                                                                                         //And then the values read
    
    Send_Data();                      //This function set the sending SMS mode, prepare the phone number to which we gonna send, and send "Data_SMS" String
    ReceiveMode();                   //Come back to Receving SMS mode and wait for other SMS
    
    M1_OK=-1;                      //If the DHT is found the variable should be reset to -1 otherwise it will be kept as !=-1 and will send SMS over and over
    M2_OK=-1;                      //Maybe not required... I did a lot of tests and maybe at the beginning the RSMS string kept concating and MLX word was kept there
                                    //And at this point I'm too lazy to reupload the code without it and test...
  }

  if(M2_OK!=-1){                   //Same thing if the "MLX" word is found, Serial.print things are optionnal
  delay(1000);  Serial.println("found M2");

  Data_SMS = "M2";

  Send_Data();
  ReceiveMode();
  
  M1_OK=-1;
  M2_OK=-1;
}

  



  if (buttonState1 == LOW)
  {
    
      simSerial.begin(SIMBaud);
    
      Serial.println("Initializing..."); 
      delay(1000);
    
      simSerial.println("AT"); //Once the handshake test is successful, it will back to OK
      updateSerial();
      simSerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
      updateSerial();
      simSerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
      updateSerial();
      simSerial.println("AT+CMGF=1"); // Configuring TEXT mode
      updateSerial();
      simSerial.println("AT+CMGS=\"+639393312009\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
      updateSerial();
      
      simSerial.print("HEALTH");
      simSerial.print("Latitude: ");
      simSerial.println(gps.location.lat(), 6);
      simSerial.print("Longitude: ");
      simSerial.println(gps.location.lng(), 6);
      simSerial.print("Altitude: ");
      simSerial.println(gps.altitude.meters());
      simSerial.print("https://www.google.com/maps/place/");
      simSerial.print(gps.location.lat(), 6);
      simSerial.print("N,");
      simSerial.print(gps.location.lng(), 6);
      simSerial.println("E");
    
      simSerial.print("Date: ");
      simSerial.print(gps.date.month());
      simSerial.print("/");
      simSerial.print(gps.date.day());
      simSerial.print("/");
      simSerial.println(gps.date.year());
    
      simSerial.print("Time: ");
      if (gps.time.hour() < 10) Serial.print(F("0"));
      simSerial.print(gps.time.hour());
      simSerial.print(":");
      if (gps.time.minute() < 10) Serial.print(F("0"));
      simSerial.print(gps.time.minute());
      simSerial.print(":");
      if (gps.time.second() < 10) Serial.print(F("0"));
      simSerial.print(gps.time.second());
      simSerial.print(".");
      if (gps.time.centisecond() < 10) Serial.print(F("0"));
      simSerial.println(gps.time.centisecond());
      updateSerial();
      simSerial.write(26);
    
      digitalWrite(ledPin1, HIGH);
      delay(100);
      digitalWrite(ledPin1, LOW);
    
          }
     else
     {
          digitalWrite(ledPin1, LOW);
 
          }

 if (buttonState2 == LOW)
  {
    
      simSerial.begin(SIMBaud);
    
      Serial.println("Initializing..."); 
      delay(1000);
    
      simSerial.println("AT"); //Once the handshake test is successful, it will back to OK
      updateSerial();
      simSerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
      updateSerial();
      simSerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
      updateSerial();
      simSerial.println("AT+CMGF=1"); // Configuring TEXT mode
      updateSerial();
      simSerial.println("AT+CMGS=\"+639393312009\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
      updateSerial();

      simSerial.print("EMERGECY");
      simSerial.print("Latitude: ");
      simSerial.println(gps.location.lat(), 6);
      simSerial.print("Longitude: ");
      simSerial.println(gps.location.lng(), 6);
      simSerial.print("Altitude: ");
      simSerial.println(gps.altitude.meters());
      simSerial.print("https://www.google.com/maps/place/");
      simSerial.print(gps.location.lat(), 6);
      simSerial.print("N,");
      simSerial.print(gps.location.lng(), 6);
      simSerial.println("E");
    
      simSerial.print("Date: ");
      simSerial.print(gps.date.month());
      simSerial.print("/");
      simSerial.print(gps.date.day());
      simSerial.print("/");
      simSerial.println(gps.date.year());
    
      simSerial.print("Time: ");
      if (gps.time.hour() < 10) Serial.print(F("0"));
      simSerial.print(gps.time.hour());
      simSerial.print(":");
      if (gps.time.minute() < 10) Serial.print(F("0"));
      simSerial.print(gps.time.minute());
      simSerial.print(":");
      if (gps.time.second() < 10) Serial.print(F("0"));
      simSerial.print(gps.time.second());
      simSerial.print(".");
      if (gps.time.centisecond() < 10) Serial.print(F("0"));
      simSerial.println(gps.time.centisecond());
      updateSerial();
      simSerial.write(26);
    
      digitalWrite(ledPin2, HIGH);
      delay(100);
      digitalWrite(ledPin2, LOW);
    
          }
     else
     {
          digitalWrite(ledPin2, LOW);
 
          }
   
  

  if (buttonState3 == LOW)
  {
    
      simSerial.begin(SIMBaud);
    
      Serial.println("Initializing..."); 
      delay(1000);
    
      simSerial.println("AT"); //Once the handshake test is successful, it will back to OK
      updateSerial();
      simSerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
      updateSerial();
      simSerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
      updateSerial();
      simSerial.println("AT+CMGF=1"); // Configuring TEXT mode
      updateSerial();
      simSerial.println("AT+CMGS=\"+639393312009\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
      updateSerial();

      simSerial.print("ACCIDENT");
      simSerial.print("Latitude: ");
      simSerial.println(gps.location.lat(), 6);
      simSerial.print("Longitude: ");
      simSerial.println(gps.location.lng(), 6);
      simSerial.print("Altitude: ");
      simSerial.println(gps.altitude.meters());
      simSerial.print("https://www.google.com/maps/place/");
      simSerial.print(gps.location.lat(), 6);
      simSerial.print("N,");
      simSerial.print(gps.location.lng(), 6);
      simSerial.println("E");
    
      simSerial.print("Date: ");
      simSerial.print(gps.date.month());
      simSerial.print("/");
      simSerial.print(gps.date.day());
      simSerial.print("/");
      simSerial.println(gps.date.year());
    
      simSerial.print("Time: ");
      if (gps.time.hour() < 10) Serial.print(F("0"));
      simSerial.print(gps.time.hour());
      simSerial.print(":");
      if (gps.time.minute() < 10) Serial.print(F("0"));
      simSerial.print(gps.time.minute());
      simSerial.print(":");
      if (gps.time.second() < 10) Serial.print(F("0"));
      simSerial.print(gps.time.second());
      simSerial.print(".");
      if (gps.time.centisecond() < 10) Serial.print(F("0"));
      simSerial.println(gps.time.centisecond());
      updateSerial();
      simSerial.write(26);
    
      digitalWrite(ledPin3, HIGH);
      delay(100);
      digitalWrite(ledPin3, LOW);
    
          }
     else
     {
          digitalWrite(ledPin3, LOW);
 
          }
   
  }

void Send_Data()
{
  Serial.println("Sending Data...");     //Displays on the serial monitor...Optional
  simSerial.print("AT+CMGF=1\r");          //Set the module to SMS mode
  delay(100);
  simSerial.print("AT+CMGS=\"+639393312009\"\r");  //Your phone number don't forget to include your country code example +212xxxxxxxxx"
  delay(500);  
  simSerial.print(Data_SMS);  //This string is sent as SMS
  delay(500);
  simSerial.print("Latitude: ");
      simSerial.println(gps.location.lat(), 6);
      simSerial.print("Longitude: ");
      simSerial.println(gps.location.lng(), 6);
      simSerial.print("Altitude: ");
      simSerial.println(gps.altitude.meters());
      simSerial.print("https://www.google.com/maps/place/");
      simSerial.print(gps.location.lat(), 6);
      simSerial.print("N,");
      simSerial.print(gps.location.lng(), 6);
      simSerial.println("E");
    
      simSerial.print("Date: ");
      simSerial.print(gps.date.month());
      simSerial.print("/");
      simSerial.print(gps.date.day());
      simSerial.print("/");
      simSerial.println(gps.date.year());
    
      simSerial.print("Time: ");
      if (gps.time.hour() < 10) Serial.print(F("0"));
      simSerial.print(gps.time.hour());
      simSerial.print(":");
      if (gps.time.minute() < 10) Serial.print(F("0"));
      simSerial.print(gps.time.minute());
      simSerial.print(":");
      if (gps.time.second() < 10) Serial.print(F("0"));
      simSerial.print(gps.time.second());
      simSerial.print(".");
      if (gps.time.centisecond() < 10) Serial.print(F("0"));
      simSerial.println(gps.time.centisecond());
      updateSerial();
      simSerial.write(26);
  delay(500);
  simSerial.println();
  Serial.println("Data Sent.");
  delay(500);

}

void ReceiveMode(){       //Set the SIM800L Receive mode
  
  simSerial.println("AT"); //If everything is Okay it will show "OK" on the serial monitor
  updateSerial();
  simSerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  simSerial.println("AT+CNMI=2,2,0,0,0"); //Configure the SIM800L on how to manage the Received SMS... Check the SIM800L AT commands manual
  updateSerial();
}
   


void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    simSerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(simSerial.available()) 
  {
    Serial.write(simSerial.read());//Forward what Software Serial received to Serial Port
  }
}
