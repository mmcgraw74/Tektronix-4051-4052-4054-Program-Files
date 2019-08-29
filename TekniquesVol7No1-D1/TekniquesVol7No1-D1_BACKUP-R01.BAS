100 REM --------------- File $BACKUP.R01 ---------------110 REM ##### Append the file manipulation routines #####120 DELETE 8,80130 N=MEMORY140 APPEND "@SYSLIB/BACKUP.R03";10000,0150 PRINT @32,26:3160 REM ##### Prepare the disk for use #####170 PRINT "

   Insert the disk to be restored in drive ";D;180 PRINT " and press RETURN :";190 INPUT Z$200 CALL "MOUNT",D,Z$210 PRINT "


   (Press UDK 1 for tape directory.)
"220 SET KEY 230 REM ##### User interrogation: which file? #####240 PRINT "
   Which file do you wish to restore? ";250 INPUT A$260 SET NOKEY 270 REM ##### If no answer, quit #####280 IF NOT(LEN(A$)) THEN 560290 REM ##### \\\ means a directory was performed: ask again #####300 IF A$="\\\" THEN 220310 REM ##### Make the file name readable #####320 IF ASC(A$)<>64 THEN 350330 A$=REP("",1,1)340 GO TO 360350 A$="SCRATCHLIB/"&A$360 A$=A$&"$"370 REM ##### Prepare for the tape directory search #####380 PRINT "   Insert the appropriate tape and press RETURN: ";390 INPUT Z$400 FIND 1410 E=0420 F=1430 REM ##### Top of the tape directory search loop #####440 INPUT @33:Z$450 IF E THEN 500460 IF POS(Z$,A$,1)=1 THEN 530470 F=F+1480 GO TO 440490 REM ##### Search failed: ask again #####500 PRINT "        ***File not found on this tape.***"510 GO TO 220520 REM ##### Found the file: restore it #####530 GOSUB 10000540 GO TO 220550 REM ##### That's all, folks! #####560 CLOSE 570 PRINT "

   Disk restore completed."580 PRINT @32,26:0590 END 990 REM ##### Tape directory subroutine #####1000 PRINT "*DIRECTORY
"1010 FIND 11020 E=01030 INPUT @33:Z$1040 INPUT @33:Z$1050 IF E THEN 11001060 N=POS(Z$,"$",1)1070 Z$=SEG(Z$,1,N-1)1080 PRINT Z$1090 GO TO 10401100 A$="\\\"1110 RETURN 10000 REM ##### Target line for append #####