My internet search for Tektronix 4050 programs with CALL "EXEC" which provided 6800 assembly language programs to be called from BASIC turned up a very interesting program titled ***Checksum gen.txt*** at http://www.archerservices.com/History/Tektronix/4051/Tapes/Checksum%20gen.txt


Actually Checksum gen.txt is a document file that contains a short 4051 program with CALL "EXEC" at the end of the document.
The document mentions a MAXIPACK and RAMPACK and line 1 of the program indicates the authors initials: MDC
This leads me to believe the author is Michael Cranford - one of the 4051 engineers and designer of a MAXIPACK and RAMPACK, both of which are installed in a working 4051 in the vintagetek.org museum! https://vintagetek.org/4051-graphic-desktop-system/

I reworked the document into a program I named 4051 ROM Checksums and uploaded it in my Universal format which only uses CR for line delimiters and includes all Tektronix control characters in printable strings.

Checksum gen as written only allowed one ROM address and Bankswitch as needed to be input and printed the checksum.

I added the ability to request a complete dump of all 4051 ROMs including the optional ROM Packs, so it is very easy to compare your 4051 ROM checksums against the MDC listed checksums.

In the screenshot from running 4051 ROM Checksums program on the 405x (actually only 4051) javascript emulator available here: https://github.com/jonbstanley/Tek405xEmulator
you can see that the emulator is using the same ROMs as in MDC's 4051, with the exceptions of no option ROMs including no Comm backpack ROMs:

![screenshot](./4051%20Javascript%20Emulator%20ROM%20Checksums%20screenshot.png)
