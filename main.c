/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>         /* XC8 General Include File */
#include <stdint.h>        /* For uint8_t definition */

#include "system.h"        /* System funct/params, like osc/peripheral config */

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

uint24_t global_time_counter = 0;
uint16_t gpio1_time_counter = 0;

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void main(void)
{
    /* Configure the oscillator for the device */
   ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();
    

    while(1)
    {
        LED = !LED;  // LED ON 
        __delay_ms(500); // 1 Second Delay
//        CLRWDT(); //com prescaler em 110, gera um intervalo de pelo menos 1s.
    }

}

void interrupt isr(void)
{
    if(T0IE && T0IF) //cada tick do tmr0 ~ 256us
    {
        T0IF = 0; //clear interrupt flag
        
        if (gpio1_time_counter-- == 0) {
            GPIO1 = !GPIO1;
            gpio1_time_counter = _1S;
        }
        
        if (global_time_counter > 0) {
            if (--global_time_counter == 0) {
                nRELAY = 1;
            }
        }
    }
    else if (GPIF)
    {
        __delay_ms(50); //avoid bounce
        if (!BTN) { //is BTN pressed?
            nRELAY = 0; //liga o relay
            global_time_counter = TIMER_COUNT;//start/restart timer count
            T0IF = 0;
        }
        GPIF = 0; //clear interrupt flag
    }
}

void InitApp(void)
{
    /* Setup analog functionality and port direction */
    CMCON = 0b00000111; //comparator off
    #ifdef _PIC12F675
    ANSEL = 0x0; //individual analog config
    ADCON0= 0b00000000; //AN0 Chanel Selected, left justified
    ADIE = 0;           //Disable adc interrupt
    #endif

    /* TODO Initialize User Ports/Peripherals/Project here */
    TRISIO = 0b00001100; //GP2, GP3 in
    GPIO   = 0b00111101; //GP0 ON
    WPU2   = 1; //Weak pullup no GP2
    nGPPU  = 0; //Enable idividual pullup setup

    /* Initialize TMR0 */
    T0CS = 0; //Internal instruction cycle clock (CLKOUT)
    T0SE = 0; //Increment on low-to-high transition on GP2/T0CKI pin
    T0IF = 0; // Inicia nao interrompido
    PSA  = 1; //Prescaler is assigned to 0:TIMER0 1:WDT
    PS2  = 1;
    PS1  = 1;
    PS0  = 0; ///111 -> TMR0:256, WDT:128
            
    /* Enable interrupts */
    GIE  = 1; //global interrupt
    PEIE = 1; //peripheral interrupt
    T0IE = 1; //timer 0 interrupt (começa ligado)
    GPIE = 1; //interrupt-on-change
    IOC2 = 1; //GP2 somente pode interromper
}

void ConfigureOscillator(void)
{
    OSCCAL = __osccal_val(); // Use when good cal value is stored at 0x3ff.
}
