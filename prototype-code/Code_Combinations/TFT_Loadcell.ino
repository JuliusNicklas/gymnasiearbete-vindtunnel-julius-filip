//Libraries:
    //Loadcells
    #include "HX711.h"
    //TFT
    #include "SPI.h"
    #include "Adafruit_GFX.h"
    #include "Adafruit_ILI9341.h"

//Pins:
    //Loadcell1
    const int DAT1 = 27;
    const int CLK1 = 12;
    //Loadcell2
    const int DAT2 = 14;
    const int CLK2 = 32;
    //TFT
    #define TFT_CS   15
    #define TFT_DC   33

//Initiaizes
    //Declares what component to use for loadcells
    HX711 loadCell1;
    HX711 loadCell2;
    //Declares pins to use for tft
    Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup() {
Serial.begin(115200); //Starts communication with serialmonitor

//Loadcells
    // Initialize the first HX711
    loadCell1.begin(DAT1, CLK1);
    
    //scale_factor = known_weight / output_value
    loadCell1.set_scale(3005.f);  // Set the scale factor for the first load cell
    loadCell1.tare();  // Tare the first scale

    // Initialize the second HX711
    loadCell2.begin(DAT2, CLK2);

    //scale_factor = known_weight / output_value
    loadCell2.set_scale(3174.f);  // Set the scale factor for the second load cell
    loadCell2.tare();  // Tare the second scale

//TFT
    tft.begin();    //Starts TFT
    tft.setRotation(1); //Tells TFT which direction to use
    tft.setTextSize(2); //Tells TFT what size of text to use
}

void loop() {
//Loadcells:
    // Read the weight of the first load cell
    float weight1 = loadCell1.get_units(10);

    // Read the weight of the second load cell
    float weight2 = loadCell2.get_units(10);

    // Print the weights to the serial monitor
    Serial.print("Weight 1: ");
    Serial.print(weight1);
    Serial.print(" Weight 2: ");
    Serial.println(weight2);
    
//TFT:
    //Clearing method
    tft.setCursor(0,0); //Moves cursor to top
    tft.fillScreen(ILI9341_BLACK); //Clears screen
    //tft.print("Fan speed: "); tft.println(fan_speed);
    tft.print("Downforce Front: "); tft.println(weight1+String("g"));
    tft.print("Downforce Rear: "); tft.println(weight2+String("g"));
    tft.print("Downforce general: "); tft.println((weight1+weight2)/2+String("g"));

//Delay between readings
delay(250);
}