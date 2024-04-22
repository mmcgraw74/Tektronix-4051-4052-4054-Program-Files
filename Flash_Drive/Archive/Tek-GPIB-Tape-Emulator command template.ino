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

 TYPE   6  (MTA 69)(MSA 102)(Type)(Delim)(Length)(Delim)(UNT) Return integer with data type and data size if binary, else size=0 
 
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
 SAVE   1  (MLA 37)(MSA 97)(ASCII Data)(UNL)                  Write ASCII strings to file, close on EOI signal. Header checked 
                                                                  for NEW or ASCII. Mark header ASCII PROGRAM
 BSAVE  17 (MLA 37)(MSA 113)(BINARY Data)(UNL)                Write binary data to file, close on EOI signal. Header checked 
                                                                  for NEW or ASCII. Mark header BINARY PROGRAM
 BOLD   17 (MTA 69)(MSA 113)(BINARY Data)(UNL)                Read binary program from file, close with EOI signal.
                                                                  
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
1 End-of-File Character
2 ASCII data (numeric or character string)
3 Binary Numeric data
4 Binary Character string

Tek 4050 file header is always the first ASCII string in a file and can be read and written without disturbing rest of the file
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
- no SdFat reserved characters <>:"/\|?*
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
- Each item contains its own header identifying the length of the item and the type
- Header is generated by the talker sending the information
- The two byte header has the following form:

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
