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
 int gravity[3]; // gx gy gz
 FreeSixIMU sixDOF = FreeSixIMU();

 void setup() {
    Serial.begin(57600);
    Wire.begin();

    delay(5);
    sixDOF.init();
    delay(5);
 }

 void loop() {

    // Compute angle from quaternion
    sixDOF.getEuler(angles);

    Serial.print("Euler angles: ");
    Serial.print(angles[0]);
    Serial.print(" | ");
    Serial.print(angles[1]);
    Serial.print(" | ");
    Serial.print(angles[2]);
    Serial.print("\n");

    delay(100);

    // Read raw (converted to float) acceleration value
    sixDOF.acc.readAccel(&gravity[0], &gravity[1], &gravity[2]);
    Serial.print("Acc values: ");
    Serial.print(gravity[0]);
    Serial.print(" | ");
    Serial.print(gravity[1]);
    Serial.print(" | ");
    Serial.print(gravity[2]);
    Serial.print("\n");

    delay(100);
 }
