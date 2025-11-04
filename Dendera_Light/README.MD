My first exposure to the Dendera "Lights" in Egypt was when I found Tektronix 4014 terminal plot files - on Jos Dreesen's ftp save of Chuck Forsberg's OMEN website in my post #10 in my first VCF thread in 2018:

**https://forum.vcfed.org/index.php?threads/tektronix-4051-4052-4052a-4054-4054a-program-archives.65705/post-802656**

Here is the 4014 bitmap image of Dendera Lights - displayed using the excellent Tektronix 4010 vector display emulator:
**https://github.com/rricharz/Tek4010**

The file is named z_aswan.plt in the pltfiles/More_pltfiles included with the Tek4010 emulator.

![z_aswan plt on Tek4010 Emulator](./z_aswan%20plt%20on%20Tek4010%20Emulator.png)

The above image was created by Tektronix engineers in the early 1970's using dots with the two brightness levels, organized in 2x2 cells as shown in this zoom of the screenshot above:

![z_aswan plt on Tek4010 Emulator - zoom to show 2x2 cells of dot](./z_aswan%20plt%20on%20Tek4010%20Emulator%20-%20zoom%20to%20show%202x2%20cells%20of%20dots.png)

I watched a very interesting video in Nov 2025: 

[Dendera Light (Re)(Discovered?)](https://www.youtube.com/watch?v=g507G2Ei2To)

This video was an interview with William Sosa, a researcher that had been working on replicating the Dendera Light for 30 years. He believes it was a plasma effect with mercury vapor and demonstrates his reproduction in the video and shows on a breadboard how it may have been created.

After watching the video I decided to make a new Dendera Light R12 bitmap image from an article on the web [dendera-light-bulb-explained](https://seshmedewnetcher.com/dendera-light-bulb-explained/) - Fig.4 Picture of actual relief – Photo credit: Mick Palarczyk

![Fig 4 Picture of actual relief](./Fig%204%20Picture%20of%20actual%20relief.jpg)

Here is the Dendera Light bitmap I created from the photo:
- Using PhotoDemon to invert the image, adjust the levels, and create a dithered dot black and white BMP file
- Exported to S19 ASCII text format with HxD
- Using my Tektronix 4050 GPIB Flash Drive bitmap create program on the Tektronix 4051 Emulator to create the 4050R12 format image

![Fig 4 Picture of actual relief (BW) (inverted) (2)](./Fig%204%20Picture%20of%20actual%20relief%20(BW)%20(inverted)%20(2).bmp)

Here is that 4050R12 image displayed on the Tektronix 4051 Emulator:

![Aswan Light MCM 2025 on 4051 Emulator](./Aswan%20Light%20MCM%202025%20on%204051%20Emulator%20(2).jpg)

And displayed on my Tektronix 4052 computer:

![Aswan Light 4052](./Aswan%20Light%204052%20cropped.jpg)

and displayed on my Tektronix 4054A computer:

![Aswan Light 4054A](./Aswan%20Light%204054A%20cropped.JPEG)

This directory includes my Dendera lights zip file with the following Tektronix 4050 BASIC Flash Drive files:

- 1 - simple bitmap viewer for Aswan Light2 R12 BMP file 5
- 3 - my DitheredBW create program using two Flash Drives in the 4051 Emulator (read from @5:4, write to @6:5)
- 4 - Aswan Light2 S19 ASCII DATA input file
- 5 - Aswan Light2 R12 BMP BINARY DATA output file
- 124 - LAST file (always required in every Flash Drive directory)
