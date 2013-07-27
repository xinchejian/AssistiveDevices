/* TODO stuff:-

------------------------------------------------------
*** MAIN GOAL ATM = easy use 
    - eg easy click button, type onscreen kb, **radio/check boxes**
    **SUCCESS** with step method!!!! ... but does need more people to test.
    Linear method is not there yet!
    
 2nd goal - MANY MOUSE MODES - because these give user different PHYSICAL MOVEMENTS for mouse control
            - tilt(gyro), 
            - trad mouse move but in air = linear movement (accel TRY REDUCED G RANGE &/OR ADD MOVING AV)
            - QUATERNIONS (W,X,Y,Z) =SO JUST TRY X,Y OR Z,Y PARTS ONLY!!!!!
------------------------------------------------------

OTHER goals & to do:-             
- try offset to cfg sensor & body alignment
    .... will also need computer GUI for every opeerating system!!!
        but this can also be used to config mouse sensitivy and other config params
- TEST on wrist, arm, head
- some videos of device on finger, wrist, arm, head AND screen captures
- gesture/tap/orientation OR accel to act as switches/cmds
- use as kb to send keys eg open browser, email, gazetalk etc, OR adj muse cfg....

- MOUSE UPDATE RATE - review recent notes, check current code/Teensy RATE, check MMU sensor cfg rate (in cfg AND via log to memory, then dump to serial when mouse OFF. LOOKING TO SEE IF DATA REAL TIME, OR LAGGING/BUNCHED UP DUE TO BUFFERING **AND** IF RATE TOO SLOW/FAST

More buttons, double and maybe triple clicking

------------------------------------------------------
EASY & ready to hand way en/disable is IMPORTANT
   .... can't hold your finger/hand/etc still for long times!
   button is OK - just need right size & physical layout
   maybe add timer on movement to auto en/disable
   ??? instead of mouse + en/disable buttons
   an alternate way might be small thumb joystick - equiv 5 buttons
   does need to be very small to fit in between thumb & forefinger
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
------------------------------------------------------

------------------------------------------------------
any sign of code freezing?
    WAS 100% reliable for many many minutes :)
   ... it is waaaaaaaay better that Arduino I2C
   dam - TWO lockups after ~ 1+hours
    cf past notes/ideas on trying to fix/bypass
        + auto try close/reset of sensor, then of Arduino
        ?? watchdog process - for sensor data, for arduino
   research freeIMU - lockup/hanging.....
   switch to MMU 6000(?) & use SPI?????
(MMU 6050 - only uses I2C, does not use SPI)
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

