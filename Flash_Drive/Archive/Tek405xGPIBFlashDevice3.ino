/*
Arduino GPIB MicroSD Flash Drive DEVICE - Emulation of a Tektronix 4924 Tape Drive - Monty McGraw Dec 2020
*/

#define DEBUG_BUILD
//#define DEEP_DEBUG_BUILD 


/*************************
 Implements only Tektronix 4051 mode 4924 tape device functions using MSA secondary addressing
 - plus DIR,CREATE,CD,DELETE directory commands as MSA COMMAND(26) with ASCII command and directoryname parameter string
**************************

 USB is only used for GPIB command debug information - not file transfer
 MicroSD filesystem is the target for all the tape commands

 Flash Drive Device GPIB Address = 5, MLA5 = ASCII 37, MTA5 = ASCII 69, MSA commands add ASCII 96 to command number
 
        Tektronix 4050 MSA command (required with INPUT or PRINT, it is default secondary address for named BASIC commands)
        --                                                    Flash Drive MSA Command description (emulates Tektronix 4924 tape drive)
            4050 MSA GPIB command syntax--------------------- -------------------------------------------------------------------------
 FIND   27 (MLA 37)(MSA 123)(FileNumber+CR)(UNL)              Open file on MicroSD card (in current directory) with file number
                                                                  
 HEADER  9 (MTA 69)(MSA 105)(Header)(UNT)                     Return header to 4050 for open file and close it, 
                                                                  if none are open, return headers for all files (TLIST). 
                                                                  on last file send EOT condition

 TLIST   9 (MTA 69)(MSA 105)(Header)(UNT)                     return headers for all files in current directory (TLIST). If a file is open - close it
                                                                  on last file send EOT condition
 
 INPUT @5,11:  implied current directory DIR LIST command     MTA=69,MSA=107 Directory List - not file list - from current directory. GPIB INIT returns to \ directory
 PRINT @5,11: "CREATE DirectoryName"                          MLA=37,MSA=107 parse received string for CREATE/CD/DELETE and DirectoryName.
               CD     DirectoryName                               then do that command.  Delay next handshake until completed 
               DELETE DirectoryName

 TYPE   6  (MTA 69)(MSA 102)(Type)(Delim)(Length)(Delim)(UNT) Return integer with data type and data size if binary, else length=0 
 
 LISTEN 25 (MLA 37)(MSA 121)(Data)                            WBYTE(s) from 4051. Write all incoming data to current file.  Mark header "BINARY LOG".
                                                                  Continue until EOI received or EOF
 TALK   26 (MTA 69)(MSA 122)(Data)                            RBYTE(s) from 4051 or handshake from GPIB LISTENER. Transmit one byte for each handshake and 
                                                                  EOI on last byte in file
 STATUS 0  (MTA 69)(MSA 96)(ReturnedStatus)(UNT)              Input command from 4050, return four status numeric variables
 STATUS 0  (MLA 37)(MSA 96)(A,B,C,D)(UNL)                     Print command from 4050, receive four tape parameters
                                                                  ex: PRINT @5,0:0,0,1 disable Headers so they can be read with INPUT after FIND
                                                                  ex: PRINT @5,0:0,0,0 enable Headers for normal file operations 
 
 ERROR  30 (MTA 69)(MSA 126)(Error Code)(UNT)                 Return last error code and clear SRQ
 
 SECRET 29 (MLA 37)(MSA 125)(FileNumber)(CR)(UNL)             Mark header of open file SECRET. Header must be NEW OR ASCII PROGRAM
                                                                  and before a SAVE command
 KILL   7  (MLA 37)(MSA 103)(FileNumber)(CR)(UNL)             NULL contents of current file number, change Header for this file number
                                                                  to NEW with current header file length
 MARK   28 (MLA 37)(MSA 124)(NUMBER)(Delim)(SIZE)(CR)(UNL)    Create NUMBER of files at current FileNumber with SIZE, then create
                                                                  LAST file.  If end of media - terminate mark & last file is not valid
 CLOSE  2  (MLA 37)(MSA 98)(CR)(UNL)                          Close current file operation (also support EOI during SAVE or PRINT
                                                                  or WRITE as close command)
 OLD    4  (MTA 69)(MSA 100)(ASCII Data)(UNT)                 Send current ASCII file contents until EOF and send EOI to 4051.
                                                                  If not ASCII header or not open, generate error
 BOLD   17 (MTA 69)(MSA 113)(BINARY Data)(UNL)                Read binary program from file, close with EOI signal.
                                                                  
 SAVE   1  (MLA 37)(MSA 97)(ASCII Data)(UNL)                  Write ASCII strings to file, close on EOI signal. Header checked 
                                                                  for NEW or ASCII. Mark header ASCII PROGRAM
 BSAVE  17 (MLA 37)(MSA 113)(BINARY Data)(UNL)                Write binary data to file, close on EOI signal. Header checked 
                                                                  for NEW or ASCII. Mark header BINARY PROGRAM
 INPUT  13 (MTA 69)(MSA 109)(ASCII Data)(UNT)                 Send ASCII strings until UNT, IFC or EOF. Check for open and Header for ASCII
                                                                  If first command after FIND - return file header
 PRINT  12 (MTA 69)(MSA 108)(ASCII Data)(UNL)                 Write one ASCII string for each Print command. If Header is NEW
                                                                  mark ASCII Data. If Binary or Secret - error
                                                                  If first command after FIND - replace file header with ASCII string
 READ   14 (MTA 69)(MSA 110)(BINARY Data)(UNT)                Send BINARY data until UNT, IFC or EOF. 
                                                                  Check Header for BINARY and file open else error
 WRITE  15 (MLA 37)(MSA 111)(BINARY Data)(UNL)                Write BINARY data until UNL or EOF. 
                                                                  Check Header for BINARY or NEW else error
 
Delimiters - any character from the set: ? = , ; CR SP
CR is always a command (or reply?) terminator

TYPE command response values
----------------------------
0 Empty (new) file or file not open
1 End-of-File Character ASCII 127 ??
2 ASCII data (numeric or character string)
3 Binary Numeric data
4 Binary Character string

Tek 4050 file header is always the first ASCII string in a separate 256 byte record and can be read and written without disturbing rest of the file
  - GPIB Flash drive will write the Header to the filename of the flash file with no file extension
  - Drive will return the actual FAT filesize appended to the header
  - Drive will return the header on the first INPUT @5 command after the file is opened

FILE HEADER FORMAT:
         1    1    2    2    3    3
1...5....0....5....0....5....0....5....
nnn    BINARY  .................Sxxxxxx

where nnnnn is left justified file number with spaces from 1 to 255 (fractions are rounded)
      BINARY, ASCII, NEW, or LAST as file type
      ...... File Usage: PROGRAM or DATA.  This can also be a general comment area
      S means SECRET
      xxxxxx MARKed filesize in bytes

Some Tektronix file header comment programs start the comment at location 9 in the header, 
but comment characters 10-15 will be overwritten if the file is rewritten

FILE HEADER custom comment limitations:
- comment strings limited to 17 characters
- no numbers 				(error checking in 4050 header programs)
- no control characters		(error checking in 4050 header programs)
- no SdFat reserved characters <>:"/\|?*  (what do I replace current / marker in headers that have namestrings?)
	- Drive firmware will replace attempts to write reserved characters in header with '

**********************************************************
The Arduino SdFat library supports long filenames - so the filename will hold the entire header string (filesize will be appended)

The file extension will be blank.

SECRET command as first command on a NEW filename will add the S in the correct location in the header string.

The 8 ASCII Character filename will default to NEW for file just MARKed.  
This flash filename will be rewritten upon the first write to the file as follows:
if the first command on a NEW file is SAVE.   Drive will use "ASCII  PROG" in the saved header string
if the first command on a NEW file is PRINT   Drive will use "ASCII  DATA" in the saved header string
if the first command on a NEW file is LISTEN  Drive will use "ASCII  LOG"  in the saved header string
if the first command on a NEW file is WRITE   Drive will use "BINARY DATA" in the saved header string


Upon creation of a directory - a filename=LAST will be created with default length 0 bytes and file number of 001.  
This LAST file will be renumbered to the last file number upon MARKing new file(s).

If a PRINT @5,0:0,0,1 to disable Headers is received so the header can be read with INPUT after FIND, 
  then the subsequent PRINT @5 string will overwrite the previous saved header string as the new filename

A subsequent PRINT @5,0:0,0,0 will enable Headers for normal file operations - and is the default operation mode.

The first input or output statement after FIND will get the first record - since the header is the filename.

***********************************************************

TAPE STATUS: Four single character ASCII numbers with delimiters: Value either 0 or 1
    Value=0               Value=1
    --------------------  -----------------
A - 256 Bytes/record      128 Bytes/record		Will be ignored by the Drive
B - Checksum              No Checksum			Will be ignored by the Drive
C - Header                No Header
D - No-Read-After-Write   Read-After-Write		Will be ignored by the Drive

BINARY DATA
- Each data item contains its own header identifying the length of the item and the type
- Header is generated by the talker sending the information
- The two byte data header has the following form:

 MSB                            LSB   MSB                        LSB
|T3  T2  T1  L13  L12  L11  L10  L9 | L8  L7  L6  L5  L4  L3  L2  L1|
|           BYTE 1                  |             BYTE 2            |

L13 to L1 comprise the length of the item including the two header bytes

BINARY data types for READ and WRITE commands
----------
T3, T2, T1 are the types of BINARY data 
 0   0   0 Unassigned
 0   0   1 Binary number
 0   1   0 Binary string
 0   1   1 Unassigned
 1   0   0 Unassigned
 1   0   1 Unassigned
 1   1   0 Unassigned
 1   1   1 EOF

 Principle of operation of the GPIB Flash Drive:
 - Receives commands from the 405x always handshaking while ATN is low
 - changes mode to LISTEN or TALK based on primary address 5 where MLA5=ASCII 37, MTA5=ASCII 69
 - changes mode to IDLE on receipt of UNL=ASCII 63 or UNT=ASCII 95 primary address from controller
 - Decodes secondary address to MSA while ATN is still asserted to a Flash Drive command,
   subtract 96 from ASCII MSA address to get MSA command
 - changes mode to IDLE and closes open files and changes current working directory to / on receipt of IFC (BASIC INIT)

Errors:
 - Return EOI with NULL character if in talk mode and file is at end of file, or is empty or file number does not exist

 */

