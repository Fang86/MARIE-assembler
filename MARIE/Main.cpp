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

string parse_sub(string sub, string &hd);				  // Returns binary as string, 4 bit instruction or n-bit value
void parse_bin(string addr, string inst, Processor proc); // Parses binary line - address, instruct, target address

int main()
{
	ifstream assemFile;
	fstream binFile;
	Processor proc;
	string curLine, curSub, addr, inst, hexdec = "", parsed = "";
	int count = 0, commentPos = string::npos;
	
	assemFile.open("assembly.txt");
	binFile.open("binary.txt", fstream::out | fstream::trunc); // Opens and clears binfile

	// Reads each line of assembly file, writes converted code to binary file
	if(assemFile.is_open()) 
	{
		while (getline(assemFile, curLine)) 
		{
			commentPos = curLine.find("/");
			if (commentPos != string::npos) // Check for and ignore comments
			{
				curLine.erase(commentPos, curLine.length() - commentPos);
				if (commentPos == 0) continue;
			}

			stringstream ss(curLine);
			while (getline(ss, curSub, ' '))
			{
				count++;
				parsed = parse_sub(curSub, hexdec);
				if (count == 1) addr = parsed; // set addr to memory address if first arg

				if (parsed == "HEX") hexdec = "HEX"; 
				else if (parsed == "DEC") hexdec = "DEC";
				else if (parsed.length() == 2) // Insert var into vars map using 7 bit binary conversion
					vars.insert(make_pair( pad(long_to_bin(long(parsed[0])), 7), addr));
				else if (parsed.length() == 1) // Insert binary conversion of label to binfile
					binFile << pad(long_to_bin(long(parsed[0])), 7) << " ";
				else binFile << parsed << " "; // Base case
			}
			binFile << endl;
			count = 0;
			addr = "";
		}
	}
	assemFile.close();
	binFile.close();
	binFile.open("binary.txt"); // Reopen binfile for reading, starting at beginning 

	map<string, string>::iterator vit = vars.begin(); // vars map iterator
	
	// Reads binary, separates address from instruction then inserts into map prog
	if (binFile.is_open())
	{
		while (getline(binFile, curLine)) 
		{
			addr = curLine.substr(0, 12);
			inst = curLine.substr(12, curLine.length());
			inst.erase(remove(inst.begin(), inst.end(), ' '), inst.end());
			if (inst.length() == 11) // If instruction is followed by label (char, e.g. x)
			{
				vit = vars.find(inst.substr(4, 7));
				inst = inst.substr(0, 4) + vit->second; // alter instruction: convert label binary to address
			}
			prog.insert(make_pair(addr, inst)); // Insert memory address and instruction into map
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

		parse_bin(it->first, it->second, proc); // Parse and execute the line
		it++;

		if (it == prog.end()) break; // failsafe
	}

	return 0;
}

string parse_sub(string sub, string &hd)
{
	if (hd == "HEX")
	{
		hd = "";
		return pad(hex_to_bin(sub), 16);
	}
	else if (hd == "DEC")
	{
		hd = "";
		return long_to_bin(stol(sub), 16); // decimal to binary string, padded to 16 bits
	}
		

	if (sub == "Load") return "0001";
	else if (sub == "Store") return "0010";
	else if (sub == "Add") return "0011";
	else if (sub == "Subt") return "0100";
	else if (sub == "Input") return "0101 000000000000"; // Ends in 0s for lack of target address
	else if (sub == "Output") return "0110 000000000000";
	else if (sub == "Halt") return "0111 000000000000";
	else if (sub == "Skipcond") return "1000";
	else if (sub == "Jump") return "1001";
	else if (sub == "DEC" or sub == "HEX") return sub;
	else if (sub.length() == 1 or sub.length() == 2) return sub; // If sub is label
	return hex_to_bin(sub); // Implies it's a number, whether it's hex or dec
}

void parse_bin(string addr, string inst, Processor proc)
{
	string opcode = inst.substr(0, 4);
	string opand = inst.substr(4, inst.length()); // Get target address (operand)

	if (opcode == "0001") proc.load(opand);
	else if (opcode == "0010") proc.store(opand);
	else if (opcode == "0011") proc.add(opand);
	else if (opcode == "0100") proc.subt(opand);
	else if (opcode == "0101") proc.input();
	else if (opcode == "0110") proc.output();
	else if (opcode == "0111") proc.halt();
	else if (opcode == "1000") proc.skipcond(opand);
	else if (opcode == "1001") proc.jump(opand);
}
