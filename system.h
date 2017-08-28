/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/

/* TODO Define system operating frequency */

/* Microcontroller MIPs (FCY) */
#define SYS_FREQ        500000L
#define FCY             SYS_FREQ/4
#define _XTAL_FREQ      4000000

const uint16_t osc_write @ 0x3FF = 0x3480;

/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/

/* Custom oscillator configuration funtions, reset source evaluation
functions, and other non-peripheral microcontroller initialization functions
go here. */

void ConfigureOscillator(void); /* Handles clock switching/osc initialization */

void InitApp(void);         /* I/O and Peripheral Initialization */


#define BTN     GPIO3
#define LED     GPIO0
#define RELAY   GPIO5
#define _10S    188
#define _1M     1131
#define _30M    33938 //cabe em 16bits