/*
 Together with the comments aside,these definitions define the mapping between the Arduino pins and the GPIB connector.
 It actually defines the hardware connections required for this sketch to work.
 NOTE:

 *********************
 CONNECT Arduino pin 10 (SS) to MicroSD chip select (CS).  SdFat will init this pin
 *********************
 GPIB pins 10, 17-24 goto GND
 GPIB pin 17 (REN) No connect. (not used by 4924 tape)
 GPIB pin 10 (SRQ) No connect. (not used by 4924 tape)
 GPIB pin 12  GND  Shield
 */
#define DIO1  A0  /* GPIB 1  : I/O data bit 1     - Arduino PORTC bit 0 */
#define DIO2  A1  /* GPIB 2  : I/O data bit 2     - Arduino PORTC bit 1 */
#define DIO3  A2  /* GPIB 3  : I/O data bit 3     - Arduino PORTC bit 2 */
#define DIO4  A3  /* GPIB 4  : I/O data bit 4     - Arduino PORTC bit 3 */
#define DIO5  A4  /* GPIB 13 : I/O data bit 5     - Arduino PORTC bit 4 */
#define DIO6  A5  /* GPIB 14 : I/O data bit 6     - Arduino PORTC bit 5 */
#define DIO7  4   /* GPIB 15 : I/O data bit 7     - Arduino PORTD bit 4 */
#define DIO8  5   /* GPIB 16 : I/O data bit 8     - Arduino PORTD bit 5 */
#define EOI   A6  /* GPIB 5  : End Or Identify    - Arduino PORTC bit 6 -  CHANGE from 12 to A6 to make room for SPI Flash */
#define DAV   3   /* GPIB 6  : Data Valid         - Arduino PORTD bit 3 -  CHANGE from 11 to 3  to make room for SPI Flash */
#define NRFD  2   /* GPIB 7  : Not Ready For Data - Arduino PORTD bit 2 -  CHANGE from 10 to 2  to make room for SPI Flash */
#define NDAC  9   /* GPIB 8  : Not Data Accepted  - Arduino PORTB bit 0 */
#define IFC   8   /* GPIB 9  : Interface Clear    - Arduino PORTB bit 1 */
#define ATN   7   /* GPIB 11 : Attention          - Arduino PORTD bit 7 */

