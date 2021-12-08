/*
 * Assignment: Create a program that converts MARIE assembler 
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
 *
 * 
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

using namespace std;

string hex_to_bin(string hexs); // Returns a binary string from a hexadecimal string
string hex_to_bin_c(char hexc); // Returns a 4 bit binary
string parse_sub(string sub); // Returns binary as string, 4 bit instruction or n-bit value
string get_contents(string addr); // Gets contents of memory at given address - always value, never inst.
void set_contents(string addr, long bin); // Sets contents of memory at given address
long bin_to_long(string bin); // Converts binary string to decimal long
string long_to_bin(long bin); // Converts long to binary string
string bin_add(string bin, long x); // Adds long x to binary string
string bin_subt(string bin, long x); // Subtracts long x from binary string

map<string, string> prog; // memory address, instruction (contents)

long ival = 0; // initial value of registers

// Registers - definitions in struct Assembler
long AC = ival, MBR = ival, InREG = ival, OutREG = ival;
string MAR = "0", PC = "0", IR = "0";
bool active = true;

struct Assembler 
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

	Assembler()
	{
		//cout << "constructed" << endl;
		/*
		AC = MBR = InREG = OutREG = 11;
		MAR = "0";
		PC = "0";
		IR = "0";*/
	}

	void init()
	{
		MAR = PC;
	}

	void load(string x) // Load the contents of address X into AC
	{
		MAR = x;
		MBR = bin_to_long(get_contents(x));
		cout << "Load: " << MBR << endl;
		AC = MBR;
	}

	void store(string x) // Store the contents of AC at address X
	{
		cout << "Store " << AC << " at " << x << endl;
		MAR = x;
		MBR = AC;
		//cout << "MBR: " << MBR << endl;
		set_contents(x, MBR);
	}

	void add(string x) // Add the contents of address X to AC and store the result in AC
	{
		
		MAR = x;
		MBR = bin_to_long(get_contents(x));
		cout << "Add: " << MBR << endl;
		AC += MBR;
	}

	void subt(string x) // Subtract the contents of address X from AC and store the result in AC
	{
		
		MAR = x;
		MBR = bin_to_long(get_contents(x));
		cout << "Subt: " << MBR << endl;
		AC -= MBR;
	}

	void input() // Input a value from the keyboard into AC (binary only)
	{
		cout << "Input: ";
		cin >> InREG;
		AC = InREG;
	}

	void output() // Output the value in AC to the display
	{
		cout << "Output: ";
		OutREG = AC;
		cout << OutREG << endl;
	}

	void halt() // Terminate the program
	{
		cout << "Halt" << endl;
		active = false;
	}

	void skipcond(string x) // Skip the next instruction on condition
	{
		cout << "Skipcond: " << x[0] << x[1] << endl;
		if (x[0] == '0' and x[1] == '0') // 0
		{
			if (AC < 0) PC = bin_add(PC, 1);
		}
		else if (x[0] == '0' and x[1] == '1') // 400
		{
			if (AC == 0) bin_add(PC, 1);
		}
		else if (x[0] == '1' and x[1] == '0') // 800
		{
			if (AC > 0) bin_add(PC, 1);
		}
	}

	void jump(string x) // Load the value of X into PC
	{
		cout << "Jump to " << x << endl;
		PC = x;
	}
};

void parse_bin(string addr, string inst, Assembler assem); // Parses binary line - address, instruct, target address

int main()
{
	ifstream assemFile;
	fstream binFile;
	string curLine, curSub, addr, inst;
	
	Assembler assem;
	
	assemFile.open("assembly.txt");
	binFile.open("binary.txt");

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

	binFile.open("binary.txt");
	
	// Use a map to map each address to each instruction
	// The map will be used to execute the instructions
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

	// Iterate through program
	map<string, string>::iterator it = prog.begin();
	PC = bin_subt(it->first, 1); // initialize PC
	//PC = it->first;

	while (active) // Program loop
	{
		//cout << "PC, it: " << PC << ", " << it->first << endl;
		// if pc = it->first - 1 // normal flow
		if (PC == bin_subt(it->first, 1)) // Normal flow
		{
			//cout << "hit" << endl;
			PC = it->first;
		}
		else if (PC == it->first) // Skipcond
		{
			it++;
			PC = it->first;
			//continue;
		}
		else // Jump
		{
			it = prog.find(PC);
		}

		// else prog.find(PC)
		
		//bin_subt(it->first, 1);
		//PC = it->first;
		//cout << "Pc: " << PC << endl;
		//cout << "it->first: " << it->first << endl;
		parse_bin(it->first, it->second, assem); // Parse and execute the line
		it++;

		//if (it == prog.end()) break;
	}

	/*
	for (it = prog.begin(); it != prog.end(); it++)
	{
		//if pc = it->first - 1 // normal flow
		// if pc = it->first // skipcond
		// else prog.find(PC)
		PC = it->first;
		cout << "Pc: " << PC << endl;
		parse_bin(it->first, it->second, assem);
	}
	*/
	return 0;
}

