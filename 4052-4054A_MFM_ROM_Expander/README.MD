Jos Dreesen 4052-4054-4052A-4054A MFM ROM Expander - EPROM Images
------
Jos designed his 4052 Multifunction Module (MFM) ROM Expander based on the Tektronix 4050E01 ROM Expander which I collected and the 4050E01 ROM Expander Instruction Manual posted at [https://w140.com/tekwiki/wiki/4050E01].

His 4052 MFM supports up to eight 4052 ROM Packs like the Tektronix 4050E01 but has several new features and a couple of differences with the Tektronix 4050E01 ROM Expander:
* 4052 MFM only supports 4052, 4054, 4052A and 4054A while the Tektronix 4050E01 supports 4051 and 4052 through 4054A.
* 4052 MFM fits in a single slot of the 4052 ROM backpack while the 4050E01 consumes one slot in the ROM backpack and is cabled to an enclosure with eight individual ROM slots.
* 4052 MFM supports adding TransEra 741 RTC real time clock hardware (see his MFM document: [ftp://ftp.dreesen.ch/TEK_MFM]
* 4052 MFM supports adding 4052 RS-232 Printer Interface hardware (see his MFM document above)
* 4052 MFM contains a single 1Mb 27C010 EPROM to contain the eight 16KB 4052 Tektronix ROM images and/or 4052 TransEra ROM images posted on this repository

-------

Here is a table of ROM Packs posted in this repository for the 4052/4054 and 4052A/4054A from the 1983 Tektronix Catalog page 100:

| ROM Pack # | Name   | Compatibility | 
|:----------:|----------|----------|
| 4052R06    | Editor   | Works in all 4052/4054 and 4052A/4054A |
| 4052R07    | Signal Processing #1 | Works in all 4052/4054 and 4052A/4054A |
| 4052R08    | Signal Processing #2 | Works in all 4052/4054 and 4052A/4054A |
| 4052R09    | Real Time Clock      | Not Available |
| 4052R10    | Advanced File Manager | Requires 4909 Multi-User Hard Disk File Management System |
| 4052R11    | Character and Symbol  | Works in all 4052/4054 and 4052A/4054A |
| 4052R12    | Graphics Enhancement  | Works in all 4052/4054 and 4052A/4054A |
| 4052R14    | GPIB Enhancement      | Only 4052/4054 but has powerup error message with v5.1 BASIC ROMs |
| 4052R14-1A | GPIB Enhancement      | Only 4052A/4054A due to TI9914 GPIB IC on I/O Board |
|            |                       |                              |
| 4052F10    | RS 232 Printer Interface | Works in all 4052/4052A 4054/4054A |
| 020-0476-00 | 4052 File Manager -00     | Requires 4907 floppy system.  Older version ROM not recommended     |
| 020-0476-01 | 4052 File Manager -01     | Requires 4907 floppy system.  Works in all 4052/4052A 4054/4054A    |


