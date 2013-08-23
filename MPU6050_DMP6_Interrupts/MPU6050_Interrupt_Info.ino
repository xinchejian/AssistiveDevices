
void printMPU6050InterruptSettings(){

    Serial.println();
    Serial.println("************************************************************");
    Serial.println("*** Start of setup interrupt info ****************************");
    // put in setup - so see inital values + only want to see once :)
    // INT_PIN_CFG register
    Serial.println("INT_PIN_CFG register 0x37 55dec");
    Serial.print("getInterruptMode = ");           Serial.println ( mpu.getInterruptMode() );
    Serial.print("getInterruptDrive = ");          Serial.println ( mpu.getInterruptDrive() );
    Serial.print("getInterruptLatch = ");          Serial.println ( mpu.getInterruptLatch() );
    Serial.print("getInterruptLatchClear = ");     Serial.println ( mpu.getInterruptLatchClear() );
    Serial.print("getFSyncInterruptLevel = ");     Serial.println ( mpu.getFSyncInterruptLevel() );
    Serial.print("getFSyncInterruptEnabled = ");   Serial.println ( mpu.getFSyncInterruptEnabled() );
    Serial.print("getI2CBypassEnabled = ");        Serial.println ( mpu.getI2CBypassEnabled() );
    Serial.print("getClockOutputEnabled = ");      Serial.println ( mpu.getClockOutputEnabled() );
    Serial.println();

    // INT_ENABLE register 0x38 56dec
    Serial.println("INT_ENABLE register 0x38 56dec - EXCLUDING THE 2 DMP BITS!!!!!");
    // ignoring this byte read - doing each bit seperately below
    //uint8_t getIntEnabled();      // I2Cdev::readByte(devAddr, MPU6050_RA_INT_ENABLE, buffer);
    Serial.print("getIntFreefallEnabled = ");                 Serial.println ( mpu.getIntFreefallEnabled() );
    Serial.print("getIntMotionEnabled = ");                   Serial.println ( mpu.getIntMotionEnabled() );
    Serial.print("getIntZeroMotionEnabled = ");               Serial.println ( mpu.getIntZeroMotionEnabled() );
    Serial.print("getIntFIFOBufferOverflowEnabled = ");       Serial.println ( mpu.getIntFIFOBufferOverflowEnabled() );
    Serial.print("getIntI2CMasterEnabled = ");                Serial.println ( mpu.getIntI2CMasterEnabled() );
    Serial.print("getIntDataReadyEnabled = ");                Serial.println ( mpu.getIntDataReadyEnabled() );
    Serial.println();
    // Note TWO more methods related to this set, but moved below because they are DMP!!!!


    /* DMP specific:
     in setup is mpu.setDMPEnabled(true); ... this is a USER feature, NOT int!!!!!
     // USER_CTRL register (DMP functions)
     bool getDMPEnabled();
     void setDMPEnabled(bool enabled);
     void resetDMP();
     */
    // INT_ENABLE register (DMP functions) 0x38 56dec
    Serial.println("INT_ENABLE register 0x38 56dec - 2 DMP BITS ONLY!!!!!");
    Serial.print("getIntPLLReadyEnabled = ");    Serial.println ( mpu.getIntPLLReadyEnabled() );
    Serial.print("getIntDMPEnabled = ");         Serial.println ( mpu.getIntDMPEnabled() );      // readBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_DMP_INT_BIT, buffer);
    Serial.println();
    /*
        void setIntPLLReadyEnabled(bool enabled);
     >>> note "setInt...." void setIntDMPEnabled(bool enabled);  // writeBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_DMP_INT_BIT, enabled);
     */
    // INT_STATUS register (DMP functions) 0x3A 58dec
    Serial.println("INT_STATUS register (DMP functions) 0x3A 58dec");
    Serial.print("getIntPLLReadyStatus = ");    Serial.println ( mpu.getIntPLLReadyStatus() );
    Serial.print("getIntDMPStatus = ");         Serial.println ( mpu.getIntDMPStatus() );      // readBit(devAddr, MPU6050_RA_INT_STATUS, MPU6050_INTERRUPT_DMP_INT_BIT, buffer);
    Serial.println();

    /*
#define MPU6050_RA_DMP_CFG_1        0x70
     #define MPU6050_RA_DMP_CFG_2        0x71
     
     bool writeDMPConfigurationSet(const uint8_t *data, uint16_t dataSize, bool useProgMem=false);
     bool writeProgDMPConfigurationSet(const uint8_t *data, uint16_t dataSize);
     
     // DMP_CFG_1 register
     uint8_t getDMPConfig1();
     void setDMPConfig1(uint8_t config);
     
     // DMP_CFG_2 register
     uint8_t getDMPConfig2();
     void setDMPConfig2(uint8_t config);
     
     and a BUCKET LOAD UNDER         // special methods for MotionApps 2.0 implementation
     
     **** remember still do not understand
     the MMU6050.h/ccp AND the other two headers
     .. esp as MMU6050.h (&cpp?) has code blocks for those as well (same code?)
     
     and MORE under         // special methods for MotionApps 4.1 implementation
     
     could not see anything about int
     */
    Serial.println("*** End of setup interrupt info ****************************");
    Serial.println("************************************************************");
}




