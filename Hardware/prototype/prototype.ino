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

 const int AVG_SIZE = 10;
 const float COL_THRESH = 2.0;   // Collision threshold
 const float TAP_THRESH = 2.0; // Tap threshold
 const float FAL_THRESH = 0.5;// Free fall threshold
 const int DOUBLE_TAP_TIME_WINDOW = 20; // At least 5 normal value between double tap
 const int FREEFALL_TIME_WINDOW = 10; // At least 5 g_rms ~ 0 is considered free fall
 const int DOUBLE_TAP_TIME_APPART= 5;
 

 // State machine states
 int state; // Current state
 const int EXPECTING = 0;
 const int FFALLING = 1; 
 const int COLLIDED = 2;
 const int TAPPED = 3;
 const int DTAPPED = 4;

 // State machine variable
 int j = 0; // special value counter
 int i = 0; // time appart counter
 

void setup() {
  Serial.begin(57600);
  Wire.begin();

  // Debug
  pinMode(13, OUTPUT);

  delay(5);
  sixDOF.init();
  sixDOF.acc.setRangeSetting(8); // Measure from -8g to +8g
  delay(5);

  state = 0;
  
  delay(100);
}

void loop() {  
  // Get quaternion
  sixDOF.getQ(&quaternion1[0]);
  sixDOF.acc.get_Gxyz(&gravity[0]);
  g_rms = sqrt(gravity[0]*gravity[0] + gravity[1]*gravity[1] + gravity[2]*gravity[2]);
  sixDOF.getQ(&quaternion2[0]);

  switch (state) {
    case EXPECTING:
      digitalWrite(13, HIGH);
      expectingState();
      break;
    case FFALLING:
      digitalWrite(13, LOW);
      ffallState();
      break;
    case COLLIDED:
      digitalWrite(13, LOW);
      if (abs(quaternion1[3] - quaternion2[3]) > 0.05)
        Serial.print("SPIN");
      else
        Serial.println("COLLIDED");
      collideState();
      break;
    case TAPPED:
      digitalWrite(13, LOW);
      tappedState();
      break;
    case DTAPPED:
      digitalWrite(13, LOW);
      Serial.println("DTAP");
      dtappedState();
      break;
    default:
      expectingState();
      break;
  }

  i = (i > 0) ? (i-1) : 0;
  j = (j > 0) ? (j-1) : 0;
}

int expectingState() {
  j = 0;
  if (g_rms > TAP_THRESH) {
    state = TAPPED;
    j = DOUBLE_TAP_TIME_WINDOW;
    i = DOUBLE_TAP_TIME_APPART;
  } else if (g_rms < FAL_THRESH) {
    state = FFALLING;
    j = FREEFALL_TIME_WINDOW;
  } else { 
    state = EXPECTING;
  }
}

int ffallState() {
  if (g_rms > COL_THRESH) {
    if (j == 0) {
      // If falling time is long enough
      state = COLLIDED;
    } else {
      // If falling time is too short
      state = EXPECTING;
      j = 0;
    }
  } else if (g_rms < FAL_THRESH) {
    state = FFALLING;
  } else { 
    // If g_rms is normal
    state = EXPECTING;
    j = 0;
  }
}

int collideState() {
  state = EXPECTING;
  // Stablizing the sensor value
  delay(50);
}

int tappedState() {
  if (g_rms > TAP_THRESH) {
    if (j > 0 && i == 0) {
      state = DTAPPED;
    } else {
      state = EXPECTING;
      j = 0;
    }
  } else if (g_rms < FAL_THRESH) {
    // Hit and fly
    state = FFALLING;
    j = FREEFALL_TIME_WINDOW;
  } else {
    state = TAPPED;
    if (j == 0) {
      Serial.println("TAP");
      state = EXPECTING;
    }
  }
}

int dtappedState() {
  state = EXPECTING;
  // Stablizing the sensor value
  delay(50);
}
