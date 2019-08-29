Tektronix 4050 Series Application Library ---- CAD_D1 062-5977-01
----

CAD_D1 is a Tektronix 4050 Applications Library 8 inch floppy disk with several different programs.

The abstract for these programs and the full user documentation files are also in this folder.

Be sure to look at the hardware requirements - some of the programs require 4052 or 4054 computers, some require 64KB RAM, etc.

-------------
The dump file is from a complete backup of the floppy to tape, that I then ran my Complete Tape Dump program to capture as a single file.

The dump file is all ASCII characters - and includes the file headers and complete copy of each 256 byte tape block.

ASCII files are captured character by character and may include garbage after the header info in that block and garbage after the end of the program. This extra 'garbage' is left over information that was previously on that block and not overritten by the header or program.

The BAS programs are the program files from the floppy - they were originally saved in BINARY format on the disk. I "OLD"ed the programs into my 4054A, then SAVE@40: the programs over the serial interface to a PC that captured the files.

The BINARY DATA files are dumped in a two ASCII HEX character per byte format. If you copy the ASCII characters into an online HEX to BINARY file converter - the resulting binary file contains the original data. I don't know how to get that file back into a floppy yet.

The ASCII file 1 in the dump lists the complete floppy filename and path. File 2 in the dump is the ASCII HEX version of the BINARY data for that file. If you convert the ASCII HEX to BINARY and load it into a binary editor like HxD - the text in program files can be viewed. However, the BASIC statements have been tokenized - so you should use the .BAS versions :)
