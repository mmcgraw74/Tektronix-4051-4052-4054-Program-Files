This is one of two Tektronix tapes labeled "4050A CYCLE 10/83" that I received from Stan Griffiths in 2000 in a box of old 4050 series tapes.
The files I recovered are from the tape with a hand written 1.5 on the top of end of the label.
Both tape labels were plotted and both had "ROGER" at the bottom of the end of the label.

The comments in the program indicate these tapes were used to test 4050 computers for about one hour.

The recovered program would only work on a Tektronix 4052A or 4054A as it made a CALL MTPACK command to the tape and that call is only built into the 4052/4054 and A-series computers.

This folder includes the original program and my modified program which runs on all 4050 computers and only runs the 9 display tests.

Interestingly the program includes a set of BASIC statements to detect the specific model (4051/4052 or 4054) and sets a parameter to scale a FOR/NEXT loop time delay to 1 second.
---------
These data statements indicate the relative timing of the 4052/4054 FOR/NEXT is about 8.3X and the 4052A/4054A FOR/NEXT is about 7.0X the speed of the 4051.

I$ is the model name which is used in the print statements and written to the tape
S9 in the data is the time constant used in the program for a 1 second delay.
T1 is the RND(-1) first returned value, which is model specific

210 DATA "4051  ",217,0.196324846518
220 DATA "4052  ",1805,0.706280095237,"4052A ",1516,0.796364876486
230 DATA "4054  ",1805,0.88093139039,"4054A ",1516,0.894514130768
240 DATA "4054  ",1805,0.505007490939,"4054A ",1516,0.590965583259
250 READ I$,S9,T1

![Label and PCB front](./4050A_CYCLE_Tape.jpg)
