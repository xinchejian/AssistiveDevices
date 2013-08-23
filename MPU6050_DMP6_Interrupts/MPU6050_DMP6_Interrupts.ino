// Exploring MMU6050 interrupts & the DMP interrupts ....looking to get a tap, gesture, orientation interrupts/data

/*
Current status
    - motion interrupts work ... but:
        - frequent lockups
        - which motion axis - only seems work 10% of time and have not even validated if it gets correct direction yet
    - code is a mess
        - WIP adding stuff to dispaly int settigns & status
        - just commented out ALL remaining serial code ... in case that part of lockup issue
            (to restore reaplace ALL //TEMPserial with serial, AND uncomment LINE while...after teh first //TEMPserial)
        - just added hardware LED watchdog LED & LED for each axis/direction output
            - wtf - pin 6 LED not working - used to in other code!

changed LED use
    1. keep watchDogLed - got pin 6 working
    2. comment out LED use for motion axis indication
    3. make other LEDs show progress through Main Loop, then reset at start,
        then on freeze - can narrow down where!
*/



// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for SparkFun breakout and InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 mpu;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };




#define LED_PIN 6 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

//#define LED_START_PIN 18
//#define LED_COUNT 8                    // *** current PYHSICAL LEDs - missing 2nd LED!
#define LED_START_PIN 5        // remember ON board TEENSTY++ 2 LED = 6 + it flahses inverse to my code!
#define LED_COUNT 26            // *** current PYHSICAL LEDs - missing LED 8!
                                //Teensy++ 2 digital pins go from 0 to 37!!! (includes +8 & +2 internal pins ... ie not around outside of board)
bool ledStates[LED_COUNT];
long ledDurations[2][LED_COUNT];    // [LED duration, current milliSecond count]


const int watchDogReallyFast = 100;
const int watchDogError = 200;
const int watchDogFast = 400;
const int watchDogStd = 700;
const int watchDogSlow = 1400;

long watchDogLedTimer = 0;
long watchDogLedDuration = 700;

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}



// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup() {

    // configure LED pins for output
    pinMode(LED_PIN, OUTPUT);

    for(int i = 0; i < LED_COUNT; i++){
        pinMode(LED_START_PIN + i, OUTPUT);
        ledDurations[0][i] = watchDogFast;    // 0 = always on, any other value = on time in millSeconds
        if (i==0)
            ledDurations[1][i] = 500;               // current period/duration =0
        else
            ledDurations[1][i] = 0;               // current period/duration =0
    }
     allLedsAboveWdogOff();

    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize device
    //TEMPserial.println(F("Initializing I2C devices..."));
    mpu.initialize();


    //add fast LED flahses on fail
    if (mpu.testConnection()){
           // is OK - do nothing :)
    }
    else{
        // error, loop forever & flash LED fast
        while (1) {toggleWatchDogLed(watchDogError);}
    }

    // load and configure the DMP
    //TEMPserial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();


//should these be set BEFORE or After the int is enabled??????
//mpu.setFreefallDetectionThreshold(10);     //units of 2mg ie Two milli-g's
//mpu.setFreefallDetectionDuration(1);        //mS
//mpu.setIntFreefallEnabled(true);



mpu.setMotionDetectionThreshold(1);        //units of 2mg ie Two milli-g's
mpu.setMotionDetectionDuration(2);         //mS



mpu.setIntMotionEnabled(true);
mpu.setIntZeroMotionEnabled(true);            // SAW TWICE ONLY, after moved accel range set before dmp init
/*
mpu.resetFIFO();
mpu.setFIFOEnabled(true);
mpu.setDMPEnabled(true);
mpu.resetDMP();
*/
                                              // ????need to set counters & other config???????
//mpu.setIntFIFOBufferOverflowEnabled(true);
//mpu.setIntI2CMasterEnabled(true);
mpu.setIntDataReadyEnabled(true);


    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // error, loop forever & flash LED fast
        while (1) {toggleWatchDogLed(watchDogError);}
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        //TEMPserial.print(F("DMP Initialization failed (code "));
        //TEMPserial.print(devStatus);
        //TEMPserial.println(F(")"));
    }
}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
toggleLed(0);    // so 1st LED also flashes

