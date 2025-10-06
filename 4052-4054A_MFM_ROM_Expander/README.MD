Jos Dreesen 4052-4054-4052A-4054A MFM ROM Expander - EPROM Images
------
Jos designed his 4052 Multifunction Module (MFM) ROM Expander based on the Tektronix 4050E01 ROM Expander which I collected and the 4050E01 ROM Expander Instruction Manual posted at https://w140.com/tekwiki/wiki/4050E01

His 4052 MFM supports up to eight 4052 ROM Packs like the Tektronix 4050E01 but has several new features and a couple of differences with the Tektronix 4050E01 ROM Expander:
* __4052 MFM only supports 4052, 4054, 4052A and 4054A__ while the __Tektronix 4050E01 supports 4051 and 4052 through 4054A__.
* 4052 MFM fits in a single slot of the 4052 ROM backpack while the 4050E01 consumes one slot in the ROM backpack and is cabled to an enclosure with eight individual ROM slots.
* 4052 MFM supports adding TransEra 741 RTC real time clock components (see his MFM document: ftp://ftp.dreesen.ch/TEK_MFM )
* 4052 MFM supports adding 4052 RS-232 Printer Interface components (see his MFM document above)
* 4052 MFM contains a single 128KB 27C010 EPROM to contain the eight 16KB 4052 Tektronix ROM images and/or 4052 TransEra ROM images posted on this repository

-------
Here is a table of ROM Packs posted in this repository for the 4052/4054 and 4052A/4054A that could be programmed into one of the eight slots in the 4052 MFM:

| ROM Pack # | Name   | Compatibility | MFM Slot# |
|:----------:|----------|----------|:-------------:|
| 4052R06    | Editor   | Works in all 4052/4054 and 4052A/4054A | any   |
| 4052R07    | Signal Processing #1 | Works in all 4052/4054 and 4052A/4054A | any   |
| 4052R08    | Signal Processing #2 | Works in all 4052/4054 and 4052A/4054A | any   |
| 4052R09    | Real Time Clock      | __Not Available__ | N/A   |
| 4052R10    | Advanced File Manager | __Requires 4909 Multi-User Hard Disk File Management System__ | any   |
| 4052R11    | Character and Symbol  | Works in all 4052/4054 and 4052A/4054A | any  |
| 4052R12    | Graphics Enhancement  | Works in all 4052/4054 and 4052A/4054A | any  |
| 4052R14    | GPIB Enhancement      | __Only 4052 and 4054. IGNORE powerup error message with v5.1 BASIC__ | any   |
| 4052R14-1A | GPIB Enhancement      | __Only 4052A and 4054A due to TI9914 GPIB IC on I/O Board__ | any   |
| 4052F10    | RS 232 Printer Interface | __Requires MFM printer components__. Works in all 4052/4052A 4054/4054A | 2  |
| 020-0476-00 | 4052 File Manager -00     | __Requires 4907 floppy system__.  __Older version ROM not recommended__ | any   |
| 020-0476-01 | 4052 File Manager -01     | __Requires 4907 floppy system__.  Works in all 4052/4052A 4054/4054A    | any   |
|  |  |  |  |
| 741RTC | TransEra Real-Time-Clock | __Requires MFM RTC components__.  Works in all 4052/4054 and 4052A/4054A | 3   |
| 750SPU | TransEra Super Utilities | Works in all 4052/4054 and 4052A/4054A | any   |
| 764AUX | TransEra Auxiliary Memory | __Requires Jos 4052 AUX ROM Pack__.  Works in all 4052-4054A. | any   |

-------
Table of MFM EPROM 'slots' and address ranges:

| MFM EPROM SLOT# | Address range in HEX |
|:----------:|:-------------:|
|     1      | 00000-03FFF   |
|     2      | 04000-07FFF   |
|     3      | 08000-0BFFF   |
|     4      | 0C000-0FFFF   |
|     5      | 10000-13FFF   |
|     6      | 14000-17FFF   |
|     7      | 18000-1BFFF   |
|     8      | 1C000-1FFFF   |

----------
Recommended MFM EPROM images:

| MFM image name | Contains these ROMs | Components required |
|----------|:-------------:|------|
| MFM_BASE | R12 Graphics, 750 Super Utilities, R11 Character & Symbol | BASE MFM components |
| MFM_RTC  | MFM_BASE + 741RTC   | BASE + RTC MFM components |
| MFM_PRINTER | MFM_BASE + RS 232 Printer Interface | BASE + Printer MFM components |
| MFM_4907 | MFM_BASE + 4907 File Manager -01 | BASE MFM components + 4907 Floppy System |

----------
Custom MFM EPROM images:

I recommend all MFM images include MFM_BASE which has the option ROMs I have been using for most of my latest 4050 BASIC programs in MFM slots 6, 7 and 8.

You can create a custom MFM image by opening the MFM_BASE.bin file with a HEX editor like HxD and __add__ a ROM using these instructions:
* Find the folder in this repository with ROM pack binary files you want to add to the MFM_BASE image
* There are one to four binary files for each ROM pack with labels including which EPROM socket they were installed: U1, U11, U13 and U15.
* Concatenate all those EPROM binary files to create a single image.  I use HxD Tools | File Tools | Concatenate the .bin files for U1 with U11, U13, U15 in that order - skipping empty sockets, and save to a new output filename on your computer.
* Open your new filename with HxD and Edit | Select All.  Now click on the HxD tab for the MFM_BASE.bin file.  Pick an unused MFM slot - or the specific slot for the Printer or RTC starting address in the MFM_BASE.bin file and Edit | Paste Write to add that ROM image to MFM_BASE.
* Save the MFM_BASE file with a new filename like MFM_R07.bin if you added the Signal Processing #1 ROM.
* Power off your 4050 computer and remove the MFM ROM Pack from the 4050 and remove the four screws in the MFM case to access the MFM circuit board.
* Carefully remove the MFM 27C010 EPROM from the MFM circuit board and plug the MFM EPROM into a compatible EPROM programmer and load the current MFM EPROM image and then save it as a backup file.
* Erase your MFM 27C010 EPROM after removing the label on the EPROM window, in a compatible EPROM UV Eraser for the required number of minutes for that eraser.
* Load your new MFM EPROM file into your EPROM programmer.
* Program your now blank MFM EPROM in your EPROM programmer.  Then label the MFM EPROM and install it into the MFM circuit board socket - making sure the dot next to pin 1 of the EPROM is nearest to the GAL1 22V10 IC.
* Reassemble your MFM ROM Pack and plug it into your 4052/4054/4052A/4054A (which must be powered off before removing or inserting ANY ROM Pack).


* If you have Jos Dreesen's 4052 Diagnostic ROM you can test your MFM ROM CRCs - plug the Diagnostic ROM Pack into backpack slot 1 and MFM into slot 2
    * Power on the computer
    * Load and run my 4052 CRC program from Flash Drive MAIN MENU or Option 1 RS-232 or manually typing: [4052_ROM_Checksums.UNI](https://github.com/mmcgraw74/Tektronix-4051-4052-4054-Program-Files/blob/4f478baca32dbd6669eb47fe67240d39400555c2/4052-4054_ROM_Checksums/4052_ROM_Checksums.UNI) 
    * If the CRC program detects an Option ROM or MFM ROM with CRC that matches a name it will print the name of the ROM


If you don't have the Diagnostic ROM Pack - but you have a 4052A or 4054A type CALL "<ins>C</ins>L<ins>I</ins>S<ins>T</ins" (where underlined characters are typed as control-character) to see a list of ROM calls in each option ROM discovered by BASIC.


