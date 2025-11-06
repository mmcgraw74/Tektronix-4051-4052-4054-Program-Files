# **General Function Plot Z=F(X,Y)**

Recovered from Tektronix 4050 Education/Research Tape T2 062-5983-01 file 2

-----
The Education/Research Tape T2 was labeled version -01

I extracted the General Function Plot instructions PDF from the TekniquesVol5No4-T1 pdf in that directory of this repository, and in that document this program was listed as program 11 in file 33.

The Tektronix Applications Library PDF from September 1981 lists this program on the Education/Research Tape T2 as _3D Function Plot_ with _280_ statements

But the General Function Plot instructions list this program (same author: Dony Roberts) as 452 statements, and examining the author's flowchart in the instructions - the General Function Plot supports X and Y grids and instead of printing the DATA statement of parameters, it displays the X, Y and Z axis MIN and MAX values.

I conclude that the Education/Research Tape T2 which was issued AFTER the Tekniques V5N4 Tape 1 has an older version of the program named _3D Function Plot_

-----

Here is the output of running the _CASE 2_ plot using the 3D Function BASIC program on the Tektronix 4051 Emulator:

![CASE 2 - 3D Function Plot](./GenFNCTplot-pg14_using_3D_Function_program.png)

and here is the CASE 2 example run with the General Function Plot program for comparison with the 3D Function Plot program output:

![CASE 2 - General Function Plot in PDF](./CASE%202%20Example%20in%20pdf.jpg)
