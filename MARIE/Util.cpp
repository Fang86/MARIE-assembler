#pragma once
#include <iostream>
#include <string>
#include <map>
#include "Asm.h"

using namespace std;

map<string, string> prog;			// Program map - memory address, instruction (contents)
bool active = true, debug = true;	// Program flags

/*
string hex_to_bin(string hexs);				// Returns a binary string from a hexadecimal string
string hex_to_bin_c(char hexc);				// Returns a 4 bit binary
string get_contents(string addr);			// Returns contents of memory at given address - always value, never inst.
void set_contents(string addr, long bin);	// Sets contents of memory at given address
long bin_to_long(string bin);				// Converts binary string to decimal long
string long_to_bin(long bin);				// Converts long to binary string
string bin_add(string bin, long x);			// Adds long x to binary string
string bin_subt(string bin, long x);		// Subtracts long x from binary string
*/
string hex_to_bin_c(char hexc);				// Returns a 4 bit binary
string long_to_bin(long bin);				// Converts long to binary string
string pad(string s, int len);				// Pads front of string with 0s - based on intended length

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
	switch (hexc)
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

string get_contents(string addr)
{
	return prog.find(addr)->second;
}

void set_contents(string addr, long bin)
{
	string s = long_to_bin(bin);
	s = pad(s, 12);
	prog.find(addr)->second = s; // contents of address = bin
}

long bin_to_long(string bin)
{
	return stol(bin, nullptr, 2); // String -> long, converted to base 10 from base 2
}

string long_to_bin(long bin)
{
	string r;
	long bin_copy = bin;
	while (bin != 0) 
	{
		r = (bin % 2 == 0 ? "0" : "1") + r; 
		bin /= 2;
	} // Long -> string, converted to base 2 from base 10
	return r;
}

string bin_add(string bin, long x)
{
	string s = long_to_bin(bin_to_long(bin) + x);;

	
	return pad(s,12);
}

string bin_subt(string bin, long x)
{
	string s = long_to_bin(bin_to_long(bin) - x);;
	
	return pad(s,12);
}

string pad(string s, int len) // Pads front of string with 0s - based on intended length
{
	for (int i = s.length(); i < len; i++)
	{
		if (s[0] != '-')
			s.insert(0, "0");
		else
			s.insert(1, "0");
	}
	return s;
}