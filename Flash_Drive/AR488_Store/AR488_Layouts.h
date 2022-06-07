#ifndef AR488_LAYOUTS_H
#define AR488_LAYOUTS_H

#include <Arduino.h>

#include "AR488_Config.h"

/***** AR488_Hardware.h, ver. 0.02.03, 10/02/2021 *****/
/*
 * Hardware pin layout definitions
 */

/*************************************/
/***** CUSTOM PIN LAYOUT SECTION *****/
/***** vvvvvvvvvvvvvvvvvvvvvvvvv *****/
#ifdef AR488_CUSTOM

// Use only pinhooks for custom mode
// (We don't know which pin interrupts will be required)
#ifdef USE_INTERRUPTS
  #undef USE_INTERRUPTS
  #define USE_PINHOOKS
#endif

#endif
/***** ^^^^^^^^^^^^^^^^^^^^^^^^^ *****/
/***** CUSTOM PIN LAYOUT SECTION *****/
/*************************************/



/**************************************/
/***** UNO/NANO LAYOUT DEFINITION *****/
/***** vvvvvvvvvvvvvvvvvvvvvvvvvv *****/
#if defined(AR488_UNO) || defined(AR488_NANO)


/***** NOTE: UNO/NANO pinout last updated 21/09/2019 *****/
#define DIO1  A0  /* GPIB 1  : PORTC bit 0 */
#define DIO2  A1  /* GPIB 2  : PORTC bit 1 */
#define DIO3  A2  /* GPIB 3  : PORTC bit 2 */
#define DIO4  A3  /* GPIB 4  : PORTC bit 3 */
#define DIO5  A4  /* GPIB 13 : PORTC bit 4 */
#define DIO6  A5  /* GPIB 14 : PORTC bit 5 */
#define DIO7   4  /* GPIB 15 : PORTD bit 4 */
#define DIO8   5  /* GPIB 16 : PORTD bit 5 */

#define IFC    8  /* GPIB 9  : PORTB bit 0 */
#define NDAC   9  /* GPIB 8  : PORTB bit 1 */
#define NRFD  10  /* GPIB 7  : PORTB bit 2 */
#define DAV   11  /* GPIB 6  : PORTB bit 3 */
#define EOI   12  /* GPIB 5  : PORTB bit 4 */

#define SRQ    2  /* GPIB 10 : PORTD bit 2 */
#define REN    3  /* GPIB 17 : PORTD bit 3 */
#define ATN    7  /* GPIB 11 : PORTD bit 7 */

/***** PIN interrupts ******/

// Interrupt registers
#ifdef USE_INTERRUPTS
  #define ATNPREG PIND
  #define SRQPREG PIND
  void interruptsEn();
#endif

#endif
/***** ^^^^^^^^^^^^^^^^^^^^^^^^^^ *****/
/***** UNO/NANO LAYOUT DEFINITION *****/
/**************************************/



/******************************************/
/***** 328P STORAGE LAYOUT DEFINITION *****/
/***** vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv *****/
#ifdef AR488_328P_S1

/***** NOTE: last updated 21/09/2019 *****/
/*
 * Pins 11 - 13 (MISO, MOSI, SCK) must be free so that 
 * the ICSP bus can be used. Pin 10 = CS on SD card.
 */
#define DIO1  A0  /* GPIB 1  : PORTC bit 0 */
#define DIO2  A1  /* GPIB 2  : PORTC bit 1 */
#define DIO3  A2  /* GPIB 3  : PORTC bit 2 */
#define DIO4  A3  /* GPIB 4  : PORTC bit 3 */
#define DIO5  A4  /* GPIB 13 : PORTC bit 4 */
#define DIO6  A5  /* GPIB 14 : PORTC bit 5 */

#define DIO7   8  /* GPIB 15 : PORTB bit 0 */
#define DIO8   9  /* GPIB 16 : PORTB bit 1 */

#define EOI    7  /* GPIB 5  : PORTD bit 7 */
#define DAV    6  /* GPIB 6  : PORTD bit 6 */
#define NRFD   5  /* GPIB 7  : PORTD bit 5 */
#define NDAC   4  /* GPIB 8  : PORTD bit 4 */
#define ATN    2  /* GPIB 11 : PORTD bit 2 */

