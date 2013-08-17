//TODO: if still using lastX/Y, only store CALCULATED val, not original val - to save on some processing time!

// enable display of debug info in serial console
#define SIMULATE_MOUSE_IN_SERIAL_CONSOLE

// actually control the mouse - for real!
#define MOVE_MOUSE


// Also using Arduino bounce library & some code based on the bounce example code
#define BOUNCE
#ifdef BOUNCE
#include <Bounce.h>

#define L_MOUSE_BUTTON 5
//#define LED 6 is defined in MPU6050 code as LED_PIN

// Instantiate a Bounce object with a 5 millisecond debounce time
Bounce deBouncer = Bounce( L_MOUSE_BUTTON, 5 );
#endif

/*
// I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class using DMP (MotionApps v2.0)
// 6/21/2012 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     2012-06-21 - added note about Arduino 1.0.1 + Leonardo compatibility error
//     2012-06-20 - improved FIFO overflow handling and simplified read process
//     2012-06-19 - completely rearranged DMP initialization code and simplification
//     2012-06-13 - pull gyro and accel data from FIFO packet instead of reading directly
//     2012-06-09 - fix broken FIFO read sequence and change interrupt detection to RISING
//     2012-06-05 - add gravity-compensated initial reference frame acceleration output
//                - add 3D math helper file to DMP6 example sketch
//                - add Euler output and Yaw/Pitch/Roll output formats
//     2012-06-04 - remove accel offset clearing for better results (thanks Sungon Lee)
//     2012-06-01 - fixed gyro sensitivity to be 2000 deg/sec instead of 250
//     2012-05-30 - basic DMP initialization working
*/

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2012 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
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

/* =========================================================================
   NOTE: In addition to connection 3.3v, GND, SDA, and SCL, this sketch
   depends on the MPU-6050's INT pin being connected to the Arduino's
   external interrupt #0 pin. On the Arduino Uno and Mega 2560, this is
   digital I/O pin 2.
 * ========================================================================= */

/* =========================================================================
   NOTE: Arduino v1.0.1 with the Leonardo board generates a compile error
   when using Serial.write(buf, len). The Teapot output uses this method.
   The solution requires a modification to the Arduino USBAPI.h file, which
   is fortunately simple, but annoying. This will be fixed in the next IDE
   release. For more info, see these links:

   http://arduino.cc/forum/index.php/topic,109987.0.html
   http://code.google.com/p/arduino/issues/detail?id=958
 * ========================================================================= */

// ONLY USE ONE of these #defs!!!
// for now only starting with OUTPUT_READABLE_REALACCEL and OUTPUT_READABLE_WORLDACCEL
//    for the Assistive wand mouse device

// uncomment "OUTPUT_READABLE_QUATERNION" if you want to see the actual
// quaternion components in a [w, x, y, z] format (not best for parsing
// on a remote host such as Processing or something though)
//#define OUTPUT_READABLE_QUATERNION

// uncomment "OUTPUT_READABLE_EULER" if you want to see Euler angles
// (in degrees) calculated from the quaternions coming from the FIFO.
// Note that Euler angles suffer from gimbal lock (for more info, see
// http://en.wikipedia.org/wiki/Gimbal_lock)
//#define OUTPUT_READABLE_EULER

// uncomment "OUTPUT_READABLE_YAWPITCHROLL" if you want to see the yaw/
// pitch/roll angles (in degrees) calculated from the quaternions coming
// from the FIFO. Note this also requires gravity vector calculations.
// Also note that yaw/pitch/roll angles suffer from gimbal lock (for
// more info, see: http://en.wikipedia.org/wiki/Gimbal_lock)
//#define OUTPUT_READABLE_YAWPITCHROLL

// uncomment "OUTPUT_READABLE_REALACCEL" if you want to see acceleration
// components with gravity removed. This acceleration reference frame is
// not compensated for orientation, so +X is always +X according to the
// sensor, just without the effects of gravity. If you want acceleration
// compensated for orientation, us OUTPUT_READABLE_WORLDACCEL instead.
#define OUTPUT_READABLE_REALACCEL

// uncomment "OUTPUT_READABLE_WORLDACCEL" if you want to see acceleration
// components with gravity removed and adjusted for the world frame of
// reference (yaw is relative to initial orientation, since no magnetometer
// is present in this case). Could be quite handy in some cases.
//#define OUTPUT_READABLE_WORLDACCEL

// uncomment "OUTPUT_T" if you want output that matches the
// format used for the InvenSense teapot demo
//#define OUTPUT_TEAPOT



#define LED_PIN 6 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

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

