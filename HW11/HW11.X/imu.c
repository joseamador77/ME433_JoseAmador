#include "imu.h"
#include "i2c_master_noint.h"




void imu_setup(){
    //read from imu_whoami
    unsigned char who = readPin(IMU_ADDR,IMU_WHOAMI);
    
    
    if (who != WHOAMI_VAL){
        while(1){}
    }
    
    setPin(IMU_ADDR,IMU_CTRL1_XL,0b10000010); //accelerometer
    setPin(IMU_ADDR,IMU_CTRL2_G,0b10001000);//gyroscope
    setPin(IMU_ADDR,IMU_CTRL3_C,0b00000100);//other stuff
}




void imu_read(unsigned char reg, signed short * data, int len){
    unsigned char charData[len*2];
    //read multiple from the imu, each data takes 2 reads so you need len*2 chars
    I2C_read_multiple(IMU_ADDR, reg, charData, len*2);
    //turn the chars into shorts 
    int i = 0;
    for(i = 0;i<len;i++){
        data[i] = (charData[2*i+1] << 8) | charData[2*i];
    }
//    signed short OUT_TEMP = (charData[1] << 8) | charData[0];
//    signed short OUTX_G = (charData[3] << 8) | charData[2];
//    signed short OUTY_G = (charData[5] << 8) | charData[4];
//    signed short OUTZ_G = (charData[7] << 8) | charData[6];
//    signed short OUTX_XL = (charData[9] << 8) | charData[8];
//    signed short OUTY_XL = (charData[11] << 8) | charData[10];
//    signed short OUTZ_XL  = (charData[13] << 8) | charData[12];
    
}