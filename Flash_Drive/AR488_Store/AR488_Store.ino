//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wtype-limits"
//#pragma GCC diagnostic ignored "-Wunused-variable"


#ifdef __AVR__
  #include <avr/wdt.h>
#endif

//#pragma GCC diagnostic pop

#include "AR488_Config.h"
#include "AR488_GPIBdevice.h"
#include "AR488_Layouts.h"
#include "AR488_SerialPorts.h"

#ifdef USE_INTERRUPTS
  #ifdef __AVR__
    #include <avr/interrupt.h>
  #endif
#endif


#ifdef EN_STORAGE
  #ifdef EN_TEK_4924
    #include "AR488_Store_Tek_4924.h"
  #endif
#endif


/***** FWVER "AR488 GPIB Storage, ver. 0.05.83, 28/06/2022" *****/

/*
  Arduino IEEE-488 implementation by John Chajecki

  Inspired by the original work of Emanuele Girlando, licensed under a Creative
  Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
  Any code in common with the original work is reproduced here with the explicit
  permission of Emanuele Girlando, who has kindly reviewed and tested this code.

  Thanks also to Luke Mester for comparison testing against the Prologix interface.
  AR488 is Licenced under the GNU Public licence.

  Thanks to 'maxwell3e10' on the EEVblog forum for suggesting additional auto mode
  settings and the macro feature.

  Thanks to 'artag' on the EEVblog forum for providing code for the 32u4.
*/

/*
   Implements most of the CONTROLLER functions;
   Substantially compatible with 'standard' Prologix "++" commands
   (see +savecfg command in the manual for differences)

   Principle of operation:
   - Commands received from USB are buffered and whole terminated lines processed
   - Interface commands prefixed with "++" are passed to the command handler
   - Instrument commands and data not prefixed with '++' are sent directly to the GPIB bus.
   - To receive from the instrument, issue a ++read command or put the controller in auto mode (++auto 1|2)
   - Characters received over the GPIB bus are unbuffered and sent directly to USB
   NOTES:
   - GPIB line in a HIGH state is un-asserted
   - GPIB line in a LOW state is asserted
   - The ATMega processor control pins have a high impedance when set as inputs
   - When set to INPUT_PULLUP, a 10k pull-up (to VCC) resistor is applied to the input
*/

/*
   Standard commands

   ++addr         - display/set device address
?   ++auto         - automatically request talk and read response
   ++eoi          - enable/disable assertion of EOI signal
   ++eos          - specify GPIB termination character
   ++eot_enable   - enable/disable appending user specified character to USB output on EOI detection
   ++eot_char     - set character to append to USB output when EOT enabled
   ++lon          - put controller in listen-only mode (listen to all traffic)
   ++read         - read data from instrument
   ++read_tmo_ms  - read timeout specified between 1 - 3000 milliseconds
   ++rst          - reset the controller
   ++savecfg      - save configration
   ++status       - set the status byte to be returned on being polled (bit 6 = RQS, i.e SRQ asserted)
   ++ver          - display firmware version
*/

/*
   Proprietry commands:

   ++default      - set configuration to controller default settings
   ++id name      - show/set the name of the interface
   ++id serial    - show/set the serial number of the interface
   ++id verstr    - show/set the version string (replaces setvstr)
   ++idn          - enable/disable reply to *idn? (disabled by default)
   ++setvstr      - set custom version string (to identify controller, e.g. "GPIB-USB"). Max 47 chars, excess truncated.
   ++ton          - put controller in talk-only mode (send data only)
   ++verbose      - verbose (human readable) mode
*/

/*
   NOT YET IMPLEMENTED

   ++help     - show summary of commands
*/

/*
   For information regarding the GPIB firmware by Emanualle Girlando see:
   http://egirland.blogspot.com/2014/03/arduino-uno-as-usb-to-gpib-controller.html
*/


/*
   Pin mapping between the Arduino pins and the GPIB connector.
   NOTE:
   GPIB pins 10 and 18-24 are connected to GND
   GPIB pin 12 should be connected to the cable shield (might be n/c)
   Pin mapping follows the layout originally used by Emanuelle Girlando, but adds
   the SRQ line (GPIB 10) on pin 2 and the REN line (GPIB 17) on pin 13. The program
   should therefore be compatible with the original interface design but for full
   functionality will need the remaining two pins to be connected.
   For further information about the AR488 see:
*/


/*********************************/
/***** CONFIGURATION SECTION *****/
/***** vvvvvvvvvvvvvvvvvvvvv *****/
// SEE >>>>> Config.h <<<<<
/***** ^^^^^^^^^^^^^^^^^^^^^ *****/
/***** CONFIGURATION SECTION *****/
/*********************************/



/**********************************/
/***** SERIAL PORT MANAGEMENT *****/
/***** vvvvvvvvvvvvvvvvvvvvvv *****/


/***** PARSE BUFFERS *****/
/*
 * Note: Arduino serial input buffer is 64 
 */
// Serial input parsing buffer
static const uint8_t PBSIZE = 128;
char pBuf[PBSIZE];
uint8_t pbPtr = 0;


/***** ^^^^^^^^^^^^^^^^^^^^^^ *****/
/***** SERIAL PORT MANAGEMENT *****/
/**********************************/



/************************************/
/***** COMMON VARIABLES SECTION *****/
/***** vvvvvvvvvvvvvvvvvvvvvvvv *****/

