// re validate board LED pin#'s!!!!!1

//TODO tidy debug stuff - MAYBE KEEP MOST - but as functions in seperate file
// change REMAINING debug prints to MACROs

// on wiki doc UPDATE Ard GUI install ( use swarm) + exact libraries AND teh wprogram/Arduino.h issue with bounce
// this happened 2 x ~ 14 & 16/8  - both on Windows??? - 2nd one was def!


/* Introduction     REVIEW - HOW MUCH HERE - VS THE WIKI!!!!!!!
     Xinchejian Hackerspace Shanghai Assistive Devices project aims to create
 many assistive devices that are easily customised to an individuals needs.
 Assistive Devices are anything, physical, electronic or software to help people accomplish everyday tasks.

 All information including assembly details are at: http://wiki.xinchejian.com/wiki/Assistive_Devices

 The projects are open source and everyone is encourage to contribute to the hardware, the software,
 the documentation (in many languages) and especially in using the devices.

 This sub-project aims to create an "Air mouse" or "wand" that is:-
 - small, lightweight
 - can be used by different parts of the body,
        including finger, wrist, arm, head, foot.
 - be highly configurable and customisable to meet an indivuals needs
 - relatively cheap and easy to get all of the parts

 It uses gyros and accelerometers to create an air-mouse, or wand to control a computers mouse.

While there are commercial devices for this purpose, but they tend to be less configurable.
This project aims to give this device MANY more features than most (all?) current commercial devices!

*/

// TODO - review HERE & user config - terms same and ALL included - nothing missing frm EITHER section
//?? merge the two sections? --- or keep this one as overview/intro
/* Air mouse capabilites / actions -  "what can you do with it?" - "How to use it?" - for a USER to read and understand!!!!!
ie they need to know how to use every feature!

 General how to info:
    - Sensor has to be aligned correctly with your body.
        eg for finger/wrist/arm mouse align marker to point at tip of limb.
    - Initially, try to aim or point the sensor at computer screen.
    - For head mouse ... to be sorted out :)
    - Your mounting method (how you "wear" the sensor) should consider this and especially user comfort.

Configuration / customisation
    Note MANY features are optional and can be turned on/off in the code
    - see "USER CONFIGURATION" section
    - in (near?) future this will be done using a computer user configuration application

    Don't forget your operating system ALSO allows configuration of mouse & button behaviour.
    And if you are using any assistive software, it may also help adjust to meet indidvuals needs.

Mouse movement control
    - Rotational control: - rotate wrist for horizontal mouse control, tilt up/down for vertical mouse control
    - TODO needs to be added! Traditional linear mouse control using the air mouse
    - more - eg keyboard arrow control, but using real or virtual switches???

Mouse button control
    - currently a simple short horizontal shake = mouse left button down.
    - if installed & enabled used the physical switches as mouse buttons

Additional actions
    - a longer shake than "Left Mouse Button down" will alternately enable or diable mouse movement control
    - looking to add a LOT more tap/shake/gesture etc actions
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
// It is organised into three sections - Hardware selection, Capability selection and Configuration

//TODO make sure ALL user content written in user language
//    especially variable names, so gyro = nonsense - convert to WristRotation.....
// and ALL the descriptive comments as well!!!!
// all all the 3defines......
// ** ESPECIAL CARE TO SEPERATE OUT **GROUPS** OF movements related to a particular setup
// eg rotation versus linear movement
// maybe this would be easier with some pictures ... and when done in the config app
// that ALSO enable user to test each action and movement range/speed/fine control


// ******* START Hardware selection ********

    // Supported processors/boards
    // Uncomment YOUR processors/board here by removing the "//" at the start of the line
    // MAKE SURE that there is ONLY ONE LINE uncommented (ie only one line without "//" at the start!
        //#define BOARD  1      // Leonardo
        //#define BOARD  2      // Teensy++ 2
        //#define BOARD  3      // TEENSYPP1_V1          // PP = ++, ie A Teensy++ 1
        //#define BOARD  4      // TEENSY2_V1
        //#define BOARD  5      // TEENSY1_V1
        //#define BOARD  6      // TEENSY3_V1
        #define BOARD  7      // Xadow
    // Note to developers: changes in the above need to be reflected in relevent code!


        // Uncomment none, one or both of these to match the switches in your setup
        //#define HAS_ENABLE_SWITCH
        //#define HAS_LEFT_MOUSE_SWITCH
        //TODO - Add right button, scroll wheel equivalent.

        //TODO - ??add sensor selection gyro, accelerometer, combined data, ...

        // Set IO pins used for buttons, if buttons used!
        #ifdef HAS_LEFT_MOUSE_SWITCH
            #define L_MOUSE_BUTTON 7        // skipped a pin because the LED is on pin 6 of many Teensy's
        #endif
        #ifdef HAS_ENABLE_SWITCH
            #define TOGGLE_MOUSE_BUTTON 5
        #endif

// ******* END Hardware selection ********
// ******* START Capability selection ********

    // en/disable simple tap detection
    #define FREEIMU_TAP
// TODO replace FREEIMU_TAP with en/disable EACH tap feature - left button, en/dsiable mouse, ....

// ******* END Capability selection ********

// ******* Configuration ********
    // Variables common to STEPGROWTH
    int WristRotation = 60;             // gyro range is +/- this value.... at least in standard cfg. otherwise can be 0-90-0!!!!
    int WristTilt = 40;                 // gyro range is +/- this value.... at least in standard cfg. otherwise can be 0-90-0!!!!

    // slow mouse movement for small rotation/movement. faster for bigger
    int stepX = WristRotation / 4;         // roll > step > step move mouse fast, else move slow!
    int stepY = WristTilt / 3;             // pitch > step > step move mouse fast, else move slow!

    // These varables cater for user with different amount of movement in any of the four directions!
    // The first two mapLrgStepLeftX and mapLrgStepRightX are used if current WristRotation > stepX
    // otherwise the matching mapSmlStep values are used.
    // Ditto for stepY and Lrg/Sml variables below.
    int mapLrgStepLeftX   = 5;    // larger numbers may casue mouse cursor to "jump" from place to place!
    int mapLrgStepRightX  = 5;    // 15 might be a good value if user has good fine control
    int mapLrgStepUpY     = 5;
    int mapLrgStepDownY   = 5;

    int mapSmlStepLeftX   = 1;    // 1 is smallest movement. 0 = no movement!
    int mapSmlStepRightX  = 1;    // 3 might be a good value if user has good fine control
    int mapSmlStepUpY     = 1;
    int mapSmlStepDownY   = 1;

    // trying more ways to get finer mouse control
    // TODO - review if required - eg instead control IN THE OPERATING system mouse configuration!!!
    int RotationScaleX = 5;
    int RotationScaleY = 5;


#ifdef FREEIMU_TAP
    // values here are purely empirical
    const int tap_threshold = 10000;        //12000;    in mill-g's??
    const long tap_duration = 22000;        //19375;    in microSeconds
#endif
// ******* END Configuration ********



// END OF USER CONFIGURATION
// End users, you can ignore everything below here, unless you want to improve the capability of this device,
// if so, then please contribute by:-
//      - sending bug reports, updated documentation, translations, or
//      - updating code
// and share with the community (push back to github!)
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

/* STATUS:- working pretty well with step method.
   See code comments & seperate file "ToDo_and_Ideas.ino" for more.
   ** Please write your progress/research notes in "ToDo_and_Ideas.ino" or add MORE files,
   and push often to the github repo, so that EVERYONE can keep up with the current work :) **

   If you are comfortable/capable using git, then consider branching the code to work on features.
*/

