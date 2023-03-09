//Includes necessary libraries
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

//Defines pins for ESP32
#define TFT_CS   15
#define TFT_DC   33

//Declares pins to use for tft
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup() {
    tft.begin();
    tft.setRotation(1);
    tft.setTextSize(1);
}

void loop() {
    tft.fillScreen(ILI9341_BLACK);
    tft.print("Fan speed: "); tft.println(fan_speed);
    tft.print("Downforce Front: "); tft.println(weight1+String("g"));
    tft.print("Downforce Rear: "); tft.println(weight2+String("g"));
    tft.print("Downforce general: "); tft.println((weight1+weight2)/2+String("g"));
    delay(250);

}