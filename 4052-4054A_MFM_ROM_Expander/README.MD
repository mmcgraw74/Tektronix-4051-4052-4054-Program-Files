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
|:----------:|----------|----------|-------------|
| 4052R06    | Editor   | Works in all 4052/4054 and 4052A/4054A |  any  |
| 4052R07    | Signal Processing #1 | Works in all 4052/4054 and 4052A/4054A |  any  |
| 4052R08    | Signal Processing #2 | Works in all 4052/4054 and 4052A/4054A |  any |
| 4052R09    | Real Time Clock      | Not Available |  N/A |
| 4052R10    | Advanced File Manager | Requires 4909 Multi-User Hard Disk File Management System |  any |
| 4052R11    | Character and Symbol  | Works in all 4052/4054 and 4052A/4054A |  any |
| 4052R12    | Graphics Enhancement  | Works in all 4052/4054 and 4052A/4054A |  any |
| 4052R14    | GPIB Enhancement      | Only 4052/4054 __IGNORE powerup error message with v5.1 BASIC ROMs__ |  any |
| 4052R14-1A | GPIB Enhancement      | Only 4052A/4054A due to TI9914 GPIB IC on I/O Board |  any |
| 4052F10    | RS 232 Printer Interface | Works in all 4052/4052A 4054/4054A | 2|
| 020-0476-00 | 4052 File Manager -00     | Requires 4907 floppy system.  __Older version ROM not recommended__ |  any |
| 020-0476-01 | 4052 File Manager -01     | Requires 4907 floppy system.  Works in all 4052/4052A 4054/4054A    |  any |
|  |  |  |  |
| 741RTC | TransEra Real-Time-Clock | Works in all 4052/4054 and 4052A/4054A |  3 |
| 750SPU | TransEra Super Utilities | Works in all 4052/4054 and 4052A/4054A |  any |
| 764AUX | TransEra Auxiliary Memory | Works in all 4052-4054A. Requires Jos 4052 AUX ROM Pack |  any |

-------
Table of MFM EPROM 'slots' and address ranges:

| MFM EPROM SLOT# | Address range in HEX |
|:----------:|-------------|
|     1      | 00000-03FFF |
|     2      | 04000-07FFF |
|     3      | 08000-0BFFF |
|     4      | 0C000-0FFFF |
|     5      | 10000-13FFF |
|     6      | 14000-17FFF |
|     7      | 18000-1BFFF |
|     8      | 1C000-1FFFF |

