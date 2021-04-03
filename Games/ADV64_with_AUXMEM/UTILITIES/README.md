To create all the required Adventure data files (*** AUXMEM will first be initialized and all files erased ***

**Make-AUXMEM-DataFiles.txt**    Transfer to 4052 and run first to INIT AUXMEM and create the first data files, plus verify them
**Make-AUXMEM-advf0-9.txt**      Transfer to 4052 and run second
**Make-AUXMEM-advf10-31.txt**    Transfer to 4052 and run third.  This pgm also creates the Save game files 1-9

Now transfer the main program to 4052 and type this to save it to AUXMEM file 51:

**CALL "MSAVE",51,"Tek Adventure for AUXMEM"**

If you want to write protect this program type this command after you have MSAVEd it:

**CALL "PROT",51**

To list the files in the AUXMEM module type:

**CALL "DIR"**

or after **CALL "MUNIT"** is typed you can shorten the command to:

**DIR**