/***** PIN interrupts ******/

// Interrupt registers
#ifdef USE_INTERRUPTS
  #define ATNPREG PIND
//  #define SRQPREG PIND
  void interruptsEn();
#endif

#endif // END AR488_328P_S1
/***** ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ *****/
/***** 328P STORAGE LAYOUT DEFINITION *****/
/******************************************/


/******************************************/
/***** 328PB STORAGE LAYOUT DEFINITION *****/
/***** vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv *****/
#ifdef AR488_328PB_S1

/***** NOTE: last updated 21/09/2019 *****/
/*
 * Pins 11 - 13 (MISO, MOSI, SCK) must be free so that 
 * the ICSP bus can be used. Pin 10 = CS on SD card.
 */
#define DIO1  A0  /* GPIB 1  : PORTC bit 0 */
#define DIO2  A1  /* GPIB 2  : PORTC bit 1 */
#define DIO3  A2  /* GPIB 3  : PORTC bit 2 */
#define DIO4  A3  /* GPIB 4  : PORTC bit 3 */
#define DIO5  A4  /* GPIB 13 : PORTC bit 4 */
#define DIO6  A5  /* GPIB 14 : PORTC bit 5 */

#define DIO7   8  /* GPIB 15 : PORTB bit 0 */
#define DIO8   9  /* GPIB 16 : PORTB bit 1 */

#define EOI    7  /* GPIB 5  : PORTD bit 7 */
#define DAV    6  /* GPIB 6  : PORTD bit 6 */
#define NRFD   5  /* GPIB 7  : PORTD bit 5 */
#define NDAC   4  /* GPIB 8  : PORTD bit 4 */
#define ATN    2  /* GPIB 11 : PORTD bit 2 */

/***** PIN interrupts ******/

// Interrupt registers
#ifdef USE_INTERRUPTS
  #define ATNPREG PIND
//  #define SRQPREG PIND
  void interruptsEn();
#endif

#endif // END AR488_328P_S1
/***** ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ *****/
/***** 328P STORAGE LAYOUT DEFINITION *****/
/******************************************/


/************************************************/
/***** MEGA2560 LAYOUT DEFINITION (Default) *****/
/***** vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv *****/
#ifdef AR488_MEGA2560_D

// NOTE: MEGA2560 pinout last updated 28/07/2019
#define DIO1  A0  /* GPIB 1  : PORTF bit 0 */
#define DIO2  A1  /* GPIB 2  : PORTF bit 1 */
#define DIO3  A2  /* GPIB 3  : PORTF bit 2 */
#define DIO4  A3  /* GPIB 4  : PORTF bit 3 */
#define DIO5  A4  /* GPIB 13 : PORTF bit 4 */
#define DIO6  A5  /* GPIB 14 : PORTF bit 5 */
#define DIO7  A6  /* GPIB 15 : PORTF bit 4 */
#define DIO8  A7  /* GPIB 16 : PORTF bit 5 */

#define IFC   17  /* GPIB 9  : PORTH bit 0 */
#define NDAC  16  /* GPIB 8  : PORTH bit 1 */
#define NRFD   6  /* GPIB 7  : PORTH bit 3 */
#define DAV    7  /* GPIB 6  : PORTH bit 4 */
#define EOI    8  /* GPIB 5  : PORTH bit 5 */
#define REN    9  /* GPIB 17 : PORTD bit 6 */

#define SRQ   10  /* GPIB 10 : PORTB bit 4 */
#define ATN   11  /* GPIB 11 : PORTB bit 5 */

// PCINT registers
#ifdef USE_INTERRUPTS
  #define ATNPREG PINB
  #define SRQPREG PINB
  void interruptsEn();
#endif  // USE_INTERRUPTS

#endif  // AR488_MEGA2560_D
/***** ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ *****/
/***** MEGA2560 LAYOUT DEFINITION (Default) *****/
/************************************************/



/*****************************************/
/***** MEGA2560 LAYOUT DEFINITION E1 *****/
/***** vvvvvvvvvvvvvvvvvvvvvvvvvvvvv *****/
#ifdef AR488_MEGA2560_E1

