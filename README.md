# Tektronix-4051-4052-4054-Program-Files

This repository is intended to provide a public location to download OR upload Tektronix 4050 series program tapes or disk images.
**************
The Tektronix 4051 (1975), 4052 and 4054 (1979) had unique storage tube vector graphics screens with very high resolution.  

The 4051 and 4052 had a 12" flat screen storage tube CRT with addressable vector resolution of 1024 by 780.   


The 4054 had a 19" curved screen storage tube CRT with 4096 by 3072 addressable vector resolution!

The photo below is my 4054, 4052 and 4631 screen printer.

![4050 Family Photo](./4052%20and%204054%20highlighted.jpg)

and this photo shows both computers running a hidden line program posted in BYTE Magazine
![4054 and 4052 running hidden line program](./Tektronix%204052%20and%204054%20running%20hidden%20line%20program.png)

************************
The Tektronix 4050 series of vector graphics computers were first introduced in 1975 with the 4051 using a Motorola 6800 CPU with 32KB of BASIC ROMs and 32KB RAM integrated with a 1024x780 vector graphics display, DC-300 quarter-inch tape drive, full keyboard, two option ROM slots and GPIB interface for attaching options like plotters, digitizing tablets, floppy and hard disk systems and other GPIB peripherals from Tektronix.

The 4052 and 4054 computers were second generation systems both introduced in 1979 with 10x performance over the 4051 through a custom 16-bit bit-slice CPU architecture which added hardware floating point and doubled the program memory capacity to 64KB and included 64KB of BASIC ROMs.

The Tektronix 4050 computers targeted scientific and business applications that could take advantage of the high resolution vector graphics and text of these computers.  Since there were no BASIC programs available for those applications - Tektronix developed a series of PLOT 50 Software packages that included DC-300 tapes with programs targeting Mathematics, Statistics, Electrical Engineering, Graph Plot, Business Planning and Analysis, Modeling and Reporting Software and General Utility programs for editing and sorting strings or data as shown in this photo:

![Tektronix PLOT 50 Software Library](./Tektronix%20PLOT%2050%20Software.png)

In addition to the Software Library, Tektronix encourage submission of application programs from users, and began publishing a newsletter with customer use cases and abstracts of these user application programs.  I submitted one of my application programs (Cubic Spline Interpolation) to this program and received a tape with three user application programs that I selected from this library.

Here is the cover of one of the PLOT 50 user application library booklets Tektronix published:

![Tektronix PLOT 50 Application Library](./Tek%20Application%20Library%20programs%20-%20cover%20page.png)
https://github.com/mmcgraw74/Tektronix-4051-4052-4054-Program-Files/blob/master/Tek%20Application%20Library%20programs%20-%20cover%20page.png

************************

Since the 4050 series graphics computers were limited to a maximum of 64K bytes of RAM memory (4052 and 4054, 4051 was limited to 32K bytes of RAM) - the programs made heavy use of the tape drive or external GPIB floppy or hard disk storage for many programs.  The DC300 tape drive in the Tektronix computers provided about 300K bytes of storage, with a file system that numbered each file and provided error checking to protect the information in each file.

Tektronix 4050 series computers had BASIC in ROM that included support not only for the internal screen, tape and keyboard, but also GPIB devices like external graphics printers, plotters, tape drives, floppy drives, hard drives, digitizers, and joysticks.

The Tektronix 4051 computer used a Motorola 6800 microprocessor, and had 32KB of BASIC in ROM and up to 32KB of RAM.

The Tektronix 4052 and 4054 computers used a custom AMD 2901 bit-slice 16-bit processor architecture that included the 8-bit 6800 instructions and added microcode for floating point and other new instructions to increase performance 10X over the 4051, doubled the RAM memory to 64K bytes, and had over 64K bytes of BASIC ROM!

***********

More details about individual programs and how I was able to recover them from 40 year old DC300 tapes on my vcfed dot org forum thread:

**https://forum.vcfed.org/index.php?threads/tektronix-4051-4052-4052a-4054-4054a-program-archives.65705/**

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
