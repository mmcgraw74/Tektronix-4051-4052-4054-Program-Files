500 REM ***** STORE FILE ($FCOPY)510 REM  File 6520 PAGE 530 PRINT "                    Store HP File on Tek Cartridge

"540 IF F THEN 580550 PRINT "Load $FCOPY Store Cartridge and press RETURN.";560 INPUT X$570 F=1580 PRINT "
File Number";590 INPUT F1600 FIND F1610 CALL "CMINIT"620 CALL "RATE",R,0,2630 CALL "MARGIN",0,0,0640 CALL "TCRLF",1,2,1650 N$="X"&C$660 CALL "RSTRIN","
",C$,N$670 CALL "CMSET"680 PRINT "

   The Tape file is now set for $FCOPY.  In response to"690 PRINT "the $FCOPY prompt ""Store where?"", press Overlay Key 3"700 PRINT "(DATA RECEIVE)."710 PRINT "

Press RETURN to go to Terminal Mode.";720 INPUT X$730 PAGE 740 CALL "TERMIN"750 PAGE 760 GO TO 350