// note first attempts using X, Y, Z & lastX etc as acceleration
// later switched to using them as amount to MOVE the mouse!!
	long X;
	long Y;
	long Z;

	long lastX;
	long lastY;
	long lastZ;

	int scaleX;
	int scaleY;
	int scaleZ;

	int MouseIncX;
	int MouseIncY;
	int MouseIncZ;

    bool run = false;

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
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication
    // (115200 chosen because it is required for Teapot Demo output, but it's
    // really up to you depending on your project)
    Serial.begin(115200);
    while (!Serial); // wait for Leonardo enumeration, others continue immediately

    // NOTE: 8MHz or slower host processors, like the Teensy @ 3.3v or Ardunio
    // Pro Mini running at 3.3v, cannot handle this baud rate reliably due to
    // the baud timing being too misaligned with processor ticks. You must use
    // 38400 or slower in these cases, or use some kind of external separate
    // crystal solution for the UART timer.

    // initialize device
    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
/*
    // wait for ready
    Serial.println(F("\nSend any character to begin ASSISTIVE PROTOTYPE wand mouse: "));
    while (Serial.available() && Serial.read()); // empty buffer
    while (!Serial.available());                 // wait for data
    while (Serial.available() && Serial.read()); // empty buffer again
*/
    run = true;


    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

    // configure LED for output
    pinMode(LED_PIN, OUTPUT);

#ifdef BOUNCE
  pinMode(L_MOUSE_BUTTON,INPUT);
//  pinMode(LED,OUTPUT);   // already done via MPU6050 code
#endif

        // *** >>>>>>> should read values & use those here!!!!!
	lastX = 0;
	lastY = 0;
	lastZ = 0;

	scaleX = 1;
	scaleY = 1;
	scaleZ = 1;

	MouseIncX = 0;
	MouseIncY = 0;
	MouseIncZ = 0;
}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
    // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
        // other program behavior stuff here
        // .
        // .
        // .
        // if you are really paranoid you can frequently test in between other
        // stuff to see if mpuInterrupt is true, and if so, "break;" from the
        // while() loop to immediately process the MPU data
        // .
        // .
        // .
    }


    // using run & serial text to allow DISABLING of reading data AND moving the mouse
     while (Serial.available() && Serial.read()){ // empty buffer again
        run = false;
    }
    if (run) {

        //pick ONE of these getData functions!
        getAccel ();
        //getGyro();

        moveMouseRelative(X, Y);
    }

    // now read & act on mouse switch(es)
     #ifdef BOUNCE
     // Update the dedeBouncer
      deBouncer.update ( );

     // Get the update value
     int MouseLeft = deBouncer.read();

     // Turn on or off the LED and mouse click
     if ( MouseLeft == HIGH) {
       Mouse.release();
       digitalWrite(LED_PIN, LOW );
     } else {
        Mouse.press();
        digitalWrite(LED_PIN, HIGH );
     }
    #else
    // blink LED to indicate activity
            blinkState = !blinkState;
            digitalWrite(LED_PIN, blinkState);
    #endif


    // using these - causes FIFO overuns!!!!!
    // so can't try (this simple way) to set mouse poll/frame rate - for now KISS - just run a max speed
    // the sensor sample rate and buffer triggering interrupts will drive the loop speed
    // as well as code execution time
    //delay(8);       // ~ standard mouse polling rate (~125Hz)
    //delay(50);
    //delay(150);
}


inline void getAccel (){
    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        //Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);

        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        // moved ALL get data calls here - checking impact of multiple calls etc
//??        mpu.dmpGetQuaternion(&q, fifoBuffer);
        //mpu.dmpGetEuler(euler, &q);
        //mpu.dmpGetGravity(&gravity, &q);
        //mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

        mpu.dmpGetAccel(&aa, fifoBuffer);
        mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
        mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);

        #ifdef OUTPUT_READABLE_REALACCEL
            // display real acceleration, adjusted to remove gravity

            /*
            Serial.print("areal\t");
            Serial.print(aaReal.x);
            Serial.print("\t");
            Serial.print(aaReal.y);
            Serial.print("\t");
            Serial.println(aaReal.z);
            */
            //Mouse.move((scaleX*aaWorld.x-scaleX*lastX)/20, (scaleY*aaWorld.y-scaleY*lastY)/20); // done this way to increase resolution
            //Mouse.move((scaleX*aaReal.x-scaleX*lastX)/40, (scaleY*aaReal.y-scaleY*lastY)/40); // done this way to increase resolution
            //Mouse.move((scaleX*aaReal.x-scaleX*lastX)/80, (scaleY*aaReal.y-scaleY*lastY)/80); // done this way to increase resolution

            X =(scaleX*aaReal.x-scaleX*lastX)/80;     // first try - using current & last acceleration
            Y =(scaleY*aaReal.y-scaleY*lastY)/80;

            //X = aaReal.x;
            //Y = aaReal.y;

