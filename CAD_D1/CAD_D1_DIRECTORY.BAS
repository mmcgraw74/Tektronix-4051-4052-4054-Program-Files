1 REM * "$DIRECTORY" DISK DIRECTORY 6/15/812 GO TO 1004 GO TO 120100 INIT 110 IMAGE 15t,2d,5x,fa120 PRINT "	APPLICATIONS LIBRARY PROGRAMS
"130 PRINT "	        CAD D1 Disk
"140 PRINT "            PROGRAM  TITLE
"150 PRINT USING 110:1,"4052/4 Drafting Program"160 PRINT USING 110:2,"Telephone Cable Layout"170 PRINT USING 110:3,"Drafting Digitizer Menu"180 PRINT USING 110:4,"Drafting Digitizer Program"190 PRINT USING 110:5,"POINT Mode Digitize"200 PRINT USING 110:6,"3-D Transformation"210 PRINT USING 110:7,"Performance Prediction of Sailcraft
"220 PRINT "Input the program of your choice, or 0 to quit: ";230 INPUT P240 P=INT(P)250 IF P=0 THEN 350260 GO TO P OF 280,290,300,310,320,330,340270 GO TO 220280 OLD "@DRAFTING/PROGRAM"290 OLD "@AUTOLOAD/MASTER"300 OLD "@DRAFTING/MENU"310 OLD "@DRAFTING/DIGITIZE"320 OLD "@DIGITIZE/POINT"330 OLD "@TRANSFORM"340 OLD "@SAILCRAFT"350 PRINT "
Goodbye. If you change your mind, press UDK #1."360 END 