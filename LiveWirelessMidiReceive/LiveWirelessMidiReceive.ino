
#include <ESP8266WiFi.h>         // https://github.com/esp8266/Arduino
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiManager.h>         // https://github.com/tzapu/WiFiManager
#include <WiFiClient.h>

char ssid[] = "SFR_34A8";                   // your network SSID (name)
char pass[] = "ab4ingrograstanstorc";       // your network password

  
#include "OSCManager.h"

// Channel Voice Messages
#define MIDI_STATUS_NOTE_OFF 0x80
#define MIDI_STATUS_NOTE_ON 0x90
#define MIDI_STATUS_POLYPHONIC_KEY_PRESSURE 0xA0
#define MIDI_STATUS_CONTROL_CHANGE 0xb0
#define MIDI_STATUS_PROGRAM_CHANGE 0xc0
#define MIDI_STATUS_CHANNEL_PRESSURE 0xd0
#define MIDI_STATUS_PITCH_WHEEL_CHANGE 0xe0

OSCManager * myOSCManager_;

byte commandByte;
byte value1;
byte value2;

void setup(){
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
      commandByte  = Serial.read();//read first byte
      value1       = Serial.read();//read next byte
      value2       = Serial.read();//read final byte

      myOSCManager_->sendOSCMessage("/midi/voicelive", commandByte, value1, value2);

      digitalWrite(D4,LOW);//turn on led
    }
  }
  while (Serial.available() > 2);//when at least three bytes available
}
    

void loop(){
  checkMIDI();
  delay(100);
  digitalWrite(D4,HIGH);//turn led off
}


