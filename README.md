# Tektronix-4051-4052-4054-Program-Files

This repository is intended to provide a public location to download OR upload Tektronix 4050 series program tapes or disk images.
**************
The Tektronix 4051 (1975), 4052 and 4054 (1979) had unique storage tube vector graphics screens with very high resolution.  

The 4051 and 4052 had a 12" flat screen storage tube CRT with addressable vector resolution of 1024 by 780.   


The 4054 had a 19" curved screen storage tube CRT with 4096 by 3072 addressable vector resolution!

The photo below is my 4054, 4052 and 4631 screen printer.

![4050 Family Photo](./4052%20and%204054%20highlighted.jpg)

Since the 4050 series graphics computers were limited to a maximum of 64K bytes of RAM memory (4052 and 4054, 4051 was limited to 32K bytes of RAM) - the programs made heavy use of the tape drive or external GPIB floppy or hard disk storage for many programs like the one in the video.  The DC300 tape drive in the Tektronix computers provided about 300K bytes of storage, with a file system that numbered each file and provided error checking to protect the information in each file.

Tektronix 4050 series computers had BASIC in ROM that included support not only for the internal screen, tape and keyboard, but also GPIB devices like external graphics printers, plotters, tape drives, floppy drives, hard drives, digitizers, and joysticks.

***********

More details about individual programs and how I was able to recover them from 40 year old DC300 tapes on my vcfed dot org forum thread:

**http://www.vcfed.org/forum/showthread.php?64726-Tektronix-4051-4052-4052A-4054-4054A-Program-Archives**

A couple of YouTube videos running my 4052 and 4054A:
**https://www.youtube.com/user/mmcgraw1974/videos**
*********
Older program files (.txt or .BAS) are only compatible with the 405x (4051) javascript emulator, download from **https://github.com/jonbstanley/Tek405xEmulator**


**TODO: I am uploading .UNI versions of new programs and I plan to convert and upload .UNI versions of the older programs over time**

Files with .UNI or commented as Universal are compatible with 4051/4052 or 4054 computers AND the 405x javascript **_universal_** emulator, unless they require special instructions or higher resolution and memory of the 4052 and 4054.
***********
You can download a single file, folder with all included files or whole archive by clicking the Clone or Download button.

If the file has a text name - you will lose that name when transferring to tape.

One of the utility programs can relabel the tape file headers with text.
*******************************

File Upload - Start by making a pull request of your uploaded files - including a name for the tape or disk.

I will then create a folder containing those files.

Since the 4050 series tapes only use file numbers - please use those same file numbers as the filename for each file.

Some tapes have additional program name information stored in the file Header, so also include the output of the TLIST command as a separate file in your submission.  You can capture the TLIST to serial from BASIC with TLIST @40:
*****
PROGRAM DOWNLOAD FROM PC TO 4051 Instructions:

- Use Hyperterminal or PUTTY on your PC, or other serial terminal program with complete configurability of rate and ability to capture and send files
- Configure PC terminal program for 2400 baud, Even Parity, 7 bits
- Configure PC terminal program to delay 500 msec after each line is transmitted
- Configure PC terminal program to delay 10 msec after each character is transmitted
- Type the following commands on the 4051:
```
CALL "RATE",2400,2,0      | to set the rate to 2400 baud, Even Parity
CALL "TSTRIN","","",""    | stops the 4051 from modifying control characters on transmit
CALL "RSTRIN","","",""    | stops the 4051 from modifying control characters on receive
```
- Now you can send and receive the programs on this site that are in Universal format (.UNI extension)

- SEND from PC terminal program to 4051
```
                          | Select PC Univeral file to transmit
OLD@40:                   | this starts the 4051 listening for the program on the RS-232 interface
                          | now start sending the file from the terminal program to the 4051
Hit 4051 BREAK key twice  | when the transfer is complete from the PC terminal program
```
- type RUN to start the program
******

For 4052/4054 computers with v5.1 firmware or 4052A/4054A computers - no need to delay the lines or characters:
```
CALL "RATE",9400,2,0      | to set the rate to 9600 baud, Even Parity
CALL "TSTRIN","","",""    | stops the 4050 from modifying control characters on transmit
CALL "RSTRIN","","",""    | stops the 4050 from modifying control characters on receive
CALL "CMFLAG",3           | set XON/XOFF protocol for TX and RX
```
- SEND from PC terminal program to 4052/4054 or 4052A/4054A
```
                          | Select PC Univeral file to transmit
OLD@40:                   | this starts the 4051 listening for the program on the RS-232 interface
                          | now start sending the file from the terminal program to the 4051
Hit 4050 BREAK key twice  | when the transfer is complete from the PC terminal program
```
now you can type RUN to execute this program

***********
If you don't have a serial interface on your Tektronix 4050 series computer - consider sending your tapes to someone who does, so the files can be archived.