/* Now 10,11,12,13 will be used for MicroSD Flash card.  11,12,13 will be picked up on ICSP connector, 10 wired separately
ICSP Pins:        MicroSD Pins:
  1 = MISO (12)   DO
  2 = +5V         +5V
  3 = SCK  (13)   CLK
  4 = MOSI (11)   DI
  5 = NC
  6 = GND         GND

Nano 10 is SS     CS

*/

/* 
 NOTE for the entire code: 
 Remember GPIB logic: HIGH means not active, deasserted; LOW means active, asserted; 
 also remember that ATmega's pins set as inputs are hi-Z (unless you enable the internal pullups).
 */
/*
GPIB BUS commands
*/


#define SUCCESS false
#define FAIL true
#define IN   1
#define OUT  0
#define CONTROLLER  false
#define DEVICE      true
/* 
***** GLOBAL BUFFERS 
*/
#define BUFFSIZE 128              // too short? 
char com[BUFFSIZE] = "",          // USB input string buffer
     *comp = com,                 // pointer floating in com buffer
     *come = com + BUFFSIZE - 1;  // pointer to the far end of com buffer
// NOTE: use of pointers rather than indexes is a personal (BAD) choice: it can lead to potential portability issues, but that's it.


#define ESC  0x1B         // the USB escape char for "+"s, CRs and NLs.
#define CR   0xD
#define NL   0xA
#define PLUS 0x2B
char *EOSs = "\r\n";      // string containing a list of all possible GPIB terminator chars.
char *DELIM = " ?=,;\r";   // ASCII string or number delimiters
boolean itwascmd=false;   // flag to know if the last USB input line was a "++" command or not.

