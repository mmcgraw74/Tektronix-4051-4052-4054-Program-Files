# Tektronix-4051-4052-4054-Program-Files

This repository is intended to provide a public location to upload Tektronix 4050 series program tapes or disk images.

Start by making a pull request of your uploaded files - including a name for the tape or disk.

I will then create a folder containing those files.

Since the 4050 series tapes only use file numbers - please use those same file numbers as the filename for each file.

Some tapes have additional program name information stored in the file Header, so also include the output of the TLIST command as a separate file in your submission.

I have posted my Tektronix 4050 serial program - which autodetects a 4051 to use a 2400 baud even parity 7-bits with 1 stop bit transfer rate to your PC.  If you have a 4052, 4052A, 4054 or 4054A the program will use 9600 baud rate.

I use PUTTY or EXTRAPUTTY program on my PC to capture the file from the 4050 serial port, and copy and paste the program listing to Notepad++ and then save the file to disk or USB flash drive.

The serial transfer instructions file contains instructions on wiring a 25 pin to 9 pin serial cable from the Tektronix to the PC serial port.  I tried using the serial transfer program to download to my 4054 - and my laptop is sending the characters too fast using PUTTY with software flow control.  It has been 20 years since I had this working - so my laptop is a lot faster than my desktop of 20 years ago.

You should be able to use the full serial transfer program to upload tape files to your PC - but you will have to type the full program in manually.  Replace tilde X tilde with CTRL-X substituting the character you see between the tilde characters for the control character.

If you don't have a serial interface on your Tektronix 4050 series computer - consider sending your tapes to someone who does, so the files can be archived.
