/*
From memory stability issues with I2Cdev lib & code, not freeIMU (which actually uses dif version {branched?} of I2Cdev!)
    - anything in freeIMU svn/past issues/forums about stability??

2013-08-12 ran 90 minutes OK, with full REAL mouse movement still functionaing at end. Stopped to add new DEBUG_FORCE_NO_MOUSE_MOVE
2013-08-12 20 minites using DEBUG_FORCE_NO_MOUSE_MOVE, watchdog still running. Stopped to test new map variables.


older notes scattered about other code & unreleased test code & notes


Here is one lot of notes - from ToDo_and_Ideas.ino
------------------------------------------------------
Code freezing - comms issue:-
   ... it (ie this gyro code) is waaaaaaaay better that Arduino I2C
   dam - TWO lockups after ~ 1+hours
    cf past notes/ideas on trying to fix/bypass
        + auto try close/reset of sensor, then of Arduino
        ?? watchdog process - for sensor data, for arduino
   switch to MMU 6000(?) & use SPI?????
(MMU 6050 - only uses I2C, does not use SPI)

   **research freeIMU - lockup/hanging.....
        add OTHER I2C sensor - see if that sensor still works when MMU6050/Ard/i2c locked up
        .. figure out if Ard code still running, or running but "blocked" by some code
            ?? will interrupt routine help here - use it to trigger an LED (serial = semi-blocking)??
            extend this to auto reset processor???
                    - watch out Teensy behaviour - wants autoupload code -or can you turn this off?
            ... but what if sensor locked up
                soft reset sensor via code??
                does sensor have reset pin?

           research Arduino watchdog


    When a lockup occurs:-
        - Does watchdog LED still flash?
        - does upload code again fix issue?
        - does processor reset fix issue?
        - do you have to unplug USB/power?
        - what operating system?
        - what processer - Teesny++ 2, ... AND what sensor
        - any serial debug printing running?

        TEsting by spanner888
            o/s = openSuse 12.3 (x8x_64 linux 3.7.10 KDE 4.10.5) and Windows 7
            Teensy++ 2 (TWO Teensy++2 boards) and (TWO sensor boards) generic MPU6050 GY-521

         JUST found freeIMU header file needs config to YOUR hardware
         had to uncomment two these lines:-
             #define GEN_MPU6050 // Generic MPU6050 breakout board. Compatible with GY-521, SEN-11028 and other MPU6050 wich have the MPU6050 AD0 pin connected to GND.
              #define DISABLE_MAGN // Uncomment this line to disable the magnetometer in the sensor fusion algorithm
         These change behaviour of prog - had to change step value from 5 to 10.
         So are/were lockups due to wrong config???

         in previous projects - similar lockups with geniune Arduino Duemonlove & HCM 5... magnetometer & Grove Accelerometer
         using Arduino ANDother I2C libraries

output from speed test sample:
Testing raw reading speed (average on 1024 samples):
--> result: 1029 microseconds .... 1 milliseconds
Testing calibrated reading speed (average on 1024 samples):
--> result: 1298 microseconds .... 1 milliseconds
Testing sensor fusion speed (average on 1024 samples):
--> result: 2478 microseconds .... 2 milliseconds
Looping again..
------------------------------------------------------

*/