/*
                ?? instead of doing move of (current-last), just detect WHICH direction(s) AND move a standard amount in that dir
                or have several standard amounts/steps or speed/acceleration
                idea is that
                  - user can nudge mouse cursor in one direction
                  - avoid the issue of hand/finger has to return to a start position at some point
                  ** is this only issue when using sensor to measure acceleration
                      alos have teh accel = ZERO issue when at rest AND when MOVING at cosntant speed
                    if measuring angle/rotation, or position or tilt - then avoid above
                  ?? need some delay to help ignore the return movement
                  or use say a double bump to indicate direction, or double then single if still going same way

                ??look into init & orientation of sensor at init - does it need to be aligned - say north???
                or if using compass is there the local angle to compensate
                ... because mouse on screen is going diagonaly!!!!!

                + need to get tap or some other gesture working (hopefully via the sensor first
                - so just read state, rather than have to write own code to track movements & recognise geestures....
*/

            lastX = aaReal.x;
            lastY = aaReal.y;
            lastZ = aaReal.z;
       #endif

        #ifdef OUTPUT_READABLE_WORLDACCEL
            // display initial world-frame acceleration, adjusted to remove gravity
            // and rotated based on known orientation from quaternion
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetAccel(&aa, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
            mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);

            /*
            Serial.print("aworld\t");
            Serial.print(aaWorld.x);
            Serial.print("\t");
            Serial.print(aaWorld.y);
            Serial.print("\t");
            Serial.println(aaWorld.z);
            */

                //Mouse.move((scaleX*aaWorld.x-scaleX*lastX)/20, (scaleY*aaWorld.y-scaleY*lastY)/20); // done this way to increase resolution
                //Mouse.move((scaleX*aaReal.x-scaleX*lastX)/40, (scaleY*aaReal.y-scaleY*lastY)/40); // done this way to increase resolution
            X =(scaleX*aaWorld.x-scaleX*lastX)/20;
            Y =(scaleY*aaWorld.y-scaleY*lastY)/20;

            lastX = aaWorld.x;
            lastY = aaWorld.y;
            lastZ = aaWorld.z;
        #endif

    }
}

inline void getGyro (){
    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        //Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);

        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        // moved ALL get data calls here - checking impact of multiple calls etc
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetEuler(euler, &q);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

        //mpu.dmpGetAccel(&aa, fifoBuffer);
        //mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
        //mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);

        #ifdef OUTPUT_READABLE_EULER
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetEuler(euler, &q);
            Serial.print("euler\t");
            Serial.print(euler[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(euler[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(euler[2] * 180/M_PI);
        #endif

        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            Serial.print("ypr\t");
            Serial.print(ypr[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(ypr[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(ypr[2] * 180/M_PI);

// prob get akward non-linear movement, may need to linearise somehow.....
            X =(ypr[0] * 180/M_PI) - lastX;
            Y =(ypr[1] * 180/M_PI) - lastY;
            Z =(ypr[2] * 180/M_PI) - lastZ;

            lastX = X;
            lastY = Y;
            lastZ = Z;
        #endif

    }
}

inline void moveMouseRelative(long accelX, long accelY){

    //Mouse.move(accelX, accelY); // first try - using current & last acceleration

// look at a min accel - below which NO CHANGE to mouseInc
    int minX = 2;
    int minY = 2;
    int MouseStepX = 1;
    int MouseStepY = 1;

// this attempt KISS
    // Note there is no change to increment if acceleration = 0
    //  ie keep moving the same (accel = 0 means velocity (movement) continues unchanged)

    #ifdef MOVE_MOUSE
        //Mouse.move(MouseIncX, MouseIncY); // first try - using current & last acceleration
        Mouse.move(MouseIncX, 0); // first try - using current & last acceleration
    #endif


    #ifdef SIMULATE_MOUSE_IN_SERIAL_CONSOLE
        Serial.print(accelX);
        Serial.print("\t");
        Serial.print(accelY);
        Serial.print("\t");
    #endif

    if (accelX > minX){
        MouseIncX = MouseStepX;
        #ifdef SIMULATE_MOUSE_IN_SERIAL_CONSOLE
            Serial.print("->");
        #endif
    }
    if (accelX < -minX){
        MouseIncX = -MouseStepX;
        #ifdef SIMULATE_MOUSE_IN_SERIAL_CONSOLE
            Serial.print("<-");
        #endif
    }

    if (accelY > minY){
        MouseIncY = MouseStepY;
        #ifdef SIMULATE_MOUSE_IN_SERIAL_CONSOLE
            Serial.println("^");
        #endif
    }
    if (accelY < -minY){
        MouseIncY = -MouseStepY;
        #ifdef SIMULATE_MOUSE_IN_SERIAL_CONSOLE
            Serial.println("v");
        #endif
    }

    #ifdef SIMULATE_MOUSE_IN_SERIAL_CONSOLE
        // print "." if there are NO X AND Y movements
        if ((accelX <= minX) && (accelX >= -minX) &&
            (accelY <= minY) && (accelY >= -minY)){
            Serial.println(".");
        }
    #endif
}
