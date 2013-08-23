// methods to record/display most/all/interesting current settings
// Settings include interrupt settings AND other parameters!


/*
>>>>> read/dump ALL initial vals here
doc as I2Cdev MMU6050 defaults!!!!!!!!!
do as generic get values
?? log to serial OR FILE - disables HID, but might be useful.
*/


/* NOTE MPU6050 lib defaults are in std AND in the motion apps code!!!!


std :
void MPU6050::initialize() {
    setClockSource(MPU6050_CLOCK_PLL_XGYRO);
    setFullScaleGyroRange(MPU6050_GYRO_FS_250);
    setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
    setSleepEnabled(false); // thanks to Jack Elston for pointing this one out!
}


motion apps:-
Many/most/all in MPU6050::dmpInitialize()  eg:-
Can see in code, or by turning debug #def on - extract is:-

    setSleepEnabled(false);

    setSlaveAddress(0, 0x7F);
    setI2CMasterModeEnabled(false);
    setSlaveAddress(0, 0x68);

            setClockSource(MPU6050_CLOCK_PLL_ZGYRO);
            setIntEnabled(0x12);                        <<<DMP and FIFO_OFLOW interrupts enabled
            setRate(4); // 1khz / (1 + 4) = 200 Hz
            setExternalFrameSync(MPU6050_EXT_SYNC_TEMP_OUT_L);
            setDLPFMode(MPU6050_DLPF_BW_42);            <<< DLPF bandwidth to 42Hz
            setFullScaleGyroRange(MPU6050_GYRO_FS_2000);    <<< gyro sensitivity to +/- 2000 deg/sec
            setDMPConfig1(0x03);                        <<< DMP configuration bytes (function unknown)
            setDMPConfig2(0x00);                        <<< DMP configuration bytes (function unknown)
            setOTPBankValid(false);
            setXGyroOffsetTC(xgOffsetTC);                <<< X/Y/Z gyro offset TCs read from MMU earlier in init code
            setYGyroOffsetTC(ygOffsetTC);
            setZGyroOffsetTC(zgOffsetTC);

            //setXGyroOffset(0);
            //setYGyroOffset(0);
            //setZGyroOffset(0);

            resetFIFO();

            setMotionDetectionThreshold(2);
            setZeroMotionDetectionThreshold(156);
            setMotionDetectionDuration(80);
            setZeroMotionDetectionDuration(0);

            resetFIFO();
            setFIFOEnabled(true);
            setDMPEnabled(true);
            resetDMP();


            DEBUG_PRINTLN(F("Disabling DMP (you turn it on later)..."));
            setDMPEnabled(false);

            dmpPacketSize = 42;

            DEBUG_PRINTLN(F("Resetting FIFO and clearing INT status one last time..."));
            resetFIFO();
            getIntStatus();



*/

