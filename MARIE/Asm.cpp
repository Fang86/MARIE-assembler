#include "Asm.h"

long AC = 0, MBR = 0, InREG = 0, OutREG = 0;
string MAR = "0", PC = "0", IR = "0";

void Assembler::init()
{
	MAR = PC;
}

void Assembler::load(string x) // Load the contents of address X into AC
{
	MAR = x;
	MBR = bin_to_long(get_contents(x));
	if (debug)
		cout << "Load: " << MBR << endl;
	AC = MBR;
}

void Assembler::store(string x) // Store the contents of AC at address X
{
	if (debug)
		cout << "Store " << AC << " at " << x << endl;
	MAR = x;
	MBR = AC;
	set_contents(x, MBR);
}

void Assembler::add(string x) // Add the contents of address X to AC and store the result in AC
{

	MAR = x;
	MBR = bin_to_long(get_contents(x));
	if (debug)
		cout << "Add: " << MBR << endl;
	AC += MBR;
}

void Assembler::subt(string x) // Subtract the contents of address X from AC and store the result in AC
{

	MAR = x;
	MBR = bin_to_long(get_contents(x));
	if (debug)
		cout << "Subt: " << MBR << endl;
	AC -= MBR;
}

void Assembler::input() // Input a value from the keyboard into AC (binary only)
{
	cout << "Input: ";
	cin >> InREG;
	AC = InREG;
}

void Assembler::output() // Output the value in AC to the display
{
	if (debug)
		cout << "Output: ";
	OutREG = AC;
	cout << OutREG << endl;
}

void Assembler::halt() // Terminate the program
{
	if (debug)
		cout << "Halt" << endl;
	active = false;
}

void Assembler::skipcond(string x) // Skip the next instruction on condition
{
	if (debug)
		cout << "Skipcond: " << x[0] << x[1] << endl;

	if (x[0] == '0' and x[1] == '0') // 000
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

void Assembler::jump(string x) // Load the value of X into PC
{
	if (debug)
		cout << "Jump to " << x << endl;
	PC = x;
}