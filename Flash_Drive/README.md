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
14. *** Added Vectrex Joystick interface to Flash Drive Firmware ***
************

**January 6, 2025**
- updated Flash Drive zip to January 6, 2025
  - updated MAIN MENU to add SimpleDemo and Michelangelo
  - added SimpleDemo for VintageTEK and other exhibitors to easily demo Mickey Mouse, Death Star and INSTANT ART
  - added Michelangelo to view vector image ported from Commodore Plus/4 BASIC
  - added 6800 Disassembler program and example source from Application Library tape
  - added MENU to SVG2FG to view and select from list of SVG files
  - updated EM Benchmark to compare with my 4051 benchmark results in 1979

************
October 14,2024 update - New Features and Fixes:
Added Player asset summary at end of Monopoly game:
- Monopoly Game (21Aug2024 latest released version), requires 4052R12 or 4051 Fast Graphics ROM (or 4051 MAXIROM)
************
August 21,2024 update - New Features and Fixes:
New Features:
- Monopoly Game (21Aug2024 latest released version), requires 4052R12 or 4051 Fast Graphics ROM (or 4051 MAXIROM)
- Main Menu updates including loading Monopoly Game binary program - requires R05 and R12 (or edit main menu to load file 1 instead of 117 and change compatibility data from 36 to 4 to remove R05 ROM requirement)

************
July 20,2024 update - New Features and Fixes:
New Features:
- Monopoly Game (work in progress), requires 4052R12 or 4051 Fast Graphics ROM (or 4051 MAXIROM)
- Five Color Demo, requires Option 30 and Option 31 Color Dynamic Graphics DVST
- Main Menu updates including adding Monopoly Game and Five Color Demo

  User Guide updated with Flash Drive schematic and updated assembly and troubleshooting information.
  
************
April 22,2024 update - New Features and Fixes:
New Features:
- Added Benchmark directory
- Updated Asteroids WIP with new programs
- Updated and added new BMP images
- Added 19 programs to Utilities including - FIVE Color Demo for my 4054A with OPTION 31 Color Enhanced Display

Fixes:
- Completely updated this repo including posting a new GPIB Flash Drive firmware image with firmware update instructions in the README
- Latest firmware including Gamepad support can be updated on all GPIB Flash Drives with Pandauino 644-Narrow and 1284-Narrow

************
Oct 5 2023 Flash Drive zip file update - New Features and Fixes:

New Features:
- Vectrex Joystick Interface added to Flash Drive Firmware!!
- New faster Main Menu loading!  ROOT/1 quickly loads and determines if it can load ROOT/2 which is BINARY Menu on ROOT/5 which is ASCII Menu
- Automatic detection of moving Flash Drive to different model of 4050 computer and automatically forces Configuration Detect in Main Menu
- New Main Menu which shows new programs with asterisk

New Programs:
- Battlestar Galactica "Cylon Attack" game for both R12 and Option 30 (requires Tek 4952 joystick or Flash Drive /w Vectrex Joystick)
- Bitmap Pictures like Mandelorian and Grogu that I demonstrated at VCF Southwest 2023
- PLOT10 Pictures that I just got working for R12
- Spacelab Pics for R12 that have new R12 image files based on my latest PLOT10 creator and viewer
- Asteroids Work in Progress for Option 30

Fixes:
- PLOT10 to R12 creator output files now work on 4051/4052/4054 with R12 or Fast Graphics ROM Packs


************
Oct 2 2022 Flash Drive update - New Features and Fixes:

New Features:
- Latest version that I exhibited at VCF West 2022 in August plus couple of new pictures like B2EMO
- New Work-in-Progress Spacelab folder, not in main menu yet but can be manually accessed
- Added WIP PLOT10 viewer and creator program and ASCII Plot10 and BINARY R12 files to SVG2FG folder - manual access
- Updated Flash Drive Arduino firmware in AR488_Store to July 12 2022 version 0.05.85

Fixes:
- fixed 4054 Option 30 files so demo runs to completion

************
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

![Main Menu](./Flash%20Drive%20MAIN%20MENU%206JAN2025.JPEG))

Main Menu now uses First Time Setup to discover installed option ROMs.
Main Menu items that are incompatible due to lack of required option ROM are marked with / characters to indicate they cannot be selected.

![First Time Setup](./First%20Time%20Setup%20Discovery%20Complete%204052%20dark.jpeg)

![Pictures Menu](./FlashDrive-Pictures.png)

![Games Menu](./FlashDrive-Games.png)
