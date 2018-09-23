These files were recovered from a tape using my 4052R12 Graphices Expansion ROM pack - CALL "IMAGES" command and then captured from Opt1 RS-232 interface using PRINT@40:A$;

All the DATA TEXT files contain the full 7-bit ASCII character set including 32 control characters.

I also captured all the DATA files in HEX using RealTerm on the PC connected to RS-232 - these files are saved in a HEX subfolder.

All these FastGraphics files REQUIRE the 4051 FastGraphics ROM PACK to run directly.

On my 4054A with 4052R12 Graphics Expansion ROM Pack the R2D2 picture in file 4 was cropped at the top of the dome.  I created a slightly downscaled version using the ROM Pack RSCALE command and saved it in a "scaled" file 4 separate from the original version.

If you have the 4051R12 or 4052/4054R12 Graphics Enhancement ROM Pack, you can see the pictures by doing the following steps:
- dimension A$(20000)
- FIND the data file you want to view on tape
- CALL "IMAGES",A$
- CALL "RDRAW",A$,1,0,0

I also created Tek BASIC programs that can view the pictures - on any 4051/4052/4054 computer - or 4051 emulator - without a FastGraphics or Graphics Expansion ROM PACK.  These programs are saved in a BASIC subfolder.

A Screenshots subfolder contains screen captures running the BASIC programs on the 4051 web-based emulator.
