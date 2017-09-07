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

uint16_t global_time_counter = 0;

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
        GPIO0 = 1;  // LED ON 
        __delay_ms(1000); // 1 Second Delay
        GPIO0 = 0;  // LED OFF
        __delay_ms(1000); // 1 Second Delay
    }

}

void interrupt isr(void)
{
    if(T0IE && T0IF)
    {
        T0IF = 0; //clear interrupt flag
        
        if (global_time_counter-- == 0) {
            RELAY = 0;
            T0IE = 0; //desliga o timer
        }
    }
    else if (GPIF)
    {
        __delay_ms(50); //avoid bounce
        if (!BTN) { //is BTN pressed?
            RELAY = 1; //liga o relay
            global_time_counter = _40M; //inicia ou reinicia a contagem
            T0IE = 1; //liga o timer
            T0IF = 0;
        }
        GPIF = 0; //clear interrupt flag
    }
}

void InitApp(void)
{
    /* TODO Initialize User Ports/Peripherals/Project here */
    TRISIO = 0b00001000; //GP3 in
    GPIO   = 0b00010001; //GP0 ON
//    WPU2   = 1; //Weak pullup no GP2
//    nGPPU  = 1; //Enable idividual pullup setup

    /* Setup analog functionality and port direction */
    CMCON = 0b00000111; //comparator off
    #ifdef _PIC12F675
    ANSEL = 0x0; //individual analog config
    ADCON0= 0b00000101; //AN1 Chanel Selected, left justified
    ADIE = 0;           //Enable adc interrupt
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
    IOC = 0b00001000; //GP3 somente pode interromper
}

void ConfigureOscillator(void)
{
    
}