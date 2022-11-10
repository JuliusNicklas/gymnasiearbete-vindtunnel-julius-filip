/*
INPUT:
Read potentiometer
LOGIC:

OUTPUT:
Print fan control duty
Change output control signal to fan
*/

#include "config.h"
#include <Adafruit_TestBed.h>
extern Adafruit_TestBed TB;


int tachInput = 0;          //The input from the tachometer built in on the fan (RPM)          
int fanCtrlVoltage = 0;     //The control voltage to output to the fan control pwm input
int potentiometerInput = 0; //The Voltage recorded from the potentiometer


#define FAN_CTRL_PIN 25
#define FAN_CTRL_CHANNEL 2
#define POTENTIOMETER_PIN 34
#define TACH_PIN 39
#define NEOPIXEL_I2C_POWER 2
#define NEOPIXEL_PIN 0


void setup() {
    Serial.begin(115200);
    delay(200);
    
    //setup channels
    //ledcAttachPin(FAN_CTRL_PIN, FAN_CTRL_CHANNEL);
    //ledcSetup(FAN_CTRL_CHANNEL, 1200, 8);
    setupLedcChannel(FAN_CTRL_PIN, FAN_CTRL_CHANNEL);
    //ledcAttachPin(POTENTIOMETER_PIN, POTENTIOMETER_CHANNEL);
    //ledcSetup(POTENTIOMETER_CHANNEL, 1200, 8);
    //setupLedcChannel(POTENTIOMETER_PIN, POTENTIOMETER_CHANNEL);
}

void loop() {

  //Blink to show that the board is running
  // turn on the QT port and NeoPixel
  pinMode(NEOPIXEL_I2C_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_I2C_POWER, HIGH);
  
  // TestBed will handle the neopixel swirl for us
  TB.neopixelPin = NEOPIXEL_PIN;
  TB.neopixelNum = 1;
  TB.begin();
  TB.setColor(0xFF0000); 
  delay(50);
  TB.setColor(0x000000);
  //

  tachInput = analogRead(TACH_PIN);
  Serial.print("RPM:");
  Serial.print(tachInput);
  Serial.print("\t");

  potentiometerInput = analogRead(POTENTIOMETER_PIN);
  Serial.print(potentiometerInput);
  Serial.print("\t");
  fanCtrlVoltage = map(potentiometerInput, 0, 4095, 0, 255);

  fanCtrlVoltage = constrain(fanCtrlVoltage, 0, 255);

  Serial.println(fanCtrlVoltage);
  ledcWrite(2, fanCtrlVoltage);

  delay(100);
}

void setupLedcChannel(uint8_t pin, uint8_t channel){
  ledcAttachPin(pin, channel);
  ledcSetup(channel, 1200, 8);
}