/****** Process status values *****/
#define OK 0
#define ERR 1

/***** Control characters *****/
#define ESC  0x1B   // the USB escape char
#define CR   0xD    // Carriage return
#define LF   0xA    // Newline/linefeed
#define PLUS 0x2B   // '+' character



//extern union AR488conf AR488;



/****** Global variables with volatile values related to controller state *****/

// GPIB bus object
GPIBbus gpibBus;

// Verbose mode
//bool isVerb = false;

// CR/LF terminated line ready to process
uint8_t lnRdy = 0;      

// GPIB data receive flags
//bool isReading = false;       // Is a GPIB read in progress?
bool autoReading = false;     // GPIB data read in progress
bool readWithEoi = false;     // Read eoi requested
bool readWithEndByte = false; // Read with specified terminator character
bool isQuery = false;         // Direct instrument command is a query
uint8_t tranBrk = 0;          // Transmission break on 1=++, 2=EOI, 3=ATN 4=UNL
uint8_t endByte = 0;          // Termination character

// Device mode - send data
bool snd = false;

// Escaped character flag
bool isEsc = false;           // Charcter escaped
bool isPlusEscaped = false;   // Plus escaped

// Read only mode flag
bool isRO = false;

// Talk only mode flag
bool isTO = false;

// GPIB command parser
bool aTt = false;
bool aTl = false;

// Data send mode flags
bool dataBufferFull = false;    // Flag when parse buffer is full

// State flags set by interrupt being triggered
//extern volatile bool isATN;  // has ATN been asserted?
//extern volatile bool isSRQ;  // has SRQ been asserted?

// SRQ auto mode
bool isSrqa = false;

// Interrupt without handler fired
//volatile bool isBAD = false;

// Whether to run Macro 0 (macros must be enabled)
uint8_t runMacro = 0;

// Send response to *idn?
bool sendIdn = false;


#ifdef EN_STORAGE
  // SD card storage
SDstorage storage;
#endif


/***** ^^^^^^^^^^^^^^^^^^^^^^^^ *****/
/***** COMMON VARIABLES SECTION *****/
/************************************/



/*******************************/
/***** COMMON CODE SECTION *****/
/***** vvvvvvvvvvvvvvvvvvv *****/


/******  Arduino standard SETUP procedure *****/
void setup() {

  // Disable the watchdog (needed to prevent WDT reset loop)
#ifdef __AVR__
  wdt_disable();
#endif

  // Turn off internal LED (set OUPTUT/LOW)
#ifdef LED_BUILTIN
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
#endif

#ifdef USE_INTERRUPTS
  // Turn on interrupts on port
  interruptsEn();
#endif

  // Initialise parse buffer
  flushPbuf();


// Initialise comms port
  DATAPORT_START();


// Initialise debug port
  DEBUG_START();


// Un-comment for diagnostic purposes
/* 
  #if defined(__AVR_ATmega32U4__)
    while(!*arSerial)
    ;
//    Serial.print(F("Starting "));
    for(int i = 0; i < 20; ++i) {  // this gives you 10 seconds to start programming before it crashes
      Serial.print(".");
      delay(500);
    }
    Serial.println("@>");
  #endif // __AVR_ATmega32U4__
*/
// Un-comment for diagnostic purposes


  // Initialise GPIB
  initGpibCfg();

  // SN7516x IC support
#ifdef SN7516X
  pinMode(SN7516X_TE, OUTPUT);
  #ifdef SN7516X_DC
    pinMode(SN7516X_DC, OUTPUT);
  #endif
  // Set listen mode on SN75161/2 (default)
  digitalWrite(SN7516X_TE, HIGH);
  #ifdef SN7516X_DC
    digitalWrite(SN7516X_DC, HIGH);
  #endif
  #ifdef SN7516X_SC
    digitalWrite(SN7516X_SC, LOW);
  #endif
#endif

  // Initialize the interface in device mode
  gpibBus.begin();
//  gpibBus.setControls(DLAS);  // Device listner active state

//  isATN = false;
//  isSRQ = false;


#ifdef SAY_HELLO
  DATA_RAW_PRINT(F("AR488-Store ready "));
  if (gpibBus.cfg.cmode==1) { DATA_RAW_PRINTLN(F("(device).") ); };
#endif

//  DB_PRINTX(F("Hello "), );

}
/****** End of Arduino standard SETUP procedure *****/


