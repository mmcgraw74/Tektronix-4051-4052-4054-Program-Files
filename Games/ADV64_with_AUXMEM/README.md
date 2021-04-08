Tektronix Adventure for 4052/4054 with Jos Dreesen's AUXMEM and Multi-function ROM pack (MFM)
-----

![Adventure running from AUXMEM module](./Updated%20Adventure%20running%20from%20AUXMEM.jpg)

Jos Dreesen's new AUXMEM Module is his hardware design emulating the TransEra Auxiliary Memory Module for Tektronix 4052/4054 computers.

His AUXMEM module design fits in a single ROM slot and contains 512KB of static RAM with a CR2032 lithium battery backup that should maintain the files for over a year.


![Jos Dreesen's AUXMEM module](./AMM_top.jpg)

In order to use the AUXMEM Module, you need Jos' modified TransEra 764MEM ROM pack image installed in his Multi-Function ROM Pack that can hold up to 8 Tektronix option ROM Packs.  Here is a photo of my assembled MFM module - which also contains a TransEra 741RTC real-time-clock and Tektronix RS-232 Printer option.

![My assembled MFM module](./My%20MFM%20assembly.jpeg)
-------------
Follow my directions in the README in the UTILITIES folder to transfer all the Adventure files and Adventure program to the AUXMEM module.

Then load and run the Adventure program with these commands:
**CALL "MOLD",51
RUN

Here is the directory listing of all the Adventure files in the AUXMEM Pack:

```
$ ./tekcmd.exe -dir -img ADV.img
This image contains 42 files,
and uses 313 blocks out of 2048 total.

 - - - - file name - - - -   size--type-prot.

advkeys.....................  830  RAND  P
advshor$....................  4037  SEQU  P
advshor#....................  158  RAND  P
advmap......................  1184  RAND  P
advitm-description..........  1578  SEQU  P
advitm-room.................  82  RAND  P
advitm-name.................  508  SEQU  P
advf0.......................  2554  SEQU  P
advf1.......................  1283  SEQU  P
advf2.......................  1326  SEQU  P
advf3.......................  996  SEQU  P
advf4.......................  843  SEQU  P
advf5.......................  918  SEQU  P
advf6.......................  1485  SEQU  P
advf7.......................  742  SEQU  P
advf8.......................  916  SEQU  P
advf9.......................  912  SEQU  P
advf10......................  465  SEQU  P
advf11......................  490  SEQU  P
advf12......................  561  SEQU  P
advf13......................  391  SEQU  P
advf20......................  639  SEQU  P
advf21......................  1678  SEQU  P
advf22......................  1520  SEQU  P
advf23......................  1336  SEQU  P
advf24......................  1621  SEQU  P
advf25......................  1674  SEQU  P
advf26......................  2735  SEQU  P
advf27......................  1412  SEQU  P
advf28......................  1309  SEQU  P
advf29......................  296  SEQU  P
advf31......................  1800  SEQU  P
Save1.......................  44  SEQU  .
Save2.......................  44  SEQU  .
Save3.......................  44  SEQU  .
Save4.......................  44  SEQU  .
Save5.......................  44  SEQU  .
Save6.......................  44  SEQU  .
Save7.......................  44  SEQU  .
Save8.......................  44  SEQU  .
Save9.......................  44  SEQU  .
Adventure for AUXMEM........  30923  PROG  .
```

See my post in vcfed.org forum for more details on how I created this program for the AUXMEM module:
http://www.vcfed.org/forum/showthread.php?70193-TransEra-764-RAM-Auxiliary-Memory-for-Tektronix-4052-4054-Computers&p=661244#post661244
