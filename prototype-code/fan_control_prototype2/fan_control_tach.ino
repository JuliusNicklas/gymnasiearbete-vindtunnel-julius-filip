#include "esp_attr.h"

int tachInput = 0;                  //The input from the tachometer built in on the fan
                                    //Sends >>TWO PULSES PER ROTATION<<
                                    //Used for counting average RPM last second
int tachCount = 0;                  //Counts pulses since laste 1-second average calculation     
int RPM = 0;                        //Calculated 1-sec average RPM
int fanCtrlVoltage = 0;             //The control voltage to output to the fan control pwm input
int fanPowerPercent = 0;
int potentiometerInput = 0;         //The Voltage recorded from the potentiometer
int potentiometerSamples = 500;     //Samples from potentiometer to get average value
unsigned long currentMillis = 0;
unsigned long previousTach = 0;
unsigned long tachInterval = 1000; //The interval between measurments of RPM. >>MUST BE 1000<<
                                    //Useäd for calculating average RPM last second
unsigned long previousUpdate = 0;
unsigned long updateInterval = 10; //The delay between general updates (ticks)


#define FAN_CTRL_PIN 25
#define FAN_CTRL_CHANNEL 2
#define POTENTIOMETER_PIN 34
#define TACH_PIN 36

void setupLedcChannel(uint8_t pin, uint8_t channel){  // Creates a channel and attaches a pin
  ledcAttachPin(pin, channel);  // Attaches pin to channel
  ledcSetup(channel, 20000, 8); // Sets up channel (channel number, frequency, resolution)
}

void tachCounter() {
    //tachInput = analogRead(TACH_PIN);
    tachCount++;        //increments tachCount by one, for each tach pulse
  //  Serial.println("count!");
}

void setup(){

    Serial.begin(115200);
    delay(200);

    setupLedcChannel(FAN_CTRL_PIN, FAN_CTRL_CHANNEL);

    pinMode(TACH_PIN, INPUT);
    pinMode(POTENTIOMETER_PIN, INPUT);
    

    attachInterrupt(TACH_PIN, tachCounter, FALLING);    //Upon detecting falling edge on tach pulse on tach pin, 
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
        //ledcWrite(FAN_CTRL_CHANNEL, 0);
        fanPowerPercent = map(fanCtrlVoltage, 0, 255, 0, 100);

        //Serial.print("Speed: ");
        Serial.print(RPM);
        Serial.print(" RPM\t");

        Serial.print(potentiometerInput);
        Serial.print("\t");

        //Serial.print("Fan power: ");
        Serial.println(fanPowerPercent);
        //Serial.print("%");
        //Serial.println("\t");
    //}

    //Calculate 1-sec average RPM
    if(currentMillis - previousTach >= tachInterval){

        //RPM = ((tachCount/2)/((currentMillis - previousTach)/1000))*60;
        RPM = (tachCount/2)*60;
        tachCount = 0;
        previousTach = currentMillis;
        
    }
}

