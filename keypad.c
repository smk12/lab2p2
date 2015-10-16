#include <xc.h>
#include "keypad.h"
#include "timer.h"

/* Initialize the rows as ODC outputs and the columns as inputs with pull-up
 * resistors. Don't forget about other considerations...
 */
//4x3 keypad will require 4 ODC output pins and 3 input pins with a pull up resister (external)
#define TRIS_0 TRISCbits.TRISC1     //row 1 output   pin18
#define TRIS_1 TRISCbits.TRISC3     //row 2 output   pin20
#define TRIS_2 TRISDbits.TRISD4     //row 3 output   pin14
#define TRIS_3 TRISDbits.TRISD10     //row 4 output  pin16
#define TRIS_4 TRISAbits.TRISA6     //col 1 input    pin1
#define TRIS_5 TRISAbits.TRISA7     //col 2 input    pin3
#define TRIS_6 TRISGbits.TRISG1     //col 3 input    pin5
#define ODC_0 ODCCbits.ODCC1        //row 1 ODC       
#define ODC_1 ODCCbits.ODCC3        //row 2 ODC 
#define ODC_2 ODCDbits.ODCD4        //row 3 ODC       
#define ODC_3 ODCDbits.ODCD10        //row 4 ODC 
#define LAT_0 LATCbits.LATC1        //row 1 write           
#define LAT_1 LATCbits.LATC3        //row 2 write
#define LAT_2 LATDbits.LATD4        //row 3 write      
#define LAT_3 LATDbits.LATD10        //row 4 write
#define PORT_0 PORTAbits.RA6        //read col1 input
#define PORT_1 PORTAbits.RA7        //read col2 input
#define PORT_2 PORTGbits.RG1        //read col3 input
#define CNEN_0 CNENAbits.CNIEA6
#define CNEN_1 CNENAbits.CNIEA7
#define CNEN_2 CNENGbits.CNIEG1

#define INPUT 1 
#define OUTPUT 0
#define ENABLE 1

void initKeypad(void)
{
    TRIS_0 = OUTPUT;TRIS_1 = OUTPUT;TRIS_2 = OUTPUT;TRIS_3 = OUTPUT;
    TRIS_4 = INPUT;TRIS_5 = INPUT;TRIS_6 = INPUT;
    ODC_0 = ENABLE;ODC_1 = ENABLE;ODC_2 = ENABLE;ODC_3 = ENABLE;
    //LAT_0 = 1;LAT_1 = 1;LAT_2 = 1;LAT_3 = 1;
    LAT_0 = 0;LAT_1 = 0;LAT_2 = 0;LAT_3 = 0;        //Button press brings input low. Lat 0 makes outut pins low (0V) and lat 1 makes output pins high (3.3V). ISR is triggered when all lat are 0
    
    CNCONAbits.ON = 1;CNCONGbits.ON = 1;        //corrisponds to col pin types
    IEC1bits.CNAIE = 1;IEC1bits.CNGIE = 1;      //corrisponds to col pin types
    
    CNEN_0 = 1;CNEN_1 = 1;CNEN_2 = 1;
    
    IPC8bits.CNIP = 7;
    IPC8bits.CNIS = 2;
    
    IFS1bits.CNAIF = 0;
    IFS1bits.CNGIF = 0;
    
}

/* This function will be called AFTER you have determined that someone pressed
 * SOME key. This function is to figure out WHICH key has been pressed.
 * This function should return -1 if more than one key is pressed or if
 * no key is pressed at all. Otherwise, it should return the ASCII character of
 * the key that is pressed.
 */
//ODC 1 is enable. Turn off/on to scan
char scanKeypad(void){
    char key = -1;
    int found = 0;
    int tempo = 0;
    
    tempo = 1;
    LAT_0 = 0;LAT_1 = 1;LAT_2 = 1;LAT_3 = 1;
    delayUs(5);
    //LAT_0 = 1;LAT_1 = 0;LAT_2 = 0;LAT_3 = 0;
    if(PORT_0 == 0){key = '1';found++;}
    if(PORT_1 == 0){key = '2';found++;}
    if(PORT_2 == 0){key = '3';found++;}
    
    
    tempo = 1;
    LAT_0 = 1;LAT_1 = 0;LAT_2 = 1;LAT_3 = 1;
    delayUs(5);
    //LAT_0 = 0;LAT_1 = 1;LAT_2 = 0;LAT_3 = 0;
    if(PORT_0 == 0){key = '4';found++;}
    if(PORT_1 == 0){key = '5';found++;}
    if(PORT_2 == 0){key = '6';found++;}
    
    
    tempo = 1;
    LAT_0 = 1;LAT_1 = 1;LAT_2 = 0;LAT_3 = 1;
    delayUs(5);
    //LAT_0 = 0;LAT_1 = 0;LAT_2 = 1;LAT_3 = 0;
    if(PORT_0 == 0){key = '7';found++;}
    if(PORT_1 == 0){key = '8';found++;}
    if(PORT_2 == 0){key = '9';found++;}
    
    //delayUs(5);
    //tempo = 1;
    LAT_0 = 1;LAT_1 = 1;LAT_2 = 1;LAT_3 = 0;
    delayUs(5);
    //LAT_0 = 0;LAT_1 = 0;LAT_2 = 0;LAT_3 = 1;
    if(PORT_0 == 0){key = 0x2a;found++;}
    if(PORT_1 == 0){key = '0';found++;}
    if(PORT_2 == 0){key = 0x23;found++;}
    
    tempo = 1;
    if(found>1)
        key = -1;
    LAT_0 = 0;LAT_1 = 0;LAT_2 = 0;LAT_3 = 0;
    delayUs(5);
    return key;
}
