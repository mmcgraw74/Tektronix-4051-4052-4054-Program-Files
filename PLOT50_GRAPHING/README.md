This folder contains files I recovered from a **PLOT50 GRAPHING tape**
***********
This program makes heavy use of a UDK (User Definable Key) overlay, I created the UDK overlay file below from the Blank overlay in the root of this repository using PAINT to add the text, by examining the instructions in the two menu screenshots for the ten UDK keys and SHIFT UDK keys:

![EE V1 UDK Overlay](./Electrical%20Engineering%20Vol%201%20UDK%20overlay.jpg)

ASCII PROGRAM files 1,5,6 and 7 are in my "Universal" (.UNI) BASIC text format which will load and run on any 4050 series computer after all the programs have been stored on tape in file number sequence.

FILE1 is the Menu program and asks if you have the BINARY PROGRAM LOADER, answer NO, and this program will load ASCII PROGRAM 5.
At this point the three ASCII programs are the entire GRAPHICs library.

I have not written a program to create the BINARY files on tape from the tape dump file - but you won't need them.

On a blank DC300 tape, mark each file with the file size listed in the TLIST file below and in this folder:

```
 1      ASCII   PROGRAM            1536   
 2      BINARY  PROGRAM            20224  
 3      BINARY  PROGRAM            20224  
 4      BINARY  PROGRAM            20224  
 5      ASCII   PROGRAM            17152  
 6      ASCII   PROGRAM            16128  
 7      ASCII   PROGRAM            15104  
 8      NEW                        10240  
 9      LAST                       768    
```

Example:
To prepare the tape for file 1:
```
FIND 1
MARK 1,1536
```
Now use the serial interface to transfer ASCII PROGRAM FILE1.UNI into 4051 memory, then SAVE the program to FILE 1 on your tape:
```
OLD@40:
FIND 1
SAVE
``` 
Then, for 'BINARY' files 2 through 4, you only need one MARK command to create all 3 'NEW' files:
```
FIND 2
MARK 3, 20224
```
These 'NEW' files are placeholders for later, if I create a program to restore BINARY files.

Then, for ASCII PROGRAM files 5 through 7:
```
FIND 5
MARK 1,17152
OLD@40: ! transfer FILE 5 into 4051 memory
FIND 5
SAVE    ! save FILE 5 to tape

FIND 6
MARK 1,16128
OLD@40: ! transfer FILE 6 into 4051 memory
FIND 6
SAVE    ! save FILE 6 to tape

FIND 7
MARK 1,15104
OLD@40: ! transfer FILE 7 into 4051 memory
FIND 7
SAVE    ! save FILE 7 to tape
```

Then, for 'NEW' file 8:
```
FIND 8
MARK 1,10240
```
This 'NEW' file is a placeholder for you to use to save GRAPHING files - don't save programs here.
 
No need to MARK the LAST file - it is always created as the file after the last one MARKed.

***********
The pdf in this folder is a two page article on this set of programs in Tekniques Volume 1 Number 8.

![Label and PCB front](./PLOT%2050%20Electrical%20Engineering%20article.jpg)
