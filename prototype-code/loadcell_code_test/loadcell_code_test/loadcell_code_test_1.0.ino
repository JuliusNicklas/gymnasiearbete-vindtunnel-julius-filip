#include "HX711.h"

// Initialize two HX711 instances for the two load cells
HX711 loadCell1;
HX711 loadCell2;

// Define the digital output (DOUT) and clock (SCK) pins for the first HX711
const int DAT1 = 27;
const int CLK1 = 12;

// Define the digital output (DOUT) and clock (SCK) pins for the second HX711
const int DAT2 = 15;
const int CLK2 = 33;

void setup() {
  Serial.begin(115200);

  // Initialize the first HX711
  loadCell1.begin(DAT1, CLK1);
  
  //scale_factor = known_weight / output_value
  loadCell1.set_scale(2280.f);  // Set the scale factor for the first load cell
  loadCell1.tare();  // Tare the first scale

  // Initialize the second HX711
  loadCell2.begin(DAT2, CLK2);

  //scale_factor = known_weight / output_value
  loadCell2.set_scale(2280.f);  // Set the scale factor for the second load cell
  loadCell2.tare();  // Tare the second scale
}

void loop() {
  // Read the weight of the first load cell
  float weight1 = loadCell1.get_units(10);

  // Read the weight of the second load cell
  float weight2 = loadCell2.get_units(10);

  // Print the weights to the serial monitor
  Serial.print("Weight 1: ");
  Serial.print(weight1);
  Serial.print(" Weight 2: ");
  Serial.println(weight2);
  delay(250);
}