#include <SoftwareSerial.h>
#include <TinyGPS++.h>

//Create software serial object to communicate with SIM800L
static const int RXPinSIM = 10, TXPinSIM = 9;
static const uint32_t SIMBaud = 9600;
SoftwareSerial SIMSerial(RXPinSIM, TXPinSIM); 

//Create software serial object to communicate with GPSModule
static const int RXPinGPS = 4, TXPinGPS = 3;
static const uint32_t GPSBaud = 9600;
SoftwareSerial GPSSerial(RXPinGPS, TXPinGPS);

TinyGPSPlus gps;

void setup() {
  GPSSerial.begin(9600);
   //SIMSerial.begin(SIMBaud);
  Serial.begin(9600); 
  Serial.println("Initializing..."); 
  delay(1000);
 Serial.println("Start");
  
}

void loop() {
    SendLoc();
  delay(500);

}

void SendLoc(){
 String locc;
    while (GPSSerial.available() > 0)
    {
     //Serial.println(gps.encode(GPSSerial.read()));
      if (gps.encode(GPSSerial.read())){
        //Serial.println(sendGpsLoc30());
      if(sendGpsLoc30()!="INVALID"){
       locc=sendGpsLoc30();
       GPSSerial.end();
       delay(100);
       SIMSerial.begin(SIMBaud);
       delay(100);
       sendSms(locc);
       Serial.println(locc+',');
       delay(100);
       SIMSerial.end();
       delay(100);
       GPSSerial.begin(GPSBaud);
      }
     
      }
    }
    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
      Serial.println(F("No GPS detected: check wiring."));
      SIMSerial.end();
      delay(100);
      GPSSerial.begin(GPSBaud);
      while(true);
    }
   
   //delay(30000);
}

void updateSerial()
{
  while (Serial.available()) 
  {
    SIMSerial.write(Serial.read());
  }
  while(SIMSerial.available()) 
  {
    Serial.write(SIMSerial.read());
  }
}

void sendSms(String loc){
  //Serial.println("Start SendSms");
  SIMSerial.println("AT"); 
  updateSerial();
  SIMSerial.println("AT+CMGF=1");
  updateSerial();
  delay(100);
  SIMSerial.println("AT+CMGS=\"+94767995117\"");
  delay(100);
  updateSerial();
  SIMSerial.println(loc);
  updateSerial();
  SIMSerial.write(26);
  updateSerial();
}
String sendGpsLoc30()
{
  String dLat;
  String nLng;
  String peComp;
  delay(1000);
  //Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    dLat =String(gps.location.lat(), 3);
    //Serial.print(F(","));
    nLng=String(gps.location.lng(), 3);
    peComp="Location: "+dLat+F(",")+nLng;
  }
  else
  {
    peComp=(F("INVALID"));
  }
    
  //Serial.println(peComp);

  return(peComp);  
  
}
