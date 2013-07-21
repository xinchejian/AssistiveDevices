/*
More buttons OR 3/4 clicks to
 - reset cursor to center or highlihgt/flash
 - en/disable
 
 
 ?/ any sign of freezing (uses SPI)????
 
 */

// actually control the mouse - for real!
// For now JUST using button to TOGGLE mouse on/off
boolean move_mouse = false;    // en/disable mouse movement
boolean enable_mouse = false;    // used as part of enable process
// NOT using #def below
//#define MOVE_MOUSE

// Also using Arduino bounce library & some code based on the bounce example code
#define BOUNCE
#ifdef BOUNCE
#include <Bounce.h>

#define TOGGLE_MOUSE_BUTTON 5
#define L_MOUSE_BUTTON 6
#define LED_PIN 6

// Instantiate a Bounce object with a 5 millisecond debounce time
Bounce deBounceToggle = Bounce( TOGGLE_MOUSE_BUTTON, 5 );
Bounce deBounceLeft = Bounce( L_MOUSE_BUTTON, 5 );
#endif



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

#ifdef BOUNCE
    pinMode(TOGGLE_MOUSE_BUTTON,INPUT);
    pinMode(L_MOUSE_BUTTON,INPUT);
    pinMode(LED_PIN,OUTPUT);
#endif
}


void loop() {

    my3IMU.getYawPitchRoll(ypr);

    // scale angles to mouse movements. You can replace 10 with whatever feels adeguate for you.
    // biggere values mean faster movements
    int x = map(ypr[1], -90, 90, -10, 10);
    int y = map(ypr[2], -90, 90, -10, 10);


    // now read & act on mouse switch(es)
#ifdef BOUNCE
    // Process the Toggle mouse switch
    deBounceToggle.update ( );
    // En/disable mouse & LED
    if ( deBounceToggle.read() == HIGH) {
        if (enable_mouse) {
            move_mouse = !move_mouse;    //toggle mouse en/disable
            enable_mouse = false;
            Mouse.release();             // if disableing mouse, ALSO release left button!
            digitalWrite(LED_PIN, move_mouse);
        }
        else {
            digitalWrite(LED_PIN, move_mouse);
        }
    } 
    else {
        enable_mouse = true;    // Setup to enable mouse AFTER switch released - avoids endless toggling 
    }

    // Process left mouse button
    deBounceLeft.update ( );
    if ( deBounceLeft.read() == HIGH) {
        Mouse.release();        // send mouse left button up/release to computer
    }
    else {
        Mouse.press();        // send mouse left button press/down to computer
    }

    // move mouse - if enabled
    if (move_mouse) Mouse.move(-x, y, 0);


#endif
}





