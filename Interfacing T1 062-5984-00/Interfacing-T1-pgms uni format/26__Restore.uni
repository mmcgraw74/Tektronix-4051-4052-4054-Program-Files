500 REM ***** RESTORE FILE ($FCOPY)510 REM  File 7520 PAGE 530 PRINT "                    Restore HP File from Tek Cartridge

"540 IF F THEN 580550 PRINT "Load $FCOPY Restore Cartridge and press RETURN.";560 INPUT X$570 F=1580 PRINT "
File Number";590 INPUT F1600 FIND F1610 CALL "CMINIT"620 CALL "RATE",R,0,2630 CALL "MARGIN",0,0,0640 CALL "RSTRIN","","",""650 CALL "PROMPT",1,200,""660 CALL "TSTRIN"," "," ","      
"670 CALL "CMSET"680 PRINT "

   The Tape file is now set for $FCOPY.  In response to"690 PRINT "the $FCOPY prompt ""Restroe from where"",press Overlay Key"700 PRINT "4 (DATA SEND)."710 PRINT "

Press RETURN to go to Terminal Mode"IN"," "," ","      
"