Tekniques Vol. 7 No. 1 D1  062-6715-01
----
These program files were captured from an 8 inch floppy disk.

An important program not listed in the Abstract but documented in the full PDF is BACKUP - which can backup all the files on a floppy disk to one or more tapes (since the tapes are typically lower capacity than the floppy disk).  This program can also restore the files from tape back to a new floppy - including the original file paths!
---------------

The dump file is from a complete backup of the floppy to tape, that I then ran my Complete Tape Dump program to capture as a single file.

The dump file is all ASCII characters - and includes the file headers and complete copy of each 256 byte tape block.

ASCII files are captured character by character and may include garbage after the header info in that block and garbage after the end of the program.  This extra 'garbage' is left over information that was previously on that block and not overritten by the header or program.

The BAS programs are the program files from the floppy - they were originally saved in BINARY format on the disk.
I "OLD"ed the programs into my 4054A, then SAVE@40: the programs over the serial interface to a PC that captured the files.

-------
The BINARY DATA files are dumped in a two ASCII HEX character per byte format.  If you copy the ASCII characters into an online HEX to BINARY file converter - the resulting binary file contains the original data.  I don't know how to get that file back into a floppy yet.

The ASCII file 1 in the dump lists the complete floppy filename and path.
File 2 in the dump is the ASCII HEX version of the BINARY data for that file.  If you convert the ASCII HEX to BINARY and load it into a binary editor like HxD - the text in program files can be viewed.  However, the BASIC statements have been tokenized - so you should use the .BAS versions :) 

----------
