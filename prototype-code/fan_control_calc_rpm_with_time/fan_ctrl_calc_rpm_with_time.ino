#include "esp_attr.h"

unsigned long volatile tachTS1;     //time stamp of second to last tach pulse reading
unsigned long volatile tachTS2;     //time stamp of last tach pulse reading
const int debounceTime = 5;         //debounce time in milliseconds 
const int fanStoppedThreshold = 500;  //threshold in time since last tach pulse where the fan is considered stopped
int RPM = 0;                        //Calculated 1-sec average RPM
int fanCtrlVoltage = 0;             //The control voltage to output to the fan control pwm input
int fanPowerPercent = 0;
int potentiometerInput = 0;         //The Voltage recorded from the potentiometer
const int potentiometerSamples = 500;     //Samples from potentiometer to get average value
unsigned long currentMillis = 0;
unsigned long previousTach = 0;
const unsigned long tachInterval = 1000;  //The interval between measurments of RPM. >>MUST BE 1000<<
                                    //Used for calculating average RPM last second
unsigned long previousUpdate = 0;   
unsigned long updateInterval = 100;  //The delay between general updates (ticks)

#define FAN_CTRL_PIN 25
#define FAN_CTRL_CHANNEL 2
#define POTENTIOMETER_PIN 34
#define TACH_PIN 39

void setupLedcChannel(uint8_t pin, uint8_t channel){
  ledcAttachPin(pin, channel);
  ledcSetup(channel, 1200, 8);
}

void tachCounter() {
  unsigned long m=millis();
  if(m-tachTS2>debounceTime){
    tachTS1 = tachTS2;
    tachTS2 = m;
  }
}

unsigned long calcRPM() {
  if(millis()-tachTS2 < fanStoppedThreshold && tachTS2!=0){
        return (60000/(tachTS2-tachTS1))/2;
  }
  else return 0;
}

void setup(){

    Serial.begin(115200);
    delay(200);

    setupLedcChannel(FAN_CTRL_PIN, FAN_CTRL_CHANNEL);

    pinMode(TACH_PIN, INPUT);
    pinMode(POTENTIOMETER_PIN, INPUT);
    

    attachInterrupt(TACH_PIN, tachCounter, RISING);    //Upon detecting rising edge on tach pulse on tachInput, 
                                                        //trigger tachCounter function
}

void loop(){

    currentMillis = millis();

    //Print values
   /*  if(currentMillis - previousUpdate >= updateInterval){
        previousUpdate = currentMillis;
 */
        for(int i = 0; i < potentiometerSamples; i++){
        potentiometerInput += analogRead(POTENTIOMETER_PIN);
        }
        potentiometerInput /= potentiometerSamples;

        fanCtrlVoltage = map(potentiometerInput, 0, 4095, 0, 255);
        fanCtrlVoltage = constrain(fanCtrlVoltage, 0, 255);
        ledcWrite(FAN_CTRL_CHANNEL, fanCtrlVoltage);
        fanPowerPercent = map(fanCtrlVoltage, 0, 255, 0, 100);

        //Serial.print("Speed: ");
        Serial.print(RPM);
        //Serial.print(" RPM");
        Serial.print("\t");

        Serial.print(potentiometerInput);
        Serial.print("\t");

        //Serial.print("Fan power: ");
        Serial.println(fanPowerPercent);
        //Serial.print("%");
        //Serial.println("\t");
    //}

    //Calculate 1-sec average RPM
    if(currentMillis - previousTach >= tachInterval){
      RPM = calcRPM();
    }
}