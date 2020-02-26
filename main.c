/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>         /* XC8 General Include File */
#include <stdint.h>     /* For uint8_t definition */
#include "system.h"     /* System funct/params, like osc/peripheral config */

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

uint32_t relay_time_counter = 0;
uint16_t led_time_counter = 0;



/*** If using IPE to program, never forget to reload HEX after each build.  ***/



/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();
    

    while(1);

}

void interrupt isr(void)
{
    /* Interruption cant take more then 1s (WDT setup). Be aware of __delays */
    
    if(T0IE && T0IF) //Each tmr0 tick is ~ 256us long
    {
        T0IF = 0; //clear interrupt flag
        CLRWDT(); 
        
        if (led_time_counter-- == 0) {
            LED = !LED;
            led_time_counter = LED_TIME;
        }
        
        if (relay_time_counter > 0) {
            if (--relay_time_counter == 0) {
                nRELAY = 1;
            }
        }
    }
    else if (GPIF)
    {
        __delay_ms(50); //Avoid bounce
        if (!BTN) { //is BTN pressed?
            nRELAY = 0; //Turn relay on
            relay_time_counter = RELAY_TIME;//start/restart timer count
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
    T0IF = 0; //Starts running
    PSA  = 1; //Prescaler is assigned to 0:TIMER0 1:WDT
    PS2  = 1; //Prescaler set to 110 yields a max of 1s tolerance.
    PS1  = 1;
    PS0  = 0; ///111 -> TMR0:256, WDT:128
            
    /* Enable interrupts */
    GIE  = 1; //global interrupt
    PEIE = 1; //peripheral interrupt
    T0IE = 1; //timer 0 interrupt (começa ligado)
    GPIE = 1; //interrupt-on-change
    IOC2 = 1; //Only GP2 can interrupt on change
}

void ConfigureOscillator(void)
{
    OSCCAL = __osccal_val(); // Use when good cal value is stored at 0x3ff.
    // must be commented out for simulating. (mplab bug)
}
