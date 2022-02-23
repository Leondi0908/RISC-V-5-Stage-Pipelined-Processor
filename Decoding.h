#ifndef DECODING_H
#define DECODING_H

#include <iostream>
#include <bitset>
#include <string>


using namespace std;

class Decoding
{
public:
	bitset<5> get_Rs1(bitset<32> instruction)
	{
		return (bitset<5>(right_Shift(instruction, 15)));
	}

	bitset<5> get_Rs2(bitset<32> instruction)
	{
		return (bitset<5>(right_Shift(instruction, 20)));
	}

	bitset<5> get_Rd(bitset<32> instruction)
	{
		return (bitset<5>(right_Shift(instruction, 7)));
	}

	bitset<12> get_imm(bitset<32> instruction)
	{
		return(bitset<12>(right_Shift(instruction, 20)));
	}

	bitset<7> get_OPCODE(bitset<32> instruction)
	{
		return(bitset<7>(right_Shift(instruction, 0)));
	}

	bitset<7> get_FUNCT7(bitset<32> instruction)
	{
		return(bitset<7>(right_Shift(instruction, 25)));
	}

	bitset<3> get_FUNCT3(bitset<32> instruction)
	{
		return(bitset<3>(right_Shift(instruction, 12)));
	}

	unsigned long right_Shift(bitset<32> instruction, int shift)
	{
		return((instruction.to_ulong()) >> shift);
	}

	bool is_R_type(bitset<7> opcode)
	{
		if (opcode.to_ulong() == 51)
		{
			return(true);
		}
		else
			return false;
	}

	bool is_I_type(bitset<7> opcode)
	{
		if (opcode.to_ulong() == 19)
		{
			return(true);
		}
		else
			return false;
	}

	bool is_LW(bitset<7> opcode)
	{
		if (opcode.to_ulong() == 3)
		{
			return(true);
		}
		else
			return false;
	}

	bool is_SW(bitset<7> opcode)
	{
		if (opcode.to_ulong() == 35)
		{
			return(true);
		}
		else
			return false;
	}

	bool is_ADD(bitset<3> funct3)
	{
		if (funct3.to_ulong() == 0)
		{
			return(true);
		}
		else
			return false;
	}

	bool is_OR(bitset<3> funct3)
	{
		if (funct3.to_ulong() == 6)
		{
			return(true);
		}
		else
			return false;
	}

	bool is_AND(bitset<3> funct3)
	{
		if (funct3.to_ulong() == 7)
		{
			return(true);
		}
		else
			return false;
	}

	bool is_SUB(bitset<7> funct7)
	{
		if (funct7.to_ulong() == 32)
		{
			return(true);
		}
		else
			return false;
	}
};

#endif // DECODING_H