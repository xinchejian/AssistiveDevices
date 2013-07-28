/*
     Xinchejian Hackerspace Shanghai Assistive Devices project aims to create
 many assistive devices that are easily customised to an individuals needs.
 Assistive Devices are anything, physical, electronic or software to help people accomplish everyday tasks.
 See http://wiki.xinchejian.com/wiki/Assistive_Devices
 
 This program uses a gyro (and maybe an accelerometer) to create an air-mouse, or wand to control a computing device.
 
 There are commercial devices for this purpose.
 
 This sub-project aims to create:-
 - a smaller, lighter device that can be used by several area of the body, 
 including finger, wrist, arm, head, foot.
 - be highly configurable and customisable to meet and indivuals needs
 - relatively cheap and easy to get all of the parts
 
 Credits:-
 Code is based in part on sample & other code from:-
 - Arduino bounce library bounce example code
 - FreeIMU library sample "inertial mouse leonardo". author Fabio Varesano - fvaresano@yahoo.it
 - Could probably ONLY use Jeff Rowbergs great I2Cdev lib instead of freeIMU (which also uses a different version of Jeff Rowbergs I2Cdev lib)
 - All the direct and many indirect contributers from Xinchejian :)
 
 
 Licence:- 
 Default Xinchejian licence is:-     CC by SA See http://creativecommons.org/licenses/by-sa/2.0/
 
 If this does not suit your needs, then get in touch to discuss.
 
 If the above licence conflicts with licences of contributors code, 
 then either youre gonna have to work it out,
 or go with the spirit of Open Source and sharing,
 or call your bank manager and lawyer!
 */


// STATUS:- working pretty well with step method. 
// See code comments & seperate file "ToDo_and_Ideas.ino" for more.
// ** Please write your progress/research notes in "ToDo_and_Ideas.ino" or add MORE files,
// and push often to the github repo, so that EVERYONE can keep up with the current work :) **
// If you are comfortable/capable using git, then consider branching the code to work on features.


/*********************************************** 
General how to info:
 Sensor has to be aligned correctly with your body.
 For finger/wrist/arm mouse align marker to point at tip of limb.
 For head mouse ... to be sorted out :)
 
 Configuration / customisation
 For now adjust parameters directly in the code, compile and updload.
 Future plan is for computer GUI.
 
 
 Don't forget your operating system ALSO allows configuration of mouse & button behaviour.
 And if you are using any assistive software, it may also help adjust to meet indidvuals needs.
 ***********************************************/

/* Notes & programming info/tips
 Have to use Free-IMU version of I2Cdev library - not one from Jeffs I2Cdev site/SVN
 ?? This lib is BUNDLED with freeIMU - CHECK!!!!
 
 Starting with ALL the main loop functions as inline so that:-
 - code is readable
 - just in case processing loop time is critical
 - can easily change this either way at any time.
 */



// For now JUST using button to TOGGLE mouse on/off
boolean mouseEnabled = false;     // en/disable mouse movement
boolean enablingMouse = false;    // used as part of enable process

int watchDogCounter = 0;
boolean watchDogLED = false;
int watchDogLimit = 0;        // Control how fast LED flashes

//********************************************************************************
// Make sure you only use ONE of the mouse movement methods at a time!!!!!
#define STEPGROWTH  // Use a (currently single) step in mouse movement speed
//#define LINEARGROWTH
//********************************************************************************

// slow mouse movement for small rotation/movement. faster for bigger
#ifdef STEPGROWTH
// The parameter(s) below should be user adjustable from COMPUTER based configuration program!
int step = 5;       // pitch or roll > step > step move mouse fast, else move slow!
#endif

#ifdef LINEARGROWTH
// The parameters below should be user adjustable from COMPUTER based configuration program!
// hmmm think about this & time between updates & thus how far/fast mouse cursor moves
// + operating sys config of mouse behaviour
int mouseMinStepX = 1;        // default for how MINIMUM distance mouse moves on screen every update
int mouseMinStepY = 1;        // default for how MINIMUM distance mouse moves on screen every update
int mouseLinearScaleX = 2;    // default for how MINIMUM distance mouse moves on screen every update
int mouseLinearScaleY = 2;    // default for how MINIMUM distance mouse moves on screen every update
int gyroMinX = 90;            // gyro range is +/- this value.... at least in standard cfg. otherwise can be 0-90-0!!!!
int gyroMinY = 90;            // gyro range is +/- this value.... at least in standard cfg. otherwise can be 0-90-0!!!!
#endif

// for "mouse" switches
#include <Bounce.h>

#define TOGGLE_MOUSE_BUTTON 5
#define L_MOUSE_BUTTON 7        // skipped a pin because the LED is on pin 6
#define LED_PIN 6               // LED on pin 6 for Teensy++ 2

