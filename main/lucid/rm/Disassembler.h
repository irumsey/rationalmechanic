#pragma once

#include <iostream>
#include <string>
#include <lucid/rm/Defines.h>
#include <lucid/rm/Machine.h>

LUCID_RM_BEGIN

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

	std::ostream &_nullary(std::ostream &stream, Instruction ins);

	std::ostream &_unary(std::ostream &stream, Instruction ins);

	std::ostream &_binary(std::ostream &stream, Instruction ins);

	std::ostream &_ternary(std::ostream &stream, Instruction ins);

	std::ostream &_mov(std::ostream &stream, Instruction ins);

	std::ostream &_bra(std::ostream &stream, Instruction ins);

	std::ostream &_brx(std::ostream &stream, Instruction ins);

	std::ostream &_bsx(std::ostream &stream, Instruction ins);

};

LUCID_RM_END
