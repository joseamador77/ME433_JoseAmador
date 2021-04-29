#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include "spi.h"

#include <stdio.h>
#include <math.h>

#define PI 3.14159265

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
            
    __builtin_enable_interrupts();

    unsigned short vA = 0;
    unsigned short vB = 0;
    unsigned char c = 0; //0 = A. 1 = B
    unsigned short pA = 0;
    unsigned short pB = 0;
    int numPoints = 100;
    double dt = 1.0/100.0;
    double sineWave[numPoints];
    double triWave[numPoints];
    
    int i = 0;
    for(i = 0; i < numPoints; i++){
        sineWave[i] = 1.65*sin(4*PI*i*dt) + 1.65;
        sineWave[i] = sineWave[i]* (1023/3.3);
        if (i <50){
            triWave[i] = 6.6*i*dt;
        }else{
            triWave[i] = -6.6*i*dt+ 6.6;
        }
        triWave[i] = triWave[i]* (1023/3.3);
    }
    
    while (1) {
        
        for(i = 0; i <numPoints; i++){
        
            c = 0;//write to A
            vA = sineWave[i];
            pA = c << 15;
            pA = pA | (0b111 << 12);
            pA = pA | (vA<<2);

            c = 1; //write to B
            vB = triWave[i];
            pB = c << 15;
            pB = pB | (0b111 << 12);
            pB = pB | (vB<<2);

            LATAbits.LATA0 = 0; //bring CS low
            spi_io(pA >> 8); //write the byte
            spi_io(pA); //write the byte
            LATAbits.LATA0 = 1;// bring CS high
            LATAbits.LATA0 = 0; //bring CS low
            spi_io(pB >> 8); //write the byte
            spi_io(pB); //write the byte
            LATAbits.LATA0 = 1;// bring CS high

            _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT() <= 24000000*0.01){ }// 1/100 of a second delay
        }
            
    }   
        
            
            
       
            

}
