/* Assignment: Create a program that converts MARIE assembler 
 * code to binary then executes the binary code
 * 
 * List of MARIE instructions:
 * Load		- Load the contents of address X into AC
 * Store	- Store the contents of AC at address X
 * Add		- Add the contents of address X to AC and store 
 *			  the result in AC
 * Subt		- Subtract the contents of address X from AC and 
 *			  store the result in AC
 * Input	- Input a value from the keyboard into AC
 * Output	- Output the value in AC to the display
 * Halt		- Terminate the program
 * Skipcond	- Skip the next instruction on condition
 * Jump		- Load the value of X into PC
 */

#include <fstream>
#include <sstream>
#include "Asm.h"

string parse_sub(string sub);				// Returns binary as string, 4 bit instruction or n-bit value
void parse_bin(string addr, string inst, Assembler assem); // Parses binary line - address, instruct, target address

int main()
{
	ifstream assemFile;
	fstream binFile;
	string curLine, curSub, addr, inst;
	Assembler assem;
	
	assemFile.open("assembly.txt");
	binFile.open("binary.txt");

	// Reads each line of assembly file, writes converted code to binary file
	if(assemFile.is_open()) 
	{
		while (getline(assemFile, curLine)) 
		{
			stringstream ss(curLine);
			while(getline(ss, curSub, ' '))
			{
				binFile << parse_sub(curSub) << " ";
			}
			binFile << endl;
		}
	}

	assemFile.close();
	binFile.close();
	binFile.open("binary.txt"); // Reopen binfile for reading at beginning 
	
	// Separates address from instruction - inserts into map prog
	if (binFile.is_open())
	{
		while (getline(binFile, curLine)) 
		{
			addr = curLine.substr(0, 12);
			inst = curLine.substr(12, curLine.length());
			inst.erase(remove(inst.begin(), inst.end(), ' '), inst.end());
			prog.insert(make_pair(addr, inst));
		}
	}
	
	binFile.close();

	map<string, string>::iterator it = prog.begin();	// Program iterator
	PC = bin_subt(it->first, 1);						// Initialize PC register

	// Program loop
	while (active)
	{
		if (PC == bin_subt(it->first, 1)) // Normal flow
			PC = it->first;
		else if (PC == it->first) // Skipcond
		{
			it++;
			PC = it->first;
		}
		else // Jump
			it = prog.find(PC);

		parse_bin(it->first, it->second, assem); // Parse and execute the line
		it++;

		if (it == prog.end()) break;
	}

	return 0;
}

string parse_sub(string sub)
{
	if (sub == "Load") return "0001";
	else if (sub == "Store") return "0010";
	else if (sub == "Add") return "0011";
	else if (sub == "Subt") return "0100";
	else if (sub == "Input") return "0101 000000000000"; // Ends in 0s for lack of target address
	else if (sub == "Output") return "0110 000000000000";
	else if (sub == "Halt") return "0111 000000000000";
	else if (sub == "Skipcond") return "1000";
	else if (sub == "Jump") return "1001";
	return hex_to_bin(sub); // Implies it's a number, whether it's hex or dec
}

void parse_bin(string addr, string inst, Assembler assem)
{
	string opcode = inst.substr(0, 4);
	string opand = inst.substr(4, inst.length()); // Get target address (operand)

	if (opcode == "0001") assem.load(opand);
	else if (opcode == "0010") assem.store(opand);
	else if (opcode == "0011") assem.add(opand);
	else if (opcode == "0100") assem.subt(opand);
	else if (opcode == "0101") assem.input();
	else if (opcode == "0110") assem.output();
	else if (opcode == "0111") assem.halt();
	else if (opcode == "1000") assem.skipcond(opand);
	else if (opcode == "1001") assem.jump(opand);
}
