# Fall 2015 - Project Based Learning - Koike Laboratory - Tokyo Institute of Technology

## Hardware:
* XBee S2 (x2)
* Arduino Fio (x1)
* Sparkfun's 6DOF (x1)
* 3.7 V 320 mAh battery pack (x1)
* XBib-U-Dev Rev 3 (x1)
* SoftSpongeBall (x1)
* Reflextive tape (Polyconic Rectangular Projection.ai laser cut file)

## XBee configuration:
* AFIO: Router configuration.
* PROG: Coordinator configuration.

## Sketches
* fio_blink: simple project to test XBees.
* data_plot: plot acceleration for collision detection.
* gyro_cube: visualize gyro data.
* digi_ball: final implementation for detection collision and spin in the ball game.

## Libaries and Driver
* CDM v2.12.14 WHQL Certified: FTDI Virtual COM Port driver. Modified ftdibus.inf and ftdiport.inf to recognize the old MaxStream XBIB-U-DEV Rev3. Windows recognized this as "MaxStream PKG-U". Need to turn off certificate check for Windows 8 and 10. Set RTS on COM port on close. Baudrate=57600.
* FreeSixIMU: bildr's libarary for Sparkfun's IMU 6DOF. Copy this folder to Documents/Arduino/libraries/.
