# Tektronix-4051-4052-4054 BITMAP picture viewer!!


**************
The Tektronix 4051, 4052 and 4054 computers were all vector graphics computers.

I wanted to display photos on my 4052 and 4054 computers and created a 4050 BASIC program to display a dithered dot monochrome picture on any 4050 computer or the 4051 Emulator with the R12 Graphics Enhancement ROM Pack (or vcfed.org Maxirom, or Jos Dressen's 4052 Multifunction ROM Pack).

I used PhotoDemon 9.0 https://github.com/tannerhelland/PhotoDemon to convert a color picture from Star Wars Empire Strikes Back movie of Luke, Leia and Han to monochrome using Sierra-3 dithering algorithm.  PhotoDemon supports 11 different dithering algorithms and I found Sierra-3 gave the best monochrome result.  I did use autocorrect autoenhance and curves on the image for clarity and to darken the image since the dots on the Tektronix screens are 'brighter' than the paper if you were using this program to create dithered dot monochrome images for a printer.

I saved the monochrome dithered dot image as a BMP file, then used HxD hex editor to export the BMP file as a Motorola S19 hex file.  Then I edited the S19 file with Notepad++ to replace the CR/LF with CR to be compatible with Tek 4050 BASIC INPUT an ASCII string.

The dithered dot create program - file 21 inputs the S19 formatted bitmap file from a PC connected to the 4051, 4052 or 4054 computer with a serial interface.  This create program then interprets the size of the bitmap image and centers the image on the 4050 display supporting up to the full 1024x780 vector dot resolution of the 4051 or 4052 computer.  Finally the program extracts each bit with a value of one and creates a vector dot at the proper location for the display and writes one line of dots to my GPIB Flash Drive https://github.com/mmcgraw74/Tektronix-4051-4052-4054-Program-Files/tree/master/Flash_Drive

The dithered dot viewer program - file 3 - simply reads a line of vector dots in R12 format and displays that line for each of the vertical lines in the image.

Here is the resulting dithered dot image displayed on my Tektronix 4054A computer:

![Luke Leia and Han on my 4054A](./Luke%20Leia%20Han%20S3%204054A.jpeg)

****************
Archived original bitmap program.  All my new images will be using dithered dot bitmaps

Since none of the 4050 computers can display grayscale directly, my program emulates grayscale pixels with an 8x8 grid of 1 to 63 dots.

This emulation reduces the size of the picture that can be displayed from the maximum vector resolution on the 4051 and 4052 computers of 1024x780 by a factor of eight to 128x98.

Here is an example of the capability of this program - Star Wars Princess Leia from the Empire Strikes Back movie:

![Leia in grayscale on my 4052](./Leia%20on%204052.jpeg)

***************
Prerequisites:

This program requires the 4052R12 Graphics Enhancement for the viewer program and the R12 plus R14 GPIB Enhancement ROMs to run the create program on a 4052 or 4054.
The 4051 computer requires the 4051 version of those two ROM Packs - or the vcfed.org MAXIROM which includes both.

Since the picture files are larger than the DC300 tape and also larger than a single 4907 8-inch single-sided double-density floppy, you will also need one of my GPIB Flash Drives for your 4050 computer.

All the files in this folder will also work with the Tektronix 4051 Emulator latest version that supports file sizes larger than 100KB (to be released)

***************
This is a work in progress.
I'm working on a higher resolution version which will require a 4054 with Option 30 Dynamic Graphics
