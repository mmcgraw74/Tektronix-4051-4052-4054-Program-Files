These files were recovered from a tape using my 4052R14 Graphices Expansion ROM pack - CALL "IMAGES" command and then captured from Opt1 RS-232 interface using PRINT@40:A$;
All the DATA TEXT files contain the full 7-bit ASCII character set including 32 control characters.
I also captured all the DATA files in HEX using RealTerm on the PC connected to RS-232 - these files are saved in a HEX folder.
All these FastGraphics files REQUIRE the 4051 FastGraphics ROM PACK to run directly
If you have the 4051R14 or 4052/4054R14 Graphics Enhancement ROM Pack, you can see the pictures by doing the following steps:
- dimension A$(20000)
- FIND the data file you want to view on tape
- CALL "IMAGES",A$
- CALL "RDRAW",A$,1,0,0
