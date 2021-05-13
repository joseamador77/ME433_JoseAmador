#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include "ws2812b.h"



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

    
    ws2812b_setup();
   
    __builtin_enable_interrupts();

    wsColor pink = HSBtoRGB(300,0.66,1);
    wsColor red = HSBtoRGB(0,1,1);
    wsColor orange = HSBtoRGB(26,1,1);
    wsColor yellow = HSBtoRGB(61,1,1);
    wsColor green = HSBtoRGB(112,1,1);
    wsColor blue = HSBtoRGB(237,1,1);
    wsColor purple = HSBtoRGB(270,0.82,1);
    
    wsColor rainbow[6];
    wsColor colorSet[4];
    rainbow[0] = red;
    rainbow[1] = orange;
    rainbow[2] = yellow;
    rainbow[3] = green;
    rainbow[4] = blue;
    rainbow[5] = purple;
    
    colorSet[0] = red;
    colorSet[1] = orange;
    colorSet[2] = yellow;
    colorSet[3] = green;
    int rainbowTracker = 0;
            
           
    while (1) {
        
        
        ws2812b_setColor(colorSet,4);
        int i = 0;
        for(i = 0; i < 4;i++){
            colorSet[i] = rainbow[(rainbowTracker+i) % 6];
        }
        rainbowTracker++;

       LATAINV = 0b10000; //HEARTBEAT 
       _CP0_SET_COUNT(0);
       while(_CP0_GET_COUNT() <= 24000000*0.5){ }// 1/100 of a second delay

    }   
    

}

