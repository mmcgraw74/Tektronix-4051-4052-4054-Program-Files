Hilbert curves - see wikipedia article: https://en.wikipedia.org/wiki/Hilbert_curve
---
This article in Tektronix Tekniques Vol 6 No 3 was intriguing.

![Tekniques article](./Hilbert%20curve%20Tekniques%20V6N3.jpg)

I have a Tektronix 4054A computer and typed in the program - which drew an Order 5 Hilbert curve.

I modified this program to draw a series from Order=3 to Order=6:  Hilbert.uni

I have friends with earlier 4051 and 4052 computers, so I modified that program to send the X,Y coordinates for each line to my PC using a serial interface.
Then I edited the data file to create the separate program files for each Order.

| Filename            | Description                                                          |
| ------------------- | -------------------------------------------------------------------- |
| Hilbert.uni         | pgm only runs on 4052A or 4054A                                      |
| HilbertOrder3.uni   | runs on any Tek 4051/4052/4054 computer /w 32KB RAM or 405x emulator |
| HilbertOrder4.uni   | runs on any Tek 4051/4052/4054 computer /w 32KB RAM or 405x emulator |
| HilbertOrder5.uni   | runs on any Tek 4051/4052/4054 computer /w 32KB RAM or 405x emulator |
| HilbertOrder6.uni   | runs on any Tek 4051/4052/4054 computer /w 32KB RAM or 405x emulator |
|                     |                                                                      |
| HilbertOrder3-6.png | Screenshots from running pgms on 405x emulator                       |


The program files can be easily edited to plot the curve on a Tektronix 4662 or 4663 plotter.
Just add @x: to the MOVE and DRAW commands where x is the GPIB address of the plotter.
