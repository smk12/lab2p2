/*
 * File:   timer.c
 * Authors:
 *
 * Created on December 30, 2014, 8:07 PM
 */

#include <xc.h>
#include "timer.h"



void delayUs(unsigned int delay)
{

    //TODO: Create a delay for "delay" micro seconds using timer 2
    TMR2 = 0;
    PR2 = delay*10-1;
    T2CONbits.TCKPS = 3;//pre-scalar
    T2CONbits.TCS = 0;//osc config
    IFS0bits.T2IF = 0;//flag
    T2CONbits.ON = 1;
    while(IFS0bits.T2IF == 0){}
    T2CONbits.ON = 0;
    IFS0bits.T2IF = 0;
    
}

void delayMs(unsigned int delay){
    //TODO: Create a delay using timer 2 for "delay" microseconds.
    TMR2 = 0;
    PR2 = delay*39;
    T2CONbits.TCKPS = 7;//pre-scalar
    T2CONbits.TCS = 0;//osc config
    IFS0bits.T2IF = 0;//flag
    T2CONbits.ON = 1;
    while(IFS0bits.T2IF == 0){}
    T2CONbits.ON = 0;
    IFS0bits.T2IF = 0;

}

void MsTMR1(unsigned int delay)
{
    TMR1 = 0;                           //configured for 10ms
    PR1 = delay*39;        //31249
    T1CONbits.TCKPS = 3;//pre-scalar 3
    T1CONbits.TCS = 0;//osc config
    IFS0bits.T1IF = 0;//flag
    T1CONbits.ON = 1;
    while(IFS0bits.T1IF == 0) {}
    T1CONbits.ON = 0;
    IFS0bits.T1IF = 0;
}



/*
void delay10Ms(unsigned int delay){
    //TODO: Create a delay using timer 2 for 10 "delay" microseconds.
    TMR1 = 0;
    PR1 = delay*3125-1;
    T1CONbits.TCKPS = 3;//pre-scalar
    T1CONbits.TCS = 0;//osc config
    IFS0bits.T1IF = 0;//flag
    T1CONbits.ON = 1;
    while(IFS0bits.T1IF == 0){}
    T1CONbits.ON = 0;
    IFS0bits.T1IF = 0;

}
*/
