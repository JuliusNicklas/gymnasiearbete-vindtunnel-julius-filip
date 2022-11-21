#include "esp_attr.h"

int tachCount = 0;
int RPM = 0;  
int fanCtrlVoltage = 0;
unsigned long currentMillis = 0;
unsigned long previousTach = 0;
unsigned long tachInterval = 1000;

#define FAN_CTRL_PIN 25
#define FAN_CTRL_CHANNEL 2
#define TACH_PIN 39

void setup(){
    Serial.begin(115200);
    delay(200);

    setupLedcChannel(FAN_CTRL_PIN, FAN_CTRL_CHANNEL);

    pinMode(TACH_PIN, INPUT);


    attachInterrupt(TACH_PIN, tachCounter, RISING);
}

void loop(){
    ledcWrite(FAN_CTRL_PIN, fanCtrlVoltage);

    if(currentMillis - previousTach >= tachInterval){

        //RPM = ((tachCount/2)/((currentMillis - previousTach)/1000))*60;
        //RPM = (tachCount/2)*60;
        Serial.print(tachCount);
        tachCount = 0;
        previousTach = currentMillis;
        
    }
}

void setupLedcChannel(uint8_t pin, uint8_t channel){
  ledcAttachPin(pin, channel);
  ledcSetup(channel, 1200, 8);
}

void tachCounter() {
    tachCount++;        //increments tachCount by one, for each tach pulse
  //  Serial.println("count!");
}

