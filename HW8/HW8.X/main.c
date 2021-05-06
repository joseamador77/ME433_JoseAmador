#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include "i2c_master_noint.h"



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

void setPin(unsigned char address, unsigned char regi, unsigned char value){
    
    i2c_master_start();
    unsigned char writeAdd = (address << 1 )& 0b11111110;
    i2c_master_send(writeAdd);//writeAdd
    i2c_master_send(regi);
    i2c_master_send(value);
    i2c_master_stop();
}  
unsigned char readPin(unsigned char address, unsigned char regi){
    //read a 
    unsigned char writeAdd = (address << 1 )& 0b11111110;
     unsigned char readAdd = (address<<1) | 0b00000001;
    i2c_master_start();
    i2c_master_send(writeAdd);//write add
    i2c_master_send(regi);
    i2c_master_restart();
    i2c_master_send(readAdd); //read add
    unsigned char readData = i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
    return readData;
        
}


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

    //unsigned char wAdd = 0b01000000;
    //unsigned char rAdd = 0b01000001;
    unsigned char addy = 0b0100000;
    
    i2c_master_setup(); 
    setPin(addy,0x00,0x00);//Set A to be output - IODIRA
    setPin(addy,0x01,0xFF);//Set B to be input - IODIRB
    
    __builtin_enable_interrupts();

    
    
    while (1) {
        
        
        unsigned char pinState = readPin(addy,0x13);// Read from GPIOB
        if (pinState == 0x00){//button is pushed
            setPin(addy,0x14,0xFF);//OLATA = 1 //turn on LED
        }else{
            setPin(addy,0x14,0x00);//OLATA = 0 //turn off LED
        }
             
        

       LATAINV = 0b10000; //on
       _CP0_SET_COUNT(0);
       while(_CP0_GET_COUNT() <= 24000000*0.5){ }// 1/100 of a second delay

    }   
    

}

