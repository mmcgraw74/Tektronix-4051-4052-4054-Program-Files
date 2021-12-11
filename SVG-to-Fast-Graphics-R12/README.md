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