LedOn(2);
// LedOn(1); is on pin 6 = in use by watchDog - so avoid!!!!!!
    // if programming failed, don't try to do anything

    if (!dmpReady) {
LedOn(3);
        delay(400);
        allLedsAboveWdogOff();
        delay(4000);
        return;    // Execution of code goes BACK to TOP OF MAIN LOOP!!!!!!!!
    }
    // wait for MPU interrupt or extra packet(s) available
    // WAIT = LOOP HERE "waiting" until interrupt OR until fifoCount >= packetSize
    // ** SO ONLY CODE IN THIS WHILE LOOP EXECUTES WHILE WAITING
    //    *** DO NOT FORGET THIS ***
    while (!mpuInterrupt && fifoCount < packetSize) {
LedOn(4);
        // other program behavior stuff here
        // .
        // if you are really paranoid you can frequently test in between other
        // stuff to see if mpuInterrupt is true, and if so, "break;" from the
        // while() loop to immediately process the MPU data
        // .
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
LedOn(5);
    mpuIntStatus = mpu.getIntStatus();        // register PU6050_RA_INT_STATUS       0x3A
LedOn(6);

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();
LedOn(7);    //....remember missing LED between 7 and 9


// cut paste line here - temp marker

// bracing ir {} this big block - so can collapse/comment it out easily!... well easier to do silly if (1==2)
if (1==2)
{
    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
//for testing HALT PROG, and flash this LED<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        while (1){
            LedOn(9);    //....remember missing LED between 7 and 9
            delay(watchDogError);
            digitalWrite(LED_START_PIN + 9, LOW);
            delay(watchDogError);
        }
    }
    else{
LedOn(0);   // flags CHECKING remaining interrupts (all except FIFO overflow done above = 0x10)



// comment out the block of if's reading the motionStatus - watchdog LED is BACK and code runs without hanging!!!!!
        if (mpuIntStatus & 0x40) {
            //digitalWrite(LED_START_PIN, !digitalRead(LED_START_PIN) );
                LedOn(14);
            if (mpu.getXNegMotionDetected()) {
                    //digitalWrite(LED_START_PIN + 1, !digitalRead(LED_START_PIN + 1) );
                LedOn(15);
                }
            if (mpu.getXPosMotionDetected()) {
                    //digitalWrite(LED_START_PIN + 2,! digitalRead(LED_START_PIN + 2) );
                LedOn(16);
                }
            if (mpu.getYNegMotionDetected()){
                    //digitalWrite(LED_START_PIN + 3, !digitalRead(LED_START_PIN + 3) );
                LedOn(17);
                }
            if( mpu.getYPosMotionDetected()) {
                    //digitalWrite(LED_START_PIN + 4, !digitalRead(LED_START_PIN + 4) );
                LedOn(18);
                }
            if (mpu.getZNegMotionDetected()){
                    //digitalWrite(LED_START_PIN + 5, !digitalRead(LED_START_PIN + 5) );
                LedOn(19);
             }
            if (mpu.getZPosMotionDetected()) {
                    //digitalWrite(LED_START_PIN + 6, !digitalRead(LED_START_PIN + 6) );
                LedOn(20);
            }
            if (mpu.getZeroMotionDetected()) {
                    //digitalWrite(LED_START_PIN + 6, !digitalRead(LED_START_PIN + 6) );
                LedOn(21);
            }
         }
        if (mpuIntStatus & 0x20) {
            //TEMPserial.println("WOOOO HOOOO - got a ZERO MOT_INT!!!!!!!!!!!!!!!!");
                 LedOn(22);
       }
        if (mpuIntStatus & 0x08) {
            //TEMPserial.println("WOOOO HOOOO - got a I2C_MST_INT!!!!!!!!!!!!!!!!");
                LedOn(23);
        }
        if (mpuIntStatus & 0x04) {
            //TEMPserial.println("WOOOO HOOOO - got a PLL_RDY_INT!!!!!!!!!!!!!!!!");
                LedOn(24);
        }
        if (mpuIntStatus & 0x01) {
           // Serial.print("DATA_RDY_INT, ");
        // otherwise, check for DMP data ready interrupt (this should happen frequently)
        }
        if (mpuIntStatus & 0x02) {
                LedOn(25);
            // wait for correct available data length, should be a VERY short wait
            while (fifoCount < packetSize) {
                fifoCount = mpu.getFIFOCount();
                LedOn(26);
            }
            // read a packet from FIFO
            mpu.getFIFOBytes(fifoBuffer, packetSize);
            // track FIFO count here in case there is > 1 packet available
            // (this lets us immediately read more without waiting for an interrupt)
            fifoCount -= packetSize;
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    LedOn(12);
        }
    }
}



