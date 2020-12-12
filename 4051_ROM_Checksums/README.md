My internet search for Tektronix 4050 programs with CALL "EXEC" which provided 6800 assembly language programs to be called from BASIC turned up a very interesting program titled ***Checksum gen.txt*** at http://www.archerservices.com/History/Tektronix/4051/Tapes/Checksum%20gen.txt


Actually Checksum gen.txt is a document file that contains a short 4051 program with CALL "EXEC" at the end of the document.
The document mentions a MAXIPACK and RAMPACK and line 1 of the program indicates the authors initials: MDC
This leads me to believe the author is Michael Cranford - one of the 4051 engineers and designer of a MAXIPACK and RAMPACK, both of which are installed in a working 4051 in the vintagetek.org museum! https://vintagetek.org/4051-graphic-desktop-system/

I reworked the document into a program I named 4051 ROM Checksums and uploaded it in my Universal format which only uses CR for line delimiters and includes all Tektronix control characters in printable strings.

Checksum gen as written only allowed one ROM address and Bankswitch as needed to be input and printed the checksum.

I added the ability to request a complete dump of all 4051 ROMs including the optional ROM Packs, so it is very easy to compare your 4051 ROM checksums against the MDC listed checksums.

In the screenshot from running 4051 ROM Checksums program on the 405x (actually only 4051) javascript emulator https://github.com/jonbstanley/Tek405xEmulator
you can see that the emulator is using the same ROMs as in MDC's 4051, with the exceptions of no option ROMs including no Comm backpack ROMs in the emulator screenshot below.

I run the **jsTEKTRONIX4051_universal.html** file with Chrome web browser.  The emulator is configured to run at a similar speed to the original 4051.  I edit the mc6800.js file line 64 and replace "408" with "40" to speed up the emulator to approximately the speed of the 4052 and 4054, and then launch the emulator.  All of the older programs (.txt or .BAS) in this 4050 repository should be run with the regular file, which has modified control characters and line delimiters unless the readme in that folder indicates they require the universal format emulator.

![screenshot](./4051%20Javascript%20Emulator%20ROM%20Checksums%20screenshot.png)

This screenshot includes the list of MDC ROM checksums to the left of the emulator screen:


![screenshot](./4051%20emulator%20ROM%20Checksums%20compared%20to%20MDC%204051%20checksums.jpg)
