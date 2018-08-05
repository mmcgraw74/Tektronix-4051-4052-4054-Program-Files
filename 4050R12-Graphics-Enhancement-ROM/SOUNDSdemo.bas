1 REM SOUNDS example
2 GO TO 100
4 REM Push function key 1 to repeat without recomputing sounds string.
5 GO TO 250
100 INIT
110 SET KEY
120 REM Generate a Magic string.
130 DIM L$(2500)
140 L$=" "
150 FOR J=1 TO 5
160 FOR I=0 TO 2500 STEP 127
170 C$=CHR(SIN(I)*63+64)
180 FOR K=1 TO J
190 C$=C$&C$
200 NEXT K
210 L$=L$&C$
220 NEXT I
230 NEXT J
240 REM Now do it.
250 CALL "SOUNDS",L$
260 GO TO 250
270 END
