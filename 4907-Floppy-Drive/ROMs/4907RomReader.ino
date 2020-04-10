/* Modified from original program at http://danceswithferrets.org/geekblog/?p=315
   "Reading a Parallel ROM with an Arduino"
   This program only requires an Arduino Mega, male to female jumper wires and 5V at 1A external power supply for ROM board
   1. Remove all but one ROM, leave FPLA and Patch ROM installed (make sure you remember where each ROM went, labels could be gone)
   2. Connect jumper wires as in the table below between the Mega board and 4907 J1
   3. Connect the external 5V 1A power supply to the 4907 J1 connector pins:
      +5V to J1 pins B3 and B24
      GND to J1 pins B2 and B25
      GND to Arduino Mega GND pin (next to Vin pin)
   4. Recheck all connections with ohmmeter from jumper wire on Mega to one of the data buffers connected directly to J1
   5. Plug USB cable into Arduino Mega and plug into PC
   6. Load this program into Arduino Mega using Arduino IDE
   7. Turn on external power supply to 4907 ROM board with only U121 A000 ROM installed
   8. Start serial monitor in Arduino at 9600 baud
   9. Program will display A000 ROM contents
   10. Copy and paste the console to a text file to save the PATCHED output for that ROM
   11. Edit the program to change the address variable to match the next ROM
   12. Download the edited program, restart the serial monitor, capture the output and repeat for U131 to U261.
   13. Edit the program to change BNKROM to LOW and address variable to 12 for the BANK ROM U271
   14. Download the edited program, restart the serial monitor, capture the U271 output
   15. Turn off the external power supply and remove the USB cable from the Arduino Mega
   16. Replace all the ROMs into their original locations

   One of my Tektronix 4907 ROM boards has thirteen 2Kx8 Motorola MCM68316E ROMS
   plus an FPLA and one Patch ROM in U631 or U641
   
   In order to download the ROMs with all the patches - I decided to brute force read the ROM board
   one ROM at a time with an Arduino MEGA board connected to a PC to record the output data.
   
   My program modifications included:
   - adding address A15, A15, and BNKROM pins for 4907 ROM board decode to work
   - adding an ADDRESS variable for the ROM start address, and changed the ROM range to 2048
   
   To use this program - be sure the BANK bit is set to LOW to read BANK ROM 271 at address 6000
   and set the BANK bit to HIGH for all the other ROMs.
   
   Set the ADDRESS to 20*512 for A000 ROM and 21 for A800 up to 31*1024 for the F800 ROM (12 for the BANK ROM at 6000 hex)
   ROM addresses shown on 4907 service manual page 8-13 ROM Board schematic 2-1:
   
   U#		Start Address (HEX)
   ----		-------------------
   U121		A000
   U131		A800
   U141		B000
   U151		B800
   U161		C000
   U201		C800
   U211		D000
   U221		D800
   U231		E000
   U241		E800
   U251		F000
   U261		F800
   U271		6000	although this ROM decode is from mainboard using BNKROM-0 J1 pin to select U271
   
   This 4907 system had a label on back indicating firmware version 1.2
   ROM Board 670-5358-03, with several ROMs and ICs dated at the end of 1980
   U141 was an Intel 2716 EPROM with a paper label:
   Tektronix 1978
   156-1069-00
   U141      V1.2
   
   The FPLA was labeled 156-0940-14
   and the U631 PROM was labeled -096008
   
*/
/*
ROM Reader. Quick Arduino program to read a parallel-accessed ROM and dump it to the serial
port in hex.

Oddbloke. 16th Feb 2014.
  Modified by Monty McGraw April 5, 2020 for Tektronix 4907 ROM board with Patch ROM
 */

//#include string.h

// How I've wired the digital pins on my Arduino to the address and data pins on
// the ROM.
/*						Arduino		4907
						Mega Board	ROM board
							pin		J1 pin
							-----	------*/
static const int kPin_A0  = 53;//	A8
static const int kPin_A1  = 51;//	A9
static const int kPin_A2  = 49;//	A10
static const int kPin_A3  = 47;//	A11
static const int kPin_A4  = 45;//	B13
static const int kPin_A5  = 43;//	B12
static const int kPin_A6  = 41;//	B11
static const int kPin_A7  = 39;//	B6
static const int kPin_A8  = 46;//	B5
static const int kPin_A9  = 48;//	B4
static const int kPin_A10 = 52;//	A1
static const int kPin_A11 = 50;//	B14
static const int kPin_A12 = 37;//	A6
static const int kPin_A13 = 44;//	B7
static const int kPin_A14 = 36;//	A7
static const int kPin_A15 = 34;//	B8
static const int kPin_BNKROM = 26;//A22

static const int kPin_D0 = 28;//	B15
static const int kPin_D1 = 30;//	A15
static const int kPin_D2 = 32;//	B16
static const int kPin_D3 = 33;//	A16
static const int kPin_D4 = 31;//	B17
static const int kPin_D5 = 29;//	B18
static const int kPin_D6 = 27;//	A18
static const int kPin_D7 = 25;//	B19

