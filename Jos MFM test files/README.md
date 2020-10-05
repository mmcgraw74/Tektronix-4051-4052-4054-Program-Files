Directory with test ROM images and screenshots from Jos Dreesen's Multifunction ROM Pack
---
This screenshot is Realterm software, showing connection baud rate
I have several crossover cables connecting the MFM to PC USB Serial converter and haven't reproduced this success yet (as of Oct 5 2020)

![Label and PCB front](./4054aRS232captureOfMFMprotoWithJumperPrinting.jpeg)
---
Download CRCDUMP6.txt and run with Jos' Diagnostic ROM Pack in the left slot to see BASIC ROM CRCs - verify the firmware version your 4052/4504 against the published CRCs
---
CRCDUMP7.txt runs with Jos' Diagnostic ROM pack in the left slot and 4050E01 ROM Expander in right slot.
This adds the CRC checks for the ROM Expander slots 51 through 58 - warning this only seemed to work in my 4054A.
---
My hypothesis is the 4052 Opt1 Comm Backpack has BSX reversed between left and right slots
