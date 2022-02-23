/*
Add all the required standard and developed libraries here.
Remember to include all those files when you are submitting the project.
*/
#include "Fetching.h"
#include "Decoding.h"
#include "Execute.h"
#include "Memory.h"
#include "Writeback.h"

using namespace std;

/*
Put/Define any helper function/definitions you need here
*/
const unsigned int MEMSIZE = 4096;

// Memory struct
struct Mem
{
	bitset<32> wrt_data;
	bitset<5> rd;
	bool lw_type;
	bool sw_type;
};

// Writeback struct
struct Wb
{
	bitset<5> rd;
	bool sw_type;
};

// State struct where it include some object that important to be passed to the next state
struct state
{
	string opcode_type;
	bitset<32> instruction;
	bitset<5> rs1;
	bitset<5> rs2;
	bitset<5> rd;
	bitset<7> opcode;
	bitset<12> imm;
	bitset<32> R_data1;
	bitset<32> R_data2;
	bitset<32> data;
	bitset<32> ALU_rslt;
	bool r_type;
	bool i_type;
	bool lw_type;
	bool sw_type;
	bool IF;
	bool ID;
	bool EX;
	bool MEM;
	bool WB;
	Mem mem;
	Wb wb;
};

int main (int argc, char* argv[]) // your project should be executed like this: ./cpusim filename.txt and should print (a0,a1) 
{
	/* This is the front end of your project.
	You need to first read the instructions that are stored in a file and load them into an instruction memory.
	*/

	// Reading the InsMem
	Fetching fetch(MEMSIZE);
	fetch.get_instr(argv[0]);
	/* Define your Instruction memory here. Each cell should store 1 byte. You can define the memory either dynamically, or define it as a fixed size with size 4MB (i.e., 4096 lines). Each instruction is 32 bits (i.e., 4 lines, saved in little-endian mode).

	Each line in the input file is stored as an unsigned char and is 1 byte (each four lines are one instruction). You need to read the file line by line and store it into the memory. You may need a mechanism to convert these values to bits so that you can read opcodes, operands, etc.
	*/

	/* OPTIONAL: Instantiate your Instruction object here. */
	
	// Defining the struct
	state cur_state, next_state;

	// Initialize all the object to NULL
	// To prevent the pipeline stages pick the wrong instruction
	int R_type_counter = 0;
	int ins_counter = 0;
	int Zero_opcode = 0;
	cur_state.opcode_type = "";
	cur_state.instruction = bitset<32>(0);
	cur_state.rs1 = bitset<5>(0);
	cur_state.rs2 = bitset<5>(0);
	cur_state.rd = bitset<5>(0);
	cur_state.opcode = bitset<7>(0);
	cur_state.imm = bitset<12>(0);
	cur_state.R_data1 = bitset<32>(0);
	cur_state.R_data2 = bitset<32>(0);
	cur_state.data = bitset<32>(0);
	cur_state.ALU_rslt = bitset<32>(0);
	cur_state.i_type = false;
	cur_state.r_type = false;
	cur_state.lw_type = false;
	cur_state.sw_type = false;
	cur_state.IF = true;
	cur_state.ID = false;
	cur_state.EX = false;
	cur_state.MEM = false;
	cur_state.WB = false;
	cur_state.mem.lw_type = false;
	cur_state.mem.sw_type = false;
	cur_state.wb.sw_type = false;
	cur_state.mem.wrt_data = bitset<32>(0);
	cur_state.mem.rd = bitset<5>(0);
	cur_state.wb.rd = bitset<5>(0);

	/* OPTIONAL: Instantiate your CPU object here. */
	Decoding decod;
	Memory memory(MEMSIZE);
	Writeback wrt_back;

	// Clock and PC
	unsigned int myClock = 0; // data-types can be changed! This is just a suggestion. 
	unsigned int myPC = 0;

	// Set the same intiliazation as the current state
	next_state = cur_state;
	while (1) // processor's main loop. Each iteration is equal to one clock cycle.  
	{

		//fetching
		if (cur_state.IF == true)
		{
			// Set to the next intruction
			next_state.instruction = fetch.read_instr(myPC);

			// Checking the Opcode of the instruction
			// It will proccess the condition if the opcode is zero
			// The zero opcode will be use to break the cycle
			if (decod.get_OPCODE(next_state.instruction).to_ulong() == 0)
			{
				Zero_opcode++;
			}
			else
				Zero_opcode = 0;
			// Intruction counter to calculate CPI and IPC 
			ins_counter++;

			next_state.ID = true;
		}

		// decoding
		if (cur_state.ID == true)
		{
			bitset<7> funct7 = decod.get_FUNCT7(cur_state.instruction);
			bitset<3> funct3 = decod.get_FUNCT3(cur_state.instruction);
			bitset<7> opcd = decod.get_OPCODE(cur_state.instruction);
			bitset<5> tempRd = decod.get_Rd(cur_state.instruction);
			next_state.rs1 = decod.get_Rs1(cur_state.instruction);
			next_state.rs2 = decod.get_Rs2(cur_state.instruction);
			next_state.rd = tempRd;

			next_state.imm = decod.get_imm(cur_state.instruction);

			next_state.opcode = opcd;
			next_state.r_type = decod.is_R_type(opcd);
			next_state.i_type = decod.is_I_type(opcd);
			next_state.lw_type = decod.is_LW(opcd);
			next_state.sw_type = decod.is_SW(opcd);

			// Getting the immediate[11:0] for Store Word
			if (decod.is_SW(opcd))
			{
				string extend;
				bitset<12> temp_imm;
				extend.append(funct7.to_string());
				extend.append(tempRd.to_string());
				next_state.imm = bitset<12>(extend);
			}

			// Determine the operand for the ALU
			if (decod.is_SUB(funct7))
			{
				next_state.opcode_type = "sub";
			}
			else if (decod.is_ADD(funct3))
			{
				next_state.opcode_type = "add";
			}
			else if (decod.is_AND(funct3))
			{
				next_state.opcode_type = "and";
			}
			else if (decod.is_OR(funct3))
			{
				next_state.opcode_type = "or";
			}

			next_state.EX = true;
		}

		// execute
		if (cur_state.EX == true)
		{
			// We use the write back class because it connects with the register file
			Execute exe;
			bitset<32> tempR_data2 = bitset<32>(0);

			next_state.R_data1 = wrt_back.read_reg(cur_state.rs1);
			tempR_data2 = wrt_back.read_reg(cur_state.rs2);

			// We save the R_data2 for the SW case
			// Since the R_data2 will be stored to the memory if it is SW - type
			next_state.mem.wrt_data = tempR_data2;

			// This is R-Type instruction counter
			if (cur_state.r_type == true)
			{
				R_type_counter += 1;
			}


			// Operating the ALUsrc
			// The return will be either imm extention or intial Reg_data2
			next_state.R_data2 = exe.ALU_src(cur_state.i_type, cur_state.lw_type, cur_state.sw_type, cur_state.imm, tempR_data2);
			
			// With given operand type, reg_data1, reg_data2, and is_lw or is_sw
			// lw and sw is also going to ALU and operating addition operand. 
			next_state.ALU_rslt = exe.get_ALU_rslt(cur_state.sw_type, cur_state.lw_type, cur_state.opcode_type, next_state.R_data1, next_state.R_data2);

			next_state.mem.rd = cur_state.rd;
			next_state.mem.lw_type = cur_state.lw_type;
			next_state.mem.sw_type = cur_state.sw_type;
			next_state.MEM = true;
		}

		// memory
		if (cur_state.MEM == true)
		{
			// If it is SW-type, it will write the memory data as Reg_data2
			// Then, ALU result will be the address in memory.
			if (cur_state.mem.sw_type == true)
			{
				memory.Mem_wrt(cur_state.ALU_rslt, cur_state.mem.wrt_data);
			}
			// If it is LW-type, it will read the data memory with the given address
			else if (cur_state.mem.lw_type == true)
			{
				next_state.data = memory.Mem_read(cur_state.ALU_rslt);
			}
			// This is the case of R-Type and I-type
			else
			{
				next_state.data = cur_state.ALU_rslt;
			}

			next_state.wb.rd = cur_state.mem.rd;
			next_state.wb.sw_type = cur_state.mem.sw_type;
			next_state.WB = true;
		}

		// writeback
		if (cur_state.WB == true)
		{
			// Since only SW-type cannot write to the register based on MemtoReg gate.
			// We set other than SW-type, it can write to the register
			if (cur_state.wb.sw_type == false)
			{
				wrt_back.write_reg(cur_state.wb.rd, cur_state.data);
			}
		}

		// _next values should be written to _current values here:
		cur_state = next_state;
		
		// Cycles
		myClock += 1;
		// Assuming No branch 
		myPC += 4; // for now we can assume that next PC is always PC + 4

		// we should break the loop if ALL instructions in the pipelinehas opcode==0 instruction 
		// IF we get 5 consecutive instruction with 0 opcode, it will end the pipelining
		if (Zero_opcode == 5)
			break;
	}
	// clean up the memory (if any)
	wrt_back.print_Reg();
	cout << "This is R type counter: " << R_type_counter << endl;
	cout << "This is Instruction counter: " << ins_counter << endl;
	cout << "This is the cycle: " << myClock << endl;
	cout << "This is the IPC (Instruction Per Clock cycle): " << ins_counter / myClock << endl;
	cout << "This is the CPI (Clock cycle Per Instruction): " << myClock / ins_counter << endl;

	// print the stats

	return 0;
}
