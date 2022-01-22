# Tektronix 4051 / 4052 / 4054 graphics computers
# convert SVG to Fast Graphics with 4051 RAMPACK or 4051R12 or 4052R12 Graphics Enhancement ROM Pack
*******
The Fast Graphics format was developed by Micheal D. Cranford at Tektronix.  Micheal developed a MAXIROM Pack for the 4051 which included his Fast Graphics which included his 6800 assembly language code to speed up the MOVEs and DRAWs by 10x! Micheal also developed a RAMPACK for the 4051 computers which provides solid-state file storage replacing the need to use the DC300 tape cartridges - which are long obsolete and typically have broken drive belts in the cartridge.  VintageTek.org is currently offering Michael's RAMPACK and MAXIROM for sale on EBAY.

I don't have a 4051 computer, I have the 'newer' (1979) 4052 and 4054A computers and the 4051 ROM Packs are not compatible since those computers have a custom bit-slice CPU :(

However, I have designed a 4050 GPIB Flash Drive that works with 4051, 4052 and 4054 computers and uses the Tektronix 4924 external GPIB DC300 Tape Drive commands, so it does not require an option ROM.  My design adds two commands not in the 4924, Change Directory - which allows different folders to contain entire "tapes", and Read Header - which returns the SdFAT filename to the computer.

Photo of my 4054A displaying my new FG image of the Utah Teapot 
![4054A Utah Teapot](./Screenshots-and-photos/4054%20Teapot.jpeg)

Photo of my 4052 running program 17, displaying C-3PO, BB-8 and R2-D2 
![4052 C3PO-BB8-R2D2](./Screenshots-and-photos/C-3PO%20BB-8%20and%20R2-D2.jpeg)

Photo of my 4054A running program 60, displaying C-3PO, BB-8 and R2-D2
Excuse the tilted angle of this photo - I was trying to eliminate glare at the top of the screen.
Note the increased detail - even though the save image file is displayed at 1024x800.
This is because the 4054 beam size is narrower to accomodate the max resolution of 4096x3072
![4052 C3PO-BB8-R2D2](./Screenshots-and-photos/4054A%20StarWars%20Droids.png)

Photo of the latest 405x emulator running program 17, displaying C-3PO, BB-8 and R2-D2
![405x C3PO-BB8-R2D2](./Screenshots-and-photos/405x%20C3PO%20BB8%20R2D2.png)

I was interested in getting new vector images to display on my Tektronix 4052 and 4054A computers.
Many of the original 4050 vector images were manually digitized.

I did not want to manually digitize images and was interested in trying to convert SVG images to run on the Tektronix graphics computers.

After writing a PLOT50 BASIC program to INPUT SVG and convert the SVG commands to PLOT50 MOVE and DRAW commands, I found that many of the SVG files contained not only vector lines but vector curves and circles.  Looking for a way to get those images converted to just MOVE and DRAW commands, I found that Inkscape could SaveAs HPGL files for plotters - or CNC machines and the files only contained HPGL commands PU (PenUp) and PD (PenDown) commands.

I was able to use Notepad++ to edit the HPGL files and replace PU with M and PD with L so I could use my SVG to FG program 10 to convert those commands to PLOT50 MOVE and DRAW commands, and then convert each of those commands to Fast Graphics/R12 format.  

Since the Fast Graphics image format only requires 3 ASCII characters to store a single MOVE or DRAW vector with 1024x1024 resolution, the image file can be read into the limited RAM of the 4051 (32KB) and displayed with a single Fast Graphics command.

Fast Graphics / R12 BASIC Command Examples:
```
CALL "RDRAW",S$,1,X,Y          | Will perform all the MOVEs and DRAWs contained in S$
                               | The "1" indicates to draw persistent to the display
                               | X and Y are offsets for the image

CALL "RSCALE",S$,H,V,X,Y       | Will scale S$ and replace S$ with the scaled image
                               | H is the horizontal scale factor, V is the vertical scale factor
                               | X and Y are the relative position for the scaled image

```
Program file 60 and the accompanying BINARY data files 54-59 are slightly different than my previous data files for the droids.
- File 60 program adds displaying a title and playing Star Wars music before displaying the Droids.  The program also removes the scaling commands for faster performance.
- File 54 is a new data file I created with three "RTAPER"ed SVG text images to simulate the movie intro scrolling text.
- File 55 is the same Star Wars vector text file as before
- File 56 is the same DROIDS vector text file as before
- File 57 is the same C3-PO full-size vector file as before
- File 58 is a scaled BB-8 vector file
- File 59 is a scaled R2-D2 vector file

```
I posted a video of running Program file 60 on youtube: https://www.youtube.com/watch?v=co8I-_uM2yI

This 573 byte BASIC program 60 completes running in only 36 seconds, loading all the image files from my GPIB Flash Drive, displaying over 25000 vectors, and plays 8 seconds of music!
