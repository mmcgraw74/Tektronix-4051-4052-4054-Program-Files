I use the Arduino IDE version 1.8.18 that can be downloaded from here: https://www.arduino.cc/en/software/OldSoftwareReleases)
You need to add support for the Pandauino ATmega644 and ATmega1284 to the Arduino IDE after installation of the IDE:
1. Install the MightyCore. Follow the installation process: https://github.com/MCUdude/MightyCore#how-to-install. 
2. Download the variant file from https://github.com/mrguen/MightyCore/tree/master/avr/variants/narrow and place it inside your MightyCore install directory. For example on Windows 10 standard install it in
   i. C:\Users\ USER_NAME \AppData\Local\Arduino15\packages\MightyCore\hardware\avr\VERSION\variants