// NOTE: last updated 28/07/2019
#define DIO1  30  /* GPIB 1  : PORTC bit 1 */
#define DIO2  32  /* GPIB 2  : PORTC bit 3 */
#define DIO3  34  /* GPIB 3  : PORTC bit 5 */
#define DIO4  36  /* GPIB 4  : PORTC bit 7 */
#define DIO5  22  /* GPIB 13 : PORTA bit 6 */
#define DIO6  24  /* GPIB 14 : PORTA bit 4 */
#define DIO7  26  /* GPIB 15 : PORTA bit 2 */
#define DIO8  28  /* GPIB 16 : PORTA bit 0 */

#define IFC   48  /* GPIB 9  : PORTD bit 7 */
#define NDAC  46  /* GPIB 8  : PORTG bit 1 */
#define NRFD  44  /* GPIB 7  : PORTL bit 7 */
#define DAV   42  /* GPIB 6  : PORTL bit 5 */
#define EOI   40  /* GPIB 5  : PORTL bit 3 */
#define REN   38  /* GPIB 17 : PORTL bit 1 */

#define SRQ   50  /* GPIB 10 : PORTB bit 1 */
#define ATN   52  /* GPIB 11 : PORTB bit 3 */

// PCINT registers
#ifdef USE_INTERRUPTS
  #define ATNPREG PINB
  #define SRQPREG PINB
  void interruptsEn();
#endif  // USE_INTERRUPTS

#endif  // AR488_MEGA2560_E1
/***** ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ *****/
/***** MEGA2560 LAYOUT DEFINITION E1 *****/
/*****************************************/



/*****************************************/
/***** MEGA2560 LAYOUT DEFINITION E2 *****/
/***** vvvvvvvvvvvvvvvvvvvvvvvvvvvvv *****/
#ifdef AR488_MEGA2560_E2

// NOTE: last updated 28/07/2019
#define DIO1  37  /* GPIB 1  : PORTA bit 1 */
#define DIO2  35  /* GPIB 2  : PORTA bit 3 */
#define DIO3  33  /* GPIB 3  : PORTA bit 5 */
#define DIO4  31  /* GPIB 4  : PORTA bit 7 */
#define DIO5  29  /* GPIB 13 : PORTC bit 6 */
#define DIO6  27  /* GPIB 14 : PORTC bit 4 */
#define DIO7  25  /* GPIB 15 : PORTC bit 2 */
#define DIO8  23  /* GPIB 16 : PORTC bit 0 */

#define IFC   49  /* GPIB 9  : PORTG bit 0 */
#define NDAC  47  /* GPIB 8  : PORTG bit 2 */
#define NRFD  45  /* GPIB 7  : PORTL bit 6 */
#define DAV   43  /* GPIB 6  : PORTL bit 4 */
#define EOI   41  /* GPIB 5  : PORTL bit 2 */
#define REN   39  /* GPIB 17 : PORTL bit 0 */

#define SRQ   51  /* GPIB 10 : PORTB bit 0 */
#define ATN   53  /* GPIB 11 : PORTB bit 2 */

// PCINT registers
#ifdef USE_INTERRUPTS
  #define ATNPREG PINB
  #define SRQPREG PINB
  void interruptsEn();
#endif  // USE_INTERRUPTS

#endif  // AR488_MEGA2560_E2
/***** ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ *****/
/***** MEGA2560 LAYOUT DEFINITION E2 *****/
/*****************************************/



/*****************************************/
/***** MEGA2560 LAYOUT DEFINITION S1 *****/
/***** vvvvvvvvvvvvvvvvvvvvvvvvvvvvv *****/
#ifdef AR488_MEGA2560_S1

// NOTE: last updated 28/07/2019
#define DIO1  30  /* GPIB 1  : PORTC bit 1 */
#define DIO2  32  /* GPIB 2  : PORTC bit 3 */
#define DIO3  34  /* GPIB 3  : PORTC bit 5 */
#define DIO4  36  /* GPIB 4  : PORTC bit 7 */
#define DIO5  22  /* GPIB 13 : PORTA bit 6 */
#define DIO6  24  /* GPIB 14 : PORTA bit 4 */
#define DIO7  26  /* GPIB 15 : PORTA bit 2 */
#define DIO8  28  /* GPIB 16 : PORTA bit 0 */

