#include "Disassembler.h"
#include <iomanip>
#include <string>

#define  _OPCODE ((0x0000001f & ins) >> 0)

#define   _FLAGS ((0x000000e0 & ins) >> 5)
#define   _FLAG0 ((0x01 & _FLAGS) != 0)
#define   _FLAG1 ((0x02 & _FLAGS) != 0)
#define   _FLAG2 ((0x04 & _FLAGS) != 0)

#define      _R0 Register(ins, _FLAG0, 0x0000ff00,  8)
#define      _R1 Register(ins, _FLAG1, 0x00ff0000, 16)
#define      _R2 Register(ins, _FLAG2, 0xff000000, 24)

#define     _IMM Immediate(ins) 

namespace /* anonymous */ {

}

namespace lucid {
namespace rm {

	//
	//
	//

	class Register
	{
	public:
		Register(Disassembler::Instruction ins, bool indirect, uint32_t mask, size_t shift)
			: index((mask & ins) >> shift)
			, indirect(indirect)
		{

		}

		std::ostream &repr(std::ostream &stream) const
		{
			if (indirect)
				stream << "[r" << std::setw(3) << std::setfill('0') << index << "]";
			else
				stream << "  r" << std::setw(3) << std::setfill('0') << index;
			return stream;
		}

	private:
		size_t index = 0;
		bool indirect = false;

	};

	inline std::ostream &operator<<(std::ostream &stream, Register const &reg)
	{
		return reg.repr(stream);
	}

	//
	//
	//

	class Immediate
	{
	public:
		Immediate(Disassembler::Instruction ins)
			: value((0xffff0000 &ins) >> 16)
		{
		}

		std::ostream &repr(std::ostream &stream) const
		{
			stream << std::hex;
			stream << "0x" << std::setw(4) << std::setfill('0') << value;
			stream << std::dec;

			return stream;
		}

	private:
		uint16_t value = 0;

	};

	inline std::ostream &operator<<(std::ostream &stream, Immediate const &imm)
	{
		return imm.repr(stream);
	}

	//
	//
	//

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

	std::ostream &Disassembler::_nullary(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE];
		return stream;
	}

	std::ostream &Disassembler::_unary(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << "\t" << _R0;
		return stream;
	}

	std::ostream &Disassembler::_binary(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << "\t" << _R0 << ", " << _R1;
		return stream;
	}

	std::ostream &Disassembler::_ternary(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << "\t" << _R0 << ", " << _R1 << ", " << _R2;
		return stream;
	}

	std::ostream &Disassembler::_mov(std::ostream &stream, Instruction ins)
	{
		if (_FLAG2)
			stream << repr[_OPCODE] << "\t" << _R0 << ", " << _IMM;
		else
			stream << repr[_OPCODE] << "\t" << _R0 << ", " << _R1;
		return stream;
	}

	std::ostream &Disassembler::_bra(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << "\t" << _IMM;
		return stream;
	}

	std::ostream &Disassembler::_brx(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << "\t" << _R0 << ", " << _IMM;
		return stream;
	}

	std::ostream &Disassembler::_bsx(std::ostream &stream, Instruction ins)
	{
		stream << repr[_OPCODE] << "\t" << _IMM;
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
		"BZ  ",
		"BNZ ",
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

	Disassembler::operation_t const Disassembler::operations[OPCODE_COUNT] =
	{
		&_binary,
		&_ternary,
		&_ternary,
		&_ternary,
		&_unary,
		&_unary,
		&_binary,
		&_ternary,
		&_ternary,
		&_ternary,
		&_ternary,
		&_ternary,
		&_bra,
		&_brx,
		&_brx,
		&_bsx,
		&_bsx,
		&_mov,
		&_binary,
		&_ternary,
		&_ternary,
		&_ternary,
		&_ternary,
		&_unary,
		&_binary,
		&_unary,
		&_nullary,
		&_ternary,
		&_unary,
		&_nullary,
		&_nullary,
		&_nullary,
	};

}	// rm
}	// lucid