/***** ARDUINO MAIN LOOP *****/
void loop() {


/*** Pin Hooks ***/
/*
 * Not all boards support interrupts or have PCINTs. In this
 * case, use in-loop checking to detect when SRQ and ATN have 
 * been signalled
 */
#ifndef USE_INTERRUPTS
//  isATN = (digitalRead(ATN)==LOW ? true : false);
//  isSRQ = (digitalRead(SRQ)==LOW ? true : false);
#endif
  /*** In loop serial buffer checking ***/
  /* SerialEvent() handles the serial interrupt but some boards
     do not support SerialEvent. In this case use in-loop checking
  */


/*** Process the buffer ***/
/* Each received char is passed through parser until an un-escaped 
 * CR is encountered. If we have a command then parse and execute.
 * If the line is data (inclding direct instrument commands) then
 * send it to the instrument.
 * NOTE: parseInput() sets lnRdy in serialEvent, readBreak or in the
 * above loop
 * lnRdy=1: process command;
 * lnRdy=2: send data to Gpib
 */

  // lnRdy=1: received a command so execute it...
  if (lnRdy == 1) {
    execCmd(pBuf, pbPtr);
  }

  // Device mode:
  if (gpibBus.isAsserted(IFC)){
    delayMicroseconds(25);
/*
  }else if (isTO) {
    if (lnRdy == 2) sendToInstrument(pBuf, pbPtr);
  }else if (isRO) {
    lonMode();
*/
  }else if (gpibBus.isAsserted(ATN)){
    attnRequired();
    if (lnRdy == 2) sendToInstrument(pBuf, pbPtr);
/*    
  }else{
    if (!gpibBus.isDeviceInIdleState()) {
      gpibBus.setControls(DIDS);
    }
*/    
  }


  // Reset line ready flag
//  lnRdy = 0;

#ifdef DATAPORT_ENABLE
  // Check serial buffer
  lnRdy = serialIn_h();
#endif
  
  delayMicroseconds(5);
}
/***** END MAIN LOOP *****/


void initGpibCfg(){
  // Set default values from Config_h
  // ({'\0'} sets version string array to null)
  gpibBus.cfg = { GPIB_EOT_ENABLE,
                  GPIB_EOI_ENABLE,
                  1,                // Device mode
                  GPIB_ADDRESS,     // This interface address
                  GPIB_ADDRESS,     // Primary address to use when addressing instrument
                  0,                // Secondary address to use when addressing instrument
                  GPIB_EOS_MODE,
                  0,                // Status byte
                  GPIB_AUTO_MODE,
                  GPIB_RTMO,
                  GPIB_EOT_CHAR,
                  {'\0'},           // Custom version string
                  GPIB_EOR_MODE,
                  {'\0'},           // Interface short name
                  0,                // Interface serial
                  0                 // Send IDN (0-disable, 1-yes,name or 2-yes,name+serial)?
        }; 
}


/***** Serial event handler *****/
/*
 * Note: the Arduino serial buffer is 64 characters long. Characters are stored in
 * this buffer until serialEvent_h() is called. parsedInput() takes a character at 
 * a time and places it into the 256 character parse buffer whereupon it is parsed
 * to determine whether a command or data are present.
 * lnRdy=0: terminator not detected yet
 * lnRdy=1: terminator detected, sequence in parse buffer is a ++ command
 * lnRdy=2: terminator detected, sequence in parse buffer is data or direct instrument command
 */ 
#ifdef DATAPORT_ENABLE
uint8_t serialIn_h() {
  uint8_t bufferStatus = 0;
  // Parse serial input until we have detected a line terminator
  while (dataPort.available() && bufferStatus==0) {   // Parse while characters available and line is not complete
    bufferStatus = parseInput(dataPort.read());
  }

#ifdef DEBUG_SERIAL_INPUT
  if (bufferStatus) {
    DB_RAW_PRINT(F("BufferStatus: "));
    DB_RAW_PRINTLN(bufferStatus);
  }
#endif

  return bufferStatus;
}
#endif


/*************************************/
/***** Device operation routines *****/
/*************************************/


/***** Unrecognized command *****/
void errBadCmd() {
  DB_PRINT(F("Unrecognized command"),"");
}


/***** Add character to the buffer and parse *****/
uint8_t parseInput(char c) {

  uint8_t r = 0;

  // Read until buffer full (buffer-size - 2 characters)
  if (pbPtr < PBSIZE) {
//    if (isVerb) DATA_RAW_PRINT(c);  // Humans like to see what they are typing...
    // Actions on specific characters
    switch (c) {
      // Carriage return or newline? Then process the line
      case CR:
      case LF:
        // If escaped just add to buffer
        if (isEsc) {
          addPbuf(c);
          isEsc = false;
        } else {
          // Carriage return on blank line?
          // Note: for data CR and LF will always be escaped
          if (pbPtr == 0) {
            flushPbuf();
//            if (isVerb) showPrompt();
            return 0;
          } else {
//            if (isVerb) dataPort.println();  // Move to new line
#ifdef DEBUG_CMD_PARSER
            DB_PRINT(F("Received: "), pBuf)
#endif
            // Buffer starts with ++ and contains at least 3 characters - command?
            if (pbPtr>2 && isCmd(pBuf) && !isPlusEscaped) {
              // Exclamation mark (break read loop command)
              if (pBuf[2]==0x21) {
                r = 3;
                flushPbuf();
              // Otherwise flag command received and ready to process 
              }else{
                r = 1;
              }
            // Buffer contains *idn? query and interface to respond
            }else if (pbPtr>3 && gpibBus.cfg.idn>0 && isIdnQuery(pBuf)){
              sendIdn = true;
              flushPbuf();
            // Buffer has at least 1 character = instrument data to send to gpib bus
            }else if (pbPtr > 0) {
              r = 2;
            }
            isPlusEscaped = false;
#ifdef DEBUG_CMD_PARSER
           DB_PRINT(F("R: "), r);
#endif
//            return r;
          }
        }
        break;
      case ESC:
        // Handle the escape character
        if (isEsc) {
          // Add character to buffer and cancel escape
          addPbuf(c);
          isEsc = false;
        } else {
          // Set escape flag
          isEsc  = true;  // Set escape flag
        }
        break;
      case PLUS:
        if (isEsc) {
          isEsc = false;
          if (pbPtr < 2) isPlusEscaped = true;
        }
        addPbuf(c);
//        if (isVerb) DATA_RAW_PRINT(c);
        break;
      // Something else?
      default: // any char other than defined above
//        if (isVerb) dataPort.print(c);  // Humans like to see what they are typing...
        // Buffer contains '++' (start of command). Stop sending data to serial port by halting GPIB receive.
        addPbuf(c);
        isEsc = false;
    }
  }
  if (pbPtr >= PBSIZE) {
    if (isCmd(pBuf) && !r) {  // Command without terminator and buffer full
/*      
      if (isVerb) {
        DATA_RAW_PRINT(F("ERROR - Command buffer overflow!"));
      }
*/      
      flushPbuf();
    }else{  // Buffer contains data and is full, so process the buffer (send data via GPIB)
      dataBufferFull = true;
      // Signal to GPIB object that more data will follow (suppress GPIB addressing)
      r = 2;
    }
  }
  return r;
}


