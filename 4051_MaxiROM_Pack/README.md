new vintageTEK.org MaxiROM Pack
-------
This new Tektronix 4051 ROM Pack was first offered for sale on EBAY in April 2021

![vintageTEK.org MaxiROM Pack](./VintageTek%204051%20MaxiRom%20pack.jpg)

MaxiROM is a 10 in 1 multi-function 4051 ROM Pack that contains:

*4051R01 Matrix Functions
*4051R05 Binary Program Loader
*4051R06 Editor
*4051R07 Signal Processing I
*4051R08 Signal Processing II
*4051R12 Graphics Enhancement
*4051R14 GPIB Enhancement
*4051 DDT51.8 6800 Assembly language Debugger
*4051 Extended BASIC
*4051 Extended Fonts


You also get a documentation CD-ROM with all the ROM Pack User Guides!

The EBAY page for this MaxiROM Pack has two photos of a new 4051 "CLIST" command which shows the all the 4051 ROM CALL functions, including ALL the MAXIROM CALLs.

The CLIST command is included in the 4051 and 4052/4054 R14 GPIB Enhancement.

The internal BASIC ROMs of the 4051 and 4052/4054 do NOT include a CLIST command.
My 4054 upgraded to 4054A DOES include a CLIST command, which is very handy.

Here is my ROM mapping table for the MaxiROM new CALL commands (from the two CLIST photos in the EBAY post):

```
EXEC          This is a standard CALL in all 4051 for assembly language programs

RATE, etc     These CALLs are included in the 4051 Option 1 RS-232 Comm backpack

RPSAFE        I don't recognize these?  Maybe ROM Pack utilities?
RPKILL
RPWIPE
RPCOPY

BOUNDS, etc   4051R12 Graphics Enhancement 

WAIT, etc     4051R14 GPIB Enhancement 

CONVL, etc    4051R07 Signal Processing I

IDN, etc      4051R08 Signal Processing II

DEBUG         DDT 51.8 Debugger for 4051 assembly language programs

EDITOR        4051R06 EDITOR

FONT00, etc   4051 Extended Fonts

WHILE         4051 Extended BASIC
```
See the two CLIST photos in this folder for all the new 4051 function CALLs in this MaxiROM Pack.


There are some very interesting new commands in the Extended BASIC ROM including QDRAW (which I imagine may be similar to the Fast Graphics ROM call?), structured BASIC calls like WHILE and CASE, VLIST and a couple of different CLIST, PEEK and POKE, MTPACK and TNAME.

