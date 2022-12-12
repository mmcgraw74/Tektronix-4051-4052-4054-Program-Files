Grayscale (or greenscale) BMP image viewer
------------------------------------------
Grayscale (or greenscale) BMP image viewer program in BASIC on Tektronix 4050 series vector graphics personal microcomputers - first introduced in 1975!  I'm using a Tektronix Graphics Enhancement ROM PACK which not only accelerates vector drawing speed but is also capable of drawing fast DOT patterns.  I have created fifteen 4x4 dot patterns at the native 1024x800 resolution of the Tektronix 4050 storage tube vector monitor.  Each dot pattern from 1 to 15 contains one vector per dot up to 15 dots to emulate variable 'pixel' brightness - with the "zero" value represented as no dot pattern.  I take advantage of the 1024 D/A resolution of the computer to position each 4x4 dot pattern in adjacent horizontal and vertical locations to have a maximum image resolution of 256 horizontal by 200 vertical grayscale 'pixels'.
My current BASIC program is 133 lines and 3904 bytes.  It can run on any Tektronix 4050 computer with at least 32KB of RAM.


These dot patterns do not represent linear brightness levels, so I have had to change the source image brightness with an exponential curve using GIMP - which makes the resulting image look very dark.


I'm also learning how to get all 16 levels into the image - which after initial grayscale conversion has 256 values.  Load a hires color source file into GIMP.  Then convert to grayscale, scale image down to 256x144, use color curve to dramatically darken the image, convert to indexed image with 15 'colors' plus black with Floyd-Steinberg dithering, convert back to grayscale and export as BMP.
Then edit the BMP with HxD binary editor, find the sixteen grayscale values and replace them with 00, 10, ... to F0 values.  Export file as Motorola S19 7-bit ASCII HEX format.  Edit the S19 file with Notepad++ to remove the linefeed characters to get CR delimited ASCII strings.
Run my Grayscale BASIC program on the 4051 Emulator or 4052 or 4054 program which reads the S19 file, error checks that the file is an S19 containings a BMP image, extracts image width and height and uses that to center the image on the display (max width 256, max height 200).  Then read S19 image data converting pair of ASCII HEX to decimal number, divide by sixteen to get grayscale value of a 'pixel'.  Convert and draw 4x4 dot pattern at X,Y location for each pixel until done!
Here is the resulting image from the 4051 emulator based on the darkened BMP and the 'original' grayscale image - all at 256x144 resolution.

![4051 Emulator](./One%20Giant%20Leap%20for%20Mankind%20(3).png)


Next step is to save the resulting image to my Tektronix 4050 GPIB Flash Drive file for quick display on the 4051/4052/4054 computer or emulator in seconds - not tens of minutes!

-------------------------------------
Scaled grayscale 'original' BMP image
![Scaled image](./one%20small%20step%20for%20man%20grayscale%20256x144.bmp)
-------------------------------------
Darkened grayscale BMP image
![Scaled image](./one%20small%20step%20for%20man%20grayscale%20leveled%20dark6%20256x144.bmp)
