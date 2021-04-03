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
