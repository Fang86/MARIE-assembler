How to use:

1) Open (or create) text file named "assembly.txt"

2) Enter your MARIE code into the file. Make sure to follow the formatting 
   originally used:
   (MEMORY ADDRESS) [LABEL,] [DIRECTIVE] [INSTRUCTION] [OPERAND] /comment

   Here is an example program:

    100 Load x       / This is a comment!
    101 Add 106      / instruction, operand (address)
    102 Store Y      / instruction, operand (label)
    103 Output       / instruction
    104 z, Halt      / label, instruction
    /Middle comment!
    105 0FF0         / operand (number)
    106 HEX FFE9     / directive, operand
    107 Y, 0000      / label, operand
    108 x, DEC 23144 / label, directive, operand

   Notes:
    - Comments are created by using '/'. Comments are optional.

    - Labels should be a single character (ASCII; ideally a-z, A-Z). 
      Label should be followed by a comma unless it is used as an operand. 
      For example:
      	100 Load x  / Operand label
      	101 x, FF09 / Regular label

    - Hexadecimal numbers should have exactly 3 digits for addresses and 
      4 digits for non-address numbers (compare lines 101 vs 105 above). 
      Decimal numbers do not have this requirement. Decimal number must
      be a signed 16-bit number.

    - Non-address numbers are assumed to be hexademical unless the DEC 
      directive is used. Directives can only be used for non-address 
      numbers


3) Save the file without changing its name. 

4) Open "Main.cpp" in an IDE such as Dev-C++ and run.

5) Now the assembly.txt file will be converted into binary in the same 
   formatting, then saved as "binary.txt." The program reads the file and 
   executes accordingly, line by line, starting on the first line.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  //////////////////////////////
 ///MARIE Instructions Guide///
//////////////////////////////

This program supports 9 basic MARIE instructions: Load, Store, Add, Subt, 
Input, Output, Halt, Skipcond, and Jump. These instructions should be used
as follows (X = target address/label):


Load (X)	- Load the contents of address X into AC

Store (X)	- Store the contents of AC at address X

Add (X)		- Add the contents of address X to AC and store 
		  the result in AC

Subt (X)	- Subtract the contents of address X from AC and 
		  store the result in AC

Input		- Input a value from the keyboard into AC (decimal)

Output		- Output the value in AC to the display

Halt		- Terminate the program

Skipcond (y)	- Skip the next instruction on condition (y). This 
		  can be 3 different hex numbers: 000, 400, or 800.
			000 skips if AC < 0
			400 skips if AC == 0
			800 skips if AC > 0

Jump (X)	- Load the value of X into PC




