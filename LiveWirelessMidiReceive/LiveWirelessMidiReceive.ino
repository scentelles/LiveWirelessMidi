
#include <ESP8266WiFi.h>         // https://github.com/esp8266/Arduino


char ssid[] = "slyzic-hotspot";                   // your network SSID (name)
char pass[] = "totototo";       // your network password

  
#include "OSCManager.h"

// Channel Voice Messages
#define MIDI_STATUS_NOTE_OFF 0x80
#define MIDI_STATUS_NOTE_ON 0x90
#define MIDI_STATUS_POLYPHONIC_KEY_PRESSURE 0xA0
#define MIDI_STATUS_CONTROL_CHANGE 0xb0
#define MIDI_STATUS_PROGRAM_CHANGE 0xc0
#define MIDI_STATUS_CHANNEL_PRESSURE 0xd0
#define MIDI_STATUS_PITCH_WHEEL_CHANGE 0xe0
#define MIDI_CLOCK 0xF8
#define MIDI_ACTIVE_SENSING 0xFE

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

  myOSCManager_ = new OSCManager(new IPAddress(10,3,141,1), 8000, 8886);
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

      //FIlter out midi clock and active sensing that would overflow the OSC link
      if((commandByte != MIDI_CLOCK) and (commandByte != MIDI_ACTIVE_SENSING))
      {
        myOSCManager_->sendOSCMessage("/midi/voicelive", commandByte, value1, value2);
        digitalWrite(D4,LOW);//turn on led
      }
    }
  }
  while (Serial.available() > 2);//when at least three bytes available
}
    

void loop(){
  checkMIDI();
  delay(100);
  digitalWrite(D4,HIGH);//turn led off
}
