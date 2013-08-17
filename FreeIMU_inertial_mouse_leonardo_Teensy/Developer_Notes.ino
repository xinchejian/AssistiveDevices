// 1. The code layout does NOT follow the Arduino guidelines
// using Tools - Auto Format (Control T) will destroy the current layout.

/* Notes & programming info/tips
 Have to use Free-IMU version of I2Cdev library - not one from Jeffs I2Cdev site/SVN
 ?? This lib is BUNDLED with freeIMU - CHECK!!!!

 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
 *** The freeIMU lib has some config TODO INSIDE freeIMU.h
 For hardware in use here - have to uncomment last two #defines (although was working with default commented out!)
 ALSO had to change step var from 5 to 10!


 // 3rd party boards. Please consider donating or buying a FreeIMU board to support this library development.
 //#define SEN_10121 //IMU Digital Combo Board - 6 Degrees of Freedom ITG3200/ADXL345 SEN-10121 http://www.sparkfun.com/products/10121
 //#define SEN_10736 //9 Degrees of Freedom - Razor IMU SEN-10736 http://www.sparkfun.com/products/10736
 //#define SEN_10724 //9 Degrees of Freedom - Sensor Stick SEN-10724 http://www.sparkfun.com/products/10724
 //#define SEN_10183 //9 Degrees of Freedom - Sensor Stick  SEN-10183 http://www.sparkfun.com/products/10183
 //#define ARDUIMU_v3 //  DIYDrones ArduIMU+ V3 http://store.diydrones.com/ArduIMU_V3_p/kt-arduimu-30.htm or https://www.sparkfun.com/products/11055
 #define GEN_MPU6050 // Generic MPU6050 breakout board. Compatible with GY-521, SEN-11028 and other MPU6050 wich have the MPU6050 AD0 pin connected to GND.

 #define DISABLE_MAGN // Uncomment this line to disable the magnetometer in the sensor fusion algorithm

 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

 Starting with ALL the main loop functions as inline so that:-
 - code is readable
 - just in case processing loop time is critical
 - can easily change this either way at any time.
 */



/* git - line endings ... related to wierdness been seeing??
https://help.github.com/articles/dealing-with-line-endings

Git will automatically manage line endings for you if you set the core.autocrlf option. On Windows, you usually want to use true for this setting.
    git config --global core.autocrlf true


Re-normalizing a repository

After you've set the core.autocrlf option and committed a .gitattributes file, you may find that git wants to commit files that you've not modified.
This is because git wants to normalize the line endings for you.
The best way to do this is wipe out your working tree (all the files except the .git directory) and then restore them.
****Make sure you've committed any work before you do this, or it will be lost.


$ git rm --cached -r .
# Remove everything from the index.

$ git reset --hard
# Write both the index and working directory from git's database.

$ git add .
# Prepare to make a commit by staging all the files that will get normalized.
# This is your chance to inspect which files were never normalized. You should
# get lots of messages like: "warning: CRLF will be replaced by LF in file."

$ git commit -m "Normalize line endings"
# Commit
*/
