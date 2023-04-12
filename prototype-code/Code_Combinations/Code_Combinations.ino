//Libraries:
    //Loadcells
    #include "HX711.h"
    //TFT
    #include "SPI.h"
    #include "Adafruit_GFX.h"
    #include "Adafruit_ILI9341.h"

//Pins:
    //Loadcell1
    const int DAT1 = 26;
    const int CLK1 = 25;
    //Loadcell2
    const int DAT2 = 14;
    const int CLK2 = 32;
    //Loadcell2
    const int DAT3 = 27;//26
    const int CLK3 = 12;//25
    //TFT
    #define TFT_CS   15
    #define TFT_DC   33

    //Output pin to fan
    #define FAN_CTRL_PIN 4
    #define FAN_CTRL_CHANNEL 2

    //Potentiometer pin
    #define POTENTIOMETER_PIN 34

//Variables:
    //Load cells
    float weight1; 
    float weight2; 
    float weight3;
    float downforceKoefficient;
    //Fan control
    int fanCtrlVoltage = 0;             //The control voltage to output to the fan control pwm input
    int fanPowerPercent = 0;
    float windSpeed = 0.0;
    //Potentiometer
    int potentiometerInput = 0;         //The Voltage recorded from the potentiometer

//Initiaizes
    //Declares what component to use for loadcells
    HX711 loadCell1;
    HX711 loadCell2;
    HX711 loadCell3;
    //Declares pins to use for tft
    Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setupLedcChannel(uint8_t pin, uint8_t channel){  // Creates a channel and attaches a pin
  ledcAttachPin(pin, channel);  // Attaches pin to channel
  ledcSetup(channel, 20000, 8); // Sets up channel (channel number, frequency, resolution)
}

void setup() {
    Serial.begin(115200); //Starts communication with serialmonitor
    delay(200);
//Loadcells
    // Initialize the first HX711
    Serial.println("starting first cell");
    loadCell1.begin(DAT1, CLK1); 
    
    //scale_factor = known_weight / output_value
    loadCell1.set_scale(3150.f);  // Set the scale factor for the first load cell ---- 
    loadCell1.tare();  // Tare the first scale -----

    // Initialize the second HX711
    Serial.println("starting second cell");
    loadCell2.begin(DAT2, CLK2); 

    //scale_factor = known_weight / output_value
    loadCell2.set_scale(2987.f);  // Set the scale factor for the second load cell  -----
    loadCell2.tare();  // Tare the second scale ----

    // Initialize the second HX711
    Serial.println("starting third cell");
    loadCell3.begin(DAT3, CLK3); 

    //scale_factor = known_weight / output_value
    loadCell3.set_scale(12181.f);  // Set the scale factor for the second load cell  -----
    loadCell3.tare();  // Tare the second scale ----

    Serial.println("done");

//TFT
    tft.begin();    //Starts TFT
    tft.setRotation(1); //Tells TFT which direction to use
    tft.setTextSize(2); //Tells TFT what size of text to use

//Fan
    setupLedcChannel(FAN_CTRL_PIN, FAN_CTRL_CHANNEL);
    pinMode(POTENTIOMETER_PIN, INPUT);

}

void loop() {
//Fan:
    potentiometerInput = analogRead(POTENTIOMETER_PIN);
    
    fanCtrlVoltage = map(potentiometerInput, 0, 4095, 0, 255);
    fanCtrlVoltage = constrain(fanCtrlVoltage, 0, 255);
    //fanCtrlVoltage = 35;
    ledcWrite(FAN_CTRL_CHANNEL, fanCtrlVoltage);

    fanPowerPercent = map(fanCtrlVoltage, 0, 255, 0, 100);

    windSpeed = 0.136 + 0.0245*fanCtrlVoltage + 0.0000148*fanCtrlVoltage*fanCtrlVoltage;

//Loadcells:
    // Read the weight of the first load cell
    weight1 = loadCell1.get_units(10); 
    //float weight1 = fanCtrlVoltage*fanCtrlVoltage*0.02+2*fanCtrlVoltage;

    // Read the weight of the second load cell
    weight2 = loadCell2.get_units(10); 
    //weight2 = fanCtrlVoltage*fanCtrlVoltage*0.015+3*fanCtrlVoltage;

    // Read the weight of the  third load cell
    weight3 = loadCell3.get_units(10);
    //weight3 = fanCtrlVoltage*fanCtrlVoltage*0.01;

    //Calculate downforce/drag ratio
    downforceKoefficient = (weight1+weight2)/weight3;

//TFT:
    //Clearing method
    tft.setCursor(0,0); //Moves cursor to top
    tft.fillScreen(ILI9341_BLACK); //Clears screen
    //tft.print("Fan speed: "); tft.println(fan_speed);
    tft.print("Downforce Front: "); tft.println(weight1+String("g"));
    tft.print("Downforce Rear:  "); tft.println(weight2+String("g"));
    tft.print("Drag:            "); tft.println(weight3+String("g"));
    tft.print("Downforce koefficient:"); tft.println(downforceKoefficient);
    //tft.print("Downforce general: "); tft.println((weight1+weight2)/2+String("g"));
    tft.print("Fan power: "); tft.println(fanCtrlVoltage); //tft.println("%");
    if(fanCtrlVoltage < 25) {
        tft.print("Wind speed: "); tft.println("0.0 m/s");
    }
    else {
        tft.print("Wind speed: "); tft.print(windSpeed); tft.println(" m/s");
    }

//Delay between readings
//delay(200);
}