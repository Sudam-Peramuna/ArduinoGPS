#include <SoftwareSerial.h>
#include <TinyGPS++.h>

//Create software serial object to communicate with SIM800L
static const int RXPinSIM = 10, TXPinSIM = 9;
static const uint32_t SIMBaud = 9600;
SoftwareSerial SIMSerial(RXPinSIM, TXPinSIM); 

char SMS[200];
int count;
char sp[50];

void setup() {
   SIMSerial.begin(SIMBaud);
  Serial.begin(9600); 
  Serial.println("Initializing..."); 
  delay(1000);
 Serial.println("Start");
  resSms();
}

void loop() {
  int count = 0;
  int ct=0;
  
   while(SIMSerial.available()){
        while(SIMSerial.available()>0){
               char character = SIMSerial.read();
              SMS[count] = character;
              count++;
              delay(3);  
        }
        Serial.println(SMS);
        delay(500);

          for (int i = 0; i <sizeof(SMS)-1; i++)
            {
               if(SMS[i]=='"'  ){
                  count=i-4;
                  break;
                }
            }
      for (int x = count; x <sizeof(SMS)-1; x++)
            {
                      sp[ct] = SMS[x];
                      ct++;
                      delay(1);
            }
            if(sizeof(sp)!=0){
               Serial.println(sp+',');
            }  
      delay(500);        
  }

}
void resSms(){
  SIMSerial.println("AT");
  updateSerial();
  SIMSerial.println("AT+CMGF=1"); 
  updateSerial();
  SIMSerial.println("AT+CNMI=1,2,0,0,0");
  //updateSerial();
  delay(500);
   while(SIMSerial.available()) 
  {
    Serial.write(SIMSerial.read());
  }
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
