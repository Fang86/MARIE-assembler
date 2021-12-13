#pragma once
#include <iostream>
#include <string>
#include <map>

using namespace std;

extern long AC, MBR, InREG, OutREG;
extern string MAR, PC, IR;
extern string hex_to_bin(string hexs);				// Returns a binary string from a hexadecimal string
extern string hex_to_bin_c(char hexc);				// Returns a 4 bit binary
extern string get_contents(string addr);			// Returns contents of memory at given address - always value, never inst.
extern void set_contents(string addr, long bin);	// Sets contents of memory at given address
extern long bin_to_long(string bin);				// Converts binary string to decimal long
extern string long_to_bin(long bin);				// Converts long to binary string
extern string bin_add(string bin, long x);			// Adds long x to binary string
extern string bin_subt(string bin, long x);			// Subtracts long x from binary string
extern string pad(string s, int len);						// Pads front of string with 0s - based on intended length
extern map<string, string> prog;					// Program map - memory address, instruction (contents)
extern map<string, string> vars;								// Variable map - variable char, memory address
extern bool active, debug;							// Program flags

struct Processor
{
	/* AC: The accumulator, which holds data values. This is a general-purpose
	 *		register, and it holds data that the CPU needs to process. Most computer
	 *		today have multiple general-purpose registers.
	 * MAR: The memory address register, which holds the memory address of the
	 *		data being referenced.
	 * MBR: The memory buffer register, which holds either the data just read from
	 *		memory or the data ready to be written to memory.
	 * PC: The program counter, which holds the address of the next instruction to
	 *		be executed in the program.
	 * IR: The instruction register, which holds the next instruction to be executed.
	 * InREG: The input register, which holds data from the input device.
	 * OutREG: The output register, which holds data for the output device.
	 */

	void init();

	void load(string x); // Load the contents of address X into AC
	void store(string x); // Store the contents of AC at address X
	void add(string x); // Add the contents of address X to AC and store the result in AC
	void subt(string x); // Subtract the contents of address X from AC and store the result in AC
	void input(); // Input a value from the keyboard into AC (binary only)
	void output(); // Output the value in AC to the display
	void halt(); // Terminate the program
	void skipcond(string x); // Skip the next instruction on condition
	void jump(string x); // Load the value of X into PC
};