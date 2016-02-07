/* fio_plot.ino
 * Author: Hoang NT
 * Date: 2016/06/02
 * Description: Arduino firmware for sending sensor data
 *              using UART (XBee) communication.
 * Hardware: Arduino Fio, Digi XBee S2, Sparkfun 6DOF
 * Reference: bildr 6DOF tutorial
 */

 #include <Wire.h> // I2C communication
 #include <FreeSixIMU.h>
 #include <FIMU_ADXL345.h>

 float angles[3]; // yaw pitch roll
 float gravity[3]; // gx gy gz
 byte values[10];
 FreeSixIMU sixDOF = FreeSixIMU();

 boolean singleTap = false;
 boolean doubleTap = false;
 char tapType = 0;

 void setup() {
    Serial.begin(57600);
    Wire.begin();

    // Debug
    pinMode(13, OUTPUT);

    delay(5);
    sixDOF.init();
    sixDOF.acc.setRangeSetting(8);
    sixDOF.acc.writeTo(ADXL345_INT_MAP, 0x9F);
    //sixDOF.acc.setTapDetectionOnZ(true);
    sixDOF.acc.writeTo(0x2A, 0x01);
    //sixDOF.acc.setTapDuration(0x10);
    sixDOF.acc.writeTo(0x21,0x10);
    // Set threshold
    sixDOF.acc.writeTo(0x1D,0x38);
    //sixDOF.acc.setTapThreshold(2);
    //sixDOF.acc.setDoubleTapLatency(0x50);
    sixDOF.acc.writeTo(0x22,0x50);
    //sixDOF.acc.setDoubleTapWindow(0xFF);
    sixDOF.acc.writeTo(0x23,0xFF);
    sixDOF.acc.writeTo(ADXL345_INT_ENABLE, 0xE0); // Enable interrupt
    sixDOF.acc.readFrom(0x30,1, &values[0]);
    delay(5);

    attachInterrupt(0, tap, RISING); // Interrupt at D2
 }

 void loop() {

    // Compute angle from quaternion
    sixDOF.getEuler(angles);

    /*
    Serial.print("Euler angles: ");
    Serial.print(angles[0]);
    Serial.print(" | ");
    Serial.print(angles[1]);
    Serial.print(" | ");
    Serial.print(angles[2]);
    Serial.print("\n");
    */
    // delay(100);

    // Read raw (converted to float) acceleration value
    sixDOF.acc.get_Gxyz(&gravity[0]);
    float val = sqrt(gravity[0]*gravity[0] + gravity[1]*gravity[1] + gravity[2]*gravity[2]);
    Serial.print(val);
    Serial.print("\n");

    if (tapType > 0) {
      digitalWrite(13, LOW);
      if (tapType == 1) {
        Serial.println("\nSINGLE\n");
      } else {
        Serial.println("\nDOUBLE\n");
      }
      detachInterrupt(0);
      delay(500);
      attachInterrupt(0, tap, RISING);
      tapType = 0;
    }
    delay(50);
    
 }

 void tap(void) {
    digitalWrite(13, HIGH);
    //sixDOF.acc.readFrom(0x30,1,values);
    //if (values[0] & (1<<5)) tapType = 2;
    //else 
    tapType = 1;
 }

