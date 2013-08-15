/* Introduction
     Xinchejian Hackerspace Shanghai Assistive Devices project aims to create
 many assistive devices that are easily customised to an individuals needs.
 Assistive Devices are anything, physical, electronic or software to help people accomplish everyday tasks.
 See http://wiki.xinchejian.com/wiki/Assistive_Devices

 The projects are open source and everyone is encourage to contribute to the hardware, the software,
 the documentation (in many languages) and especially in using the devices.

 This sub-project aims to create:-
 - a smaller, lighter device that can be used by several area of the body,
 including finger, wrist, arm, head, foot.
 - be highly configurable and customisable to meet an indivuals needs
 - relatively cheap and easy to get all of the parts

This program uses a gyros and accelerometers to create an air-mouse, or wand to control a computers mouse.

While there are commercial devices for this purpose, but they tend to be less configurable.
This project aims to give this device MANY more features than most (all?) current commercial devices!

*/

/* Air mouse capabilites / actions -  "what can you do with it?" - "How to use it?" - for a USER to read and understand!!!!!
ie they need to know how to use every feature!

 General how to info:
    - Sensor has to be aligned correctly with your body.
    - Try to aim or point the sensor at teh computer screen.
    - For finger/wrist/arm mouse align marker to point at tip of limb.
    - For head mouse ... to be sorted out :)

Configuration / customisation
    Note MANY features are optional and can be turned on/off in the code - see "USER CONFIGURATION" section
    - in near? future this will be done using a computer user configuration application

    Don't forget your operating system ALSO allows configuration of mouse & button behaviour.
    And if you are using any assistive software, it may also help adjust to meet indidvuals needs.

Mouse movement control
    - Rotational control: - rotate wrist for horizontal mouse control, tilt up/down for vertical mouse control
    - Traditional linear mouse control using the air mouse = TODO needs to be added!
    - more - eg keyboard arrow control, but using real or virtual switches???

Mouse button control
    - currently a simple short horizontal shake = mouse left button down.
    - if installed & enabled used the physical switches as mouse buttons

Additional actions
    - none yet but looking to add a LOT more tap/shake/gesture etc actions
        that can control more mouse buttons/scroll wheel and keyboard, .....

En/disable mouse movement
    - LOONGER shake will toggle the mouse MOVEMENT on/off - at present the virtual button/s are left enabled!

En/disable mouse buttons
    - only via the code at present.

*/


//***************************************************************************************************
//***************************************************************************************************
//***************************************************************************************************
// START OF USER CONFIGURATION
// You can change the variables here to change how the mouse behaves.


//endable simple tap detection
#define FREEIMU_TAP

//TODO make sure ALL user content written in user language
//    especially variable names, so gyro = nonsense - convert to WristRotation.....
// and ALL the descriptive comments as well!!!!
// all all the 3defines......
// ** ESPECIAL CARE TO SEPERATE OUT **GROUPS** OF movements related to a particular setup
// eg rotation versus linear movement
// maybe this would be easier with some pictures ... and when done in the config app
// that ALSO enable user to test each action and movement range/speed/fine control


//TODO make sure all features documented, especially those turned on/off here!
// eg switches, tap/shake.....


// Variables common to STEPGROWTH
int WristRotation = 60;            // gyro range is +/- this value.... at least in standard cfg. otherwise can be 0-90-0!!!!
int WristTilt = 40;            // gyro range is +/- this value.... at least in standard cfg. otherwise can be 0-90-0!!!!

// slow mouse movement for small rotation/movement. faster for bigger

// The parameter(s) below should be user adjustable from COMPUTER based configuration program!
//int step = 15;       // pitch or roll > step > step move mouse fast, else move slow!
int stepX = WristRotation / 4;         // roll > step > step move mouse fast, else move slow!
int stepY = WristTilt / 3;             // pitch > step > step move mouse fast, else move slow!

// These varables cater for user with different amount of movement in any of the four directions!
int mapLrgStepLeftX   = 5;
int mapLrgStepRightX  = 5;
int mapLrgStepUpY     = 5;
int mapLrgStepDownY   = 5;

int mapSmlStepLeftX   = 1;    // 1 is smallest movement. 0 = no movement!
int mapSmlStepRightX  = 1;
int mapSmlStepUpY     = 1;
int mapSmlStepDownY   = 1;

/* original setting used
float mapLrgStepLeftX   = 15;
float mapLrgStepRightX  = 15;
float mapLrgStepUpY     = 15;
float mapLrgStepDownY   = 15;

float mapSmlStepLeftX   = 3;
float mapSmlStepRightX  = 3;
float mapSmlStepUpY     = 3;
float mapSmlStepDownY   = 3;
*/

// trying more ways to get finer mouse control
// TODO - review if required - eg instead control IN THE OPERATING system mouse configuration!!!
int RotationScaleX = 5;
int RotationScaleY = 5;