void printMPU6050InterruptStatus(){
    // thinking 1 - print WHAT int occurred
    // thinking 2 - print data for THAT int, in way noted below - last/current 
    // thinking - store values - compare with next time around loop - highlight any changes

        // remember this is in main loop and called AFTER interrupt detected
    // so code on CRITICAL PATH
    // too much Serial.print = FIFO overruns etc!!!!!
    // Serial.print from INT method --- just one "*"???
    // or turn LED ON in int method AND ONLY turn off at end of main loop or printMPU6050InterruptStatus

        Serial.print("==>mpuIntStatus (from reg 0x3A) in HEX & Binary formats, then as per bit data: ");
    Serial.print(mpuIntStatus, HEX);
    Serial.print("\t");
    Serial.println(mpuIntStatus, BIN);

    Serial.print ("IntFreefall = ");     Serial.println (mpuIntStatus & 0x20);
    Serial.print ("IntMotion = ");       Serial.println (mpuIntStatus & 0x10);
    Serial.print ("IntZeroMotion = ");   Serial.println (mpuIntStatus & 0x08);
    Serial.print ("IntFIFOBufferOverflow = "); Serial.println (mpuIntStatus & 0x04);
    Serial.print ("IntI2CMaster = ");    Serial.println (mpuIntStatus & 0x02);
    Serial.print ("IntDataReady = ");    Serial.println (mpuIntStatus & 0x01);
    //Serial.println("MUST carefully check above bit data - may have & masks incorrect!!!!");
    Serial.println();

    // INT_STATUS register (DMP functions) 0x3A 58dec
    Serial.println("DMP INT_STATUS reg 0x3A 58dec");
    Serial.print("IntPLLReady = ");     Serial.println ( mpu.getIntPLLReadyStatus() );
    Serial.print("IntDMP = ");          Serial.println ( mpu.getIntDMPStatus() );      // readBit(devAddr, MPU6050_RA_INT_STATUS, MPU6050_INTERRUPT_DMP_INT_BIT, buffer);
    Serial.println();


    //  ?? Only read these if getIntDMPStatus() = true        
    // DMP_INT_STATUS 0x39 59dec
    Serial.println("DMP_INT_STATUS 0x39 59dec");
    Serial.print("5 = ");     Serial.println ( mpu.getDMPInt5Status() );                        // readBit(devAddr, MPU6050_RA_DMP_INT_STATUS, MPU6050_DMPINT_5_BIT, buffer);
    Serial.print("4 = ");     Serial.println ( mpu.getDMPInt4Status() );                        // readBit(devAddr, MPU6050_RA_DMP_INT_STATUS, MPU6050_DMPINT_4_BIT, buffer);
    Serial.print("3 = ");     Serial.println ( mpu.getDMPInt3Status() );                        // readBit(devAddr, MPU6050_RA_DMP_INT_STATUS, MPU6050_DMPINT_3_BIT, buffer);
    Serial.print("2 = ");     Serial.println ( mpu.getDMPInt2Status() );                        // readBit(devAddr, MPU6050_RA_DMP_INT_STATUS, MPU6050_DMPINT_2_BIT, buffer);
    Serial.print("1 = ");     Serial.println ( mpu.getDMPInt1Status() );                        // readBit(devAddr, MPU6050_RA_DMP_INT_STATUS, MPU6050_DMPINT_1_BIT, buffer);
    Serial.print("0 = ");     Serial.println ( mpu.getDMPInt0Status() );                        // readBit(devAddr, MPU6050_RA_DMP_INT_STATUS, MPU6050_DMPINT_0_BIT, buffer);
    Serial.println();
}