#ifdef FREEIMU_TAP
    int raw_values[11];
    char str[512];
    float val[9], q[4];

    unsigned long tap_window_start, time_in_window;
    bool tap_in_window = 0;
#endif


// Mouse will not move from sensor control,
// BUT mouse move commands (0,0,0) still sent - to keep testing realsistic!
//#define DEBUG_FORCE_NO_MOUSE_MOVE

boolean mouseEnabled = false;     // en/disable mouse movement
boolean enablingMouse = false;    // used as part of enable process
boolean leftMouseButtonPressed = false;

// LED flashes slow if mouse control disabled, fast if enabled
// NOT "real" watchdog functionality - but does indicate if still executing main loop :)
int watchDogCounter = 0;
boolean watchDogLED = false;
int watchDogLimit = 0;        // Control how fast LED flashes

/* Board specific stuff, eg LED pin number
this list reproduced here - just for reference!
TO DO add the LED pin #s to the list!
 BOARD  1      Leonardo
 BOARD  2      Teensy++ 2
 BOARD  3      TEENSYPP1_V1          PP = ++, ie A Teensy++ 1
 BOARD  4      TEENSY2_V1
 BOARD  5      TEENSY1_V1
 BOARD  6      TEENSY3_V1
 BOARD  7      Xadow
*/
#if (BOARD == 1) || (BOARD == 6)
    #define LED_PIN 13
#elif (BOARD == 2)|| (BOARD == 3) || (BOARD == 4) || (BOARD == 5)
    #define LED_PIN 6
#elif BOARD == 7
    #define LED_PIN 17          // 17 = red LED, 11 = Green LED
#else
    # error Unknown BOARD #define value!!!!
#endif



#ifdef HAS_LEFT_MOUSE_SWITCH
#include <Bounce.h>  // Has been working, but under windows7, just had to edit bounce.cpp & rename WProgam.h to Arduino.h
// Instantiate a Bounce object with a 5 millisecond debounce time
Bounce deBounceLeft = Bounce( L_MOUSE_BUTTON, 5 );
#endif

#ifdef HAS_ENABLE_SWITCH
#include <Bounce.h>  // Has been working, but under windows7, just had to edit bounce.cpp & rename WProgam.h to Arduino.h
// Instantiate a Bounce object with a 5 millisecond debounce time
Bounce deBounceToggle = Bounce( TOGGLE_MOUSE_BUTTON, 5 );
#endif



int x;        // mouse relative movement, derived from the user selected sensor (gyro, accelerometer, combined data, ...)
int y;        // mouse relative movement


//TODO review this - comments probably nonsense!
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

//TODO test debug!!!!!!!!!!!!!!!!!!!!!!
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

    #ifdef HAS_ENABLE_SWITCH
        pinMode(TOGGLE_MOUSE_BUTTON,INPUT);
    #endif
    #ifdef HAS_LEFT_MOUSE_SWITCH
        pinMode(L_MOUSE_BUTTON,INPUT);
    #endif

// debugging - On Leonardo type boards, does mouse.begin & later mousemove interfere with code uploading?
// Teensy's have difffent behaviour
Serial.begin(115200);
Serial.println("howdy - debug wait for 10 seconds - does mouse.begin & later mousemove interfere with code uploading?") ;
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
//Step 2. Use the data to control mouse movement and switches and any other keyboard etc actions
////////////////////////////////////////////////////////////
    controlMouse();    // Move mouse, click mouse buttons according to button press, or gesture control
}




///////////////////////////////////////////////////////////////////////////////////////////////
// Subroutines and functions below here
///////////////////////////////////////////////////////////////////////////////////////////////

// slow mouse movement for small rotation/tilt. faster for bigger
inline void stepGrowth(){

//TODO - continue refining fine(small) and large movement mouse cursor movement control!
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
