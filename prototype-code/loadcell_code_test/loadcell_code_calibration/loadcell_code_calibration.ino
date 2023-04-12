// Calibrating the load cell
#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 26;
const int LOADCELL_SCK_PIN = 25;

HX711 scale;

void setup() {
  //Begins communication with serialmonitor
  Serial.begin(115200);
  //Starts communication with scale using LOADCELL_DOUT_PIN and LOADCELL_SCK_PIN
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}

void loop() {

  if (scale.is_ready()) //When scale communication is ready starts the calibration process
  {
    //Tares scale and defines what value is zero
    scale.set_scale();
    Serial.println("Tare... remove any weights from the scale.");
    delay(5000);
    scale.tare();
    Serial.println("Tare done...");

    //Weights known item and displays raw output from 10 measurings
    Serial.print("Place a known weight on the scale...");
    delay(5000);
    long reading = scale.get_units(10);
    Serial.print("Result: ");
    Serial.println(reading);
  } 
  else //If communication is not established with the scale send an error message
  {
    Serial.println("HX711 not found.");
  }
  //Delay between next reading
  delay(1000);
}

//calibration factor will be the (reading)/(known weight)
//Cell1: 3150
//Cell2: 2987
//Cell3: 12181