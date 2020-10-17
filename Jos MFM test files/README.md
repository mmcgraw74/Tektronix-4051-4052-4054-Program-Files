Directory with test ROM images and screenshots from Jos Dreesen's Multifunction ROM Pack
---
This screenshot is Realterm software, showing connection baud rate.
I rewired my MFM DB-9 connector:

 - to disconnect pin 1 from the board and connect the board to pin 7
 - and disconnect pin 4 from the board and connect the board to pin 8.
 
 These two changes modify the board to use RTS/CTS hardware handshake instead of DTR/CD.
 
 I was successful in using an old "Laplink" red serial cable with DB-9 to DB-9 crossover connectors
 
 NOTE: for the 4052 with MFM I had to address the RS-232 Printer Interface in Expander slot 43 if the MFM is installed in the right slot.  Use 53 if the MFM is in the left slot


![Label and PCB front](./4054aRS232captureOfMFMprotoWithJumperPrinting.jpeg)
---
Download CRCDUMP-MFM18.txt and run with Jos' Diagnostic ROM Pack in the left slot to see MFM ROM CRCs
---
CRCDUMP-MFM18.txt runs with Jos' Diagnostic ROM pack in the left slot and Jos' Multifunction Module in right slot.
This adds the CRC checks for MFM or ROM Expander slots.

![Label and PCB front](./4052%20CRC18%20closeup%20column%20descriptions.jpg)
---
My hypothesis on why the MFM in 4052 right slot is reported in the left slot is the 4052 Opt1 Comm Backpack has BSX reversed between left and right slots

MFM18.BIN is my latest MFM ROM image including the corrected File Manager-01 ROMs.

If you put the Diag ROM in the MFM slot 1 - you can only use it with the CRCDUMP6 to display your BASIC ROM CRCs.

You cannot use the Diag ROM in MFM slot 1 with CRCDUMP18MFM, I suspect because the Diag ROM hardcodes itself to bankswitch to the true LEFT slot NOT the Slot 1 in an expander.