/***** Is this a command? *****/
bool isCmd(char *buffr) {
  if (buffr[0] == PLUS && buffr[1] == PLUS) {
#ifdef DEBUG_CMD_PARSER
    DB_PRINT(F("Command detected."),"");
#endif
    return true;
  }
  return false;
}


/***** Is this an *idn? query? *****/
bool isIdnQuery(char *buffr) {
  if (strncmp(buffr, "*idn?", 5)==0) {
#ifdef DEBUG_CMD_PARSER
    DB_PRINT(F("Detected IDN query."),"");
#endif
    return true;
  }
  return false;
}


/***** ++read command detected? *****/
bool isRead(char *buffr) {
  char cmd[4];
  // Copy 2nd to 5th character
  for (int i = 2; i < 6; i++) {
    cmd[i - 2] = buffr[i];
  }
  // Compare with 'read'
  if (strncmp(cmd, "read", 4) == 0) return true;
  return false;
}


/***** Add character to the buffer *****/
void addPbuf(char c) {
  pBuf[pbPtr] = c;
  pbPtr++;
}


/***** Clear the parse buffer *****/
void flushPbuf() {
  memset(pBuf, '\0', PBSIZE);
  pbPtr = 0;
}


struct cmdRec { 
  const char* token; 
  void (*handler)(char *);
};


static cmdRec cmdHidx [] = { 
 
  { "addr",        addr_h      }, 
  { "auto",        amode_h     },
//  { "default",     (void(*)(char*)) default_h },
//  { "eoi",         eoi_h       },
//  { "eor",         eor_h       },
//  { "eos",         eos_h       },
//  { "eot_char",    eot_char_h  },
//  { "eot_enable",  eot_en_h    },
//  { "id",          id_h        },
//  { "idn",         idn_h       },
//  { "lon",         lon_h       },
//  { "macro",       macro_h     },
//  { "mode" ,       cmode_h     },
  { "read",        read_h      },
  { "read_tmo_ms", rtmo_h      },
//  { "repeat",      repeat_h    },
  { "rst",         (void(*)(char*)) rst_h },
//  { "savecfg",     (void(*)(char*)) save_h },
//  { "setvstr",     setvstr_h   },
  { "status",      stat_h      },
#ifdef EN_STORAGE
//  { "storage",     store_h     },
#endif
//  { "ton",         ton_h       },
  { "ver",         ver_h       },
//  { "verbose",     (void(*)(char*)) verb_h    },
//  { "tmbus",       tmbus_h     },
#ifdef EN_STORAGE  
  // Tek 4924 commands using Prologix ++ format
  { "find",        find_h      },
  { "tlist",       (void(*)(char*)) tlist_h     },
  { "cd",          cd_h        },
//  { "help",        help_h      },  //help for Tek 4924 commands
  { "old",         (void(*)(char*)) old_h       },
#endif
  { "xdiag",       xdiag_h     }
};


#ifdef EN_STORAGE

/***** FIND command (debug output to serial) *****/
void find_h(char * params){
  uint8_t fnum = atoi(params);
  if (storage.findFile(fnum)) {
    return;    
  }
  DATA_RAW_PRINTLN(F("Not found!"));
}


/***** TLIST command (debug output to serial *****/
void tlist_h(){
  storage.listFiles(dataPort);
}


/***** CD command (debug output to serial) *****/
void cd_h(char * params){
  char dir[13];
  if (params != NULL) {
    storage.setDirectory(params);
    strncpy(dir, params, 12);   // Copy max 12 characters (dir name = 10 plus characters + 2 slashes)
    storage.setDirectory(dir);  // Changes directory, closes previous file and sets index to 0
    dataPort.print(F("Dir now = "));
    dataPort.println(dir);
  } else {
    storage.getDirectory(dir);
    dataPort.println(dir);
  }
}


/*
void help_h(char * params){
}
*/


/***** OLD command (debug output to serial) *****/
void old_h(){
  storage.viewCurrentFile(dataPort);
}

#endif


/***** Show a prompt *****/
/*
void showPrompt() {
  // Print prompt
  dataPort.print("> ");
}
*/


/****** Send data to instrument *****/
/* Processes the parse buffer whenever a full CR or LF
 * and sends data to the instrument
 */
