Tektronix PLOT 50: System Software Tape for 4050 computers
-----
What is the PLOT50: System Software Tape?  Here is the screenshot of running the MENU file 1 on the 405x Emulator:

![PLOT 50 System Software Tape Menu](./Screenshots%20and%20Photos/PLOT50_System_Software_Tape_Menu.png)

Programs 1-4 plot functions: Y-only Data, X vs Y Data, Function Plot and Histogram data.

Program 5 begins the tutorial of many of the new BASIC commands in PLOT50 BASIC including graphics commands (includes most of the programs on this tape).

Program 6 (files 98-100) allow the user to verify that all Keyboard keys, Graphics, and Tape are working (I would call this hardware verification).

Program 7 (file 101 and 102) only runs on the 4051 as it uses the CALL "EXEC" command to run an assembly program to test RAM (not the ROM, so I would call this a RAM test, not FIRMWARE).

Program 8 (file 103 and 104) uses the RND(0) to detect one of the two 4054 models, and if detected it runs program 104 to demonstrate the unique features of the 4054 including the higher resolution, the graphic pointer with X and Y thumbwheels on the right side of the keyboard, four character sizes of 9 fonts with vector characters instead of dot matrix characters, and dashed line drawing capabilities using an 8-bit mask to define the on/off dash sequence.
