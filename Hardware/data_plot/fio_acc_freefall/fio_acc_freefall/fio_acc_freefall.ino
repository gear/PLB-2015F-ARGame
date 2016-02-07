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
ADXL345 acc;

boolean freefall = false;


void setup() {
  // Setting up Serial and I2C
  Serial.begin(57600);
  Wire.begin();

  // Point to accelerometer
  acc = sixDOF.acc;

  delay(5);

  // Init the 6DOF and set operational parameter for the ADXL345
  sixDOF.init();
  acc.setRangeSetting(8); // Measure from -8g to 8g
  acc.setInterrupt(2, true); // Enable interrupt for free fall, bitpos=2
  acc.setInterruptMapping(2, false); // Map interrupt to INT1

  freefall = acc.getInterruptSource(2);

  delay(5);

  // Register interrupt at Arduino's D2 pin
  attachInterrupt(0, fall, RISING);
}

void loop() {
  if (freefall) {
    freefall = false;
  }
  int_source = acc.getInterruptSource();
}

void fall(void) {
  freefall = sixDOF.acc.getInterruptSource(2);
  Serial.println("FALL!\n");
}

