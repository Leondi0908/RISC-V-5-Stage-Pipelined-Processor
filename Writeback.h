#ifndef WRITEBACK_H
#define WRITEBACK_H

#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>


using namespace std;

class Writeback
{
public:
	Writeback()
	{
		reg.resize(32);
		//Preventing reading a random value of the register.
		reg[0] = bitset<32>(0);
	}

	~Writeback()
	{
		for (unsigned int i = 0; i < reg.size(); i++)
		{
			reg.pop_back();
		}
	}

	bitset<32> read_reg(bitset<5> address)
	{
		return(reg[address.to_ulong()]);
	}

	// Write the register 
	void write_reg(bitset<5> address, bitset<32> reg_data)
	{
		reg[address.to_ulong()] = reg_data;
	}

	// Getting the return and display it to the terminal
	void print_Reg()
	{
		cout << "(" << reg[10].to_ulong() << ", " << reg[11].to_ulong() << ")" << endl;

	}

private:
	vector<bitset<32>> reg;
};
#endif // WRITEBACK_H