/* debugging notes 1
1. make ledON(ledIndex++), reset index at lop of loop. then can easily move these about .... to narrow down the issue
.. after coding 1+ hour with stupid errors - find this = SILLY IDEA - you can know lnger tell EXACTLY where you are in the code
because code branches mean dif # leds are on/off each time around!!!!
.. so just continuing with existing method and moving on/offs around + have more LEDs connected on my test board now!
2. try 6050 lib debug on
3. ditto I2Cdev debug on
4 cf gyro code - is stable, add leds, add delay
5 try without dmp, then look at ALSO without raw..., so only do basic init
6. look at removing / changing which data sets are READ.....
*/

/* debugging notes 2
In this LED cfg
0    - toggles every loop
1    - watchdog
2    - reg flashes
3    - OFF
4    - OFF
5    - reg flashes
6    - reg flashes
7    - flashes slower ~ 1/2 speed of others for ~ 1-2 min, then stops
            LED7 = FIFO overflow & reset
....remember missing LED between 7 and 9
9    - OFF
10    - OFF
11    - OFF
12    - OFF

Lockup 1. LEDS ON = 2, 5, 6
Lockup 2. LEDS ON = 2, 5, 6 - took ~ 5 minutes AFTER LED 7 stopped flashing
Lockup 3. LEDS ON = 2, 5, 6 - did not see timing
Lockup 4. LEDS ON = 2, 5!!!!! - did not see timing, except 2nd stage after 7 stopped was about 15+ minutes
                        5-6 = short code just: mpuIntStatus = mpu.getIntStatus


>>>>>>>UPDATED LED cfg/pos based on above
In this LED cfg
0    - toggles every loop
1    - watchdog
2    - reg flashes
3    - OFF
4    - OFF
5    - reg flashes
6    - reg flashes
7    - reg flashes
....remember missing LED between 7 and 9
9    - flashes slower ~ 1/2 speed of others for ~ 1-2 min, then stops
            LED9 = FIFO overflow & reset
10    - reg flashes
11    - OFF
12    - OFF

Lockup 1. LEDS ON = 2, 5, 6 after ~ 5 mins ... forgot to check if LED 9 ran all time or stopped earlier.
Lockup 2. LEDS ON = 2, 5, 6 after 7min. 9 stopped after ~2-2.5 mins

So prog runs loop
    ... every so often (ie NOT every loop), FIFO gets reset
    then after a while FIFO NO LONGER gets reset
    then after much longer while  prog crashes while executing :     mpuIntStatus = mpu.getIntStatus();        // register PU6050_RA_INT_STATUS       0x3A

going to retest with ledOn(5/6) immediately either side of mpuIntStatus = mpu.getIntStatus();
oops last commit had ledOn(5) moved alread - was just before:

    LedOn(5);

        // reset interrupt flag and get INT_STATUS byte
        mpuInterrupt = false;
        mpuIntStatus = mpu.getIntStatus();        // register PU6050_RA_INT_STATUS       0x3A

Lockup 1. LEDS ON = 2, 5, 6 crashed after 3 mins. This time led9 WAS runnign ENTIRE TIME!!!!
run 2: started 1:20 @1:28 saw #9 stopped - not sure when it stopped but was MOST of this time
    at 1:50 - got tired of waiting, all but#9 still flahsing.

so removed the two delays to make it all run faster adn test if same/similar/?? lockups occur
... in this "mode" - ie no delays, can only see the watchdog LED
started ~1:51 - still going at 14:04
so stopped it - and ran again ... looking to see if consisntent or.....
start 14:05 - I stopped it at 14:09

added ledOn(0) to indicate ALL movements including ZERO movement!
started:14:10 - crashed at 14:11
    ** v dif behaviour - could see led (0) flashing breifly
        **NO WATCHDOG LED!!!!!!!
        LEDS 2, 567 = ON, and 10 & 12 on dimmly & occasional brief flicker off

so commented teh onLed(0); OUT again - back to nice behaviour - only see watchdog LED
uncommented/activated onLed(0) again and ..... same as above then crash after ~ 1 minute
Lockup 1. LEDS ON = 2, 5 - after 1 min
Lockup 2. LEDS ON = 2, 5 after ~20 seconds
3 - uploaded code - nothing, had to do again
Lockup 1. LEDS ON = 2, 5, 6 - after 1 min
=========================================================================================================
?????? BIG PUZZLE - why go from watchdog only to seeing so many LEDs with this little change!!!!!!!!!!!!!
    including **NO WATCHDOG LED!!!!!!!
.. at least lock up code positionS are same!
can see if overflow - LED 9
ELSE NO LED 9, but might get 9, 10 , 11 (forget - already changed code - too lazy to re-check)

updated really bad if else logic.

14:26
had to upload 3 times - now watchdog is back adn is only led!
still running at 14:58


re-added the individual motion axis/direction checks + zero motion
started test around 15:00 to 15:05 - still going 15:36



ADDED more LEDs + test code
... now have issue test code delay NOT working even when set to 10,000
prob is issue with NOT uploading - can see if change# LEDs does not change # LEDs that light up!
hmm after many stuffing arounds and minor tweak and minor bug fixes
it just started working!!!!
pretty sure the code issues NOT related to code not uploading
.. because LEDs were doing the running light thing, just timing was off & NO NEW chnages uploaded!!!
wondering if this is Teensy or even Arduino bug
+ remember earlier the if else issue behaviour did not seem to match code
AND all the upload/whatever it is issues!


renabled motion interrupts

.. getting unstable again seems several dif lockup places.

??time to try a Leonardo, or Xadow!!!!!!

NOT yet - watchdog time not showing ... so prob something else in the code!!!!!!!!!!!!!
.. comment out the block of if's reading the motionStatus - watchdog LED is BACK and code runs without hanging!!!!!


=========================================================================================================


*/


