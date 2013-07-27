/* TODO stuff:-

refactor - get ready for 
    multiple sensor data types/averaging/..., 
    dif data sets (head mount), 
    more data processing (more steps, non-linear etc)
    auto en/disable
    .....

>>>>Check what freeIMU lib does if IMU buffer over-runs!!!!!!
.. eg if add delays to main loop to control mouse update rate!
or if serial console debug printing slows things down...

?/ any sign of freezing (uses SPI - NO not with MMU 6050 - only I2C)????
    WAS 100% reliable for many many minutes :)
    dam - TWO lockups after ~ 1+hours
    cf past notes/ideas on trying to fix/bypass
        + auto try close/reset of sensor, then of Arduino
        ?? watchdog process - for sensor data, for arduino
   research freeIMU - lockup/hanging.....
   switch to MMU 6000(?) & use SPI?????
   ... it is waay better that Arduino I2C

More buttons OR 3/4 clicks to
 - reset cursor to center or highlihgt/flash
 - en/disable
 
*** test head mouse - may need try dif axis - y!!!

	*** MAIN GOAL ATM = easy use - eg easy click button, type onscreen kb, **radio/check boxes**
	- try offset to cfg sensor & body alignement
	- TEST on wrist, arm, head
	2nd goal - MOUSE MODES - tilt(gyro), trad mouse move but in air = linear movement (accel TRY REDUCED G RANGE &/OR ADD MOVING AV), QUATERNIONS (W,X,Y,Z) =SO JUST TRY X,Y OR Z,Y PARTS ONLY!!!!!
	- some videos of device on finger, wrist, arm, head AND screen captures

	OTHER goals - gesture/tap/orientation OR accel to act as switches/cmds
	OHTER goals - use as kb to send keys eg open browser, email, gazetalk etc, OR adj muse cfg....
	MOUSE UPDATE RATE - review recent notes, check current code/Teensy RATE, check MMU sensor cfg rate (in cfg AND via log to memory, then dump to serial when mouse OFF. LOOKING TO SEE IF DATA REAL TIME, OR LAGGING/BUNCHED UP DUE TO BUFFERING **AND** IF RATE TOO SLOW/FAST

IGNORE FOR NOW!!!!!
	- try mouse on = auto center cursor - just testing if OK - add sep switch/action to trigger 
		  - CAN'T DO from Arduino Mouse cmd - only relative
		  - o/s mouse cfg, eg press ctl to highlight mouse, or some other o/s API.......



   seem like EASY & ready to hand way en/disable is IMPORTANT
   .... can't hold your finger/hand/etc still for long times!
   button is OK - just need right size & physical layout
   maybe add timer on movement to auto en/disable
   ??? instead of mouse + en/disable buttons
   an alternate way might be small thumb joystick - equiv 5 buttons
   does need to be very small to fit in between thumb & forefinger
*/

