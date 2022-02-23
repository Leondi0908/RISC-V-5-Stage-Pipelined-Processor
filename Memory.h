#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <bitset>
#include <vector>

using namespace std;

class Memory
{
public:
	Memory()
	{
		data_Mem.resize(32);
	}

	Memory(unsigned int Memsize)
	{
		data_Mem.resize(Memsize);
	}

	~Memory()
	{
		for (unsigned int i = 0; i < data_Mem.size(); i++)
		{
			data_Mem.pop_back();
		}
	}
	// Write the data memory and with the specific address in the memory
	void Mem_wrt(bitset<32> address, bitset<32> data)
	{
		data_Mem[address.to_ulong()] = data;
	}
	// Reading the data memory with the given address
	bitset<32> Mem_read(bitset<32> address)
	{
		return (data_Mem[address.to_ulong()]);
	}
private:
	vector<bitset<32>> data_Mem;
};
#endif // MEM0RY_H