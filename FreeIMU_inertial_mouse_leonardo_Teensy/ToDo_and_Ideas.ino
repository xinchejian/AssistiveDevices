/* TODO stuff:-

mouse update rate
    - what should it be?
    - what is too slow?
    - issues with interrupts &/or FIFO overflow (how does freeIMU handle overflows?)
        - NOTE: Xadow currently running this code WITHOUT interrupt pin connected!
    - Teensy vs Xadow. Just have first impression can see each individual Xadow mouse move = fast jumps.
        need to recheck Teensy!



Example of how to do debug printing/other code
//#define DEBUG
#ifdef DEBUG
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTF(x, y) Serial.print(x, y)
    #define DEBUG_PRINTLN(x) Serial.println(x)
    #define DEBUG_PRINTLNF(x, y) Serial.println(x, y)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTF(x, y)
    #define DEBUG_PRINTLN(x)
    #define DEBUG_PRINTLNF(x, y)
#endif
.... then in code use above macros as required, ie DEBUG_PRINT(x); or DEBUG_PRINT("Hello");



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

