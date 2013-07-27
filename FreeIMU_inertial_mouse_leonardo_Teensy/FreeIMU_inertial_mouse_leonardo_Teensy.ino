// working pretty well with step method



/* 
 Sensor has to be aligned correctly with your body.
 For finger/wrist/arm mouse align marker to point at tip of limb.
 For head mouse ... to be sorted out :)
 
 Configuration / customisation
 
 
 Don't forget your operating system ALSO allows configuration of mouse & button behaviour.
 And if you are using any assistive software, it may also help.\
 */



// Have to use Free-IMU version of I2Cdev library - not one from Jeffs I2Cdev site/SVN


// actually control the mouse - for real!
// For now JUST using button to TOGGLE mouse on/off
boolean mouseEnabled = false;    // en/disable mouse movement
boolean enablingMouse = false;    // used as part of enable process


#define STEPGROWTH
#ifdef STEPGROWTH
int step = 5;       // pitch or roll > step > step move mouse fast, else move slow!
#endif

//#define LINEARGROWTH
#ifdef LINEARGROWTH
// hmmm think about this & time between updates & thus how far/fast mouse cursor moves
// + operating sys config of mouse behaviour
int mouseMinStepX = 1;    // default for how MINIMUM distance mouse moves on screen every update
int mouseMinStepY = 1;    // default for how MINIMUM distance mouse moves on screen every update
int mouseLinearScaleX = 2;    // default for how MINIMUM distance mouse moves on screen every update
int mouseLinearScaleY = 2;    // default for how MINIMUM distance mouse moves on screen every update
int gyroMinX = 90;            // gyro range is +/- this value.... at least in standard cfg. otherwise can be 0-90-0!!!!
int gyroMinY = 90;            // gyro range is +/- this value.... at least in standard cfg. otherwise can be 0-90-0!!!!
#endif

// Also using Arduino bounce library & some code based on the bounce example code
// for "mouse" switches
#include <Bounce.h>

#define TOGGLE_MOUSE_BUTTON 5
#define L_MOUSE_BUTTON 7        // skipped a pin because the LED is on pin 6
#define LED_PIN 6               // LED on pin 6 for Teensy++ 2

// Instantiate a Bounce object with a 5 millisecond debounce time
Bounce deBounceToggle = Bounce( TOGGLE_MOUSE_BUTTON, 5 );
Bounce deBounceLeft = Bounce( L_MOUSE_BUTTON, 5 );



/**
 * Example program for using the FreeIMU connected to an Arduino Leonardo.
 * The program reads sensor data from the FreeIMU, computes the yaw, pitch
 * and roll using the FreeIMU library sensor fusion and use them to move the
 * mouse cursor. The mouse is emulated by the Arduino Leonardo using the Mouse
 * library.
 * 
 * @author Fabio Varesano - fvaresano@yahoo.it
 */

// TESTING - commented out 3 #def's below (ADXL345, bma180, ITG3200) 
// - still compiles & runs - BUT seems a LOT more sensitive - ie larger mouse movement & harder to control
#include <ADXL345.h>
#include <bma180.h>
#include <HMC58X3.h>
#include <ITG3200.h>
#include <MS561101BA.h>
#include <I2Cdev.h>
#include <MPU60X0.h>
#include <EEPROM.h>

//#define DEBUG
#include "DebugUtils.h"
#include "FreeIMU.h"
#include <Wire.h>
#include <SPI.h>

#include <math.h>

float ypr[3]; // yaw pitch roll

// Set the FreeIMU object
FreeIMU my3IMU = FreeIMU();


void setup() {
  Mouse.begin();
  //Serial.begin(115200);
  Wire.begin();

  my3IMU.init(true);

  pinMode(TOGGLE_MOUSE_BUTTON,INPUT);
  pinMode(L_MOUSE_BUTTON,INPUT);
  pinMode(LED_PIN,OUTPUT);
}


void loop() {

  int x;
  int y;
  my3IMU.getYawPitchRoll(ypr);

  //Below works - but would specific hard coded 2/3 levels work better?
  // Then can (user) config to your own preferences!
  // scale angles to mouse movements. You can replace 10 with whatever feels adeguate for you.
  // bigger values mean faster movements
  // Also remember you change config mouse movement via your 
  // operating system in-built features!
  //int x = map(ypr[1], -90, 90, -10, 10);
  //int y = map(ypr[2], -90, 90, -10, 10);

#ifdef STEPGROWTH
  // If last - current pitch or roll > step, move mouse fast, else move slow!
  if ((abs(ypr[1]) > step) || (abs(ypr[2]) > step)){
    x = map(ypr[1], -90, 90, -15, 15);
    y = map(ypr[2], -90, 90, -15, 15);
  }
  else {
    x = map(ypr[1], -90, 90, -3, 3);
    y = map(ypr[2], -90, 90, -3, 3);
  }
  
  delay(10);
#endif


#ifdef LINEARGROWTH
  int gyroX = ypr[1];
  int gyroY = ypr[2];


  int mouseStepX = mouseMinStepX + (gyroX + gyroMinX) * mouseLinearScaleX / gyroMinX;    // adding gyroMinX to shift range to 0 - 180
  int mouseStepY = mouseMinStepY + (gyroY + gyroMinY) * mouseLinearScaleY / gyroMinY;
  Serial.print(mouseStepX);
  Serial.print("\t");
  Serial.print(mouseStepY);
  Serial.print("\t");
  //.. and another way = a x2 or exponential formula
  x = map(gyroX, -gyroMinX, gyroMinX, -mouseStepX, mouseStepX);
  y = map(gyroY, -gyroMinY, gyroMinY, -mouseStepY, mouseStepY);
  Serial.print(x);
  Serial.print("\t");
  Serial.println(y);
#endif LINEARGROWTH


  // Process the "Toggle en/disable mouse" switch
  deBounceToggle.update ( );
  // En/disable mouse & LED
  if ( deBounceToggle.read() == HIGH){
    if ( enablingMouse) {
      mouseEnabled = !mouseEnabled;            //toggle mouse en/disable
      enablingMouse = false;
      //if (!mouseEnabled) Mouse.release();    // if disabling mouse, ALSO release left button!
      digitalWrite(LED_PIN, mouseEnabled);
    }
    else {
      digitalWrite(LED_PIN, mouseEnabled);
    }
  } 
  else {
    enablingMouse = true;    // Setup to enable mouse AFTER switch released - avoids endless toggling 
  }


  // Action mouse buttons & movement  - if enabled
  if (mouseEnabled) {
    Mouse.move(-x, y, 0);    // 3rd parameter = scroll wheel movement

    // Process left mouse button
    deBounceLeft.update ( );
    if ( deBounceLeft.read() == HIGH) {
      Mouse.release();        // send mouse left button up/release to computer
    }
    else {
      Mouse.press();        // send mouse left button press/down to computer
    }

  }
}








