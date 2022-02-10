#include "Disassembler.h"
#include <string>

#define  _OPCODE ((0x0000001f & ins) >> 0)

#define   _FLAGS ((0x000000e0 & ins) >> 5)
#define   _FLAG0 ((0x01 & _FLAGS) != 0)
#define   _FLAG1 ((0x02 & _FLAGS) != 0)
#define   _FLAG2 ((0x04 & _FLAGS) != 0)
#define      _R0 ((0x0000ff00 & ins) >>  8)
#define      _R1 ((0x00ff0000 & ins) >> 16)
#define      _R2 ((0xff000000 & ins) >> 24)

namespace /* anonymous */ {

	std::string const flags[8] = {
		"\t000",
		"\t001",
		"\t010",
		"\t011",
		"\t100",
		"\t101",
		"\t110",
		"\t111",
	};

}

namespace lucid {
namespace rm {

	std::ostream &Disassembler::disassemble(std::ostream &stream, Program const &program)
	{
		for (size_t i = 0; i < program.size(); ++i)
		{
			Instruction ins = program[i];
			(this->*operations[_OPCODE])(stream, ins);
			stream << std::endl;
		}
		return stream;
	}

	std::string const Disassembler::repr[OPCODE_COUNT] =
	{
		"NEG ",
		"ADD ",
		"SUB ",
		"MUL ",
		"INC ",
		"DEC ",
		"NOT ",
		"AND ",
		"OR  ",
		"XOR ",
		"LSL ",
		"LSR ",
		"BRA ",
		"BEQ ",
		"BNE ",
		"BSE ",
		"BSNE",
		"MOV ",
		"NEGF",
		"ADDF",
		"SUBF",
		"MULF",
		"DIVF",
		"CGI ",
		"CAGN",
		"CAGE",
		"MHD ",
		"RDGN",
		"RDGE",
		"PUSH",
		"POP ",
		"NOP ",
	};

	std::ostream &Disassembler::_neg(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0;
		return stream;
	}

	std::ostream &Disassembler::_add(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1 << ", " << _R2;
		return stream;
	}

	std::ostream &Disassembler::_sub(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1 << ", " << _R2;
		return stream;
	}

	std::ostream &Disassembler::_mul(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1 << ", " << _R2;
		return stream;
	}

	std::ostream &Disassembler::_inc(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0;
		return stream;
	}

	std::ostream &Disassembler::_dec(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0;
		return stream;
	}

	std::ostream &Disassembler::_not(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0;
		return stream;
	}

	std::ostream &Disassembler::_and(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1 << ", " << _R2;
		return stream;
	}

	std::ostream &Disassembler::_or(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1 << ", " << _R2;
		return stream;
	}

	std::ostream &Disassembler::_xor(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1 << ", " << _R2;
		return stream;
	}

	std::ostream &Disassembler::_lsl(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1;
		return stream;
	}

	std::ostream &Disassembler::_lsr(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1;
		return stream;
	}

	std::ostream &Disassembler::_bra(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0;
		return stream;
	}

	std::ostream &Disassembler::_beq(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1 << ", " << _R2;
		return stream;
	}

	std::ostream &Disassembler::_bne(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1 << ", " << _R2;
		return stream;
	}

	std::ostream &Disassembler::_bse(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1;
		return stream;
	}

	std::ostream &Disassembler::_bsne(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1;
		return stream;
	}

	std::ostream &Disassembler::_mov(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1;
		return stream;
	}

	std::ostream &Disassembler::_negf(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1;
		return stream;
	}

	std::ostream &Disassembler::_addf(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1 << ", " << _R2;
		return stream;
	}

	std::ostream &Disassembler::_subf(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1 << ", " << _R2;
		return stream;
	}

	std::ostream &Disassembler::_mulf(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1 << ", " << _R2;
		return stream;
	}

	std::ostream &Disassembler::_divf(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1 << ", " << _R2;
		return stream;
	}

	std::ostream &Disassembler::_cgi(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1;
		return stream;
	}

	std::ostream &Disassembler::_cagn(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1 << ", " << _R2;
		return stream;
	}

	std::ostream &Disassembler::_cage(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1 << ", " << _R2;
		return stream;
	}

	std::ostream &Disassembler::_mhd(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0;
		return stream;
	}

	std::ostream &Disassembler::_rdgn(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1 << ", " << _R2;
		return stream;
	}

	std::ostream &Disassembler::_rdge(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS] << "\t" << _R0 << ", " << _R1 << ", " << _R2;
		return stream;
	}

	std::ostream &Disassembler::_push(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS];
		return stream;
	}

	std::ostream &Disassembler::_pop(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << flags[_FLAGS];
		return stream;
	}

	std::ostream &Disassembler::_nop(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE];
		return stream;
	}

	Disassembler::operation_t const Disassembler::operations[OPCODE_COUNT] =
	{
		&_neg,
		&_add,
		&_sub,
		&_mul,
		&_inc,
		&_dec,
		&_not,
		&_and,
		&_or,
		&_xor,
		&_lsl,
		&_lsr,
		&_bra,
		&_beq,
		&_bne,
		&_bse,
		&_bsne,
		&_mov,
		&_negf,
		&_addf,
		&_subf,
		&_mulf,
		&_divf,
		&_cgi,
		&_cagn,
		&_cage,
		&_mhd,
		&_rdgn,
		&_rdge,
		&_push,
		&_pop,
		&_nop,
	};

}	// rm
}	// lucid