//TODO maybe set up a #def for each supported board, that then in turn sets the correct LEDs etc for each board.
//Select the pin number of the LED on the microcontroller board
#define LED_PIN 6                // LED on pin 6 for Teensy++ 2, Teensy++ 1, Teensy 2 and Teensy 1.
                                // Pin 13 on Leonardo, Teensy3
                                // Pin 17 or 11 on Xadow

// Uncomment either of both of these if your setup has the matching switch
//#define HAS_ENABLE_SWITCH
//#define HAS_LEFT_MOUSE_SWITCH
//TODO move tap #def/s here as well


//TODO - add sensor selection gyro, accelerometer, combined data, ...



// END OF USER CONFIGURATION
// End users, you can ignore everything below here, unless you want to improve the capability of this device,
// if so, then please contribute to the code and share with the community (push back to github!)
//***************************************************************************************************
//***************************************************************************************************
//***************************************************************************************************










/* Licence(s)

 Credits:-
 Code is based in part on sample & other code from:-
 - Arduino bounce library bounce example code
 - FreeIMU library sample "inertial mouse leonardo", "tap_detection" . author Fabio Varesano - fvaresano@yahoo.it
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

// TODO - add MORE virtual switches/tap/shake/other gestures
// TODO - may need to switch to I2Cdev lib instead of freeIMU
// TODO - add auto en/disable - if no movement (gyro or accel - just one/both?) for a few seconds.


/* Notes & programming info/tips
 Have to use Free-IMU version of I2Cdev library - not one from Jeffs I2Cdev site/SVN
 ?? This lib is BUNDLED with freeIMU - CHECK!!!!

 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
 *** The freeIMU lib has some config TODO INSIDE freeIMU.h
 For hardware in use here - have to uncomment last two #defines (although was working with default commented out!)
 ALSO had to change step var from 5 to 10!


 // 3rd party boards. Please consider donating or buying a FreeIMU board to support this library development.
 //#define SEN_10121 //IMU Digital Combo Board - 6 Degrees of Freedom ITG3200/ADXL345 SEN-10121 http://www.sparkfun.com/products/10121
 //#define SEN_10736 //9 Degrees of Freedom - Razor IMU SEN-10736 http://www.sparkfun.com/products/10736
 //#define SEN_10724 //9 Degrees of Freedom - Sensor Stick SEN-10724 http://www.sparkfun.com/products/10724
 //#define SEN_10183 //9 Degrees of Freedom - Sensor Stick  SEN-10183 http://www.sparkfun.com/products/10183
 //#define ARDUIMU_v3 //  DIYDrones ArduIMU+ V3 http://store.diydrones.com/ArduIMU_V3_p/kt-arduimu-30.htm or https://www.sparkfun.com/products/11055
 #define GEN_MPU6050 // Generic MPU6050 breakout board. Compatible with GY-521, SEN-11028 and other MPU6050 wich have the MPU6050 AD0 pin connected to GND.

 #define DISABLE_MAGN // Uncomment this line to disable the magnetometer in the sensor fusion algorithm

 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

 Starting with ALL the main loop functions as inline so that:-
 - code is readable
 - just in case processing loop time is critical
 - can easily change this either way at any time.
 */


// TODO - hey use the THIRD gyro as another input/switch!!!!
// it could be a natural for point at screen = mouse on, relaxed finger/hand points away = mouse off!





//TO DO some of thse var might need to be user vars - move above!!!!
#ifdef FREEIMU_TAP
int raw_values[11];
char str[512];
float val[9], q[4];

unsigned long tap_window_start, time_in_window;
bool tap_in_window = 0;

// values here are purely empirical
const int tap_threshold = 10000;        //12000;
const long tap_duration = 22000;        //19375;
#endif

// mouse will not move from sensor control,
//BUT mouse move commands (0,0,0) still sent - to keep testing realsistic!
//#define DEBUG_FORCE_NO_MOUSE_MOVE

// For now JUST using button to TOGGLE mouse on/off
boolean mouseEnabled = false;     // en/disable mouse movement
boolean enablingMouse = false;    // used as part of enable process
boolean leftMouseButtonPressed = false;

//TODO - currently LED flashes slow if mouse control disabled, fast if enabled = OK!
//    BUT there is NO watchdog functionality, other than LED flashes if processor is still looping :)
int watchDogCounter = 0;
boolean watchDogLED = false;
int watchDogLimit = 0;        // Control how fast LED flashes


#ifdef HAS_LEFT_MOUSE_SWITCH
#include <Bounce.h>  // Has been working, but under windows7, just had to edit bounce.cpp & rename WProgam.h to Arduino.h
#define L_MOUSE_BUTTON 7        // skipped a pin because the LED is on pin 6
// Instantiate a Bounce object with a 5 millisecond debounce time
Bounce deBounceLeft = Bounce( L_MOUSE_BUTTON, 5 );
#endif

