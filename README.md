# Tektronix-4051-4052-4054-Program-Files

This repository is intended to provide a public location to upload Tektronix 4050 series program tapes or disk images.
*********
All files are compatible with the entire 4050 series - UNLESS the FOLDER or FILE says only works with one type
***********
You can download a single file, folder with all included files or whole archive by clicking the Clone or Download button.
*******************************

File Upload - Start by making a pull request of your uploaded files - including a name for the tape or disk.

I will then create a folder containing those files.

Since the 4050 series tapes only use file numbers - please use those same file numbers as the filename for each file.

Some tapes have additional program name information stored in the file Header, so also include the output of the TLIST command as a separate file in your submission.  You can capture the TLIST to serial from BASIC with TLIST @40:

*****
Updated serial transfer program steps:

Instead of having to type in my serial transfer program - use the built in CALL "TERMIN" terminal program:

Type the following commands and use the User Definable Keys above the keyboard:

CALL "RATE",9600,2                          only on 4052/54 - use 2400 max on 4051
CALL "TSTRIN","",""                         Prevents NULLs and DC3 characters from being transmitted
FIND x                                      find the file on the tape you want to transfer
CALL "TERMIN"                               this puts in the 4050 in terminal mode
press User Definable Key 16 (shift UDK 6)   turns off local echo (you only need to do this once)

get your PC program ready to capture an ASCII file

press UDK 4 Data Send

if the transfer completes without error - the tape stops
error messages like READ errors will be displayed if they occur

After the tape stops - close the file on the PC transfer program

****
I really like RealTerm (free download) - which can capture and send files or act as a terminal. To use it with the Tek - configure the Port tab for the same speed as in the RATE command. I use 9600 7E1 with no handshake. Then click the Capture tab - create a filename - click Direct Capture (no echo on the pc) - Start Overwrite.

Now Press UDK 4

after the transfer stops - click Stop on RealTerm

Press UDK 5 to return to Tek BASIC

Now you can FIND another file, type CALL "TERMIN", and you are ready to repeat the above steps to capture that file.  Instead of pressing UDK 5 you could press SHIFT UDK 3 and type a file number to FIND.  I like to be in BASIC and also do a CALL "HEADER" (4052 and 4054 only) to check what kind of file it is.  If you do that you need to do the FIND again before CALL "TERMIN" or you will get an error.

*****
The serial transfer instructions file contains instructions on wiring a 25 pin to 9 pin serial cable from the Tektronix to the PC serial port.  I tried using the serial transfer program to download to my 4054 - and my laptop is sending the characters too fast using PUTTY with software flow control.  It has been 20 years since I had this working - so my laptop is a lot faster than my desktop of 20 years ago.

******
If you don't have a serial interface on your Tektronix 4050 series computer - consider sending your tapes to someone who does, so the files can be archived.