const char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7',
              '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void setup()
{
  // set the address lines as outputs ...
  pinMode(kPin_A0, OUTPUT);     
  pinMode(kPin_A1, OUTPUT);     
  pinMode(kPin_A2, OUTPUT);     
  pinMode(kPin_A3, OUTPUT);     
  pinMode(kPin_A4, OUTPUT);     
  pinMode(kPin_A5, OUTPUT);     
  pinMode(kPin_A6, OUTPUT);     
  pinMode(kPin_A7, OUTPUT);     
  pinMode(kPin_A8, OUTPUT);     
  pinMode(kPin_A9, OUTPUT);     
  pinMode(kPin_A10, OUTPUT);     
  pinMode(kPin_A11, OUTPUT);     
  pinMode(kPin_A12, OUTPUT);     
  pinMode(kPin_A13, OUTPUT);
  pinMode(kPin_A14, OUTPUT);
  pinMode(kPin_A15, OUTPUT);
  pinMode(kPin_BNKROM, OUTPUT);
 
  // set the data lines as inputs ...
  pinMode(kPin_D0, INPUT); 
  pinMode(kPin_D1, INPUT); 
  pinMode(kPin_D2, INPUT); 
  pinMode(kPin_D3, INPUT); 
  pinMode(kPin_D4, INPUT); 
  pinMode(kPin_D5, INPUT); 
  pinMode(kPin_D6, INPUT); 
  pinMode(kPin_D7, INPUT); 
  
  Serial.begin(9600);
}

void SetAddress(int addr)
{
  // update the address lines to reflect the address we want ...
  digitalWrite(kPin_A0, (addr & 1)?HIGH:LOW);
  digitalWrite(kPin_A1, (addr & 2)?HIGH:LOW);
  digitalWrite(kPin_A2, (addr & 4)?HIGH:LOW);
  digitalWrite(kPin_A3, (addr & 8)?HIGH:LOW);
  digitalWrite(kPin_A4, (addr & 16)?HIGH:LOW);
  digitalWrite(kPin_A5, (addr & 32)?HIGH:LOW);
  digitalWrite(kPin_A6, (addr & 64)?HIGH:LOW);
  digitalWrite(kPin_A7, (addr & 128)?HIGH:LOW);
  digitalWrite(kPin_A8, (addr & 256)?HIGH:LOW);
  digitalWrite(kPin_A9, (addr & 512)?HIGH:LOW);
  digitalWrite(kPin_A10, (addr & 1024)?HIGH:LOW);
  digitalWrite(kPin_A11, (addr & 2048)?HIGH:LOW);
  digitalWrite(kPin_A12, (addr & 4096)?HIGH:LOW);
  digitalWrite(kPin_A13, (addr & 8192)?HIGH:LOW);
  digitalWrite(kPin_A14, (addr & 16384)?HIGH:LOW);
  digitalWrite(kPin_A15, (addr & 32768)?HIGH:LOW);
}

byte ReadByte()
{
  // read the current eight-bit byte being output by the ROM ...
  byte b = 0;
  if (digitalRead(kPin_D0)) b |= 1;
  if (digitalRead(kPin_D1)) b |= 2;
  if (digitalRead(kPin_D2)) b |= 4;
  if (digitalRead(kPin_D3)) b |= 8;
  if (digitalRead(kPin_D4)) b |= 16;
  if (digitalRead(kPin_D5)) b |= 32;
  if (digitalRead(kPin_D6)) b |= 64;
  if (digitalRead(kPin_D7)) b |= 128;
  
  return(b);
}

void loop()
{
  byte d[16];
  int x, y, i;
  unsigned int addr, address, dataNumber;
  char rx_byte = 0;
  String rx_str = "";
  boolean not_number = false;
  boolean end_marker = false;
  
  Serial.print("Input number for ROM to be READ: 12=6000, 20=A000, 21=A800, ... 30=F000, 31=F800: ");
  
  while (Serial.available() == 0)  // is a character available?
  
    while (end_marker == false) {
      
    rx_byte = Serial.read();       // get the character
    
    if ((rx_byte >= '0') && (rx_byte <= '9')) {
      rx_str += rx_byte;
    }
    else if (rx_byte == '\n') {
      // end of string
      end_marker = false;
      
      if (not_number==true && end_marker==true) {
        Serial.println("Not a number");
      }
      else {
        dataNumber = rx_str.toInt();
        // print the result
        Serial.print(rx_str);
        Serial.println("\n");
        end_marker=true;
      }
      
    }
    else {
      // non-number character received
      not_number = true;    // flag a non-number
    }
   // end: while (end_marker == false)
  }
  //
  digitalWrite(kPin_BNKROM, HIGH); // LOW FOR BANK ROM, HIGH FOR ALL OTHERS
 
  address = dataNumber * 2048;  //set to 12 for BANK ROM, 20 for A000 up to 31 for F800 ROM

  if (dataNumber==12) {
    digitalWrite(kPin_BNKROM, LOW); // LOW FOR BANK ROM
  }
   
  // The only reason I'm choosing to read in blocks of 16 bytes
  // is to keep the hex-dump code simple. You could just as easily
  // read a single byte at a time if that's all you needed.
  
  Serial.print("\nROM at ");
  Serial.print(address,HEX);
  Serial.println("\n");
  
  //for (addr = address; addr < address+2048; addr += 16)
  for (i=0; i<2048; i +=16)
    {
    addr = address + i;
    
    // read 16 bytes of data from the ROM ...
    for (x = 0; x < 16; x++)
    {
      SetAddress(addr + x); // tells the ROM the byte we want ...
      d[x] = ReadByte(); // reads the byte back from the ROM
     
    }
  
    // now we'll print each byte in hex ...
    
    Serial.print(address + i,HEX);
    Serial.print(" ");
    for (y = 0; y < 16; y++)
    {
      Serial.print(hex[ (d[y] & 0xF0) >> 4  ]);
      Serial.print(hex[ (d[y] & 0x0F)       ]);
    }
          
    // and print an ASCII dump too ...
    
    Serial.print(" ");
    for (y = 0; y < 16; y++)
    {
      char c = '.';
      if (d[y] > 32 && d[y]<127) c = d[y];
      Serial.print(c);
    }
      
    Serial.println("");
  }
 // All done, so lockup ...
 while (true) {delay(10000);} 
}