// Utility functions

string hex_to_bin(string hexs)
{
	string bin;
	
	for (unsigned int i = 0; i < hexs.length(); i++)
	{
		bin += hex_to_bin_c(char(hexs[i]));
	}

	return bin;
}

string hex_to_bin_c(char hexc)
{
	//cout << "|hexc = " << hexc << "|";
	switch(hexc)
	{
		case '0': return "0000";
		case '1': return "0001";
		case '2': return "0010";
		case '3': return "0011";
		case '4': return "0100";
		case '5': return "0101";
		case '6': return "0110";
		case '7': return "0111";
		case '8': return "1000";
		case '9': return "1001";
		case 'A': return "1010";
		case 'B': return "1011";
		case 'C': return "1100";
		case 'D': return "1101";
		case 'E': return "1110";
		case 'F': return "1111";
		default:
			cout << "There was an ERROR, hexc = " << hexc << endl;
			return "ERROR";
	}
	
}

string parse_sub(string sub)
{
	if (sub == "Load") return "0001";
	else if (sub == "Store") return "0010";
	else if (sub == "Add") return "0011";
	else if (sub == "Subt") return "0100";
	else if (sub == "Input") return "0101 000000000000";
	else if (sub == "Output") return "0110 000000000000";
	else if (sub == "Halt") return "0111 000000000000"; // Halt inst. ends in 0s
	else if (sub == "Skipcond") return "1000";
	else if (sub == "Jump") return "1001";
	return hex_to_bin(sub); // Implies it's a number, whether it's hex or dec
}

void parse_bin(string addr, string inst, Assembler assem)
{
	// Get target address (if applicable)
	string opcode = inst.substr(0, 4);
	string opand = inst.substr(4, inst.length());
	
	if (opcode == "0001") assem.load(opand);
	else if(opcode == "0010") assem.store(opand);
	else if (opcode == "0011") assem.add(opand);
	else if (opcode == "0100") assem.subt(opand);
	else if (opcode == "0101") assem.input();
	else if (opcode == "0110") assem.output();
	else if (opcode == "0111") assem.halt();
	else if (opcode == "1000") assem.skipcond(opand);
	else if (opcode == "1001") assem.jump(opand);
}

string get_contents(string addr)
{
	return prog.find(addr)->second;
}

void set_contents(string addr, long bin)
{
	//cout << "Before: " << prog.find(addr)->second << endl;
	string s = long_to_bin(bin);
	for (int i = s.length(); i < 12; i++)
	{
		s.insert(0, "0");
	}
	prog.find(addr)->second = s; // contents of address = bin
	//cout << "After: " << prog.find(addr)->second << endl;
}

long bin_to_long(string bin)
{
	//cout << "String "  << bin << " => " << stoul(bin, nullptr, 2) << endl;
	return stol(bin, nullptr, 2);
}

string long_to_bin(long bin)
{
	string r;
	while (bin != 0) { r = (bin % 2 == 0 ? "0" : "1") + r; bin /= 2; }
	return r;

}

string bin_add(string bin, long x)
{
	string s = long_to_bin(bin_to_long(bin) + x);;

	//cout << "S = " << s << endl;
	for (int i = s.length(); i < 12; i++)
	{
		s.insert(0, "0");
	}
	
	//cout << "s = " << s << endl;
	return s;
}

string bin_subt(string bin, long x)
{
	string s = long_to_bin(bin_to_long(bin) - x);;

	//cout << "S = " << s << endl;
	for (int i = s.length(); i < 12; i++)
	{
		s.insert(0, "0");
	}

	//cout << "s = " << s << endl;

	return s;
}