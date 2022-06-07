#ifndef AR488_CONFIG_H
#define AR488_CONFIG_H


/*********************************************/
/***** AR488 GLOBAL CONFIGURATION HEADER *****/
/***** vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv *****/


/***** Firmware version *****/
#define FWVER "AR488 GPIB storage, ver. 0.05.74, 04/05/2022"


/***** BOARD CONFIGURATION *****/
/*
 * Platform will be selected automatically based on
 * Arduino definition.
 * Only ONE board/layout should be selected per platform
 * Only ONE Serial port can be used to receive output
 */


/*** Custom layout ***/
/*
 * Uncomment to use custom board layout
 */
//#define AR488_CUSTOM

/*
 * Configure the appropriate board/layout section
 * below as required
 */
#ifdef AR488_CUSTOM
  /* Board layout */
  /*
   * Define board layout in the AR488 CUSTOM LAYOUT
   * section below
   */

/*** UNO and NANO boards ***/
#elif __AVR_ATmega328P__
  /* Board/layout selection */
  #define AR488_UNO
  //#define AR488_NANO

#elif __AVR_ATmega328PB__
  /* Board/layout selection */
  #define AR488_POLOLU_MICRO

/*** MEGA 32U4 based boards (Micro, Leonardo) ***/
#elif __AVR_ATmega32U4__
  /*** Board/layout selection ***/
  #define AR488_MEGA32U4_MICRO  // Artag's design for Micro board
  //#define AR488_MEGA32U4_LR3  // Leonardo R3 (same pin layout as Uno)
  
/*** MEGA 2560 board ***/
#elif __AVR_ATmega2560__
  /*** Board/layout selection ***/
  #define AR488_MEGA2560_D
  //#define AR488_MEGA2560_E1
  //#define AR488_MEGA2560_E2
  //#define AR488_MEGA2560_S1
  //#define AR488_MEGA2560_S2

/***** Panduino Mega 644 or Mega 1284 board *****/
#elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284P__)
  /* Board/layout selection */
  #define AR488_MEGA644P_MCGRAW
  
#endif  // Board/layout selection



/***** SERIAL PORT CONFIGURATION *****/
/*
 * Serial port should be either:
 * AR_SERIAL_PORT portname - e.g. AR_SERIAL_PORT Serial
 * or when using SoftwareSerial:
 * AR_SERIAL_SWPORT
 * Only ONE of the above should be uncommented!
 * The pins being used for the SoftwareSerial port should be configured in the 'Configure 
 * SoftwareSerial Port' section.
 * 
 * Note: On most boards the default serial device is named 'Serial'. Where boards that have 
 *       a secondary UART port this will have a numerical suffix, e.g.'Serial1'. The Mega2560 
 *       also supports 'Serial3' and 'Serial4'
 */
/***** Communication port *****/
//#define DATAPORT_ENABLE
#ifdef DATAPORT_ENABLE
  // Serial port device
  // (for SoftwareSerial comment out DB_SERIAL_PORT and uncomment DB_SERIAL_SWPORT)
  #define AR_SERIAL_PORT Serial
//  #define AR_SERIAL_SWPORT
  // Set port operating speed
  #define AR_SERIAL_SPEED 115200
#endif

/***** Debug port *****/
//#define DEBUG_ENABLE
#ifdef DEBUG_ENABLE
  // Serial port device
  // (for SoftwareSerial comment out DB_SERIAL_PORT and uncomment DB_SERIAL_SWPORT)
  #define DB_SERIAL_PORT Serial
//  #define DB_SERIAL_SWPORT
  // Set port operating speed (SoftwareSerial max speed is 57600 baud)
  #define DB_SERIAL_SPEED 115200
#endif
/***** Configure SoftwareSerial Port *****/
/*
 * Configure the SoftwareSerial TX/RX pins.
 * Note: SoftwareSerial support conflicts with PCINT support
 * When using SoftwareSerial, disable USE_INTERRUPTS.
 */
#if defined(AR_SERIAL_SWPORT) || defined(DB_SERIAL_SWPORT)
  #define SW_SERIAL_RX_PIN 11
  #define SW_SERIAL_TX_PIN 12
#endif
/*
 * Note: SoftwareSerial reliable only up to a MAX of 57600 baud only
 */



/***** GPIB parameters configuration *****/
#define GPIB_ADDRESS 5          // Interface address
#define GPIB_EOT_ENABLE false   // Enable/disable append EOT char to string received from GPIB bus before sending to USB
#define GPIB_EOT_CHAR 0         // EOT character to append to USB output when EOI signal detected (character ASCII code)
#define GPIB_EOI_ENABLE true    // Enable sending EOI with last character of data written to GPIB - false-disable, true-enable
#define GPIB_EOS_MODE 1         // EOS (end of send to GPIB) characters [0=CRLF, 1=CR, 2=LF, 3=None]
#define GPIB_AUTO_MODE 0        // Auto mode setting (0=off; 1=Prologix; 2=onquery; 3=continuous);
#define GPIB_RTMO 5000          // Read timout (read_tmo_ms) in milliseconds - 0-30000 - value may depend on instrument
#define GPIB_EOR_MODE 1         // EOR (end of receive from GPIB instrument) characters [0=CRLF, 1=CR, 2=LF, 3=None, 4=LFCR, 5=ETX, 6=CRLF+ETX, 7=SPACE]
#define GPIB_SEND_IDN 0         // Send ID in response to *idn? 0=disable, 1=send name; 2=send name+serial      


