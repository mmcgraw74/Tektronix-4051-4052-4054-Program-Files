100 INIT 110 PAGE 120 PRINT "This program will digitize a graph or magnetogram "130 PRINT "and then list the data out to the Mod. 33"140 DIM M(6)150 PRINT "
Enter X-MIN value of data ";160 INPUT M(1)170 PRINT "
Enter X-MAX value of data ";180 INPUT M(2)190 PRINT "
Enter the Y-MIN value of data ";200 INPUT M(3)210 PRINT "
Enter the Y-MAX value of data ";220 INPUT M(4)230 REM---Find the range of X and Y240 M(2)=M(2)-M(1)250 M(4)=M(4)-M(3)260 PRINT "
Range of X= ";M(2);" Range of Y= ";M(4)270 PRINT "
Set the ORIGIN of the board at X-MIN,Y-MIN"280 INPUT @8:M(5),M(6),Z$290 PRINT "
ORIGIN=";M(5);",";M(6)300 PRINT "
Now digitize the far right of data (X-MAX,Y-Min)"310 INPUT @8:M(5),M(6),Z$320 REM --- Get the rotation information330 A=ATN(M(6)/M(5))340 B=SIN(A)350 C=COS(A)360 REM --- Get X-MAX and Y-MAX370 PRINT "
Now digitize the upper right (X-MAX,Y-MAX)"380 INPUT @8:X,Y,Z$390 PRINT ""400 REM --- Calculate the rotation410 R0=X*C+Y*B420 R1=Y*C-X*B430 PRINT "Now go and sample the data "440 REM --- MAIN DIGITIZING LOOP ---450 PRINT @D:""460 PRI @D:"     X         Y         X          Y          X          Y"470 FOR I=1 TO 6 STEP 2480 INPUT @8:X,Y,Z$490 Z=VAL(Z$)500 GO TO Z OF 510,610510 R2=X*C+Y*B520 R3=Y*C-X*B530 X=M(1)+M(2)*R2/R0540 Y=M(3)+M(4)*R3/R1550 PRINT @D: USING 560:X,Y560 IMAGE 2(6D.3D),s570 NEXT I580 PRINT @D:"
"590 GO TO 470600 PRINT @D:""610 END 