void sendToInstrument(char *buffr, size_t dsize) {

#ifdef DEBUG_SEND_TO_INSTR
  if (buffr[dsize] != LF) DB_RAW_PRINTLN();
//  debugStream.print(F("sendToInstrument: Received for sending: "));
//  printHex(buffr, dsize);
  DB_PRINT(F("Received for sending: "),"");
  DB_HEXB_PRINT(buffr, dsize);
#endif

  // Is this an instrument query command (string ending with ?)
  if (buffr[dsize-1] == '?') isQuery = true;

  // Send string to instrument
  gpibBus.sendData(buffr, dsize, true);

#ifdef DEBUG_SEND_TO_INSTR
  DB_PRINT(F("Sent."),"");
#endif
 
  // Show a prompt on completion?
/*  
  if (isVerb) {
      showPrompt();
  }
*/

  // Flush the parse buffer
  flushPbuf();
  lnRdy = 0;
}


/***** Execute a command *****/
void execCmd(char *buffr, size_t dsize) {
  char line[PBSIZE];

  // Copy collected chars to line buffer
  memcpy(line, buffr, dsize);

  // Flush the parse buffer
  flushPbuf();
  lnRdy = 0;

#ifdef DEBUG_CMD_PARSER
  DB_PRINT(F("Command received: "),"");
  DB_HEXB_PRINT(line, dsize);
#endif

  // Its a ++command so shift everything two bytes left (ignore ++) and parse
  for (size_t i = 0; i < dsize-2; i++) {
    line[i] = line[i + 2];
  }
  // Replace last two bytes with a null (\0) character
  line[dsize - 2] = '\0';
  line[dsize - 1] = '\0';
#ifdef DEBUG_CMD_PARSER
  DB_PRINT(F("Sent to command processor: "),"");
  DB_HEXB_PRINT(line, dsize-2);
#endif
  // Execute the command
//  if (isVerb) dataPort.println(); // Shift output to next line
  getCmd(line);

  // Show a prompt on completion?
//  if (isVerb) showPrompt();
}


/***** Extract command and pass to handler *****/
void getCmd(char *buffr) {

  char *token;  // Pointer to command token
  char *params; // Pointer to parameters (remaining buffer characters)
  
  int casize = sizeof(cmdHidx) / sizeof(cmdHidx[0]);
  int i = 0;

#ifdef DEBUG_CMD_PARSER
//  debugStream.print("getCmd: ");
//  debugStream.print(buffr); debugStream.print(F(" - length:")); debugStream.println(strlen(buffr));
  DB_PRINT(F("Buffer: "), buffr);
  DB_PRINT(F("Length: "), strlen(buffr));
#endif

  // If terminator on blank line then return immediately without processing anything 
  if (buffr[0] == 0x00) return;
  if (buffr[0] == CR) return;
  if (buffr[0] == LF) return;

  // Get the first token
  token = strtok(buffr, " \t");

#ifdef DEBUG_CMD_PARSER
  DB_PRINT(F("Process token: "), token);
#endif

  // Check whether it is a valid command token
  i = 0;
  do {
    if (strcasecmp(cmdHidx[i].token, token) == 0) break;
    i++;
  } while (i < casize);

  if (i < casize) {
    // We have found a valid command and handler
#ifdef DEBUG_CMD_PARSER
   DB_PRINT(F("Found handler for: "), cmdHidx[i].token);
#endif
    // If command is found then execute it
    // If its a command with parameters
    // Copy command parameters to params and call handler with parameters
    params = token + strlen(token) + 1;
  
    // If command parameters were specified
    if (strlen(params) > 0) {
#ifdef DEBUG_CMD_PARSER
      DB_PRINT(F("Calling handler with parameters: "), params);
#endif
      // Call handler with parameters specified
      cmdHidx[i].handler(params);
        
    }else{
      // Call handler without parameters
      cmdHidx[i].handler(NULL);
    }
  } else {
    // No valid command found
    errBadCmd();
  }
}


/***** Check whether a parameter is in range *****/
/* Convert string to integer and check whether value is within
 * lowl to higl inclusive. Also returns converted text in param
 * to a uint16_t integer in rval. Returns true if successful, 
 * false if not
*/
bool notInRange(char *param, uint16_t lowl, uint16_t higl, uint16_t &rval) {

  // Null string passed?
  if (strlen(param) == 0) return true;

  // Convert to integer
  rval = 0;
  rval = atoi(param);

  // Check range
  if (rval < lowl || rval > higl) {
    errBadCmd();
/*    
    if (isVerb) {
      dataPort.print(F("Valid range is between "));
      dataPort.print(lowl);
      dataPort.print(F(" and "));
      dataPort.println(higl);
    }
*/    
    return true;
  }
  return false;
}


/*************************************/
/***** STANDARD COMMAND HANDLERS *****/
/*************************************/

/***** Show or change device address *****/
void addr_h(char *params) {
  //  char *param, *stat;
  uint16_t val;
  if (params != NULL) {

    // Primary address
    if (notInRange(params, 1, 30, val)) return;
    if (val == gpibBus.cfg.caddr) {
      errBadCmd();
//      if (isVerb) dataPort.println(F("That is my address! Address of a remote device is required."));
      return;
    }
    gpibBus.cfg.paddr = val;
/*  
    if (isVerb) {
      dataPort.print(F("Set device primary address to: "));
      dataPort.println(val);
    }
*/

  } else {
//    dataPort.println(gpibBus.cfg.paddr);
    DATA_RAW_PRINTLN(gpibBus.cfg.paddr);
  }
}


