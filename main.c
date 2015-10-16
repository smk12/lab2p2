// ******************************************************************************************* //
// File:         main.c
// Date:         
// Authors:      
//
// Description: 
// ******************************************************************************************* //

#include <xc.h>
#include <sys/attribs.h>
#include "lcd.h"
#include "timer.h"
#include "keypad.h"
#include "config.h"
#include "interrupt.h"

#define PORT_0 PORTAbits.RA6    //col 1
#define PORT_1 PORTAbits.RA7    //col 2
#define PORT_2 PORTGbits.RG1    //col 3

#define PRESS 0
#define RELEASE 1


typedef enum stateTypeEnum{
    write, wait, debouncePress, debounceRelease
} stateType;

volatile stateType state;
volatile int xlcdPos;
volatile int ylcdPos;
volatile int temp;
volatile int temp2;
volatile int temp3;
volatile char keyScan;


int main(void)
{
    SYSTEMConfigPerformance(40000000);
    enableInterrupts();
    initKeypad();
    initLCD();
    state = wait;
    xlcdPos = 0;
    ylcdPos = 0;
    
    while(1)
    {
        switch(state)
        {
            case wait:
                
                break;
            case write:
                keyScan = scanKeypad();
                if(keyScan==-1)
                {
                    state = wait;
                    break;
                }
                else
                {
                    printCharLCD(keyScan);
                    xlcdPos++;
                    if(xlcdPos==8)
                    {
                        xlcdPos = 0;
                        ylcdPos++;
                        if(ylcdPos==2)
                            ylcdPos=0;
                        moveCursorLCD(ylcdPos, xlcdPos);
                    }
                }
                state = wait;
                break;
            case debouncePress:
                delayMs(5);
                state = write;
                break;
            case debounceRelease:
                delayMs(5);
                state = wait;
                break;
        }
    }
    
    return 0;
     
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(void)
{
    IFS1bits.CNAIF = 0;
    IFS1bits.CNGIF = 0;
    temp = PORT_0;temp2 = PORT_1;temp3 = PORT_2;
    if ((temp==RELEASE)&(temp2==RELEASE)&(temp3==RELEASE))
        state = debounceRelease;
    else
        state = debouncePress;
    
    /*
    if((temp==PRESS)&(temp2==RELEASE)&(temp3==RELEASE))
        colNum = 1;
    else if((temp==RELEASE)&(temp2==PRESS)&(temp3==RELEASE))
        colNum = 2;
    else if((temp==RELEASE)&(temp2==RELEASE)&(temp3==PRESS))
        colNum = 3;
    else if ((temp==RELEASE)&(temp2==RELEASE)&(temp3==RELEASE))
    {
        colNum = -1;
        state = debounceRelease;
    }
    else
        colNum = -1;
    if(state!=debounceRelease)
        state = debouncePress;
     * */
}
