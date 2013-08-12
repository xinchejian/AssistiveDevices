/* TODO stuff:-

mouse update rate
    - what should it be?
    - what is too slow?
    - issues with interrupts &/or FIFO overflow (how does freeIMU handle overflows?)
    - Teensy vs Xadow. Just have first impression can see each individual Xadow mouse move = fast jumps.
        need to recheck Teensy!

------------------------------------------------------
*** MAIN GOAL ATM = easy use
    - eg easy click button, type onscreen kb, **radio/check boxes**
    **SUCCESS** with step method!!!! ... but does need more people to test.
    Linear method is not there yet!

    NOW - need to work:-
        - on PHYSICAL mounting of sensor, processor & cables
            eg - velcro, or glove for limbs, and maybe headband for head.
        - extensive real life usage = use THIS device as mouse FULL TIME & and on different body locations!

 2nd goal - MANY MOUSE MODES - because these give user different PHYSICAL MOVEMENTS for mouse control
            - tilt(gyro),
            - trad mouse move but in air = linear movement (accel TRY REDUCED G RANGE &/OR ADD MOVING AV)
            - QUATERNIONS (W,X,Y,Z) =SO JUST TRY X,Y OR Z,Y PARTS ONLY!!!!!
------------------------------------------------------


------------------------------------------------------
computer GUI for every operating system to allow configuration changes
    ====>>> see seperate TAB above for "User_setup_and_configuration" that discussess this!
------------------------------------------------------

------------------------------------------------------
Code freezing - comms issue:-
   ... it is waaaaaaaay better that Arduino I2C
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

OTHER goals & to do:-
- try offset to cfg sensor & body alignment
- TEST on wrist, arm, head
- some videos of device on finger, wrist, arm, head AND screen captures
- gesture/tap/orientation OR accel to act as switches/cmds
    .. freeIMU tap sample - think about using abs(accel), and exploring dif axis as well!
    BUT FIRST look into jRowberg I2Cdev interupt tap & shake!
- use as kb to send keys eg open browser, email, gazetalk etc, OR adj muse cfg....

- MOUSE UPDATE RATE - review recent notes, check current code/Teensy RATE, check MMU sensor cfg rate (in cfg AND via log to memory, then dump to serial when mouse OFF. LOOKING TO SEE IF DATA REAL TIME, OR LAGGING/BUNCHED UP DUE TO BUFFERING **AND** IF RATE TOO SLOW/FAST
    - from watchdog LED - 2000 loops is about 3 seconds, so about 666 loops/second WITH CURRENT CODE!


More buttons, double and maybe triple clicking

------------------------------------------------------
EASY & ready to hand way en/disable is IMPORTANT
   .... can't hold your finger/hand/etc still for long times!
   button is OK - just need right size & physical layout
   maybe add timer on movement to auto en/disable
   ??? instead of mouse + en/disable buttons
   an alternate way might be small thumb joystick - equiv 5 buttons
   does need to be very small to fit in between thumb & forefinger

   Prob ALSO still need switch/gesture control of en/disable
   as some people may wish to use this with a 'normal' keyboard,
   or other situations not yet considered!
------------------------------------------------------

------------------------------------------------------
*** test head mouse - may need try dif axis - y!!!
    dif ypr data sets (head mount) - eg yp instead of current pr
    more data processing (more steps, non-linear etc)
    auto en/disable based on timeouts & maybe moving/not-moving interupts
    .....
------------------------------------------------------

------------------------------------------------------
>>>>Check what freeIMU lib does if IMU buffer over-runs!!!!!!
.. eg if add delays to main loop to control mouse update rate!
or if serial console debug printing slows things down...

freeIMU has doxygen docs in lib folder

.. and how to use interrupts gestures & no/motion detection, orientation etc
    .. tap detection - sample code supplied
tap sample ONLY uses raw_values[0], just reads - not using interrupt
freeIMU raw_values structure:
      raw_values[0] = ax;    // acceleration x
      raw_values[1] = ay;    // acceleration y
      raw_values[2] = az;    // acceleration z
      raw_values[3] = gx;    // gyro x
      raw_values[4] = gy;    // gyro y
      raw_values[5] = gz;    // gyro z
    magn.getValues(&raw_values[6], &raw_values[7], &raw_values[8]);
    raw_values[9] = temp;      ie temperature
    raw_values[10] = press;    ie pressure

Tap works nicely - gentle & harder taps are detected
if you try to shake device - need a VERY hard shake before is detected as tap = nice!




????? thinking
use #includes - so can switch between jeff/freeIMU AND keep main code (mostly) same???
OR other way round - sep code for each BUT include main code :) - unconventional & possibly confusing!
becasue really want to explore jeffs full(er) data/feature access - eg (no?)/motion detect, tap, orientation...


------------------------------------------------------

------------------------------------------------------
- explore other sensors - eg gyro only - looking at camera replacement parts - with gyro on PFC!

- multiple sensor data types/averaging/...
        for accelerometer - strong recomendation is to use Kalman filters
	    -but watch out every bit of code specific to some circumstance - UNDERSTAND
	    - and hard to cfg
	    - suggested run prog, dumps data to spreadsheet, analyse, refine & update code params, ......
------------------------------------------------------




IGNORE FOR NOW!!!!!
	- try mouse on = auto center cursor - just testing if OK - add sep switch/action to trigger
		  - CAN'T DO easliy from Arduino Mouse cmd - only relative
		  - o/s mouse cfg, eg press ctl to highlight mouse, or some other o/s API.......


*/