/***** Show or set read timout *****/
void rtmo_h(char *params) {
  uint16_t val;
  if (params != NULL) {
    if (notInRange(params, 1, 32000, val)) return;
    gpibBus.cfg.rtmo = val;
/*    
    if (isVerb) {
      dataPort.print(F("Set [read_tmo_ms] to: "));
      dataPort.print(val);
      dataPort.println(F(" milliseconds"));
    }
*/    
  } else {
//    dataPort.println(gpibBus.cfg.rtmo);
    DATA_RAW_PRINTLN(gpibBus.cfg.rtmo);
  }
}


/***** Show or enable/disable auto mode *****/
void amode_h(char *params) {
  uint16_t val;
  if (params != NULL) {
    if (notInRange(params, 0, 3, val)) return;
/*   
    if (val > 0 && isVerb) {
      dataPort.println(F("WARNING: automode ON can cause some devices to generate"));
      dataPort.println(F("         'addressed to talk but nothing to say' errors"));
    }
*/    
    gpibBus.cfg.amode = (uint8_t)val;
    if (gpibBus.cfg.amode < 3) autoReading = false;
/*    
    if (isVerb) {
      dataPort.print(F("Auto mode: "));
      dataPort.println(gpibBus.cfg.amode);
    }
*/    
  } else {
    DATA_RAW_PRINTLN(gpibBus.cfg.amode);
  }
}



/***** Display the controller version string *****/
void ver_h(char *params) {
  // If "real" requested
  if (params != NULL && strncasecmp(params, "real", 3) == 0) {
    DATA_RAW_PRINTLN(F(FWVER));
    // Otherwise depends on whether we have a custom string set
  } else {
    if (strlen(gpibBus.cfg.vstr) > 0) {
      DATA_RAW_PRINTLN(gpibBus.cfg.vstr);
    } else {
      DATA_RAW_PRINTLN(F(FWVER));
    }
  }
}


/***** Address device to talk and read the sent data *****/
void read_h(char *params) {
  // Clear read flags
  readWithEoi = false;
  readWithEndByte = false;
  endByte = 0;
  // Read any parameters
  if (params != NULL) {
    if (strlen(params) > 3) {
//      if (isVerb) dataPort.println(F("Invalid termination character - ignored!")); void addr_h(char *params);
    } else if (strncmp(params, "eoi", 3) == 0) { // Read with eoi detection
      readWithEoi = true;
    } else { // Assume ASCII character given and convert to an 8 bit byte
      readWithEndByte = true;
      endByte = atoi(params);
    }
  }
  if (gpibBus.cfg.amode == 3) {
    // In auto continuous mode we set this flag to indicate we are ready for continuous read
    autoReading = true;
  } else {
    // If auto mode is disabled we do a single read
    gpibBus.receiveData(dataPort, readWithEoi, readWithEndByte, endByte);
  }
}


/***** Reset the controller *****/
/*
 * Arduinos can use the watchdog timer to reset the MCU
 * For other devices, we restart the program instead by
 * jumping to address 0x0000. This is not a hardware reset
 * and will not reset a crashed MCU, but it will re-start
 * the interface program and re-initialise all parameters. 
 */
void rst_h() {
#ifdef WDTO_1S
  // Where defined, reset controller using watchdog timeout
  unsigned long tout;
  tout = millis() + 2000;
  wdt_enable(WDTO_1S);
  while (millis() < tout) {};
  // Should never reach here....
/*  
  if (isVerb) {
    dataPort.println(F("Reset FAILED."));
  };
*/  
#else
  // Otherwise restart program (soft reset)
  asm volatile ("  jmp 0");
#endif
}


/***** Set the status byte (device mode) *****/
void stat_h(char *params) {
  uint16_t val = 0;
  // A parameter given?
  if (params != NULL) {
    // Byte value given?
    if (notInRange(params, 0, 255, val)) return;
    gpibBus.setStatus((uint8_t)val);
  } else {
    // Return the currently set status byte
    DATA_RAW_PRINTLN(gpibBus.cfg.stat);
  }
}

/***********************************/
/***** CUSTOM COMMAND HANDLERS *****/
/***********************************/


/***** Bus diagnostics *****/
/*
 * Usage: xdiag mode byte
 * mode: 0=data bus; 1=control bus
 * byte: byte to write on the bus
 * Note: values to switch individual bits = 1,2,4,8,10,20,40,80
 */
void xdiag_h(char *params){
  char *param;
  uint8_t mode = 0;
  uint8_t val = 0;
  
  // Get first parameter (mode = 0 or 1)
  param = strtok(params, " \t");
  if (param != NULL) {
    if (strlen(param)<4){
      mode = atoi(param);
      if (mode>2) {
        DATA_RAW_PRINTLN(F("Invalid: 0=data bus; 1=control bus"));
        return;
      }
    }
  }
  // Get second parameter (8 bit byte)
  param = strtok(NULL, " \t");
  if (param != NULL) {
    if (strlen(param)<4){
      val = atoi(param);
    }

    if (mode) {   // Control bus
      // Set to required state
      gpibBus.setControlVal(0xFF, 0xFF, 1);  // Set direction
      gpibBus.setControlVal(~val, 0xFF, 0);  // Set state (low=asserted so must be inverse of value)
      // Reset after 10 seconds
      delay(10000);  
      gpibBus.setControls(DIDS);
    }else{        // Data bus
      // Set to required value
      gpibBus.setDataVal(val);
      // Reset after 10 seconds
      delay(10000);
      gpibBus.setDataVal(0);
    }
  }
}