#define IFC   48  /* GPIB 9  : PORTD bit 7 */
#define NDAC  46  /* GPIB 8  : PORTG bit 1 */
#define NRFD  44  /* GPIB 7  : PORTL bit 7 */
#define DAV   42  /* GPIB 6  : PORTL bit 5 */
#define EOI   40  /* GPIB 5  : PORTL bit 3 */
#define REN   38  /* GPIB 17 : PORTL bit 1 */

#define SRQ    2  /* GPIB 10 : PORTE bit 4 */
#define ATN    3  /* GPIB 11 : PORTE bit 5 */

// PCINT registers
#ifdef USE_INTERRUPTS
  #define ATNPREG PINE
  #define SRQPREG PINE
  void interruptsEn();
#endif  // USE_INTERRUPTS

#endif  // AR488_MEGA2560_S1
/***** ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ *****/
/***** MEGA2560 LAYOUT DEFINITION S1 *****/
/*****************************************/



/*****************************************/
/***** MEGA2560 LAYOUT DEFINITION S2 *****/
/***** vvvvvvvvvvvvvvvvvvvvvvvvvvvvv *****/
#ifdef AR488_MEGA2560_S2

// NOTE: last updated 28/07/2019
#define DIO1  37  /* GPIB 1  : PORTA bit 1 */
#define DIO2  35  /* GPIB 2  : PORTA bit 3 */
#define DIO3  33  /* GPIB 3  : PORTA bit 5 */
#define DIO4  31  /* GPIB 4  : PORTA bit 7 */
#define DIO5  29  /* GPIB 13 : PORTC bit 6 */
#define DIO6  27  /* GPIB 14 : PORTC bit 4 */
#define DIO7  25  /* GPIB 15 : PORTC bit 2 */
#define DIO8  23  /* GPIB 16 : PORTC bit 0 */

#define IFC   49  /* GPIB 9  : PORTG bit 0 */
#define NDAC  47  /* GPIB 8  : PORTG bit 2 */
#define NRFD  45  /* GPIB 7  : PORTL bit 6 */
#define DAV   43  /* GPIB 6  : PORTL bit 4 */
#define EOI   41  /* GPIB 5  : PORTL bit 2 */
#define REN   39  /* GPIB 17 : PORTL bit 0 */

#define SRQ    2  /* GPIB 10 : PORTE bit 4 */
#define ATN    3  /* GPIB 11 : PORTE bit 5 */

// PCINT registers
#ifdef USE_INTERRUPTS
  #define ATNPREG PINE
  #define SRQPREG PINE
  void interruptsEn();
#endif  // USE_INTERRUPTS

#endif  // AR488_MEGA2560_S2
/***** ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ *****/
/***** MEGA2560 LAYOUT DEFINITION S2 *****/
/*****************************************/



/****************************************************************/
/***** MICRO PRO (32u4) LAYOUT DEFINITION for MICRO (Artag) *****/
/***** vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv *****/
#ifdef AR488_MEGA32U4_MICRO

#define DIO1  3   /* GPIB 1  : PORTD bit 0   data pins assigned for minimum shifting */
#define DIO2  15  /* GPIB 2  : PORTB bit 1 */
#define DIO3  16  /* GPIB 3  : PORTB bit 2 */
#define DIO4  14  /* GPIB 4  : PORTB bit 3 */
#define DIO5  8   /* GPIB 13 : PORTB bit 4 */
#define DIO6  9   /* GPIB 14 : PORTB bit 5 */
#define DIO7  10  /* GPIB 15 : PORTB bit 6 */
#define DIO8  6   /* GPIB 16 : PORTD bit 7 */

#define IFC   4   /* GPIB 9  : PORTD bit 4 */
#define NDAC  A3  /* GPIB 8  : PORTF bit 4   fast control pins assigned to same port */
#define NRFD  A2  /* GPIB 7  : PORTF bit 5 */
#define DAV   A1  /* GPIB 6  : PORTF bit 6 */
#define EOI   A0  /* GPIB 5  : PORTF bit 7 */
#define REN   5   /* GPIB 17 : PORTC bit 6 */
#define SRQ   7   /* GPIB 10 : PORTE bit 6 */
#define ATN   2   /* GPIB 11 : PORTD bit 1 */

