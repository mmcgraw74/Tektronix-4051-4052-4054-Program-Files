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
No.   File Name                     Type         Rec Len  Used Bytes

 1    advkeys                       Random       3        830     wp
 2    advshor$                      Sequential   -        4037    wp
 3    advshor#                      Random       1        158     wp
 4    advmap                        Random       1        1184    wp
 5    advitm-description            Sequential   -        1578    wp
 6    advitm-room                   Random       1        82      wp
 7    advitm-name                   Sequential   -        508     wp
 10   advf0                         Sequential   -        2554    wp
 11   advf1                         Sequential   -        1283    wp
 12   advf2                         Sequential   -        1326    wp
 13   advf3                         Sequential   -        996     wp
 14   advf4                         Sequential   -        843     wp
 15   advf5                         Sequential   -        918     wp
 16   advf6                         Sequential   -        1485    wp
 17   advf7                         Sequential   -        742     wp
 18   advf8                         Sequential   -        916     wp
 19   advf9                         Sequential   -        912     wp
 20   advf10                        Sequential   -        465     wp
 21   advf11                        Sequential   -        490     wp
 22   advf12                        Sequential   -        561     wp
 23   advf13                        Sequential   -        391     wp
 30   advf20                        Sequential   -        639     wp
 31   advf21                        Sequential   -        1678    wp
 32   advf22                        Sequential   -        1520    wp
 33   advf23                        Sequential   -        1336    wp
 34   advf24                        Sequential   -        1621    wp
 35   advf25                        Sequential   -        1674    wp
 36   advf26                        Sequential   -        2735    wp
 37   advf27                        Sequential   -        1412    wp
 38   advf28                        Sequential   -        1309    wp
 39   advf29                        Sequential   -        296     wp
 41   advf31                        Sequential   -        1800    wp
 42   Save1                         Sequential   -        44      
 43   Save2                         Sequential   -        44      
 44   Save3                         Sequential   -        44      
 45   Save4                         Sequential   -        44      
 46   Save5                         Sequential   -        44      
 47   Save6                         Sequential   -        44      
 48   Save7                         Sequential   -        44      
 49   Save8                         Sequential   -        44      
 50   Save9                         Sequential   -        44      
 51   Adventure for AUXMEM          Program      -        30923   

 42 Files  444160 Bytes Free

```

See my post in vcfed.org forum for more details on how I created this program for the AUXMEM module:
http://www.vcfed.org/forum/showthread.php?70193-TransEra-764-RAM-Auxiliary-Memory-for-Tektronix-4052-4054-Computers&p=661244#post661244
