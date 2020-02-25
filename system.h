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


#define BTN     GPIO2
#define LED     GPIO0
#define nRELAY  GPIO4
#define _1S     3903
#define _10S    40000
#define _1M     234183
#define _40M    37700 //fit in 16bits (65536)
#define _1H30M    84825 //requires 24bits (524288)
#define TIMER_COUNT _10S