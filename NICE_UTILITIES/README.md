**This tape was labeled "NICE UTILITIES" and has several unique programs, and some duplicates of Tektronix programs
```
 1      ASCII   PROG /LABELLER /   2048   
 2      ASCII   PROG    ROOTER     9216   
 3      ASCII   PROG    INTEREST   3328   
 4      ASCII   PROG   CYBERJUNK   1792   
 5      ASCII   PROG   OLDTITLER   3072   
 6      ASCII   PROG   TAPE COPY   8192   
 7      ASCII   PROG /FLOWCHART/   9216   
 8      ASCII   PROG /AVERAGING/   768    
 9      ASCII   PROG  PLOT LABEL   768    
 10     ASCII   PROG    TAPETEST   2560   
 11     ASCII   PROG PRINT TFILE   2048   
 12     ASCII   PROG CY TERMINAL   1024   
 13     ASCII   PROG F GRAF PLT    3072   
 14     ASCII   DATA     EXAMPLE   2048   
 15     LAST                       768    
``` 

Labeler, 4924 Tape Copy and Flowchart are duplicates of programs posted in other folders.

The other programs appear to be unique.

The **TAPETEST program is very interesting - it uses the CALL "EXEC" assembly language and adds CALL "MTPACK", "MTFWD", "MTBACK", and "MTREAD", in the short assembly language program for the 4051!

I don't have a 4051 to test this TAPETEST program, but the program loads and runs on the 405x javascript emulator and then errors due to no tape drive.
******************
**Someone (hint, hint) should disassemble the 4051 Assembly program to see how they added those CALLs :)
****************