// check current settings against the defaults in std & motion app init code.
void checkDefaults(){
    
    char dfltMsgEnd[] = " data setting";

    Serial.println();
    Serial.println();
    Serial.println("*** Standard data settings ***");

    Serial.print("getClockSource = ");
    Serial.print(mpu.getClockSource());
    if (mpu.getClockSource() == MPU6050_CLOCK_PLL_XGYRO){ 
        Serial.print("  = default");
    }
    else {
        Serial.print("  *** NON DEFAULT (or the DMP has not been initialised yet!)");
    }
    Serial.println(dfltMsgEnd);
    
    Serial.print("getFullScaleGyroRange = ");
    Serial.print(mpu.getFullScaleGyroRange());
    if (mpu.getFullScaleGyroRange() == MPU6050_GYRO_FS_250){ 
        Serial.print("  = default");
    }
    else {
        Serial.print("  *** NON DEFAULT");
    }
    Serial.println(dfltMsgEnd);

    Serial.print("getFullScaleAccelRange = ");
    Serial.print(mpu.getFullScaleAccelRange());
    if (mpu.getFullScaleAccelRange() == MPU6050_ACCEL_FS_2){ 
        Serial.print("  = default");
    }
    else {
        Serial.print("  *** NON DEFAULT");
    }
    Serial.println(dfltMsgEnd);

    Serial.print("getSleepEnabled = ");
    Serial.print(mpu.getSleepEnabled());
    if (mpu.getSleepEnabled() == false){ 
        Serial.print("  = default");
    }
    else {
        Serial.print("  *** NON DEFAULT");
    }
    Serial.println(dfltMsgEnd);


//motion apps
    Serial.println();
    Serial.println();
    Serial.println("motion apps data settings");

    Serial.print("getSleepEnabled = ");
    Serial.print(mpu.getSleepEnabled());
    if (mpu.getSleepEnabled() == false){ 
        Serial.print("  = default");
    }
    else {
        Serial.print("  *** NON DEFAULT");
    }
    Serial.println(dfltMsgEnd);

    Serial.print("INIT code comments about 'weird slave stuff', show just ignore I2c non-default warning(S) below!");
    Serial.print("getSlaveAddress = ");
    Serial.print(mpu.getSlaveAddress(0));
    if (mpu.getSlaveAddress(0) == 0x7F){ 
        Serial.print("  = default");
    }
    else {
        Serial.print("  *** NON DEFAULT");
    }
    Serial.println(dfltMsgEnd);

    Serial.print("getI2CMasterModeEnabled = ");
    Serial.print(mpu.getI2CMasterModeEnabled());
    if (mpu.getI2CMasterModeEnabled() == false){ 
        Serial.print("  = default");
    }
    else {
        Serial.print("  *** NON DEFAULT");
    }
    Serial.println(dfltMsgEnd);

    Serial.print("getSlaveAddress = ");
    Serial.print(mpu.getSlaveAddress(0));
    if (mpu.getSlaveAddress(0) == 0x68){ 
        Serial.print("  = default");
    }
    else {
        Serial.print("  *** NON DEFAULT");
    }
    Serial.println(dfltMsgEnd);






    Serial.print("getClockSource = ");
    Serial.print(mpu.getClockSource());
    if (mpu.getClockSource() == MPU6050_CLOCK_PLL_ZGYRO){ 
        Serial.print("  = default");
    }
    else {
        Serial.print("  *** NON DEFAULT");
    }
    Serial.println(dfltMsgEnd);

     // default is DMP and FIFO_OFLOW interrupts enabled
    Serial.print("getIntEnabled = ");
    Serial.print(mpu.getIntEnabled());
    if (mpu.getIntEnabled() == 0x12){ 
        Serial.print("  = default");
    }
    else {
        Serial.print("  *** NON DEFAULT");
    }
    Serial.println(dfltMsgEnd);

    // 1khz / (1 + 4) = 200 Hz
    Serial.print("getRate = ");
    Serial.print(mpu.getRate());
    if (mpu.getRate() == 4){ 
        Serial.print("  = default");
    }
    else {
        Serial.print("  *** NON DEFAULT");
    }
    Serial.println(dfltMsgEnd);

    Serial.print("getExternalFrameSync = ");
    Serial.print(mpu.getExternalFrameSync());
    if (mpu.getExternalFrameSync() == MPU6050_EXT_SYNC_TEMP_OUT_L){ 
        Serial.print("  = default");
    }
    else {
        Serial.print("  *** NON DEFAULT");
    }
    Serial.println(dfltMsgEnd);

    // DLPF bandwidth to 42Hz
    Serial.print("getDLPFMode = ");
    Serial.print(mpu.getDLPFMode());
    if (mpu.getDLPFMode() == MPU6050_DLPF_BW_42){ 
        Serial.print("  = default");
    }
    else {
        Serial.print("  *** NON DEFAULT");
    }
    Serial.println(dfltMsgEnd);

    // gyro sensitivity to +/- 2000 deg/sec
    Serial.print("getFullScaleGyroRange = ");
    Serial.print(mpu.getFullScaleGyroRange());
    if (mpu.getFullScaleGyroRange() == MPU6050_GYRO_FS_2000){ 
        Serial.print("  = default");
    }
    else {
        Serial.print("  *** NON DEFAULT");
    }
    Serial.println(dfltMsgEnd);

    //DMP configuration bytes (function unknown)
    Serial.print("getDMPConfig1 = ");
    Serial.print(mpu.getDMPConfig1());
    if (mpu.getDMPConfig1() == 0x03){ 
        Serial.print("  = default");
    }
    else {
        Serial.print("  *** NON DEFAULT");
    }
    Serial.println(dfltMsgEnd);

    // DMP configuration bytes (function unknown)
    Serial.print("getDMPConfig2 = ");
    Serial.print(mpu.getDMPConfig2());
    if (mpu.getDMPConfig2() == 0x00){ 
        Serial.print("  = default");
    }
    else {
        Serial.print("  *** NON DEFAULT");
    }
    Serial.println(dfltMsgEnd);

    Serial.print("getOTPBankValid = ");
    Serial.print(mpu.getOTPBankValid());
    if (mpu.getOTPBankValid() == false){ 
        Serial.print("  = default");
    }
    else {
        Serial.print("  *** NON DEFAULT");
    }
    Serial.println(dfltMsgEnd);


Serial.println("Have NOT checked X/Y/Z gyro offsets as do not actually know the default values for these!");

    Serial.print("getMotionDetectionThreshold = ");
    Serial.print(mpu.getMotionDetectionThreshold());
    if (mpu.getMotionDetectionThreshold() == 2){ 
        Serial.print("  = default");
    }
    else {
        Serial.print("  *** NON DEFAULT");
    }
    Serial.println(dfltMsgEnd);

    Serial.print("getZeroMotionDetectionThreshold = ");
    Serial.print(mpu.getZeroMotionDetectionThreshold());
    if (mpu.getZeroMotionDetectionThreshold() == 156){ 
        Serial.print("  = default");
    }
    else {
        Serial.print("  *** NON DEFAULT");
    }
    Serial.println(dfltMsgEnd);

    Serial.print("getMotionDetectionDuration = ");
    Serial.print(mpu.getMotionDetectionDuration());
    if (mpu.getMotionDetectionDuration() == 80){ 
        Serial.print("  = default");
    }
    else {
        Serial.print("  *** NON DEFAULT");
    }
    Serial.println(dfltMsgEnd);

    Serial.print("getZeroMotionDetectionDuration = ");
    Serial.print(mpu.getZeroMotionDetectionDuration());
    if (mpu.getZeroMotionDetectionDuration() == 0){ 
        Serial.print("  = default");
    }
    else {
        Serial.print("  *** NON DEFAULT");
    }
    Serial.println(dfltMsgEnd);



Serial.println("==== END of Default checking ===");
Serial.println("================================");
Serial.println();
}

// just simple way to group/break up info
// most likely does NOT include ALL settings ... eg due to un-documented, or un-wanted, ....
void printAllSettings(){
    printAllMotionSettings();
}


void printAllMotionSettings(){
    
}

