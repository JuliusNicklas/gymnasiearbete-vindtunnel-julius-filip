#include "esp_attr.h"

int tachInput = 0;                  //The input from the tachometer built in on the fan (RPM)
                                    //Sends >>TWO PULSES PER ROTATION<<
                                    //Used for counting average RPM last second
int tachCount = 0;                  //Counts pulses since laste 1-second average calculation     
int RPM = 0;                        //Calculated 1-sec average RPM
int fanCtrlVoltage = 0;             //The control voltage to output to the fan control pwm input
int fanPowerPercent = 0;
int potentiometerInput = 0;         //The Voltage recorded from the potentiometer
unsigned long currentMillis = 0;
unsigned long previousTach = 0;
unsigned long tachInterval = 1000; //The interval between measurments of RPM. >>MUST BE 1000<<
                                    //Used for calculating average RPM last second
unsigned long previousUpdate = 0;
unsigned long updateInterval = 100; //The delay between general updates (ticks)

#define FAN_CTRL_PIN 25
#define FAN_CTRL_CHANNEL 2
#define POTENTIOMETER_PIN 34
#define TACH_PIN 39

void setup(){

    Serial.begin(115200);
    delay(200);

    setupLedcChannel(FAN_CTRL_PIN, FAN_CTRL_CHANNEL);

    attachInterrupt(TACH_PIN, tachCounter, RISING);    //Upon detecting rising edge on tach pulse on tachInput, 
                                                        //trigger tachCounter function
}

void loop(){

    currentMillis = millis();

    //Print values
    if(currentMillis - previousUpdate >= updateInterval){
        previousUpdate = currentMillis;

        potentiometerInput = analogRead(POTENTIOMETER_PIN);

        fanCtrlVoltage = map(potentiometerInput, 0, 4095, 0, 255);
        fanCtrlVoltage = constrain(fanCtrlVoltage, 0, 255);
        ledcWrite(2, fanCtrlVoltage);
        fanPowerPercent = map(fanCtrlVoltage, 0, 255, 0, 100);

        Serial.print("Speed: ");
        Serial.print(RPM);
        Serial.print(" RPM");
        Serial.print("\t");

        Serial.print("Fan power: ");
        Serial.print(fanPowerPercent);
        Serial.print("%");
        Serial.println("\t");
    }

    //Calculate 1-sec average RPM
    if(currentMillis - previousTach >= tachInterval){

        //RPM = ((tachCount/2)/((currentMillis - previousTach)/1000))*60;
        RPM = (tachCount/80)*60;
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