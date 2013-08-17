// COMPILES WITH UPDATED MMA7660 LIB FOR TRUCHESS I2C ...... BUT EVERYTHING TOTALLY UNTESTED - ESP THE getXYZ()!!!!!!!!!!!!!!!
// 1st tests - results all zero, code never gets past 	while(I2c.available())

/* as part of above ND more general validation
  - switch back to wire lib
  - read and validate reg data
  ... maybe also test mode
  
  esp Sampling Rate Status Read only - 
 
 Active Mode measurement intervals: 1, 2, 4, 8, 16, 32, 64, 120 indicated in AMSR [2:0].
 
    Auto-Wake: Sleep Mode (uses AMSR sample rate in SR [0x08] register)
       = En/DISable via Bit 0 of $04 SRST Sampling Rate Status
       = set rate in B0-B2 of $08 SR
         000 = 120/s
         001 = 64/s
         ....
         111 = 1/s
    Auto-Sleep: Run Mode (uses AWSR sample rate in SR [0x08] register) 
       = En/DISable via Bit 1 of $04 SRST Sampling Rate Status
       = set rate in B3-B4 of $08 SR
         00 = 32 samples/s auto wake mode
         01 = 16 samples/s auto wake mode
         10 = 8 samples/s auto wake mode
         11 = 1 samples/s auto wake mode

  
  maybe tilt status Read only - need to POSITIVELY know orientation!!!!
      Sleep Count, Interrupt Setup, Mode, SR (Auto-Wake/Sleep and Portrait/Landscape samples per seconds and Debounce Filter)
    ... Tap Detection/Debounce, 
*/

/*

Testing:-
EACH Av ~ = current val in steady state
EACH Av moves in sensible way as accel moves
EACH  delta moves in sensible way as accel moves

on screen cursor moves in sensible way as accel moves
... ditto other computers & o/s's
... driver/???

If know TWO accel axis values, does this imply value of THIRD axis - and thus can this be used for testing????
might need multiple values as absolute vals for a given location/sensor may need calibration
.. use that datasheet table???? ... or calc???

..... OR if axis is vertical - then accel = 1 (or raw val =21/whatever) AND OTHER TWO axis = 0
so this gives clear orientation and cal of each axis!!!

?? also use Processing to visualise??? .. although if just focussing on one axis & other two=0, then may not need




p27 APPENDIX B - SENSING DIRECTION
.. can tell orientation of device
- so can use it MORE LIKE A POINTER (point up/down or l/r {tilt?} - NO MOVEMENT, just point!),
        than a mouse (move this much/fast that way in HOR PLANE)
- so can use this for config - eg:-
    - tell user adjust angles to use "pure" x/y
    - or use tilted combo of x/y/z

MMA7660
Configurable Samples per Second from 1 to 120 samples a second
.. so what is cfg??? ... current library AUTO_SLEEP_32 = 32 times/sec

library - accel = val/21, ie x/21, y/21, z/21
... and datasheet p28,9 also same, but as table
++ datasheet shows units as 'g' force (not m2/s etc or ft2/s)
range raw values = +/1 0-27, 27-31 = shaken
27=1.266g


?is tilt status a simple way of controlling?

Edward said someone at XCJ fixed wire lib issues

*/




//http://www.dorkbotpdx.org/blog/paul/el_wire_color

/*
maybe saw other refs to this or other teensy port
FOLLOW UP!!!!!!!!!!!!!!!
http://www.pjrc.com/teensy/projects/component_tester.html


http://www.kickstarter.com/projects/jrowberg/keyglove-wearable-input-device
http://learn.adafruit.com/usb-snes-gamepad
*/

/*****************************************************************************/
//	Code derived from:-

//  Hardware:    Grove - 3-Axis Digital Accelerometer(±1.5g)
//	Author:	 Frankie.Chu
//	Date: 	 Jan 10,2013
//	Version: v0.9b
//	by www.seeedstudio.com
//

//   Teensy becomes a USB mouse and moves the cursor in a triangle
//   You must select Mouse from the "Tools > USB Type" menu
/*******************************************************************************/

#include "RunningAverage.h"

#define USE_WIRE
// tried using #def's to switch between libs.... but guess ard build process includes BOTH libs, going by error msgs
// so manually comment in/out in this spot (below #ifdef works!)
#ifdef USE_WIRE
  #include <Wire.h>
  #include "MMA7660.h"

  MMA7660 accelemeter;

#else
//  #include <I2C.h>
//  #include "MMA7660_I2C.h"
#endif

    RunningAverage runAvX(20);
    RunningAverage runAvY(20);
    RunningAverage runAvZ(20);

#ifdef USE_WIRE
	int8_t x;
	int8_t y;
	int8_t z;
#else
	int x;
	int y;
	int z;
#endif

	long lastX;
	long lastY;
	long lastZ;

	int scaleX;
	int scaleY;
	int scaleZ;

void setup()
{
	accelemeter.init();
	accelemeter.getXYZ(&x,&y,&z);
	lastX = x;
	lastY = y;
	lastZ = z;

	scaleX = 15;
	scaleY = 15;
	scaleZ = 15;

    runAvX.clear(); // explicitly start clean
    runAvY.clear(); // explicitly start clean
    runAvZ.clear(); // explicitly start clean

	//Serial.begin(57600);    // NOT REQUIRED FOR TEENSY!!!! for serial debug messages only
}


void loop()
{
    accelemeter.getXYZ(&x,&y,&z);

    runAvX.addValue(x);
    runAvY.addValue(y);
    runAvZ.addValue(z);
    long ravX = runAvX.getAverage();
    long ravY = runAvY.getAverage();
    long ravZ = runAvZ.getAverage();

    Mouse.move(scaleX*ravX-scaleX*lastX, scaleY*ravY-scaleY*lastY); // done this way to increase resolution

/*
    Serial.print("x/av/scaled dX = ");
    Serial.print(x);
    Serial.print(", ");
    Serial.print(ravX);
    Serial.print(", ");
    Serial.print(scaleX*ravX-scaleX*lastX);
    Serial.print("\ty/av/scaled dY = ");
    Serial.print(y);
    Serial.print(", ");
    Serial.print(ravY);
    Serial.print(", ");
    Serial.print(scaleY*ravY-scaleY*lastY);
    Serial.print("\tz/av/scaled dZ = ");
    Serial.print(z);
    Serial.print(", ");
    Serial.print(ravZ);
    Serial.print(", ");
    Serial.println(scaleZ*ravZ-scaleZ*lastZ);

*/
	//delay(8);       // ~ standard mouse polling rate (~125Hz)
	delay(50);
	//delay(150);

	lastX = ravX;
	lastY = ravY;
	lastZ = ravZ;


/*
    Serial.print("z = ");
    Serial.println(z);

	float ax,ay,az;
	accelemeter.getAcceleration(&ax,&ay,&az);
    Serial.println("accleration of X/Y/Z: ");
	Serial.print(ax);
	Serial.println(" g");
	Serial.print(ay);
	Serial.println(" g");
	Serial.print(az);
	Serial.println(" g");
	Serial.println("*************");
*/
}




/*
http://www.gamermc.com/2012/02/16/how-to-optimize-frame-rates-and-mouse-response/
polling rate deals with how fast that information is transferred from your mouse to the computer.
The default value for most mice is 125hz which transmits with a delay of 8 ms.
Most gaming mice can be set to run up to 1000hz which transmits with a delay of only 1 ms.

*/