#ifdef USE_INTERRUPTS
  #define ATNPREG PIND
  #define SRQPREG PINE
  void interruptsEn();
#endif  // USE_INTERRUPTS

#endif  // AR488_MEGA32U4_MICRO
/***** ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ *****/
/***** MICRO PRO (32u4) LAYOUT DEFINITION for MICRO (Artag) *****/
/****************************************************************/



/*****************************************/
/***** LEONARDO R3 LAYOUT DEFINITION *****/
/***** vvvvvvvvvvvvvvvvvvvvvvvvvvvvv *****/
#ifdef AR488_MEGA32U4_LR3

/***** NOTE: LEONARDO R3 pinout last updated 06/04/2020 *****/
#define DIO1  A0  /* GPIB 1  : PORTF bit 7 */
#define DIO2  A1  /* GPIB 2  : PORTF bit 6 */
#define DIO3  A2  /* GPIB 3  : PORTF bit 5 */
#define DIO4  A3  /* GPIB 4  : PORTF bit 4 */
#define DIO5  A4  /* GPIB 13 : PORTF bit 3 */
#define DIO6  A5  /* GPIB 14 : PORTF bit 2 */
#define DIO7   4  /* GPIB 15 : PORTD bit 4 */
#define DIO8   5  /* GPIB 16 : PORTC bit 6 */

#define IFC    8  /* GPIB 9  : PORTB bit 4 */
#define NDAC   9  /* GPIB 8  : PORTB bit 5 */
#define NRFD  10  /* GPIB 7  : PORTB bit 6 */
#define DAV   11  /* GPIB 6  : PORTD bit 6 */
#define EOI   12  /* GPIB 5  : PORTC bit 7 */

#define SRQ    2  /* GPIB 10 : PORTD bit 1 */
#define REN    3  /* GPIB 17 : PORTD bit 0 */
#define ATN    7  /* GPIB 11 : PORTE bit 6 */

/***** PIN interrupts ******/

// Interrupt registers
#ifdef USE_INTERRUPTS
//  #define ATNPREG PIND
//  #define SRQPREG PIND
  void atnISR();
  void srqISR();
  void interruptsEn();
#endif

uint8_t reverseBits(uint8_t dbyte);


#endif // AR488_MEGA32U4_LR3
/***** ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ *****/
/***** LEONARDO R3 LAYOUT DEFINITION *****/
/*****************************************/



/********************************************************/
/***** PANDUINO/MIGHTYCORE MCGRAW LAYOUT DEFINITION *****/
/***** vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv *****/
#ifdef AR488_MEGA644P_MCGRAW

#define DIO1  10   /* GPIB 1  */
#define DIO2  11   /* GPIB 2  */
#define DIO3  12   /* GPIB 3  */
#define DIO4  13   /* GPIB 4  */
#define DIO5  14   /* GPIB 13 */
#define DIO6  15   /* GPIB 14 */
#define DIO7  16   /* GPIB 15 */
#define DIO8  17   /* GPIB 16 */

#define IFC   22   /* GPIB 9  */
#define NDAC  21   /* GPIB 8  */
#define NRFD  20   /* GPIB 7  */
#define DAV   19   /* GPIB 6  */
#define EOI   18   /* GPIB 5  */

#define SRQ   23   /* GPIB 10 */
#define REN   24   /* GPIB 17 */
#define ATN   31   /* GPIB 11 */

#endif // AR488_MEGA644P_MCGRAW
/***** ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ *****/
/***** PANDUINO/MIGHTYCORE MCGRAW LAYOUT DEFINITION *****/
/********************************************************/



/**************************************/
/***** GLOBAL DEFINITIONS SECTION *****/
/***** vvvvvvvvvvvvvvvvvvvvvvvvvv *****/

void readyGpibDbus();
uint8_t readGpibDbus();
void setGpibDbus(uint8_t db);
void setGpibState(uint8_t bits, uint8_t mask, uint8_t mode);
uint8_t getGpibPinState(uint8_t pin);

/***** ^^^^^^^^^^^^^^^^^^^^^^^^^^ *****/
/***** GLOBAL DEFINITIONS SECTION *****/
/**************************************/


#endif // AR488_LAYOUTS_H