/*
  Device state variables.
  
 */     
#define MYADDR 5          // my DEVICE gpib adress  */
int htimeout = 200;       // Handshake timeout. Also known as read_tmo_ms. Its actual value has to be found by
                          // trial&error depending on the actual application.

 
 
void setup() {

  Serial.begin(115200); 
  flush_serial();
  
  // initialize DEVICE gpib signals as tri-state inputs
    pinMode(ATN, INPUT_PULLUP);   
  Serial.print(" ATN=");   
  Serial.print(digitalRead(ATN));
  pinMode(DAV, INPUT_PULLUP);   
  Serial.print(", DAV=");  
  Serial.print(digitalRead(DAV));
  pinMode(NRFD, INPUT_PULLUP);  
  Serial.print(", NRFD="); 
  Serial.print(digitalRead(NRFD));
  pinMode(NDAC, INPUT_PULLUP);  
  Serial.print(", NDAC="); 
  Serial.print(digitalRead(NDAC));
  pinMode(EOI, INPUT_PULLUP);   
  Serial.print(", EOI=");  
  Serial.print(digitalRead(EOI));
  Serial.print(", DIO8-1=");
  pinMode(DIO8, INPUT_PULLUP);  
  Serial.print(digitalRead(DIO8));
  pinMode(DIO7, INPUT_PULLUP);  
  Serial.print(digitalRead(DIO7));
  pinMode(DIO6, INPUT_PULLUP);  
  Serial.print(digitalRead(DIO6));
  pinMode(DIO5, INPUT_PULLUP);  
  Serial.print(digitalRead(DIO5));
  pinMode(DIO4, INPUT_PULLUP);  
  Serial.print(digitalRead(DIO4));
  pinMode(DIO3, INPUT_PULLUP);  
  Serial.print(digitalRead(DIO3));
  pinMode(DIO2, INPUT_PULLUP);  
  Serial.print(digitalRead(DIO2));
  pinMode(DIO1, INPUT_PULLUP);  
  Serial.println(digitalRead(DIO1));
  
}


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
/////////// MAIN LOOP
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
void loop() {
  
  flush_serial();                        // ensure debug serial buffer is empty..
  if (verbose) Serial.println("IDLE");   
  
  waitATN(byte Role,byte MSA);          // wait for ATN to transition to active LOW and decode two address bytes for command
  
#ifdef DEBUG_BUILD
    Serial.print("Command:");
	Serial.println(cmd);
#endif      


    cmdparse(cmd);
 
} // end loop



