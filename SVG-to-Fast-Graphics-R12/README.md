# Tektronix 4051 / 4052 / 4054 graphics computers
convert SVG to Fast Graphics with 4051 RAMPACK or 4051R12 or 4052R12 Graphics Enhancement ROM Pack
*******

![4052 C3PO-BB8-R2D2](./Screenshots-and-photos/C-3PO%20BB-8%20and%20R2-D2.jpeg)


![405x C3PO-BB8-R2D2](./Screenshots-and-photos/405x%20C3PO%20BB8%20R2D2.png)

1 - Replace all semicolons with ;\r (extended expression)

2 - Replace all PD with PD\r

3 - Regex (regular expression) to replace all HPGL 2nd comma with L
          -------
     Find: (.*?\,.*?)[\L]
     Replace with: \1L

4 - Replace all L with spaceLspace (extended expression)

6 - Replace ;\rPD\r with spaceLspace

6 - Replace PU with Mspace

7 - Delete beginning lines with IN; M ; and SP1;

8 - Delete ending lines with SP0; M 0 0; and IN;

9 - Add END/r as last line

10 - Save as "FILENAMEmod.SVG" file for processing to FG format

