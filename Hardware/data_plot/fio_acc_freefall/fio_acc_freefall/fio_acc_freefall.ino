/* fio_acc_freefall
 * Author: Hoang NT
 * Date: 2016/07/02
 * Description: Arduino firmware for detecting free fall of the device.
 * Hardware: Arduino Fio, Digi XBee S2, Sparkfun 6DOF
 * Reference: ADXL345 datasheet.
 */

#include <Wire.h> // I2C communication
#include <FreeSixIMU.h>
#include <FIMU_ADXL345.h>

float gravity[3]; // gx, gy, gz
byte int_source;  // save INT_SOURCE value, indicating which action causes interrupt
FreeSixIMU sixDOF = FreeSixIMU();

boolean freefall = false;


void setup() {
  // Setting up Serial and I2C
  Serial.begin(57600);
  Wire.begin();

  delay(5);

  // Init the 6DOF and set operational parameter for the ADXL345
  sixDOF.init();
  sixDOF.acc.setRangeSetting(8); // Measure from -8g to 8g
  sixDOF.acc.writeTo(0x2A, 0x01); // Set 

}

void loop() {
  // put your main code here, to run repeatedly:

}
