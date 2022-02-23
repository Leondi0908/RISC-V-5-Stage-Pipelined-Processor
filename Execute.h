#ifndef EXECUTE_H
#define EXECUTE_H

#include <iostream>
#include <bitset>

using namespace std;

class Execute
{
public:
	// Defining the Immgen Gate
	// It is an sign extention for the immediate binary form 12bits
	bitset<32> immGen(bitset<12> imm)
	{
		string extension;
		if (imm[11] == 0)
		{
			extension = "0000000000000000000" + imm.to_string();
		}
		else
		{
			extension = "1111111111111111111" + imm.to_string();
		}
		return(bitset<32>(extension));
	}

	// ALU_src is a MUX to determine whether we will use the Reg_data2 or Imm extention
	bitset<32> ALU_src(bool is_I, bool is_lw, bool is_sw, bitset<12> imm, bitset<32> reg_data2)
	{
		if (is_I || is_lw || is_sw)
		{

			return (immGen(imm));
		}
		else
		{
			return(reg_data2);
		}
	}

	// ALU Operation with the bunch of command
	bitset<32> get_ALU_rslt(bool is_sw, bool is_lw, string command, bitset<32> R_data1, bitset<32> R_data2)
	{
		if (is_sw || is_lw)
		{
			return (bitset<32>(R_data1.to_ullong() + R_data2.to_ullong()));
		}
		else if (command == "sub")
		{
			return (bitset<32>(R_data1.to_ullong() - R_data2.to_ullong()));
		}
		else if (command == "add")
		{
			return (bitset<32>(R_data1.to_ullong() + R_data2.to_ullong()));
		}
		else if (command == "or")
		{
			return (R_data1 | R_data2);
		}
		else if (command == "and")
		{
			return (R_data1 & R_data2);
		}
	}

};
#endif // EXECUTE_H