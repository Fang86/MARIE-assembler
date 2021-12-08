How to use:

1) Open the text file named "assembly.txt"

2) Enter your MARIE code into the file. Make sure to follow the formatting 
originally used:
(MEMORY ADDRESS) (INSTRUCTION) (OPERAND)

Here is an example:
100 Load 105
101 Add 106
102 Store 107
103 Output
104 Halt
105 0023
106 FFE9
107 0000

3) Save the file without changing its name. 

4) Open "Main.cpp" in a compiler such as Dev-C++

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Now the assembly.txt file will be converted into binary in the same 
formatting, then saved as "binary.txt." The program reads the file and 
executes accordingly, line by line.

This program supports 9 basic MARIE instructions: Load, Store, Add, Subt, 
Input, Output, Halt, Skipcond, and Jump.


  /////////////////////////////
 //MARIE Instructions Guide///
/////////////////////////////

Load (target_address)	- Load the contents of address X into AC

Store (target_address)	- Store the contents of AC at address X

Add (target_address)	- Add the contents of address X to AC and store 
			  the result in AC

Subt (target_address)	- Subtract the contents of address X from AC and 
			  store the result in AC

Input			- Input a value from the keyboard into AC (decimal)

Output			- Output the value in AC to the display

Halt			- Terminate the program

Skipcond y		- Skip the next instruction on condition (y). This 
			  can be 3 different hex numbers: 000, 400, or 800.
			  000 skips if AC < 0
			  400 skips if AC == 0
			  800 skips if AC > 0

Jump (target_address)	- Load the value of X into PC