// Instantiate a Bounce object with a 5 millisecond debounce time
Bounce deBounceToggle = Bounce( TOGGLE_MOUSE_BUTTON, 5 );
Bounce deBounceLeft = Bounce( L_MOUSE_BUTTON, 5 );

int x;        // mouse relative movement
int y;        // mouse relative movement



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
    if (mouseEnabled) 
        watchDogLimit = 150;         //flash faster when mouse enabled.
    else
        watchDogLimit = 500;         //flash slower when mouse DISabled.
    
    if (watchDogCounter++ > watchDogLimit) {
            digitalWrite(LED_PIN, watchDogLED);    // just toggle LED - over-ride the mouse enble LED use for now
            watchDogCounter = 0;
            watchDogLED = !watchDogLED;
    }

//hmm - if lockups are due to coms/sensor issue 
//then ONLY read data when mouse active - will reduce number of lockups
    my3IMU.getYawPitchRoll(ypr);    // read the gyro data

    // Calculate mouse relative movement distances x,y using chosen method
    #ifdef STEPGROWTH
        stepGrowth();          // if user moved a small amount, move mouse slowly, else move faster
    #endif
    #ifdef LINEARGROWTH
        linearGrowth();        // dif algorithm to move mouse slow & fast - works but not that good.
    #endif LINEARGROWTH


    enableMouseControl();     // Toggle mouse control on/off based on switch or maybe not moving or moving timeout
    moveMouseAndButtons();    // Click computer mouse buttons according to button press, or gesture control
}




///////////////////////////////////////////////////////////////////////////////////////////////
// Subroutines and functions below here
///////////////////////////////////////////////////////////////////////////////////////////////

// slow mouse movement for small rotation/movement. faster for bigger
#ifdef STEPGROWTH
inline void stepGrowth(){
    // If current pitch or roll > step, move mouse fast, else move slow!
    if ((abs(ypr[1]) > step) || (abs(ypr[2]) > step)){
        x = map(ypr[1], -90, 90, -15, 15);                // The +/-15 should be user adjustable from COMPUTER based configuration program!
        y = map(ypr[2], -90, 90, -15, 15);                // The +/-15 should be user adjustable from COMPUTER based configuration program!
    }
    else {
        x = map(ypr[1], -90, 90, -3, 3);                // The +/-3 should be user adjustable from COMPUTER based configuration program!
        y = map(ypr[2], -90, 90, -3, 3);                // The +/-3 should be user adjustable from COMPUTER based configuration program!
    }
}
#endif STEPGROWTH

// slow mouse movement for small rotation/movement. faster for bigger
// using a linear growth formula - works, but mouse control not that good with current code & parameters!
#ifdef LINEARGROWTH
inline void linearGrowth(){
    int gyroX = ypr[1];
    int gyroY = ypr[2];

    // The parameters below should be user adjustable from COMPUTER based configuration program!
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
}
#endif LINEARGROWTH


inline void enableMouseControl(){
    // Process the "Toggle en/disable mouse" switch
    deBounceToggle.update ( );
    // En/disable mouse & LED
    if ( deBounceToggle.read() == HIGH){
        if ( enablingMouse) {
            mouseEnabled = !mouseEnabled;            //toggle mouse en/disable
            enablingMouse = false;
            //if (!mouseEnabled) Mouse.release();    // if disabling mouse, ALSO release left button!
            //digitalWrite(LED_PIN, mouseEnabled);     // temp disabling this while playing with watchdog LED use
        }
        else {
            //digitalWrite(LED_PIN, mouseEnabled);     // temp disabling this while playing with watchdog LED use
        }
    } 
    else {
        enablingMouse = true;    // Setup to enable mouse AFTER switch released - avoids endless toggling 
    }
}


inline void moveMouseAndButtons(){

Serial.print(mouseEnabled);
Serial.print(", ");

    // Action mouse buttons & movement  - if enabled
    if (mouseEnabled) {
        Mouse.move(-x, y, 0);    // 3rd parameter = scroll wheel movement





/*MOUSE CURSOR CONTROL HAS STOPPED WORKING
mouse button still works & is en/disabled as expected
 cutecom - data is changing
... no change to code, apart form trying to debug - started working
.. took multiple code uploads!!!!
??did I do one power off/on - NOT sure, but thought so

issue mouse on/off via button ok (mouse cursor control work) - BUT watchdog flashing always fast!
re-prog, ditto EXCEPT mouse cursor NOT working (but left mouse button OK)

power off/on
ditto last time -  mouse cursor NOT working (but left mosue button OK)



*/

Serial.print(x);
Serial.print(", ");
Serial.print(y);
Serial.print(", ");
Serial.println();
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



