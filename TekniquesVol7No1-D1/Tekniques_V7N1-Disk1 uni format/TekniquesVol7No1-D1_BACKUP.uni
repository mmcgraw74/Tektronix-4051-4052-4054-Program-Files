1 REM --------------- File $BACKUP ---------------2 GO TO 1104 GO TO 10008 GO TO 200012 GO TO 300016 GO TO 400020 SET NOKEY 21 GO TO 500084 REM ##### End of file subroutine #####85 E=186 RETURN 89 REM ##### File appends #####90 DELETE 101,6553591 N=MEMORY92 APPEND O$;100,0100 REM ##### Program initialization #####110 INIT 120 PAGE 130 DIM A$(10000),M$(1),T$(257),Z$(300)140 M$=CHR(13)150 PRINT "

   Enter the unit number of the disk drive -- ";160 INPUT D170 CALL "MOUNT",D,Z$180 CALL "UNIT",D190 ON EOF (0) THEN 85200 ON EOF (1) THEN 85210 ON EOF (2) THEN 85220 REM ##### Interrogate for activity #####230 PRINT "

   Do you want to backup or restore a disk (B_/R)? ";240 INPUT Z$250 IF Z$="B" OR Z$="R" THEN 270260 Z$="B"270 O$="@SYSLIB/BACKUP."&Z$280 PRINT "

   Selective or total (S_/T)? ";290 INPUT Z$300 IF Z$="T" THEN 330310 O$=O$&"01"320 GO TO 90330 O$=O$&"02"340 GO TO 90