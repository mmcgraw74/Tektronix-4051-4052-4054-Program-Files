# MONOPOLY Game 
for 4051 with 32KB RAM, 4052/4052A, and 4054/4054A computers with R12/Fast Graphics ROM PACK and GPIB Flash Drive

![MONOPOLY](./Wilma%20is%20bankrupt%20-%20GAME%20OVER.png)

----------------------------------

- [Topics](#topics)
- [Requirements](#requirements)
- [Game board](#game-board)
- [Features Not Supported](#features-not-supported)
- [History](#history)

----------------------------------

# Requirements
- **Monopoly** requires a **Tektronix 4051 with 32KB of RAM**, or **any 4052/4052A** or **4054/4054A** computer **with R12 or Fast Graphics ROM Pack** and my **GPIB Flash Drive.**
  - If you don't have a Tektronix 4050 computer - you may enjoy playing my Monopoly game on the **latest Tektronix 405x Emulator** [Latest 405x experimental web-browser based emulator](https://github.com/Twilight-Logic/Tek405xEmulator/blob/master/experimental/JonStanley-Mod-Storage-Pix-20240427.zip).  Supported web browsers include Chrome, Firefox and Edge.  Follow my instructions in this post https://forum.vcfed.org/index.php?threads/tek-405x-web-browser-emulator.62548/post-1357155 - be sure to **Click to Expand** the instructions in my previous post but use my link above to download the latest version of the 405x emulator.  The 405X Emulator includes 32KB of RAM, 32KB Tek 4050 BASIC ROM, the R12 option ROM Pack, and GPIB Flash Drive but only supports importing a single directory of 4050 Flash Drive files.  I used this specific emulator to develop my Monopoly game.  To speed up development, I typically increase the emulator speed from the default 1x 4051 to at least 10x which is close to the performance running Monopoly on my 4054A computer.  Use the PC or MAC function keys 1-4 and 8 to operate the game running on the emulator.

- My Monopoly game uses the Tektronix User Definable Keys 1-4 and 8.

- To enable the game on a 4050 computer with my GPIB Flash Drive to use a Vectrex gamepad in addition to the UDK keys, after the game is loaded, press the BREAK key to stop the program and type 6110 and press the RECALL LINE editing key and then press the RUB OUT key to delete the TURN characters and type M to change the text to REM and press the RETURN key to save the change.  Now you can use the Vectrex gamepad buttons 1-4 to perform the same operations as pressing UDK 1-4.

----------------------------------

# Game Board
(yellow numbers on screenshot below correspond to numbered list below) 

![MONOPOLY](./Wilma%20is%20bankrupt%20-%20GAME%20OVER-markups.png)

* **Player**
 1. Player **TOKEN** is first initial of player name (up to 6 characters)
 2. Blinking Player **TOKEN** is current player

* **Action Keys** (UDK on Tek keyboard, Function keys on PC or MAC with 405x Emulator)
3. **F1** **ROLL** dice or **CONTINUE** (by clearing screen for next player or continues to next action)
4. **F2** **BUY** property at the current player location or buys a house or hotel on an owned property group
5. **F3** **VIEW Assets** displays list of asset prices - used for Income Tax in addition to player cash
6. **F4** **USE Jail Card** if player is in Jail and has a Get Out of Jail card
7. **F8** **QUIT** will stop the game and reload the Flash Drive MAIN MENU

* **Property**
8. Property Owner marked with **Owner TOKEN**
9. Properties in the same property group have the same **dashed UNDERLINE** under each property name
10. When all properties in a property group are owned by the same player the group has a **group outline** around the **Owner TOKENs**

* **Game Messages**
11. Show which property the current player is on.  At the start of each turn - the "Deed" area is blank and the player can use any of the five Action Keys.
12. After F1 is pressed, the two dice are displayed with a total of the two dice and the player **TOKEN** is advanced that number of spaces and then the name of the property is displayed along with the property DEED in the middle of the screen.
13. If that property is owned, the player pays the rent as shown on the deed.

* **Houses and Hotel**
14. After all the properties in a property group has the same owner, the owner may buy a house for any of the properties in that group. You must buy houses evenly following the standard Monopoly rules.  After four houses are on all the properties in a group you may buy a hotel one or more of the group of properties.

* **JAIL**
15. There are four jail cells - one for each possible player starting from the right cell for player1.
16. Possession of a Get Out of Jail card is a number displayed at the top of the jail cell column for that player.
  
----------------------------------

# Features Not Supported
Limited 32KB memory space for more features to be compatible with the 4051 is the primary reason.

 1. MORTGAGE and UNMORTGAGE are not supported.    
 2. SELL and AUCTION are not supported.

----------------------------------

# History
I started writing a Monopoly game on a Tektronix 4051 in 1979 - but ran out of 32KB of 4051 memory just displaying the Monopoly board graphics in Tektronix 4050 BASIC.  I made a hard copy printout of my program using a Tektronix 4631 Hard Copy unit attached to the 4051. 

![MONOPOLY orig scan](./Original%20Monopoly%20page1%20scan.jpg)

The 4051 and 4052 had a 12" flat screen storage tube CRT with addressable vector resolution of 1024 by 780. The 4054 and 4054A had a 19" curved screen storage tube and different display hardware that accelerated all graphics 10x over the 4052/4052A.

This screenshot shows my original Monopoly game board layout.  The 4051 and 4052 use dot matrix for text, so my program put the text for the properties at the top and bottom of the screen as shown.

![MONOPOLY](./my%20original%20Monopoly%20game%20board.jpg)

My completed game board is completely redesigned and used a Tek 4050 BASIC program that creates Leroy character set vector lettering at any size and any rotation angle!

Since I needed to reduce the size of the BASIC program statements, I use the 4050R12 Graphics Enhancement ROM Pack and BASIC programs I created to use BASIC MOVE and DRAW commands for the vectors and then an R12 CALL "AGIN" X,Y command to capture the X,Y coordinates in GDUs (native 130,100 graphic display units). Next each X,Y vector is saved in 3 byte R12 format with the R12 CALL "CHANGE",D,D$ where D is dimensioned as D(2) with D(1)=-X and D(2)=-Y.  The resulting vector string combining multiple MOVE and DRAW vectors was then written to a single binary data file.

All the Monopoly board lines, text, and images like the Jailer and Free Parking are in a single file 42 with a size of 17613 bytes representing about 5800 move or draw X/Y vectors - since the Tek 4050 BASIC binary data format adds a two byte header and one byte trailer to each data block.  The two byte header includes the length of the binary string.  The BASIC programs I created to make each of the binary files for the deeds include an end of card string |EOC| which my code uses to know the number of lines of text displayed on the card have all been read and the deed display code is done.

My Monopoly BASIC program is the largest Tektronix BASIC program I have written and I learned a lot of 4050 BASIC tricks to move as much graphics data and numeric data to external files to allow the most space for the Monopoly game logic.

I hope you enjoy this game as much as I do!

I limited the string variable length for drawing the entire Monopoly board to 1023 bytes which represents 341 MOVE or DRAW X,Y vectors. 


