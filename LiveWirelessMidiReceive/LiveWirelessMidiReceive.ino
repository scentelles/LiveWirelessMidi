
#include <ESP8266WiFi.h>         // https://github.com/esp8266/Arduino
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiManager.h>         // https://github.com/tzapu/WiFiManager
#include <WiFiClient.h>

char ssid[] = "SFR_34A8";                   // your network SSID (name)
char pass[] = "ab4ingrograstanstorc";       // your network password


#include "OSCManager.h"

OSCManager * myOSCManager_;

byte commandByte;
byte noteByte;
byte velocityByte;

void setup(){
  //Serial.begin(115200);
  Serial.begin(31250); 
  WiFi.begin(ssid, pass);



  delay(500);

  pinMode(D4,OUTPUT);
  digitalWrite(D4,HIGH);


//TODO : add Wifi timeout
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        digitalWrite(D4,HIGH);
        delay(100);
        digitalWrite(D4,LOW);                
    }

  myOSCManager_ = new OSCManager();
  myOSCManager_->setup();
}

void checkMIDI(){
  do{
    //Serial.println("checking serial");
    if (Serial.available()){
       //Serial.println("Midi received");
      commandByte = Serial.read();//read first byte
      noteByte = Serial.read();//read next byte
      velocityByte = Serial.read();//read final byte

       myOSCManager_->sendNote((short)noteByte, (short)velocityByte, 0);
       myOSCManager_->sendCC((short)commandByte, (short)noteByte);
        
      //Serial.print("Note : ");
      //Serial.println(noteByte);
       digitalWrite(D4,LOW);//turn on led
    /*  if (commandByte == noteOn){//if note on message
        //check if note == 60 and velocity > 0
        if (noteByte == 60 && velocityByte > 0){
          digitalWrite(13,HIGH);//turn on led
        }
      }*/
    }
  }
  while (Serial.available() > 2);//when at least three bytes available
}
    

void loop(){
  checkMIDI();
  delay(100);
  digitalWrite(D4,HIGH);//turn led off
}


