My 4050 GPIB Flash Drive for all the Tektronix 4050 computers is now ready for ordering!!
------------------
![Flash Drive](./Tektronix%204050%20Flash%20Drive-labeled.jpg)
--------------------
**4050 GPIB Flash Drive Features**

1. Completely replaces 4050 internal tape drive for ALL program and data storage
2. Ready to run with 500 or more files in 15 ‘tape’ directories including 35 games and 33 R12/Fast Graphics pictures on the MicroSD card
3. MicroSD card provides Gigabytes of program data and storage
a. Plug MicroSD into USB-MicroSD adapter to transfer program & data files to/from your PC
4. Faster access and loading of all files than internal tape
5. Stores each ‘tape’ in separate directory – 100’s of tapes can be stored on same Flash Drive
6. Compatible with ALL Tektronix 4051, 4052, 4052A, 4054 and 4054A computers
7. Supports all 4050 BASIC GPIB tape commands:
a. FIND, MARK, KILL, OLD, BOLD, SAVE, BSAVE, APPEND, BAPPEN, PRINT, INPUT, READ, WRITE
8. Plugs into 4050 GPIB connector – no Option ROM needed to use the Flash Drive
9. First Time Setup – discovers your 4050 computer model and installed option ROMs
10. Main Menu – organizes access to all installed directories and programs and uses First Time Setup discovery of installed Option ROMs to only allow you to select and run programs compatible with your 4050 computer detected configuration.
11. Flash Drive AUTO LOAD – uses the RTC (Real-Time-Clock) Option (included in the 4052/4054 Multi-Function Option ROM available separately) to AUTO LOAD your 4050 computer at power-on to your Favorite Directory which is the last directory you selected from the Main Menu
12. Flash Drive Micro-USB power cord included. USB 5V 1A power adapter is not included as it requires a country specific power connector.
13. One Flash Drive zip file with the all the latest ‘tapes’ and programs can be downloaded from the internet and be unzipped to your MicroSD card to update your Flash Drive.

You can build it yourself or I can assemble it.
--------------
Flash Drive GPIB interface PCB https://oshpark.com/shared_projects/HIeNjLBF
--------------
See my Flash Drive thread on vcfed.org for more details: 
https://forum.vcfed.org/index.php?threads/tektronix-4050-gpib-flash-drive-now-available.1238891/

My 4050 Flash Drive does NOT require any ROM Pack for use, but does require minor modifications to programs that were designed for the internal tape drive.

The primary modification to FIND, OLD, SAVE, INPUT, PRINT, READ and WRITE statements would be to add the @5: GPIB address of the Flash Drive to those commands.  

The Flash Drive supports multiple directories on the FAT32 formatted MicroSD card, which uses a PRINT @5,9:"folder name" command to the Flash Drive to change the directory from the current directory (default "ROOT") to an existing directory on the microSD card.

Note that some programs - such as the Fast Graphics / R12 Pictures requires the R12 ROM Pack - Either the vintagetek.org MaxiROM for the 4051 or Jos Dreesen's Multi-Function ROM Pack for the 4052/4054 and A-Series 4052/4054.

Tektronix 4051 MaxiRom is currently available on EBAY from its developer: vintagetek.org and provides Tektronix 4051 with all the 4051 Option ROMs including R12 Graphics and Music.

Jos' MFM ROM Pack for the 4052/4054 emulates an 8-slot 4050E01 ROM Expander and includes a TransEra-741 Real-Time-Clock plus a 4052 RS-232 Printer Interface!  I have used the Real-Time-Clock to automatically boot my 4052 into the Pictures Menu (you can easily pick your own program for boot).  Jos' MFM ROM Pack for the 4052/4054 is available - contact him on his vcfed.org thread: 
https://forum.vcfed.org/index.php?threads/tektronix-4052-4054-multifunction-modules-available.76192/post-76192

Here are screenshots of the new Main Menu and new First Time Setup Option ROM Detect, Pictures Menu and the Games Menu.

![Main Menu](./4050%20GPIB%20Flash%20Drive%20-%20Main%20Menu%204052%20dark.jpeg)

Main Menu now uses First Time Setup to discover installed option ROMs.
Main Menu items that are incompatible due to lack of required option ROM are marked with / characters to indicate they cannot be selected.

![First Time Setup](./First%20Time%20Setup%20Discovery%20Complete%204052%20dark.jpeg)

![Pictures Menu](./FlashDrive-Pictures.png)

![Games Menu](./FlashDrive-Games.png)
