This folder contains files I recovered from a **PLOT50 Electrical Engineering Volume 1 tape**
***********
This program makes heavy use of a UDK (User Definable Key) overlay, I created the UDK overlay file below from the Blank overlay in the root of this repository using PAINT to add the text, by examining the instructions in the two menu screenshots for the ten UDK keys and SHIFT UDK keys:

![EE V1 UDK Overlay](./Electrical%20Engineering%20Vol%201%20UDK%20overlay.jpg)

All these PROGRAM files are in my "Universal" (.UNI) BASIC text format which will load and run on any 4050 series computer after all the programs have been stored on tape in file number sequence:

On a blank DC300 tape, mark each file with the file size listed in the TLIST file below and in this folder:

```
 1      ASCII   PROGRAM           4608   
 2      ASCII   PROGRAM           10240  
 3      ASCII   PROGRAM           6912   
 4      ASCII   PROGRAM           2048   
 5      ASCII   PROGRAM           7168   
 6      ASCII   PROGRAM           5888   
 7      ASCII   PROGRAM           14848  
 8      ASCII   PROGRAM           14848  
 9      ASCII   PROGRAM           14848  
 10     ASCII   PROGRAM           14848  
 11     ASCII   PROGRAM           14848  
 12     ASCII   PROGRAM           14848  
 13     NEW                       2816   
 14     NEW                       2816   
 15     NEW                       2816   
 16     NEW                       2816   
 17     NEW                       2816   
 18     NEW                       2816   
 19     NEW                       2816   
 20     NEW                       2816   
 21     NEW                       2816   
 22     NEW                       2816   
 23     NEW                       2816   
 24     NEW                       2816   
 25     NEW                       2816   
 26     NEW                       2816   
 27     NEW                       2816   
 28     NEW                       2816   
 29     NEW                       2816   
 30     NEW                       2816   
 31     NEW                       2816   
 32     NEW                       2816   
 33     LAST                      768    
```

Example:
To prepare the tape for file 1:
```
FIND 1
MARK 1,4608
```
Now use the serial interface to transfer ElecEngFILE1.UNI into 4051 memory, then SAVE the program to FILE 1 on your tape:
```
OLD@40:
FIND 1
SAVE
``` 
Repeat the three steps above for each of the 12 program files in file number order.

Then, for 'NEW' files 19 through 38, you only need one MARK command to create them:
```
FIND 13
MARK 20,2816
```
These 'NEW' files are placeholders for you to use to save CODE and DATA files - don't save programs here.
 
No need to MARK the LAST file - it is always created as the file after the last one MARKed.

***********
The pdf in this folder is a two page article on this set of programs in Tekniques Volume 1 Number 8.

![Label and PCB front](./PLOT%2050%20Electrical%20Engineering%20article.jpg)
