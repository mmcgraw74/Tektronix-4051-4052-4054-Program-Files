Tektronix 4050 Michelangelo - VECTOR art - programs and data files
-------------
![Michelangelo vectors on my 4054A](./Michelangelo%20-%20vectors%204054A.png) 
**Michelangelo - Non-filled vector polygons on my 4054A**


![Michelangelo non-filled polygons on my 4054A](./Michelangelo%20-%20filled%20polygons%204054A.png)
**Michelangelo - Filled and dilated polygons on my 4054A**

----
Overview:

I was inspired by Georgios Patsos filled polygon image of Michelangelo on the facebook group "BASIC Programming" on 19 NOV 2024.


![Georgios Patsos Michelangelo screenshot](./Commodore%20Plus-64%20Michelangelo.jpg) 

Georgios posted his raw vector data file for me to use with my Tektronix 4050 vector graphics computers - file 20 in this directory.

I created the simple BASIC program - file 10 - to view Georgios data - which is the first photo in this README.  This data is just the vector triangle and quadrangle polygons.

I wanted to emulate his posted image with filled polygons with black outlines.  Georgios created that image with bitmapped graphics on a Commodore home computer and drew each polygon outline in black.

The Tektronix 4050 vector graphics computers use a custom Tektronix storage tube CRT which stores each vector drawn in the phosphor in the CRT faceplate.  The entire display must be erased to clear the screen, so I needed a different technique to achieve a dark surround for each polygon.  I chose to use Dilation to reduce the size of each polygon - and maintain the position of that polygon by moving the polygon back to its original Centroid X and Y location.  My program reduces the size of each polygon by 85%.

The resulting image is the second photo in this README.

----
Main Program:

File 1 is the main program and has the following selectable features:

- Display the non-filled vector data file (just press RETURN key on both questions)
  - This takes over **6 minutes** to calculate and draw non-filled vector polygons on a 4051 computer or 4051 emulator at 1x speed
- Display the data file with dilated and filled vectors (type Y for filled vectors then RETURN and press RETURN on the second question)
  - This takes 2 minutes 30 seconds on the 4051 Emulator at **100x** speed, so likely over **4 hours** on the 4051 computer and about **45 minutes** on a 4052 or 4054 computer which are about 10x faster than the 4051.
 
If the 4050 computer has the R12/Fast Graphics ROM Pack - included in Vintagetek.org 4051 MAXIROM pack or Jos Dreesen's 4052/4054 Multifunction ROM Pack, then the main program can speed up viewing either non-filled or filled images:

- Create or Display non-filled R12 vector file (type Y for filled vectors then RETURN and type V with RETURN to display file 3, or C with RETURN to create a new file 3)
  - This takes only **12 seconds** on the 4051 with MAXIROM or 4051 Emulator which includes the R12 ROM and even quicker on a 4052 or 4054 computer with R12 ROM pack or Multifunciton ROM Pack
- Create or Display the dilated and filled polygons from an R12 vector file (type Y for filled vectors then RETURN and type V with RETURN to display file 3, or C with RETURN to create a new file 4 **requires two Flash Drives or 4051 Emulator)
  - This takes only **2 1/2 minutes** on the 4051 Emulator or 4051 computer and even quicker on a 4052 or 4054 computer with R12 ROM pack or Multifunciton ROM Pack
