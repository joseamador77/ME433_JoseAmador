#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include "font.h"
#include "spi.h"
#include "ST7789.h"
#include <stdio.h>
#include "i2c_master_noint.h"
#include "imu.h"

// DEVCFG0
#pragma config DEBUG = OFF // disable debugging
#pragma config JTAGEN = OFF // disable jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // disable flash write protect
#pragma config BWP = OFF // disable boot write protect
#pragma config CP = OFF // disable code protect

// DEVCFG1
#pragma config FNOSC = FRCPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // disable secondary oscillator
#pragma config IESO = OFF // disable switching clocks
#pragma config POSCMOD = OFF // high speed crystal mode
#pragma config OSCIOFNC = OFF // disable clock output
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // disable clock switch and FSCM
#pragma config WDTPS = PS1048576 // use largest wdt
#pragma config WINDIS = OFF // use non-window mode wdt
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz

// DEVCFG3
#pragma config USERID = 0 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations



int main() {

    __builtin_disable_interrupts(); // disable interrupts while initializing things

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // do your TRIS and LAT commands here
    TRISBbits.TRISB4 = 1;//B4 IS AN INPUT
    TRISAbits.TRISA4  = 0;//A4 IS AN OUTPUT
    LATAbits.LATA4 = 0;//A4 IS OFF

   initSPI();
   LCD_init();
   i2c_master_setup(); 
   imu_setup();
   
    __builtin_enable_interrupts();

    int dataPoints = 7;
    signed short imu_output[dataPoints];
    while (1) {
        
        
        imu_read(IMU_OUT_TEMP_L,imu_output,dataPoints);

        LCD_clearScreen(BLUE);
           
        _CP0_SET_COUNT(0); //init time
        char m[240];
        sprintf(m,"GYROSCOPE: %d %d %d ",imu_output[1],imu_output[2],imu_output[3]);
        drawString(28,90,MAGENTA,m);
        sprintf(m,"ACCELERATION: %d %d %d ",imu_output[4],imu_output[5],imu_output[6]);
        drawString(28,120,MAGENTA,m);
        sprintf(m,"TEMPERATURE: %d",imu_output[0]);
        drawString(28,150,MAGENTA,m);
        
        sprintf(m,"FPS is %f", 24000000.0/_CP0_GET_COUNT());//final time
        drawString(28,67,MAGENTA,m); 
   
       LATAINV = 0b10000; //HEARTBEAT 
       _CP0_SET_COUNT(0);
       while(_CP0_GET_COUNT() <= 24000000*0.5){ }

    }   
    

}