LedOn(10);   //....remember missing LED between 7 and 9
        // blink LED to indicate activity
        toggleWatchDogLed(watchDogStd);
        allLedsAboveWdogOff();
}


////////////////////////////////////////////////////////////////////////////
// sub-routines & functions below here :)
////////////////////////////////////////////////////////////////////////////
inline void toggleWatchDogLed(long duration){
        if ((millis() - watchDogLedTimer) > duration){
            watchDogLedTimer = millis();
            blinkState = !blinkState;
            digitalWrite(LED_PIN, blinkState);
        }
}

// toggle state of LED at LedIndex (ZERO based index!) at duration in ledDurations[0, LedIndex]
// duration = 0 = LED always on.
inline void toggleLed(int LedIndex){
        // if duration is 0, LED always ON, else toggle on/off at end of duration
        if (ledDurations[0][LedIndex] == 0){
                ledStates[LedIndex] = LOW;
                digitalWrite(LED_START_PIN + LedIndex, ledStates[LedIndex]);
        }
        else{
            if ((millis() - ledDurations[1][LedIndex]) > ledDurations[0][LedIndex]){
                ledDurations[1][LedIndex] = millis();                                // store new duration for next time around
                ledStates[LedIndex] = !ledStates[LedIndex];
                digitalWrite(LED_START_PIN + LedIndex, ledStates[LedIndex]);
            }
        }
}

