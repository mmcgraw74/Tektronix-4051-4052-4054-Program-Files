# Tektronix-4051-4052-4054 BITMAP picture viewer!!


**************
The Tektronix 4051, 4052 and 4054 computers were all vector graphics computers.

I wanted to display photos on my 4052 and 4054 computers and created a 4050 BASIC program to display a grayscale picture from my PC.

Since none of the 4050 computers can display grayscale directly, my program emulates grayscale pixels with an 8x8 grid of 1 to 63 dots.

This emulation reduces the size of the picture that can be displayed from the maximum vector resolution on the 4051 and 4052 computers of 1024x780 by a factor of eight to 128x98.

Here is an example of the capability of this program - Star Wars Princess Leia from the Empire Strikes Back movie:

![4050 Family Photo](./Leia%20on%204052.jpeg)

***************
Prerequisites:

This program requires the 4052R12 Graphics Enhancement for the viewer program and the R12 plus R14 GPIB Enhancement ROMs to run the create program on a 4052 or 4054.
The 4051 computer requires the 4051 version of those two ROM Packs - or the vcfed.org MAXIROM which includes both.

Since the picture files are larger than the DC300 tape and also larger than a single 4907 8-inch single-sided double-density floppy, you will also need one of my GPIB Flash Drives for your 4050 computer.

All the files in this folder will also work with the Tektronix 4051 Emulator latest version that supports file sizes larger than 100KB (to be released)

***************
This is a work in progress.
I'm working on a higher resolution version which will require a 4054 with Option 30 Dynamic Graphics
