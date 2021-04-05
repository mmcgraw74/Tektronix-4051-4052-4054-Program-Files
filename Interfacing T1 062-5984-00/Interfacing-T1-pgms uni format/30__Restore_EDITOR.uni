500 REM  *****  RESTORE EDITOR FILE  *****510 REM  File 9520 PAGE 530 PRINT "                   Restore $EDITOR File on Tek Cartidge

"540 CALL "CMINIT"550 CALL "RATE",R,0,2560 CALL "MARGIN",0,0,0570 CALL "RSTRIN","","",C$580 CALL "PROMPT",1,200,""590 N$="//BREAK"600 N$=N$&C$610 CALL "TSTRIN","","",N$620 CALL "CMSET"630 IF F THEN 670640 PRINT "Load $EDITOR Restore Cartridge and press RETURN.";650 INPUT X$660 F=1670 PRINT "
File Number";680 INPUT F1690 FIND F1700 PRINT "
The Tektronix is now set for restoring an HP $EDITOR File."710 PRINT "After you have returned to $EDITOR on the HP system,"720 PRINT "in response to the "">"" prompt, enter:"730 PRINT "
ADD
"740 PRINT "Press Overlay Key 4 (DATA SEND) instead of pressing"750 PRINT "Load $EDITOR Restore Cartridge and press RETURN.";760 PRINT "

To go to TERMINAL mode, press RETURN.";770 INPUT X$780 CALL "TERMIN"790 PAGE 800 GO TO 350