// turn LED at LedIndex (ZERO based index!) ON
inline void LedOn(int LedIndex){
    ledStates[LedIndex] = LOW;
    digitalWrite(LED_START_PIN + LedIndex, ledStates[LedIndex]);
}

// turn LED all LEDs off - EXCEPT the watchdog LED!!!!
inline void allLedsAboveWdogOff(){
    for(int i = 2; i < LED_COUNT; i++){
        digitalWrite(LED_START_PIN + i, HIGH);
        ledStates[i]=HIGH;
    }
}



void temp(){



}

/* debugging notes 3
Summary from dbg notes 2:
code either runs:-
    - watchdog & NO LEDS (maybe too quick to see)
    - OR runs & shows motion PLUS code pos LEDs

fixed watchdog & led0 issue



Issues now see
        - still get freq upload - jsut not working with latest code - even thoguh verify is on!!!
            ??? check latest Teesny app/libs &/or Ard gui ... ALLL beta ATM!!!!
        - lockups - typically led 2, 5, 6 on as is past!
        AND THESE LOCKUPS OCCUR WHEN MOST OF MAIN LOOP REMOVED!!!!

start looking at teensy & arduino issues &/or latest sw versions
OR KEEP STRIPPING FURTHER
+ REMEMBER i2cDEV & PROB THE 7660 LIBS HAVE #DEF DEBUG
+ more notes in 2nd/3rd tab of the code!!!!!

****remember motion settings have been set & int(s?)  ON!!!

prob another sep issue - but also running into NOW!!!!
    = BIG CLUE??? = "check for overflow (this should never happen unless our code is too inefficient)"
SO halt/flag if this occurs and work out WHY code so slow!!!!!
    - delays(), Serial.print, ????






*****ALSO KEEP IN MIND STILL GETTING OCCASIONAL ISSUE WHERE UPLOAD CODE - SAYS OK, & LEDs CHANGE, BUT PROG NOT RUNNING!
pos fix/change re this was use Ard/Teensy menu USB type as disccussed below:
            I had been using the "USB type" as "Serial + Keyboard + Mouse + Joystick". This worked - but I did have trouble connecting via a serial terminal (may not have showed you this step).
            Then recently a day or so ago, as part of looking into my current issue, I tried "USB type" as "Keyboard + Mouse + Joystick" - ie without the "Serial" and that works and the other serial terminal issue seems fixed.
... OR CRASHED REAAAAAALLLLLYYYYY EARLY




with loop ONLY like this:

        void loop() {
        toggleLed(0);    // so 1st LED also flashes

        LedOn(2);
        // LedOn(1); is on pin 6 = in use by watchDog - so avoid!!!!!!
            // if programming failed, don't try to do anything
                allLedsAboveWdogOff();
        }

Led0 = on = correct
watchdog LED toggles FASTER
led2 = off
    1. faster = ~ double speed, about 1 sec for on & off
    2. WTF
    3. led2 = off = OK as turned ON, tehn immediately OFF

.. took two/three tries uploading code to see above.
more uploads - same behaviour


added watchdog line back, now get msg "Please press the RESET BUTTON on your Teensy to upload your sketch.  Auto-reboot only works if the Teensy is running a previous sketch."
even after pressing reset - get same msg.

so  1. remember have pre-release/alpha teensy version
    2. going to close/restart ard gui & teensy loader
        ... still same issue, code not running un/re-plugged, code runs, still wants teensy reset, so reset, SAME
        did just move teensy to usb hub this morn.
        so off hub, to direct port - UPLOADED AND w'dog + led 10 no on and led0 flashing + led 2 on (1/2 intensisty)
    3. get sidetracked by Xadow
*/