/******************************************************/
/***** Device mode GPIB command handling routines *****/
/******************************************************/

/***** Attention handling routine *****/
/*
 * In device mode is invoked whenever ATN is asserted
 */
void attnRequired() {

  const uint8_t cmdbuflen = 35;
  uint8_t cmdbytes[cmdbuflen] = {0};
  uint8_t db = 0;
  uint8_t stat = 0;
  bool eoiDetected = false;
  uint8_t gpibcmd = 0;
  uint8_t bytecnt = 0;
  uint8_t atnstat = 0;
  uint8_t ustat = 0;
  bool addressed = false;
  
#ifdef EN_STORAGE
  uint8_t saddrcmd = 0;
#endif
#ifdef DEBUG_DEVICE_ATN
  uint8_t cmdbyteslist[cmdbuflen] = {0};
  uint8_t listbytecnt = 0;
#endif

  // Get ready to listen
  // Set device listner active state (assert NDAC+NRFD (low), DAV=INPUT_PULLUP)
  gpibBus.setControls(DLAS);

  /***** ATN read loop *****/

  // Read bytes
  while ( (gpibBus.isAsserted(ATN)) && (bytecnt<cmdbuflen)  && (stat==0) ) {
    // Read the next byte from the bus, no EOI detection
    stat = gpibBus.readByte(&db, false, &eoiDetected);
    if (!stat) {
      // Untalk or unlisten
      if ( (db == 0x5F) || (db == 0x3F) ) {
        if (db == 0x3F) {
          if (device_unl_h()) ustat |= 0x01;
        }
        if (db == 0x5F) {
          if (device_unt_h()) ustat |= 0x02; 
        }
      // Other command
      }else{
        cmdbytes[bytecnt] = db;
        bytecnt++;
      }
#ifdef DEBUG_DEVICE_ATN
      cmdbyteslist[listbytecnt] = db;
      listbytecnt++;
#endif
    }
  }

  atnstat |= 0x01;

  /***** Command process loop *****/

  if (bytecnt>0) {  // Some command tokens to process

    // Process received command tokens
    for (uint8_t i=0; i<bytecnt; i++) { 

      if (!cmdbytes[i]) break;  // End loop on zero

      db = cmdbytes[i];

      // Device is addressed to listen
      if (gpibBus.cfg.paddr == (db ^ 0x20)) { // MLA = db^0x20
        atnstat |= 0x02;
        addressed = true;
        gpibBus.setControls(DLAS);

      // Device is addressed to talk
      }else if (gpibBus.cfg.paddr == (db ^ 0x40)) { // MLA = db^0x40
        // Call talk handler to send data
        atnstat |= 0x04;
        addressed = true;
        gpibBus.setControls(DTAS);

#ifdef EN_STORAGE
      }else if (db>0x5F && db<0x80) {
        // Secondary addressing command received
        if (addressed) { // If we have been addressed (talk or listen)
          saddrcmd = db;
          atnstat |= 0x10;
        }
#endif

      }else if (db<0x20){
        // Primary command received
        if (addressed) { // If we have been addressed (talk or listen)
          gpibcmd = db;
          atnstat |= 0x08;
        }
      }
    }   // End for

    // If we have not been adressed then back to idle and exit loop
    if (!addressed) {
      gpibBus.setControls(DINI);
      return;
    }

    // If we have been adressed, then execute commands

    /***** Perform GPIB primary command actions *****/
    if (gpibcmd) {
      // Respond to GPIB command
      execGpibCmd(gpibcmd);
      // Clear flags
      gpibcmd = 0;
      atnstat |= 0x20;
    }

    /***** Perform GPIB secondry address command actions *****/
#ifdef EN_STORAGE
    if (saddrcmd) {
      // Execute the GPIB secondary addressing command
      storage.storeExecCmd(saddrcmd);
      // Clear secondary address command
      saddrcmd = 0;
      atnstat |= 0x40;
    }
#endif

    /***** Otherwise perform controller mode read or write *****/
    if (gpibBus.cfg.cmode == 2) { 

      // Listen for data
      if (gpibBus.isDeviceAddressedToListen()) {
/*
#ifdef DEBUG_DEVICE_ATN
          DB_PRINT(F("Listening..."),);
#endif
*/
        device_listen_h();
        atnstat |= 0x80;
      }

      // Talk (send data)
      if (gpibBus.isDeviceAddressedToTalk()) {
/*
#ifdef DEBUG_DEVICE_ATN
          DB_PRINT(F("Talking..."),);
#endif
*/
        device_talk_h();
        atnstat |= 0x80;
          
      }

    }  // End mode = 2

#ifdef DEBUG_DEVICE_ATN
  }else{
    DB_PRINT(F("No command to process!"),"");
#endif
  }

#ifdef DEBUG_DEVICE_ATN
  showATNStatus(atnstat, ustat, cmdbyteslist, listbytecnt, stat);
  DB_PRINT(F("END attnReceived.\n\n"),"");
#endif

}


