My **4052/4054 File Manager 020-0476-00 ROM Pack** contained a single 8Kx8 MOSTEK 36xxx mask ROM

To read the MOSTEK 36xxx mask ROMs - I found you can use the Data I/O Unipak and Unipak 2B FAMILY/PINOUT 27 50 with pin 21 pulled low or high to read a 4KB chunk.

I used a second ZIF socket - and carefully bent pin 21 of the socket out and used a jumper clip to clip pin 21 to ground (PIN 12) or +5V (PIN 24) to read each half of the ROM.

See the 4052 Editor photos for capturing the two halves of a MOSTEK ROM.

I used HxD to import the .MOT (Motorola format 82), and save the file as a .BIN file.  Then I used HxD to concatenate the two files into a single 8KB .BIN file

![4052 File Manager label and PCB](./4052%20File%20Manager-00.jpg)
