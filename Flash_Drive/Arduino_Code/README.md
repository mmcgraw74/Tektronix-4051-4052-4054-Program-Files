I use the Arduino IDE version 1.8.18 that can be downloaded from here: https://www.arduino.cc/en/software/OldSoftwareReleases)
You need to add support for the Pandauino ATmega644 and ATmega1284 to the Arduino IDE after installation of the IDE:
1. Install the MightyCore. Follow the installation process: https://github.com/MCUdude/MightyCore#how-to-install. 
2. Download the variant file from https://github.com/mrguen/MightyCore/tree/master/avr/variants/narrow and place it inside your MightyCore install directory. For example on Windows 10 standard install it in C:\Users\ USER_NAME \AppData\Local\Arduino15\packages\MightyCore\hardware\avr\VERSION\variants
3. Modify the MightyCore install board.txt (in C:\Users\USER_NAME
\AppData\Local\Arduino15\packages\MightyCore\ hardware\avr\VERSION ) by adding these lines to the 1284 or 644 menu.pinout sections:

> 1284.menu.pinout.narrow=Narrow pinout
> 1284.menu.pinout.narrow.build.variant=narrow
> 1284.menu.pinout.narrow.build.bootloader_led=B7

> 644.menu.pinout.narrow=Narrow pinout
> 644.menu.pinout.narrow.build.variant=narrow
> 644.menu.pinout.narrow.build.bootloader_led=B7
   
4. Now close Arduino IDE (if open) and reopen Arduino IDE, connect your Flash Drive using the included USB mini B - cable and select your Flash Drive controller board – either ATmega644 or ATmega1284 (label on that IC on your Flash Drive) using Board Manager: MightyCore-Pandauino (in sketchbook) ATmega644 or ATmega1284.  Then select Port connected to your Flash Drive.  If you have several ports connected – use Windows Device Manager to find the serial port# connected to CH340.
