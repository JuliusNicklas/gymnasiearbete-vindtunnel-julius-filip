#include <HX711.h>

// Define the digital output (DOUT) and clock (SCK) pins for the first HX711
const int DAT1 = 27;
const int CLK1 = 12;

// Define the digital output (DOUT) and clock (SCK) pins for the second HX711
const int DAT2 = 15;
const int CLK2 = 33;

// Initialize the HX711 load cells
HX711 cell1(DAT1, CLK1);
HX711 cell2(DAT2, CLK2);

// Define the known weight
int known_weight = 1000;

void setup() {
  // Start the serial communication
  Serial.begin(9600);

  // Set the gain for the HX711 amplifier
  cell1.set_gain(128);
  cell2.set_gain(128);
}

void loop() {
  // Apply the known weight to cell 1 and measure the output value
  cell1.tare();
  int output_value_cell1 = cell1.read();

  // Calculate the scale factor for cell 1
  float scale_factor_cell1 = (float) known_weight / output_value_cell1;

  // Apply the known weight to cell 2 and measure the output value
  cell2.tare();
  int output_value_cell2 = cell2.read();

  // Calculate the scale factor for cell 2
  float scale_factor_cell2 = (float) known_weight / output_value_cell2;

  // Print the scale factors for each cell
  Serial.print("Scale factor for cell 1: ");
  Serial.println(scale_factor_cell1);
  Serial.print("Scale factor for cell 2: ");
  Serial.println(scale_factor_cell2);

  // Wait for a little bit before repeating
  delay(1000);
}