/*

    - eg for mouse sensitivy, adjust orientation/offsets and other config params
    - simplest = use Arduino GUI, change directly in code! .... is OK way, but not so end user freindly
    
    ** Note freeIMU has a CALIBRATION gui/process - check if required in for this project
        ??maybe not - we do not need such high accuracy - just relative direction indication???



============================================================================
============================================================================
============================================================================
============================================================================
Drafting/thinking about Configuration "app"  

IDEAL application = fully integrated into EVERY operating system mouse configuration app!!
Just like touchpads or writing tablets are!!!


  If using Arduino serial terminal/otehr terminal - user may need someone help type in terminal!!!!
  
  keep in mind - later have PC xplat app instead of serial term - ie while coding!


ALSO LOT of coding
so do x-plat app NOW, not later??????
need some x-plat dev env with serial coms lib/ability
qt/CB/????
>>research tools people use with Ard PC gui app
... or just x-plat dev env with serial coms lib/ability 

prob should be carefull with cmds AND data
	- add checs/chckksums
	- use of temp vars while making data changes
	- receive/validdate entire sequnce before committing/changing
	- ???
	- again how do others do this

will need commuicate with ard
	- run desired test (when there is ard code to run)
		- eg mouse on/off/ hor or vert move only
	- update settings

FIRST STEP = SAVE SETTINGS IN ARD MEMORY
.. care with Teensy - use Leonardo compatable method!!!!
?? just ONE set in memory, or several????
AND in setup READ/LOAD settings IF AVAILABLE

An EEPROM write takes 3.3 ms to complete. 
EEPROM memory has life of 100,000 write/erase cycles 
read(addr), write(addr, byte)
so need to manage splitting vars into bytes.....
other ext contributor libs??

Used Progmem_strings for spannerbot
.. only for srings & a bit clunky!
??recent Ard gui builtin ... saw something recently!!!!!!!!
.. but does Teensy support it??
?? search arduino write int to eeprom


in sketchbook see on lib under attiny!
... it seems to cater for many data types

sketchbook\_SORT OUT!\hardware Attiny - lots not compiling\tools\avr\avr\include\avr\eeprom.h

 Copyright (c) 2002, 2003, 2004, 2007 Marek Michalkiewicz
   Copyright (c) 2005, 2006 Bjoern Haase
   Copyright (c) 2008 Atmel Corporation
   Copyright (c) 2008 Wouter van Gulik
....................
 $Id: eeprom.h,v 1.21.2.6 2008/08/19 22:10:39 arcanum Exp $ 












LOTS OF TEXT = WATCH MEMORY ... or is this OK with Teensy (Leonardo??)

Main Menu (Type menu item number IN THE "BOX" and press enter)
 1. Select mouse type  .. ie rotation, linear, ???
 2. Select mouse body position ie ??? & finger, foot, head...
 3. Configure Settings
 4. Save Settings
 5. Load Settings {configuration pc/eeprom/flash (teensy only)}
 0. Exit menu (discards ALL unsaved changes!)


3. Configure Settings Menu (Type menu item number IN THE "BOX" and press enter)
Note: You can re-run any configuration step at any time.
 1. Movement threshold
To adjust movement threshold, air mouse steady for five seconds. 
Press any key or button when ready.
 2. Horizontal range
Move/ROTATE/BUMP/???? your XYZ a comfortable movement that you expect will move the mouse from left to right side of the screen.
Repeat this 5 to 10 times at a comfortable speed.
??The mouse cursor WILL NOT MOVE during this test.
above words KNOWN once mouse type & body location chosen!

Then prog says "HORIZONTAL mouse movement is NOW enabled, so you may test these settings. Please ... same msg as above to move."
Does this movement range comfortable need adjstment?
 3. Vertical range
ditto 2
 4. Test buttons <<<< test or configure??? inconsistent
mouse click speed??? - or is that via o/s
 5. future stuff ...gestures, taps, shakes, orientation, ....
 0. Return to Main menu (discards ALL unsaved changes!)


Prompt user to adjust via their operating system:
mouse click speed
mouse movement speed
mouse cursor visibility & trails, snap to buttons, centere of screen, flash etc
and auto scan & click software

============================================================================
============================================================================
============================================================================
*/
