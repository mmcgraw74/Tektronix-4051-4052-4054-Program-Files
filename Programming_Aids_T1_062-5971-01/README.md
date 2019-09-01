Programming_Aids_T1_062-5971-01
-----
The dump file is a complete list of all the tape records.  
Binary files are saved as HEX ASCII character files with extension ASC.  
Program files are saved as .BAS files - and will directly load into 4050 computers.

Also includes a screenshot of the file 1 menu: ProgrammingAidsT1_MENU.jpg

The dump file contains everything including headers and all contents of each tape file block.
- you will notice extra data in the header block, and extra data after the end of the program or data files.
- this extra data is left over from previous tape use as the MARK command and SAVE commands do not clear out previous data
-------
Documentation and Abstracts are included
-------
BASIC files extracted from the dump are ready to run on 4051/4052/4054 computers
The first number in the filename is the Tape file number - the text is from the file header label or from the program comments.
---
For the 405x emulator - the Tek 4050 BASIC control characters must be replaced with underscore followed by the ASCII character using a text editor like Notepad++

Example: control-J character must be replaced with _J