#ifdef DEBUG_DEVICE_ATN
void showATNStatus(uint8_t atnstat, uint8_t ustat, uint8_t atnbytes[], size_t bcnt, uint8_t stat) {

  if (ustat & 0x01) DB_PRINT(F("unlistened."),"");
  if (ustat & 0x02) DB_PRINT(F("untalked."),"");

  if (atnstat & 0x01) DB_PRINT(F("ATN read loop completed."),"");
  if (atnstat & 0x02) DB_PRINT(F("addressed to LISTEN."),"");
  if (atnstat & 0x04) DB_PRINT(F("addressed to TALK."),"");
  if (atnstat & 0x08) DB_PRINT(F("primary command received."),"");
  if (atnstat & 0x10) DB_PRINT(F("secondary command received."),"");
  if (atnstat & 0x20) DB_PRINT(F("primary command done."),"");
  if (atnstat & 0x40) DB_PRINT(F("secondary command done."),"");
  if (atnstat & 0x80) DB_PRINT(F("attnRequired done."),"");

  DB_PRINT(F("commands received:"),"");
  for (size_t i=0; i<bcnt; i++) {
    DB_HEX_PRINT(atnbytes[i]);
//    DB_RAW_PRINT(" ");
  }
  DB_RAW_PRINTLN();

  DB_PRINT(F("ATN loop end."),"");
  DB_PRINT(bcnt, F("bytes read."));
  DB_PRINT(F("Status: "), stat);

}
#endif








/***** Execute GPIB command *****/
void execGpibCmd(uint8_t gpibcmd){

  // Respond to GPIB command
  switch (gpibcmd) {
    case GC_SPE:
      // Serial Poll enable request
#ifdef DEBUG_DEVICE_ATN
//        DB_PRINTLN(F("Received serial poll enable."));
#endif
        device_spe_h();
        break;
      case GC_SPD:
        // Serial poll disable request
#ifdef DEBUG_DEVICE_ATN
//        DB_PRINTLN(F("Received serial poll disable."));
#endif
        device_spd_h();
        break;
/*         
    case GC_UNL:
        // Unlisten
        device_unl_h();
        break;
    case GC_UNT:
        // Untalk
        device_unt_h();
        break;
*/
    case GC_SDC:
        // Device clear (reset)
        device_sdc_h();
        break;
#ifdef PIN_REMOTE
    case GC_LLO:
        // Remote lockout mode
        device_llo_h();
        break;
    case GC_GTL:
        // Local mode
        device_gtl_h();
        break;
#endif
  } // End switch
}


/***** Device is addressed to listen - so listen *****/
void device_listen_h(){
  // Receivedata params: stream, detectEOI, detectEndByte, endByte
  gpibBus.receiveData(dataPort, false, false, 0x0);
}


/***** Device is addressed to talk - so send data *****/
void device_talk_h(){
  if (lnRdy == 2) sendToInstrument(pBuf, pbPtr);
}


/***** Selected Device Clear *****/
void device_sdc_h() {
  // If being addressed then reset
#ifdef DEBUG_DEVICE_ATN
//  DB_PRINTLN(F("Reset request received... "));
#endif
  rst_h();
#ifdef DEBUG_DEVICE_ATN
  // Shouldn't get here!
//  DB_PRINTLN(F("Reset failed!"));
#endif
}


/***** Serial Poll Disable *****/
void device_spd_h() {
#ifdef DEBUG_DEVICE_ATN
//  DB_PRINTLN(F("<- serial poll request ended."));
#endif
//  gpibBus.setDeviceAddressedState(DIDS);
  gpibBus.setControls(DIDS);
}


/***** Serial Poll Enable *****/
void device_spe_h() {
#ifdef DEBUG_DEVICE_ATN
//  DB_PRINTLN(F("Serial poll request received from controller ->"));
#endif
  gpibBus.sendStatus();
#ifdef DEBUG_DEVICE_ATN
//  DB_PRINTLN(F("Status sent."));
#endif
  // Check if SRQ bit is set and clear it
  if (gpibBus.cfg.stat & 0x40) {
    gpibBus.setStatus(gpibBus.cfg.stat & ~0x40);
#ifdef DEBUG_DEVICE_ATN
//    DB_PRINTLN(F("SRQ bit cleared."));
#endif
  }
}


/***** Unlisten *****/
bool device_unl_h() {
  // Stop receiving and go to idle
  readWithEoi = false;
  // Immediate break - shouldn't ATN do this anyway?
  tranBrk = 3;  // Stop receving transmission
  // Clear addressed state flag and set controls to idle
  if (gpibBus.isDeviceAddressedToListen()) {
    gpibBus.setControls(DIDS);
    return true;
  }
  return false;
}


/***** Untalk *****/
bool device_unt_h(){
  // Stop sending data and go to idle
  if (gpibBus.isDeviceAddressedToTalk()) {
    gpibBus.setControls(DIDS);
    gpibBus.clearDataBus();
    return true;
  }
  return false;
}


#ifdef REMOTE_SIGNAL_PIN
/***** Enabled remote mode *****/
void device_llo_h(){
  digitalWrite(PIN_REMOTE, HIGH);
}


/***** Disable remote mode *****/
void device_gtl_h(){
  digitalWrite(PIN_REMOTE, LOW);
}
#endif


/***** Listen only mode *****/
/*
void lonMode(){
  // Receivedata params: stream, detectEOI, detectEndByte, endByte
  gpibBus.receiveData(dataPort, false, false, 0x0 );

  // Clear the buffer to prevent it getting blocked
  if (lnRdy==2) flushPbuf();
}
*/