/*
    command parser
 */
struct cmd_info { 
  char* opcode; 
  void (*handler)(void); 
};

static cmd_info cmds [] = { 
  { "ver" ,       print_ver   },
  { "find",       find_h      }, 
  { "header" ,    header_h    },
  { "type" ,      type_h      },
  { "listen" ,    listen_h    },
  { "talk",       talk_h      },
  { "getstatus",  getstatus_h },
  { "setstatus",  setstatus_h },
  { "error",      error_h     },
  { "secret",     secret_h    },
  { "kill",       kill_h      },
  { "mark",       mark_h      },
  { "close",      close_h     },
  { "old",        old_h       },
  { "save",       save_h      },
  { "input",      input_h     },
  { "print",      print_h     },
  { "read",       read_h      },
  { "write",      write_h     },
}; 

/*
  Called when a our primary and secondary address are decoded from GPIB during handshake
*/
void cmdparse() { 
cmd_info *cmdp,
         *cmde = cmds + sizeof( cmds ) / sizeof( cmds[0] );
char *param; // pointer to command parameter(s)
  
#ifdef DEBUG_BUILD
  Serial.print("Parse: "); 
  Serial.print(com); Serial.print(" - length:");Serial.println(strlen(com));
#endif

  if (*com == (NULL || CR || NL) ) return; // empty line: nothing to parse.
  
  param=strtok(com, " \t"); // additional calls to strtok possible in handlers routines
  for (cmdp = cmds; cmdp != cmde; cmdp++ ) {
#ifdef DEEP_DEBUG_BUILD
    Serial.print("Parse: "); 
    Serial.print(cmdp->opcode); Serial.print(" Vs ");Serial.println(param);
#endif  
    if( 0 == strcmp(cmdp->opcode, param) ) {    // found a valid command
      (*cmdp->handler)();                       // call the corresponding handler
      break;                                    // done.
    }
  }
  
  if (cmdp == cmde && verbose) {
    Serial.print(param); Serial.println(F(": unreconized command.")); 
  }

*com=NULL; comp=com; //Done.

}


/*
command handlers
 */
void find_h() { 
  char *param, *pend; 
  int temp;

  if ( (param=strtok(NULL, " \t")) ) {  
    // Serial.print("param: "); Serial.println(param);
    temp = strtol(param, &pend, 10);
    if (*pend != NULL) { if (verbose) Serial.println(F("Syntax error.")); return;}
    if (temp<1 || temp > 30) { 
        if (verbose) Serial.println(F("address out of range: valid address range is [1-30]."));return;} 
    addr=temp; if (!verbose) return;
  }
  Serial.println(addr); 
}