#ifdef HAS_ENABLE_SWITCH
#include <Bounce.h>  // Has been working, but under windows7, just had to edit bounce.cpp & rename WProgam.h to Arduino.h
#define TOGGLE_MOUSE_BUTTON 5
// Instantiate a Bounce object with a 5 millisecond debounce time
Bounce deBounceToggle = Bounce( TOGGLE_MOUSE_BUTTON, 5 );
#endif



int x;        // mouse relative movement, derived from the user selected sensor (gyro, accelerometer, combined data, ...)
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
    pinMode(LED_PIN,OUTPUT);

    Serial.begin(115200);
Serial.println("howdy - debug wait for 10 seconds - does mouse.begin & later mousemove interfere with code uploading?") ;

    // debugging - does mouse.begin & later mousemove interfere with code uploading?
    for (int i = 0; i <20; i++){
        digitalWrite(LED_PIN, LOW);
        delay(250);
        digitalWrite(LED_PIN, HIGH);
        delay(250);
    }
Serial.println("AFTER - debug wait for 10 seconds") ;

    Mouse.begin();

    Wire.begin();

    my3IMU.init(true);                // parameter = "true" says init fast mode = 400KHz I2C

#ifdef FREEIMU_TAP
  my3IMU.accgyro.setFullScaleAccelRange(1); // set accelerometer to 4g range
  delay(10);
#endif

#ifdef HAS_ENABLE_SWITCH
    pinMode(TOGGLE_MOUSE_BUTTON,INPUT);
#endif
#ifdef HAS_LEFT_MOUSE_SWITCH
    pinMode(L_MOUSE_BUTTON,INPUT);
#endif
}


void loop() {
    // this is just for debugging
    watchDogLed();      // ALSO can put any main loop debug stuff here ... just to keep loop looking clean :)

////////////////////////////////////////////////////////////
//Step 1. get sensor data, including from physical switches
////////////////////////////////////////////////////////////
    //hmm - if lockups are due to coms/sensor issue - then ONLY read data when mouse active - will reduce number of lockups
    if (mouseEnabled){
        my3IMU.getYawPitchRoll(ypr);    // read the gyro data

        // Calculate mouse relative movement distances x,y using chosen method
        stepGrowth();          // if user moved a small amount, move mouse slowly, else move faster
    }

    #ifdef HAS_ENABLE_SWITCH
        // current code is ONLY for physical switch, not the virtual tap switch - look into merging tap code!!!
        enableMouseControl();     // Toggle mouse control on/off based on switch or maybe not moving or moving timeout
    #endif

////////////////////////////////////////////////////////////
//Step 2. Use te data to control mouse movement and switches and any other keyboard etc actions
////////////////////////////////////////////////////////////
    controlMouse();    // Move mouse, click mouse buttons according to button press, or gesture control
}




///////////////////////////////////////////////////////////////////////////////////////////////
// Subroutines and functions below here
///////////////////////////////////////////////////////////////////////////////////////////////

// slow mouse movement for small rotation/movement. faster for bigger
inline void stepGrowth(){

//TODO - changes in mapping values do not seem to produce expected mouse movement changes!
// try Serial.print input & output of map commands to see what is going on!
    // Settings used with Xadow
    // If current pitch or roll > step, move mouse fast, else move slow!
    if (abs(ypr[1]) > stepX){
Serial.print("LrgStep X");
        x = map(ypr[1] / RotationScaleX, -WristRotation, WristRotation, -mapLrgStepLeftX, mapLrgStepRightX);                // The +/-15 should be user adjustable from COMPUTER based configuration program!
    }
    else
    {
Serial.print("SmlStep X");
        x = map(ypr[1] / RotationScaleX, -WristRotation, WristRotation, -mapSmlStepLeftX, mapSmlStepRightX);                // The +/-3 should be user adjustable from COMPUTER based configuration program!
    }
    if (abs(ypr[2]) > stepY){
Serial.print("\t\tLrgStep Y");
        y = map(ypr[2] / RotationScaleY, -WristTilt, WristTilt, -mapLrgStepUpY, mapLrgStepDownY);                // The +/-15 should be user adjustable from COMPUTER based configuration program!
    }
    else {
Serial.print("\t\tSmlStep Y");
        y = map(ypr[2] / RotationScaleY, -WristTilt, WristTilt, -mapSmlStepUpY, mapSmlStepDownY);                // The +/-3 should be user adjustable from COMPUTER based configuration program!
    }
Serial.print("ypr[1], x = ");
Serial.print(ypr[1]);
Serial.print(", ");
Serial.print(x);
Serial.print(" ---- ");
Serial.print("ypr[2], y = ");
Serial.print(ypr[2]);
Serial.print(", ");
Serial.println(y);
}


