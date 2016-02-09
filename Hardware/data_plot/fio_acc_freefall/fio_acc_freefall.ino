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

int timer = 500;


void setup() {
  // Setting up Serial and I2C
  Serial.begin(57600);
  Wire.begin();

  // Debug
  pinMode(13, OUTPUT);

  // Point to accelerometer
  acc = sixDOF.acc;

  delay(5);

  // Init the 6DOF and set operational parameter for the ADXL345
  sixDOF.init();
  acc.setRangeSetting(8); // Measure from -8g to 8g
  /*
  acc.setInterrupt(ADXL345_INT_FREE_FALL_BIT, 2); // Enable interrupt for free fall, bitpos=2
  acc.setInterrupt(ADXL345_INT_SINGLE_TAP_BIT, 0); 
  acc.setInterrupt(ADXL345_INT_DATA_READY_BIT, 0); 
  acc.setInterrupt(ADXL345_INT_DOUBLE_TAP_BIT, 0);
  acc.setInterrupt(ADXL345_INT_ACTIVITY_BIT, 0); 
  acc.setInterrupt(ADXL345_INT_INACTIVITY_BIT, 0); 
  acc.setInterrupt(ADXL345_INT_WATERMARK_BIT, 0); 
  acc.setInterrupt(ADXL345_INT_OVERRUNY_BIT, 0); */
  acc.writeTo(0x2E, 0x04);
  acc.setInterruptMapping(ADXL345_INT_FREE_FALL_BIT, ADXL345_INT1_PIN); // Map interrupt to INT1
  acc.setFreeFallThreshold(0x09); // 500mg threshold
  acc.setFreeFallDuration(0x14); // 150ms min fall time

  freefall = acc.getInterruptSource(2);

  delay(5);

  // Register interrupt at Arduino's D2 pin
  attachInterrupt(0, fall, RISING);

  Serial.print("Enable: ");
  Serial.print(acc.isInterruptEnabled(ADXL345_INT_FREE_FALL_BIT));
  Serial.println();
}

void loop() {
  if (freefall) {
    freefall = false;
  }
  int_source = acc.getInterruptSource();
  Serial.println(int_source);

  // Debug
  digitalWrite(13, HIGH);
  delay(timer);
  digitalWrite(13,LOW);
  delay(timer);
}

void fall() {
  timer = 5;
  Serial.println("FALL!\n");
  freefall = sixDOF.acc.getInterruptSource(2);
}