void talk_h() { 
  char *param, *pend; 
  int temp;

  if ( (param=strtok(NULL, " \t")) ) {  
    // Serial.print("param: "); Serial.println(param);
    temp = strtol(param, &pend, 10);
    if (*pend != NULL) { if (verbose) Serial.println(F("Syntax error.")); return;}
    if (temp<100 || temp > 9999) { 
        if (verbose) Serial.println(F("read_tmo_ms out of range: valid address range is [100-9999]."));return;}
    htimeout=temp; if (!verbose) return;
  }
  Serial.println(htimeout); 
}
void close_h() { 
  char *param, *pend; 
  int temp;

  if ( (param=strtok(NULL, " \t")) ) {  
    // Serial.print("param: "); Serial.println(param);
    temp = strtol(param, &pend, 10);
    if (*pend != NULL) { if (verbose) Serial.println(F("Syntax error.")); return;}
    if (temp<0|| temp > 3) { 
        if (verbose) Serial.println(F("eos out of range: valid address range is [0-3]."));return;}
    eos=temp; if (!verbose) return;
  }
  Serial.println(eos); 
}
void old_h() { 
  char *param, *pend; 
  int temp;

  if ( (param=strtok(NULL, " \t")) ) {  
    temp = strtol(param, &pend, 10);
    if (*pend != NULL) { if (verbose) Serial.println(F("Syntax error.")); return;}
    if (temp<0|| temp > 1) { 
        if (verbose) Serial.println(F("eoi: valid address range is [0|1]."));return;}
    eoi = temp ? true : false;  if (!verbose) return;
  }
  Serial.println(eoi); 
}
void listen_h() { 
  char *param, *pend; 
  int temp;

  if ( (param=strtok(NULL, " \t")) ) {  
    temp = strtol(param, &pend, 10);
    if (*pend != NULL) { if (verbose) Serial.println(F("Syntax error.")); return;}
    if (temp<0|| temp > 1) { 
        if (verbose) Serial.println(F("mode: valid address range is [0|1]."));return;}
    // cmode = temp ? CONTROLLER : DEVICE;  if (!verbose) return;
       cmode=DEVICE;  // only controller mode implemented here... 
  }
  Serial.println(cmode); 
}
void save_h() { 
  char *param, *pend; 
  int temp;

  if ( (param=strtok(NULL, " \t")) ) {  
    temp = strtol(param, &pend, 10);
    if (*pend != NULL) { if (verbose) Serial.println(F("Syntax error.")); return;}
    if (temp<0|| temp > 1) { 
        if (verbose) Serial.println(F("eoi: valid address range is [0|1]."));return;}
    eot_enable = temp ? true : false;  if (!verbose) return;
  }
  Serial.println(eot_enable); 
}
void input_h() { 
  char *param, *pend; 
  int temp;

  if ( (param=strtok(NULL, " \t")) ) {  
    temp = strtol(param, &pend, 10);
    if (*pend != NULL) { if (verbose) Serial.println(F("Syntax error.")); return;}
    if (temp<0|| temp > 256) { 
        if (verbose) Serial.println(F("eot_char: valid address range is [0-256]."));return;}
    eot_char = temp;  if (!verbose) return;
  }
  Serial.println((byte)eot_char); 
}

void getstatus_h() { 
char *param, *pend; 
int temp;

  if ( (param=strtok(NULL, " \t")) ) {  
    temp = strtol(param, &pend, 10);
    if (*pend != NULL) { if (verbose) Serial.println(F("Syntax error.")); return;}
    if (temp<0|| temp > 1) { 
        if (verbose) Serial.println(F("automode: valid address range is [0|1]."));return;}
    automode = temp ? true : false;  if (!verbose) return;
    !automode ? 0 : Serial.println(F("WARNING: automode ON can generate \"addressed to talk but nothing to say\" errors in the devices \nor issue read command too soon."));
  }
  Serial.println(automode); 
}

void print_ver() {
  Serial.println(F("ARDUINO GPIB DEVICE mode by Monty McGraw June 2018")); 
}
void setstatus_h() { 
char *param, *pend; 
int temp;

  if ( (param=strtok(NULL, " \t")) ) 
    if (0 == strncmp(param, "eoi", 3)) eoi_only=true;
  gpibReceive();
  eoi_only=false;
}


void error_h() { 
  if ( sendGPIB_Acmd(SDC) )  { 
    Serial.println(F("error_h: sendGPIB_Acmd failed")); 
    return; 
  }  
}
void kill_h() {
   if ( sendGPIB_Ucmd(LLO) )  { 
    Serial.println(F("kill_h: sendGPIB_Ucmd failed")); 
    return; 
  }  
}