// en/disable ALL mouse control (cursor and buttons) as desired
#ifdef HAS_ENABLE_SWITCH
inline void enableMouseControl(){

    // Process the "Toggle en/disable mouse" switch
    deBounceToggle.update ( );
    // En/disable mouse & LED
    if ( deBounceToggle.read() == HIGH ){
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
#endif

// Control mouse - move mouse cursor and de/select mouse buttons
inline void controlMouse(){

    // Control the mouse LEFT button via a switch { TO BE IMPLEMTENTED:- OR tap, shake or other gesture}
    #ifdef HAS_LEFT_MOUSE_SWITCH
        // ALWAYS Process left mouse button - in case using for debug serial print or something
        deBounceLeft.update ( );
        leftMouseButtonPressed = deBounceLeft.read();

        if ( leftMouseButtonPressed ) {
            Mouse.release();        // send mouse left button up/release to computer
        }
        else {
            Mouse.press();        // send mouse left button press/down to computer
        }
    #else


        // below just forcefully enables if NO enable switch!
        //mouseEnabled=true;

//TODO  - tap/shake should be in #def
        // trying tap for mouse left button control
        if ( updateTapStatus() ) {
            Mouse.press();        // send mouse left button press/down to computer
        }
        else {
            Mouse.release();        // send mouse left button up/release to computer
        }

//Here is a SIMPLE tap/shake method to en/disable mouse control
// Works :)
// You can see if mouse is enabled or not by
//  - see if mouse cursor moves (haha)
//  - watching Arduino LED - flashes 1/2 sec on then 1/2 sec off if mouse disabled, and flashes about 3 per second if enabled.
/*
        //TODO finish tap code here ALSO some overlap between BELOW AND enableMouseControl()!!!
    // Check & ACT on virtual buttons BEFORE checking/moving mouse to avoid unwanted mouse movement!
    if (updateTapStatus()){
        mouseEnabled = !mouseEnabled;   // just toggle for now - MAY NEED TO DO THE ENABLING?WAIT FOR "release" ie stop tapping!!!
        // dumb wating for tapping to stop - prob better to use same code as for the button!
        delay(300);     // prob no need delay when serial printing!
    }
*/


    #endif

    // Move the mouse  - if enabled
    if (mouseEnabled) {
        #ifdef DEBUG_FORCE_NO_MOUSE_MOVE
            Mouse.move(0, 0, 0);    // debugging, don't move mouse, but still send command - want realistic debugging!!!
        #else
            Mouse.move(-x, y, 0);    // 3rd parameter = scroll wheel movement
        #endif // def

    }
}


inline boolean updateTapStatus() {
//TODO bad bad bad currenlty updateTapStatus ALSO trying to en/disable mouse!!!
// when already have enableMouseControl .. although it currently set for SWICTH only
// but have two totally dif approaces.....
  my3IMU.getRawValues(raw_values);
  int a_x = raw_values[0];

  if(a_x > tap_threshold && !tap_in_window) {
    tap_window_start = micros();
    tap_in_window = true;
  }

  if(tap_in_window) {
    time_in_window = micros() - tap_window_start;

    if(a_x < tap_threshold && time_in_window < tap_duration) {
      //sprintf(str, "TAP! time: %dus\n", time_in_window);
      //Serial.print(str);
      tap_in_window = false;
      return true;
    }
    if(time_in_window > tap_duration) { // time exceeded
      tap_in_window = false;
      mouseEnabled = !mouseEnabled;            // use this to toggle mouse dis/enabled! ?? tempory/testing??
                                               // LED flashing speed changes ~1 per sec disabled, ~3/s enabled - see watchDogLimit
/*
      sprintf(str, "TAP! time: %dus, toggling mouse ", time_in_window);
      Serial.print(str);
      Serial.println(mouseEnabled);
     delay(5000);
*/
      return false;
    }
  }
    return false;
}


inline void watchDogLed(){
    if (mouseEnabled)
        watchDogLimit = 150;         //flash faster when mouse enabled.
    else
        watchDogLimit = 500;         //flash slower when mouse DISabled.

    if (watchDogCounter++ > watchDogLimit) {
        digitalWrite(LED_PIN, watchDogLED);    // just toggle LED - over-ride the mouse enble LED use for now
        watchDogCounter = 0;
        watchDogLED = !watchDogLED;
    }

    // for debugging - log some info to serial console WHILE en/disable button pressed
    if (enablingMouse){
        Serial.print(mouseEnabled);
        Serial.print(", ");
        Serial.print(ypr[1]);
        Serial.print(", ");
        Serial.print(ypr[2]);
        Serial.print(", ");
        Serial.print(x);
        Serial.print(", ");
        Serial.print(y);
        Serial.println();
    }
}
