/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/

/* Microcontroller MIPs (FCY) */
#define SYS_FREQ        500000L
#define FCY             SYS_FREQ/4
#define _XTAL_FREQ      4000000

//const uint16_t osc_write @ 0x3FF = 0x3480;

#define _PIC12F675      1

/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/

/* Custom oscillator configuration funtions, reset source evaluation
functions, and other non-peripheral microcontroller initialization functions
go here. */

void ConfigureOscillator(void); /* Handles clock switching/osc initialization */

void InitApp(void);         /* I/O and Peripheral Initialization */


#define BTN         GPIO2
#define LED         GPIO0
#define nRELAY      GPIO4
#define _1S         3903
#define _10S        40000
#define _1M         234183 //usar este como ref. mais preciso
#define _40M        9367320 //16bits (65,536), 24bits (524,288), 32bits 4,294,967,295
#define _1H30M      21076470 //requer 32bits
#define RELAY_TIME  _1H30M
#define LED_TIME    _1S