/*
The (Universal) Device Clear (DCL) can be sent by any active controller and is recognized by all devices,
 however it is a message to the device rather than to its interface. 
 So it is left to the device how to react on a Universal Device Clear.
 There is no assumption (at least not in IEEE-488.1) what the device should do (it can even ignore the DCL).
 */
 void print_h() {
   if ( sendGPIB_Ucmd(DCL) )  { 
    Serial.println(F("print_h: sendGPIB_Ucmd failed")); 
    return; 
  }  
}
void mark_h() {
  if ( sendGPIB_Acmd(GTL) )  { 
    Serial.println(F("mark_h: sendGPIB_Acmd failed")); 
    return; 
  }  
}
 
/*
 The Interface Clear (IFC) is conducted by asserting the IFC line for at least 100 milliseconds, 
 which is reserved to the system controller. 
 The defined effect is that all interfaces connected to the bus return to their idle state 
 (putting the whole bus into a quiescent state), 
 and the controller role is returned to the system controller (if there is another controller active).
 */
void read_h() { 
  gpibIFC(); 
}

void write_h() { 
  gpibIFC(); 
}
void secret_h() { 
if ( sendGPIB_Acmd(GET) )  { 
    Serial.println(F("secret_h: sendGPIB_Acmd failed")); 
    return; 
  }  

}

void type_h() { 
    verbose = !verbose;
    Serial.print("verbose: ");
    Serial.println(verbose ? "ON" : "OFF");
}
/*
    end of command handlers
 */

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
/////////// GPIB LOW LEVEL ROUTINES
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
#ifdef DEBUG_BUILD
void gpibLineStatus(void) {
  pinMode(ATN, INPUT_PULLUP);   
  Serial.print(" ATN=");   
  Serial.print(digitalRead(ATN));
  pinMode(DAV, INPUT_PULLUP);   
  Serial.print(", DAV=");  
  Serial.print(digitalRead(DAV));
  pinMode(NRFD, INPUT_PULLUP);  
  Serial.print(", NRFD="); 
  Serial.print(digitalRead(NRFD));
  pinMode(NDAC, INPUT_PULLUP);  
  Serial.print(", NDAC="); 
  Serial.print(digitalRead(NDAC));
  pinMode(EOI, INPUT_PULLUP);   
  Serial.print(", EOI=");  
  Serial.print(digitalRead(EOI));
  Serial.print(", DIO8-1=");
  pinMode(DIO8, INPUT_PULLUP);  
  Serial.print(digitalRead(DIO8));
  pinMode(DIO7, INPUT_PULLUP);  
  Serial.print(digitalRead(DIO7));
  pinMode(DIO6, INPUT_PULLUP);  
  Serial.print(digitalRead(DIO6));
  pinMode(DIO5, INPUT_PULLUP);  
  Serial.print(digitalRead(DIO5));
  pinMode(DIO4, INPUT_PULLUP);  
  Serial.print(digitalRead(DIO4));
  pinMode(DIO3, INPUT_PULLUP);  
  Serial.print(digitalRead(DIO3));
  pinMode(DIO2, INPUT_PULLUP);  
  Serial.print(digitalRead(DIO2));
  pinMode(DIO1, INPUT_PULLUP);  
  Serial.println(digitalRead(DIO1));

  //reset DEVICE GPIB control pins to tri-state inputs at inital status
  pinMode(ATN, INPUT_PULLUP);
  digitalWrite(ATN, HIGH);
  pinMode(EOI, INPUT_PULLUP); 
  digitalWrite(EOI, HIGH);
  pinMode(DAV, INPUT_PULLUP);  
  digitalWrite(DAV, HIGH);
  pinMode(NDAC, INPUT_PULLUP);  
  digitalWrite(NDAC, HIGH);
  pinMode(NFRD, INPUT_PULLUP); 
  digitalWrite(NRFD, HIGH);
  pinMode(IFC, INPUT_PULLUP); 
  digitalWrite(IFC, HIGH);
}
#endif


