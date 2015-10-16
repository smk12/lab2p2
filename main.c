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
    write, wait, debouncePress, debounceRelease, reset
} stateType;

volatile stateType state;
volatile int inNum = 0;          //counts # of current inputs
volatile char passIn[4][10];
volatile int passX;
volatile int passY;
volatile char currIn[4];
volatile int currX;        //First array in currIn
volatile int setMode;      //0 when checking pass. 1 when setting pass
volatile int temp;
volatile int temp2;
volatile int temp3;
volatile char keyScan;
volatile int loop;


int main(void)
{
    SYSTEMConfigPerformance(40000000);
    enableInterrupts();
    initKeypad();
    initLCD();
    state = reset;
    currX = 0;
    setMode = 0;
    
    while(1)
    {
        switch(state)
        {
            case reset:
                clearLCD();
                printStringLCD("Enter");
                moveCursorLCD(1,0);
                inNum = 0;
                setMode = 0;
                state = wait;
                break;
            case wait:
                
                break;
            case write:
                keyScan = scanKeypad();
                if(keyScan==-1)
                {
                    state = wait;
                    break;
                }
                else if(keyScan==0x23)
                {
                    clearLCD();
                    moveCursorLCD(0,0);
                    printStringLCD("Bad");
                    for(loop=0;loop<=20;loop++)
                        delayMs(200);
                    currX = 0;
                    state = reset;
                    break;
                }
                else if(keyScan==0x2a)
                {
                    if((currX==0)&(setMode==0))
                    {
                        currIn[0] = 0x2a;
                        printCharLCD(0x2a);
                        state = wait;
                    }
                    else if((currIn[0] == 0x2a)&(currX==1)&(setMode==0))
                    {
                        clearLCD();
                        moveCursorLCD(0,0);
                        printStringLCD("Set Mode");
                        moveCursorLCD(1,0);
                        setMode = 1;
                        currX = 0;
                        state = wait;
                        break;
                    }
                    else
                    {
                        clearLCD();
                        moveCursorLCD(0,0);
                        printStringLCD("Invalid");
                        for(loop=0;loop<=20;loop++)
                            delayMs(200);
                        currX = 0;
                        setMode = 0;
                        state = reset;
                        break;
                    }
                }
                else if(setMode==0)
                {
                    currIn[currX] = keyScan;
                    printCharLCD(keyScan);
                }
                else if(setMode==1)
                {
                    passIn[passX][passY] = keyScan;
                    printCharLCD(keyScan);
                }
                else
                {
                    clearLCD();
                    moveCursorLCD(0,0);
                    printStringLCD("Error");
                    for(loop=0;loop<=20;loop++)
                        delayMs(200);
                    currX = 0;
                    setMode = 0;
                    state = reset;
                }
                if((currX==3)&(setMode==0))
                {
                    
                    //check if valid password
                    clearLCD();
                    moveCursorLCD(0,0);
                    printStringLCD("Bacon");
                    for(loop=0;loop<=20;loop++)
                        delayMs(200);
                    currX = 0;
                    setMode = 0;
                    state = reset;
                    break;
                }
                else if((currX==3)&(setMode==1))
                {
                    clearLCD();
                    moveCursorLCD(0,0);
                    printStringLCD("Valid");
                    for(loop=0;loop<=20;loop++)
                        delayMs(200);
                    currX = 0;
                    setMode = 0;
                    state = reset;
                    passX = 0;
                    passY++;
                    break;
                }
                currX++;
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
    {
        //state = debounceRelease;
        //delayMs(5);
    }
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
