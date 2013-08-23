/*
    now have watchdogLED and main loop progress LEDs/code in place
    
    
    Behaviours:
        - upload of code does NOT ALWAYS suceed = processor runs code (might seem to upload, & Ard/Teesny gui's say OK, but LEDs NOT reset, run as expected)
        - normal running = LEDs 3,4 on
        - when does reset, run - freezes - LED 3 on, LED 4 sometimes on, sometimes off
        - freezing can occur in < 1 second or > 30 seconds
        - current gryo - mouse code works WAY longer than this!!!!!

CURRENT CODE:
LedOn(3);

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();        // register PU6050_RA_INT_STATUS       0x3A
LedOn(4);

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
LedOn(5);
    } else if (mpuIntStatus & 0x40) {   
LedOn(6);



added 100mS delay at end of loop so that can see ALL LEDs that light up
(possible risk of FIFO overflows)
code immediately run ~ 1 minute = longer than usual ATM!
THEN indicator LEDs stopped changing BUT watchdog kept flashing ... at about watchDogStd speed
*** watchdog STILL flashing MANY minutes later!!!
.. still going 30+ mins later!!!!

made change in wait for int/fifo while loop ... even though this NOT seem to be where issue is ... at least this ONE issue
committed- but not tested as waiting to see how long wdog led goes for!!!

?? is main loop running, but not resettingLEDs, or not getting into code to change each LED or ?????





    
    ??pos issue Teensy sw NOT uploading code, or Teensy NOT receiving new code
    ... have turned on verify + detailed upload info


Updated LEDs used and added one more LED indicator in the code
NOW see that every5/6 loop that LED1 flashes => !dmpReady
Ran some test code - return inside LOOP .... goes back to top of loop!!!
OOPS - NO that is watchDogLED on pin 6!!!!!!



*/





/* PREVIOUS notes extracted from code
Need to understand which settings REQUIRE reset of mmu &/or DMP &/or of some other sub-system (FIFO, ???)
AND highly likely that reset of some sub/systems then resets OTHER settings ... which then need to be set again!
*/

// Code based on (started from) a stripped down version of:
// I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class using DMP (MotionApps v2.0)
// 6/21/2012 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib

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

/*

*/


/******************************************************************************************
// WORKS!!!!!!! 10x2mG, 10ms when drop sensor on desk
// WORKS!!!!!!! 4x2mG, 2ms when SHAKE sensor ... still need adjust - but works!!!!
//BUT have noticed more lockups - at least when using cuteCom serial term!!!!!!!!!!
// Sometimes power off/on fixes, othertimes need re-load code!!!
// maybe look for some keepalive method - like in my other recent code LED etc
// if do sw reset - there are MANY things to reset!!!!!

FIRST drop use of serial - just flash the LED for now!!!!!!


// any way to tell which axis????
// shake vs tap - use raw accel data ... av/max in last "duration" ie setMotionDetectionDuration(duration)??????
*******************************************************************************************/

/*
Motion:
Every milli-second (acceleration > g threshold), duration threshold counter increments.
When duration threshold counter > set threshold, interrupt occurs.

Free Fall, Zero Motion (or no motion) increment duration threshold counter, when (acceleration < g threshold)

 * The Motion interrupt will indicate the axis and polarity of detected motion
 * in MOT_DETECT_STATUS (Register 97).

 
Zero motion

  ALL THREE axis!!!!!

 * Unlike Free Fall or Motion detection, Zero Motion detection triggers an
 * interrupt both when Zero Motion is first detected and when Zero Motion is no
 * longer detected.

 * When a zero motion event is detected, a Zero Motion Status will be indicated
 * in the MOT_DETECT_STATUS register (Register 97). When a motion-to-zero-motion
 * condition is detected, the status bit is set to 1. When a zero-motion-to-
 * motion condition is detected, the status bit is set to 0.
*/



/*
BOTH code sets below THEN, (ie after setting the params) do this:
            resetFIFO();
            setFIFOEnabled(true);
            setDMPEnabled(true);
            resetDMP();


https://github.com/jrowberg/i2cdevlib/blob/master/MSP430/MPU6050/MPU6050_6Axis_MotionApps20.h
setMotionDetectionThreshold(2);
setZeroMotionDetectionThreshold(156);
setMotionDetectionDuration(80);
setZeroMotionDetectionDuration(0);

http://home.eblcom.ch/loewe/Modellbau/Diverses/DemoQuaternion3.txt
            getFIFOBytes(fifoBuffer, fifoCount);
            setMotionDetectionThreshold(2);
            setZeroMotionDetectionThreshold(2);
            setMotionDetectionDuration(80);
            setZeroMotionDetectionDuration(0);
*/

// seems to fail init code 2 with Threshold(6) below, Duration(300) is OK (although no ZeroMotion detected!)!!!!!
//mpu.setZeroMotionDetectionThreshold(6);        //units of 2mg ie Two milli-g's
//mpu.setZeroMotionDetectionDuration(300);         //mS
                                              


//mpu.setMotionDetectionThreshold(2);
//mpu.setZeroMotionDetectionThreshold(156);
//mpu.setMotionDetectionDuration(80);
//mpu.setZeroMotionDetectionDuration(0);




/* freezing
LEDs - prove NO LONGER LOOPING in main loop!
...even though LED flshing v rough ATM

so think about dflt timeout in i2cdev lib ... esp for fast i2c @ 400kHz
    dflt = 1000
    tried 500 - ran for about 40secs
look for clues via i2cdev forum
via 6050 info

dif approach = RTC -> int -> reset.....
?? does teesny uP have onboard watchdog or other hw timer -> int????


still do not know WHAT is issue - mmu, or i2c or teensy or code, ......

???is it a stack overflow - eg if main loop slow (eg due to coms issue) & interrupts pile up.....
***remember aTM may not be reading any data/fifo .... is this causing issue????

is it serial related????  ++ Teensy has to be set as serial + mouse + ....
maybe in part - the connect serail to get started = that while to wait.....
but still fails with ALL serail code commented out!!!!



ATM - related to adding motion int's!!!!!
Seems like have to upload code = reset
**** ALSO have to connect serial term !!!!!!
    (no need to POWER OFF/ON)
>>>>>>RECHECK IF THIS LIB HAS THE COMPASS ETC REMOVED!!!!!!!!!!!!!!!!!!!!!!
NOPE - that must be freeIMU :)

Note also getting occasional other int's, even when NOT enabled
    eg: I2C_MST_INT & PLL_RDY_INT & ZERO MOT_INT...

Without trigering ANY int's - still get lockup
occurs at dif time - eg 10, 17, 30 flashes (with i2cdev lib timeout at 100)
*/


