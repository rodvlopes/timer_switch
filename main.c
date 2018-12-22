/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>         /* XC8 General Include File */
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "system.h"        /* System funct/params, like osc/peripheral config */

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

uint24_t global_time_counter = 0;

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
        LED = 1;  // LED ON 
        __delay_ms(1000); // 1 Second Delay
        LED = 0;  // LED OFF
        __delay_ms(1000); // 1 Second Delay
        
        GPIO1 = BTN; //GPIO1 used for debug purpose
    }

}

void interrupt isr(void)
{
    if(T0IE && T0IF)
    {
        T0IF = 0; //clear interrupt flag
        
        if (global_time_counter-- == 0) {
            nRELAY = 1;
            T0IE = 0; //shut timer down
        }
    }
    else if (GPIF)
    {
        __delay_ms(50); //avoid bounce
        if (!BTN) { //is BTN pressed?
            nRELAY = 0; //liga o relay
            global_time_counter = TIMER_COUNT;//start/restart timer count
            T0IE = 1; //liga o timer
            T0IF = 0;
        }
        GPIF = 0; //clear interrupt flag
    }
}

void InitApp(void)
{
    /* TODO Initialize User Ports/Peripherals/Project here */
    TRISIO = 0b00001100; //GP2, GP3 in
    GPIO   = 0b00111101; //GP0 ON
    WPU2   = 1; //Weak pullup no GP2
    nGPPU  = 0; //Enable idividual pullup setup

    /* Setup analog functionality and port direction */
    CMCON = 0b00000111; //comparator off
    #ifdef _PIC12F675
    ANSEL = 0x0; //individual analog config
    ADCON0= 0b00000000; //AN0 Chanel Selected, left justified
    ADIE = 0;           //Disable adc interrupt
    #endif

    /* Initialize peripherals */
    T0CS = 0; //Internal instruction cycle clock (CLKOUT)
    T0SE = 0; //Increment on low-to-high transition on GP2/T0CKI pin
    T0IF = 0; // Inicia nao interrompido
    PSA  = 0; //Prescaler is assigned to the TIMER0 module
    PS2  = 1;
    PS1  = 1;
    PS0  = 1; ///111 -> 256
    
    /* Enable interrupts */
    GIE  = 1; //global interrupt
    PEIE = 1; //peripheral interrupt
    T0IE = 0; //timer 0 interrupt (começa desligado)
    GPIE = 1; //interrupt-on-change
    IOC2 = 1; //GP2 somente pode interromper
}

void ConfigureOscillator(void)
{
    OSCCAL = __osccal_val(); // Use when good cal value is stored at 0x3ff.

}