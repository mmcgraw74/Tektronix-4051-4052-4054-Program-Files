# MONOPOLY Game 
for 4051 with 32KB RAM, 4052/4052A, and 4054/4054A computers with R12/Fast Graphics ROM PACK and GPIB Flash Drive

![MONOPOLY](./Wilma%20is%20bankrupt%20-%20GAME%20OVER.png)

----------------------------------

- [Topics](#topics)
- [Game board](#game-board)
- [History](#history)
- [List](#list)
	- [List CheckBox](#list-checkbox)
- [Link](#link)
	- [Anchor links](#anchor-links)
- [Blockquote](#blockquote)
- [Image | GIF](#image--gif)
- [Style Text](#style-text)
	- [Italic](#italic)
	- [Bold](#bold)
	- [Strikethrough](#strikethrough)
- [Code](#code)
- [Email](#email)
- [Table](#table)
	- [Table Align](#table-align)
    	- [Align Center](#align-center)
    	- [Align Left](#align-left)
    	- [Align Right](#align-right)
- [Escape Characters](#escape-characters)
- [Emoji](#emoji)
- [Shields Badges](#Shields-Badges)
- [Markdown Editor](#markdown-editor)
- [Some links for more in depth learning](#some-links-for-more-in-depth-learning)

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
# History
I started writing a Monopoly game on a Tektronix 4051 in 1979 - but ran out of 32KB of 4051 memory just displaying the Monopoly board graphics in Tektronix 4050 BASIC.  I made a hard copy printout of my program using a Tektronix 4631 Hard Copy unit attached to the 4051.  

The 4051 and 4052 had a 12" flat screen storage tube CRT with addressable vector resolution of 1024 by 780. The 4054 and 4054A had a 19" curved screen storage tube and different display hardware that accelerated all graphics 10x over the 4052/4052A.

This screenshot shows my original Monopoly game board layout.  The 4051 and 4052 use dot matrix for text, so my program put the text for the properties at the top and bottom of the screen as shown.

![MONOPOLY](./my%20original%20Monopoly%20game%20board.png)

My completed game board is completely redesigned and used a Tek 4050 BASIC program that creates Leroy character set vector lettering at any size and any rotation angle!

Since I needed to reduce the size of the BASIC program statements, I use the 4050R12 Graphics Enhancement ROM Pack and BASIC programs I created to use BASIC MOVE and DRAW commands for the vectors and then an R12 CALL "AGIN" X,Y command to capture the X,Y coordinates in GDUs (native 130,100 graphic display units) and then saved each X,Y vector in 3 byte R12 format with the R12 CALL "CHANGE",D,D$ where D was dimensioned as D(2) with D(1)=-X and D(2)=-Y.  The resulting vector string was then written to a binary data file.

All the Monopoly board lines, text, and images like the Jailer and Free Parking are in a single file 42 with a size of 17613 bytes representing about 5800 move or draw X/Y vectors - since the Tek 4050 BASIC binary data format adds a two byte header and one byte trailer to each data block.  The two byte header includes the length of the binary string.  The BASIC programs I created to make each of the binary files for the deeds include an end of card string |EOC| which my code uses to know the number of lines of text displayed on the card have all been read and the deed display code is done.

My Monopoly BASIC program is the largest Tektronix BASIC program I have written and I learned a lot of 4050 BASIC tricks to move as much graphics data and numeric data to external files to allow the most space for the Monopoly game logic.

I hope you enjoy this game as much as I do!

I limited the string variable length for drawing the entire Monopoly board to 1023 bytes which represents 341 MOVE or DRAW X,Y vectors. 


