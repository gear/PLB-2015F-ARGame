/* prototype.ino
 * Author: Hoang NT
 * Date: 2016/06/02
 * Description: Arduino firmware for sending sensor data to game.
 * Hardware: Arduino Fio, Digi XBee S2, Sparkfun 6DOF.
 * Reference: ADXL345, FreeSixIMU library by bildr.org, Tutorial 17: Rotations by opengl-tutorial.org
 */

 #include <Wire.h>
 #include <FreeSixIMU.h>

 float quaternion1[4]; // Gyro data 
 float quaternion2[4]; // Gyro data
 float gravity[3]; // Acc data
 float g_rms;
 FreeSixIMU sixDOF = FreeSixIMU();

void setup() {
  Serial.begin(57600);
  Wire.begin();

  // Debug
  pinMode(13, OUTPUT);

  delay(5);
  sixDOF.init();
  sixDOF.acc.setRangeSetting(8); // Measure from -8g to +8g
  delay(5);

}

void loop() {
  // Get quaternion
  sixDOF.getQ(&quaternion1[0]);
  sixDOF.acc.get_Gxyz(&gravity[0]);
  g_rms = sqrt(gravity[0]*gravity[0] + gravity[1]*gravity[1] + gravity[2]*gravity[2]);
  sixDOF.getQ(&quaternion2[0]);
  
  Serial.print(g_rms);
  Serial.print(",");
  // Print spin rate (delta quaternion[3])
  Serial.print(quaternion2[3] - quaternion1[3]);
  Serial.println();
  Serial.flush();
  delay(20);
}