/***** Pin State Detection *****/
/*
 * With UNO. NANO and MEGA boards with pre-defined layouts,
 * USE_PCINTS can be used.
 * With the AR488_CUSTOM layout and unknown boards, USE_PINHOOKS must  
 * be used. Interrupts are used on pre-defined AVR board layouts and will 
 * respond faster, however "pinhooks" (in-loop checking for state of pin) 
 * can be supported with any board layout.
 */
#ifdef __AVR__
  // For supported boards use interrupt handlers
  #if defined (AR488_UNO) || defined (AR488_NANO) || defined (AR488_MEGA2560) || defined (AR488_MEGA32U4)
    #define USE_INTERRUPTS
  #else
    // For other boards use in-loop checking
    #define USE_PINHOOKS
  #endif
#else
  #define USE_PINHOOKS
#endif


/***** Enable SN7516x chips *****/
/*
 * Uncomment to enable the use of SN7516x GPIB tranceiver ICs.
 * This will require the use of an additional GPIO pin to control
 * the read and write modes of the ICs.
 */
//#define SN7516X
#ifdef SN7516X
  #define SN7516X_TE 7
//  #define SN7516X_DC 13
//  #define SN7516X_SC 12
#endif


/***** Bluetooth (HC05) support *****/
/*
 * Uses built-in LED on GPIO pin 13 to signal status
 */
/*
//#define AR_BT_EN 12             // Bluetooth enable and control pin
#ifdef AR_BT_EN
  #define AR_BT_BAUD 115200     // Bluetooth module preferred baud rate
  #define AR_BT_NAME "AR488-BT" // Bluetooth device name
  #define AR_BT_CODE "488488"   // Bluetooth pairing code
#endif
*/

/***** Storage devices *****/
/*
 * Only ONE storage device can be used at a time!
 */
#define EN_STORAGE
#ifdef EN_STORAGE
  #define SDCARD_CS_PIN 4  // SS pin on Pandauino 644-Narrow / 1284-Narrow (644P/1284P)
  #define SDCARD_CLK 16    // Clock speed in MHz
  #define EN_TEK_4924
//  #define EN_PET_9060
#endif

//#define SD_TEST 10


/***** Acknowledge interface is ready *****/
#define SAY_HELLO


/***** Debug options *****/

// Configure debug level
#ifdef DB_SERIAL_ENABLE
  //#define DEBUG_SERIAL_INPUT    // serialIn_h(), parseInput_h()
  #define DEBUG_CMD_PARSER      // getCmd
  //#define DEBUG_SEND_TO_INSTR   // sendToInstrument();
  //#define DEBUG_DEVICE_ATN      // attnRequired

//  //#define DEBUG_IDFUNC          // ID command
  
  // GPIB module
  //#define DEBUG_GPIBbus_RECEIVE
  //#define DEBUG_GPIBbus_SEND
//  //#define DEBUG_GPIBbus_CONTROL
//  //#define DEBUG_GPIBbus_COMMANDS

  // Store Module - General
  //#define DEBUG_STORE_EXEC
  //#define DEBUG_STORE_BINARYCOPY
  //#define DEBUG_STORE_COMMANDS
  //#define DEBUG_STORE_FILESEARCH
  //#define DEBUG_STORE_GETLAST
  //#define DEBUG_STORE_RENAME
  //#define DEBUG_STORE_MAKENEW
  //#define DEBUG_STORE_EXEC
  // Store Module - GPIB commands
  //#define DEBUG_STORE_APPEND
  //#define DEBUG_STORE_BINARYIO
  //#define DEBUG_STORE_CLOSE
  //#define DEBUG_STORE_COPY
  //#define DEBUG_STORE_DIR
  //#define DEBUG_STORE_ERROR
  //#define DEBUG_STORE_FIND
  //#define DEBUG_STORE_INPUT
  //#define DEBUG_STORE_KILL
  //#define DEBUG_STORE_MARK
  //#define DEBUG_STORE_READ
  //#define DEBUG_STORE_SAVE
  //#define DEBUG_STORE_STATUS
  //#define DEBUG_STORE_TLIST
  //#define DEBUG_STORE_TYPE
  //#define DEBUG_STORE_WRITE

#endif

/***** ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ *****/
/***** AR488 GLOBAL CONFIGURATION HEADER *****/
/*********************************************/


/*******************************/
/***** AR488 CUSTOM LAYOUT *****/
/***** vvvvvvvvvvvvvvvvvvv *****/
#ifdef AR488_CUSTOM

// AR488_CUSTOM defs for Pandauino 644-Narrow plugged into Monty's Tektronix 4924 Emulator GPIB PCB
#define DIO1  11   /* GPIB 1  */
#define DIO2  24   /* GPIB 2  */
#define DIO3  26   /* GPIB 3  */
#define DIO4  25   /* GPIB 4  */
#define DIO5  16   /* GPIB 13 */
#define DIO6  17   /* GPIB 14 */
#define DIO7  27   /* GPIB 15 */
#define DIO8  14   /* GPIB 16 */

#define IFC   12   /* GPIB 9  */
#define NDAC  3    /* GPIB 8  */
#define NRFD  2    /* GPIB 7  */
#define DAV   1    /* GPIB 6  */
#define EOI   0    /* GPIB 5  */

#define SRQ   15   /* GPIB 10 */
#define REN   13   /* GPIB 17 */
#define ATN   10   /* GPIB 11 */
#endif
/***** ^^^^^^^^^^^^^^^^^^^ *****/
/***** AR488 CUSTOM LAYOUT *****/
/*******************************/



/******************************************/
/***** !!! DO NOT EDIT BELOW HERE !!! *****/
/******vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv******/






#endif // AR488_CONFIG_H
