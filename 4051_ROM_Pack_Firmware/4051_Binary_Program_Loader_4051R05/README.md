My **4051R05 Binary Program Loader ROM Pack** contained two 2Kx8 MOTOROLA MCM6832 mask ROMs

To read the MOTOROLA MCM6832 mask ROMs - I found you can use the Data I/O Unipak and Unipak 2B FAMILY/PINOUT 23/28 for a TMS2716 EPROM.
See my document http://../../Read%20MCM6832%20ROMs.pdf

I included photos of end of the ROM case and front of the case with front and back sides of the circuit board.
Each BIN file has the verified checksum at the end of the filename.

![Front of Editor ROM PCB](./4051%20Binary%20Program%20Loader%20Front.jpg)

I used HxD to import the .MOT (Motorola format 82), and save the file as a .BIN file.  Then I used HxD to concatenate the two files into a single 8KB .BIN file
