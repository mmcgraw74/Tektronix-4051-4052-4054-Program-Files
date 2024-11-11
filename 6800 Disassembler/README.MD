Motorola 6800 Disassembler - Tektronix 4050 BASIC Program
-------------

This BASIC program was contributed to the 4050 Application Library with this abstract in Tekniques

![4052/4054 and A-series Opcode table](./MC6800%20Disassembler%20Program%20abstract.png)

I recovered this file and the example 6800 object code data file in ASCII HEX format in an Application Library tape from the estate of Jack Everson - retired Tektronix Sales Engineer.

This program has been modified by me to 
**4052/4054 CPU Design goals**
1. 6800 compatibility
   - Must be able to leverage the 4051 32KB BASIC ROM firmware development
2. 8 times faster than the 4051
3. 3 Factors of Two
   - Double CPU clock speed (2MHz vs 800KHz)
   - Double memory bandwidth with 16-bit memory width
     - This also doubled the address space which doubled the RAM to 64KB and doubled the ROM space to 64KB!
   - Harvard Architecture - overlap instruction and data fetch
