#pragma once

#include <iostream>
#include <string>
#include <lucid/rm/Machine.h>

namespace lucid {
namespace rm {

	class Disassembler
	{
	public:
		typedef Machine::Instruction Instruction;
		typedef Machine::Program Program;

		Disassembler() = default;

		virtual ~Disassembler() = default;

		std::ostream &disassemble(std::ostream &stream, Program const &program);

	private:
		enum { OPCODE_COUNT = Machine::OPCODE_COUNT };

		static std::string const repr[0x20];

		typedef std::ostream &(Disassembler:: *operation_t)(std::ostream &stream, Instruction ins);
		static operation_t const operations[OPCODE_COUNT];

		std::ostream &_neg(std::ostream &stream, Instruction ins);

		std::ostream &_add(std::ostream &stream, Instruction ins);

		std::ostream &_sub(std::ostream &stream, Instruction ins);

		std::ostream &_mul(std::ostream &stream, Instruction ins);

		std::ostream &_inc(std::ostream &stream, Instruction ins);

		std::ostream &_dec(std::ostream &stream, Instruction ins);

		std::ostream &_not(std::ostream &stream, Instruction ins);

		std::ostream &_and(std::ostream &stream, Instruction ins);

		std::ostream &_or(std::ostream &stream, Instruction ins);

		std::ostream &_xor(std::ostream &stream, Instruction ins);

		std::ostream &_lsl(std::ostream &stream, Instruction ins);

		std::ostream &_lsr(std::ostream &stream, Instruction ins);

		std::ostream &_bra(std::ostream &stream, Instruction ins);

		std::ostream &_beq(std::ostream &stream, Instruction ins);

		std::ostream &_bne(std::ostream &stream, Instruction ins);

		std::ostream &_bse(std::ostream &stream, Instruction ins);

		std::ostream &_bsne(std::ostream &stream, Instruction ins);

		std::ostream &_mov(std::ostream &stream, Instruction ins);

		std::ostream &_negf(std::ostream &stream, Instruction ins);

		std::ostream &_addf(std::ostream &stream, Instruction ins);

		std::ostream &_subf(std::ostream &stream, Instruction ins);

		std::ostream &_mulf(std::ostream &stream, Instruction ins);

		std::ostream &_divf(std::ostream &stream, Instruction ins);

		std::ostream &_cgi(std::ostream &stream, Instruction ins);

		std::ostream &_cagn(std::ostream &stream, Instruction ins);

		std::ostream &_cage(std::ostream &stream, Instruction ins);

		std::ostream &_mhd(std::ostream &stream, Instruction ins);

		std::ostream &_rdgn(std::ostream &stream, Instruction ins);

		std::ostream &_rdge(std::ostream &stream, Instruction ins);

		std::ostream &_push(std::ostream &stream, Instruction ins);

		std::ostream &_pop(std::ostream &stream, Instruction ins);

		std::ostream &_nop(std::ostream &stream, Instruction ins);

	};

}	// rm
}	// lucid