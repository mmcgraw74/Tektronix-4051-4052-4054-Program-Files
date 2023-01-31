# Tektronix-4051-4052-4054 BITMAP picture viewer!!


**************
The Tektronix 4051, 4052 and 4054 computers were all vector graphics computers.

I wanted to display photos on my 4052 and 4054 computers and created a 4050 BASIC program to display a dithered dot monochrome picture on any 4050 computer or the 4051 Emulator with the R12 Graphics Enhancement ROM Pack (or vcfed.org Maxirom, or Jos Dressen's 4052 Multifunction ROM Pack).

This latest BITMAP program takes advantage of the full vector resolution of the 4051 or 4052 for bitmap pictures up to 1024x780 resolution.  

These programs are also compatible with the 4054 and 4054A computers - and their higher 4096x3072 resolution means smaller dot size and even more detail than the 4051 or 4052 on the same bitmap images.

**************
File 1 is the Bitmap viewer program with a menu to select the image:

1  Spock holding model of the Enterprise   (Star Trek 1966)

2  Astronaut Irwin on the Moon with Lander and Rover (1971)

3  Silent Running - Planting a Tree                  (1972)

4  Silent Running - Playing Poker                    (1972)

5  Luke, Leia and Han Solo       (Star Wars ESB movie 1980)

6  Ultimate Sandbox          (Michael Whelan painting 1984)

7  Mandalorian and Grogu      (Disney+ Mandalorian S3 2023)

Input picture number or press 0 key to Show ALL: ";

![Spock](./Spock%20537x780%20on%204054A.jpeg)

![Astronaut Irwin on Moon](./Astronaut%20Irwin%20on%20Moon.png)

![Silent Running - Planting a Tree.png](./Silent%20Running%20-%20Planting%20a%20Tree.png)

![Silent Running - Playing Poker](./Silent%20Running%20-%20Playing%20Poker.png)

![Luke Leia and Han Solo](./Luke%20Leia%20and%20Han%20Solo.png)

![Ultimate Sandbox](./Ultimate%20Sandbox.png)

![Mandalorian and Grogu](./Mandalorian%20and%20Grogu.png)

**************

I used PhotoDemon 9.0 https://github.com/tannerhelland/PhotoDemon to convert a color picture from Star Wars Empire Strikes Back movie of Luke, Leia and Han to monochrome using Sierra-3 dithering algorithm.  PhotoDemon supports 11 different dithering algorithms and I found Sierra-3 gave the best monochrome result.  I did use autocorrect autoenhance and curves on the image for clarity and to darken the image since the dots on the Tektronix screens are 'brighter' than the paper if you were using this program to create dithered dot monochrome images for a printer.

I saved the monochrome dithered dot image as a BMP file, then used HxD hex editor to export the BMP file as a Motorola S19 hex file.  Then I edited the S19 file with Notepad++ to replace the CR/LF with CR to be compatible with Tek 4050 BASIC INPUT an ASCII string.

The dithered dot create program - file 21 inputs the S19 formatted bitmap file from a PC connected to the 4051, 4052 or 4054 computer with a serial interface.  This create program then interprets the size of the bitmap image and centers the image on the 4050 display supporting up to the full 1024x780 vector dot resolution of the 4051 or 4052 computer.  Finally the program extracts each bit with a value of one and creates a vector dot at the proper location for the display and writes one line of dots to my GPIB Flash Drive https://github.com/mmcgraw74/Tektronix-4051-4052-4054-Program-Files/tree/master/Flash_Drive

The dithered dot viewer program - file 3 - simply reads a line of vector dots in R12 format and displays that line for each of the vertical lines in the image.

Here is the resulting dithered dot image displayed on my Tektronix 4054A computer:

![Luke Leia and Han on my 4054A](./Luke%20Leia%20Han%20S3%204054A.jpeg)

****************
I've had questions on how my bitmap create program works - so they can use the ASCII or BINARY DATA files to display these images on other vintage computers.

File 2 is the CREATOR BASIC program which reads the ASCII DATA file - which contains Motorola S19 or S28 HEX output exported from HxD from the BMP file. I do this conversion because the 4050 computers do not have enough RAM to load the entire binary bitmap file into memory. Since the original 4051 BASIC only supported 7-bit ASCII characters, I export the S19 or S28 formats which encode each byte of the 8-bit binary BMP file into a pair of 7-bit ASCII HEX characters. My BASIC CREATOR program reads one carriage return delimited string from the S19 or S28 ASCII DATA file, computes the 8-bit numeric value of the pair of HEX characters with a second ROM Pack R14 CALL "HEXDEC" (from R14 quick reference https://w140.com/tekwiki/images/a/ac/070-4315-01.pdf), then uses Tektronix 4050 BASIC DEF FN 'B' (define function call) to extract the binary bit (1 to 8 ) passed in as a parameter from the global variable V9 (see line 230 in the create program). Bits that have a value of 0 are skipped and value of 1 are dots. You also need to understand the BINARY DATA file format to extract the dot data (http://www.bitsavers.org/.../070-2056-01_4050_ref_Jul79.pdf page 7-169) has the two byte Binary format header described. This binary data header is created for each BASIC WRITE command. My create program does one WRITE for each horizontal line of dots in the bitmap. Since each line of the bitmap has a variable number of dots - the length of the block of data is a variable. Tektronix 4050 BASIC encodes the data type in bits 7,6 and 5 of the first header byte according to the table on page 7-169. All the WRITES of my program are strings so the type is "010" for every WRITE. The remaining bits of the header encode the length of the string - up to 8191 which is 0 based so up to 8192 bytes of string maximum length. There is also a 1-byte 'trailer' for each binary string write. The following byte is the first byte of the next binary data header. 

CALL "RDOTS",A$,X,Y This command is described in Tektronix 4050 R12 Graphics Enhancement ROM Pack user manual at http://www.dvq.com/tektronix/4052R12-opt.pdf on page 3-110 and the BASIC example program for this command is described on page 3-111. All the commands in this ROM Pack use 3-byte 7-bit ASCII string variables for the vector X,Y data which encodes 10-bit X, 10-bit Y and 1-bit MOVE/DRAW as described in Appendix C page C-1. My bitmap program takes advantage of the RDOTS command draws a dot for every MOVE or DRAW command - so I only put MOVE commands into the string variable. RDOTS draws the dots in the string relative to X and Y variables passed in as parameters to the CALL. In my program these X and Y variables are always zero because I create the MOVE vectors at absolute 10-bit X and Y locations based on the BITMAP dot locations as the program reads a single horizontal line of dots where "Y" is a constant for each line of dots.

****************
Archived folder contains my original bitmap program.  

All my new bitmap images will be using dithered dot bitmaps

***************
Prerequisites:

The VIEWER program requires the 4052R12 Graphics Enhancement for the viewer program.

The CREATOR program requires the R12 plus R14 GPIB Enhancement ROMs.

The 4051 computer requires the 4051 version of those two ROM Packs - or the vcfed.org MAXIROM which includes both.

Since the picture files are larger than the DC300 tape and could be larger than a single 4907 8-inch single-sided double-density floppy, you will also need one of my GPIB Flash Drives for your 4050 computer.

All the files in this folder will also work with the Tektronix 4051 Emulator experimental version that supports file sizes larger than 100KB (to be released)

