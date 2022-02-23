#ifndef FETCHING_H
#define FETCHING_H

#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Fetching
{
public:
	Fetching(unsigned int memSize)
	{
		insMem.resize(memSize);
	}
	~Fetching()
	{
		for (unsigned int i = 0; i < insMem.size(); i++)
		{
			insMem.pop_back();
		}
	}
	void get_instr(string txt_File)
	{
		ifstream in;

		in.open(txt_File);
		if (in.is_open())
		{
			int inst;
			int i = 0;
			while (in >> inst)
			{
				insMem[i] = bitset<8>(inst);
				i++;
			}
		}
		else
		{
			cout << "File does not exist. \n";
		}
		in.close();

	}

	// Taking the little edian form instruction to binary form
	bitset<32> read_instr(unsigned int address)
	{
		string insmem;
		bitset<32> instruction;
		insmem.append(insMem[address + 3].to_string());
		insmem.append(insMem[address + 2].to_string());
		insmem.append(insMem[address + 1].to_string());
		insmem.append(insMem[address].to_string());
		instruction = bitset<32>(insmem);
		return (instruction);
	}

private:
	vector<bitset<8>> insMem;
};

#endif // FETCHING.H