void printOneLineMPU6050InterruptStatus(){

        Serial.print(mpuIntStatus, HEX);
        Serial.print(", ");
        Serial.print(mpuIntStatus, BIN);

        Serial.print (": "); 
        Serial.print (mpuIntStatus & 0x20);
        Serial.print (", "); 
        Serial.print (mpuIntStatus & 0x10);
        Serial.print (", "); 
        Serial.print (mpuIntStatus & 0x08);
        Serial.print (", "); 
        Serial.print (mpuIntStatus & 0x04);
        Serial.print (", "); 
        Serial.print (mpuIntStatus & 0x02);
        Serial.print (", "); 
        Serial.print (mpuIntStatus & 0x01);

    if (mpuIntStatus & 0x02) {
        // INT_STATUS register (DMP functions) 0x3A 58dec
        Serial.print(": "); 
        Serial.print ( mpu.getIntPLLReadyStatus() );
        Serial.print(", "); 
        Serial.print ( mpu.getIntDMPStatus() );      // readBit(devAddr, MPU6050_RA_INT_STATUS, MPU6050_INTERRUPT_DMP_INT_BIT, buffer);

        //  ?? Only read these if getIntDMPStatus() = true        
        // DMP_INT_STATUS 0x39 59dec
        Serial.print(": "); 
        Serial.print ( mpu.getDMPInt5Status() );                        // readBit(devAddr, MPU6050_RA_DMP_INT_STATUS, MPU6050_DMPINT_5_BIT, buffer);
        Serial.print(", "); 
        Serial.print ( mpu.getDMPInt4Status() );                        // readBit(devAddr, MPU6050_RA_DMP_INT_STATUS, MPU6050_DMPINT_4_BIT, buffer);
        Serial.print(", "); 
        Serial.print ( mpu.getDMPInt3Status() );                        // readBit(devAddr, MPU6050_RA_DMP_INT_STATUS, MPU6050_DMPINT_3_BIT, buffer);
        Serial.print(", "); 
        Serial.print ( mpu.getDMPInt2Status() );                        // readBit(devAddr, MPU6050_RA_DMP_INT_STATUS, MPU6050_DMPINT_2_BIT, buffer);
        Serial.print(", "); 
        Serial.print ( mpu.getDMPInt1Status() );                        // readBit(devAddr, MPU6050_RA_DMP_INT_STATUS, MPU6050_DMPINT_1_BIT, buffer);
        Serial.print(", "); 
        Serial.print ( mpu.getDMPInt0Status() );                        // readBit(devAddr, MPU6050_RA_DMP_INT_STATUS, MPU6050_DMPINT_0_BIT, buffer);
    }
        Serial.println(); 
}


void printOneLineBinaryMPU6050InterruptStatus(){

        Serial.print(mpuIntStatus, BIN);
        if (mpuIntStatus & 0x02) {
        byte dmpIntData = mpu.getDMPInt5Status() << 1;
        dmpIntData = mpu.getDMPInt4Status() << 1;
        dmpIntData = mpu.getDMPInt3Status() << 1;
        dmpIntData = mpu.getDMPInt2Status() << 1;
        dmpIntData = mpu.getDMPInt1Status() << 1;
        dmpIntData = mpu.getDMPInt0Status();
        Serial.print(": "); 
        Serial.print(dmpIntData, BIN); 
    }
        Serial.println(); 
}

