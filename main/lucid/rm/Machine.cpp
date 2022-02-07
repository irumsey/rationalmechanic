#include "Machine.h"

#define  _OPCODE ((0x0000001f & ins) >> 0)

#define   _FLAGS ((0x000000e0 & ins) >> 5)
#define   _FLAG0 ((0x01 & _FLAGS) != 0)
#define   _FLAG1 ((0x02 & _FLAGS) != 0)
#define   _FLAG2 ((0x04 & _FLAGS) != 0)

#define      _R0 ((0x0000ff00 & ins) >>  8)
#define      _R1 ((0x00ff0000 & ins) >> 16)
#define      _R2 ((0xff000000 & ins) >> 24)
#define       _0 (_FLAG0 ? memory[registers[_R0]] : registers[_R0])
#define       _1 (_FLAG1 ? memory[registers[_R1]] : registers[_R1])
#define       _2 (_FLAG2 ? memory[registers[_R2]] : registers[_R2])
#define     _IMM ((0xffff0000 & ins) >> 16)

#define     _FP0 (*((float32_t *)(_FLAG0 ? memory + (0xfffe & registers[_R0]) : registers + (0xfe & _R0))))
#define     _FP1 (*((float32_t *)(_FLAG1 ? memory + (0xfffe & registers[_R1]) : registers + (0xfe & _R1))))
#define     _FP2 (*((float32_t *)(_FLAG2 ? memory + (0xfffe & registers[_R2]) : registers + (0xfe & _R2))))   

#define   _FIRST (0)
#define    _LAST (_graph.nodeCount() - 1)
#define    _HEAD (head[0x01 & _FLAGS])

namespace /* anonymous */
{

}

namespace lucid {
namespace rm {

	Machine::Machine()
	{
		registers = new uint16_t [REGISTER_COUNT];
		memory = new uint16_t [MEMORY_SIZE];
	}

	Machine::~Machine()
	{
		delete[] registers;
		delete[] memory;
	}

	void Machine::execute(Program const &program, size_t limit)
	{
		reset();

		size_t len = program.size();
		size_t lcv = 0;

		pc = 0;
		while ((pc < len) && (lcv < limit))
		{
			Instruction ins = program[pc++];
			(this->*operations[_OPCODE])(ins);
			++lcv;
		}
	}

	void Machine::reset()
	{
		_graph.reset();

		stack.clear();

		::memset(registers, 0, REGISTER_COUNT * sizeof(uint16_t));
		::memset(memory, 0, MEMORY_SIZE * sizeof(uint16_t));

		// start with one node for robust-ness
		head[0] = head[1] = _graph.addNode(Node(0.f, 0.f));
	}

	void Machine::_neg(Instruction ins)
	{
		// explicit 2's complement
		_0 = ~(_1) + 1;
	}

	void Machine::_add(Instruction ins)
	{
		_0 = _1 + _2;
	}

	void Machine::_sub(Instruction ins)
	{
		_0 = _1 - _2;
	}

	void Machine::_mul(Instruction ins)
	{
		_0 = _1 * _2;
	}

	void Machine::_inc(Instruction ins)
	{
		_0 = _0 + 1;
	}

	void Machine::_dec(Instruction ins)
	{
		_0 = _0 - 1;
	}

	void Machine::_not(Instruction ins)
	{
		_0 = ~_0;
	}

	void Machine::_and(Instruction ins)
	{
		_0 = _1 & _2;
	}

	void Machine::_or(Instruction ins)
	{
		_0 = _1 | _2;
	}

	void Machine::_xor(Instruction ins)
	{
		_0 = _1 ^ _2;
	}

	void Machine::_lsl(Instruction ins)
	{
		_0 = _1 << _2;
	}

	void Machine::_lsr(Instruction ins)
	{
		_0 = _1 >> _2;
	}

	void Machine::_bra(Instruction ins)
	{
		pc = pc + _0;
	}

	void Machine::_beq(Instruction ins)
	{
		pc = (_0 == _1) ? pc + _2 : pc;
	}

	void Machine::_bne(Instruction ins)
	{
		pc = (_0 != _1) ? pc + _2 : pc;
	}

	void Machine::_bse(Instruction ins)
	{
		pc = (stack.empty()) ? pc + _0 : pc;
	}

	void Machine::_bsne(Instruction ins)
	{
		pc = (!stack.empty()) ? pc + _0 : pc;
	}

	void Machine::_mov(Instruction ins)
	{
		_0 = (_FLAG2) ? _IMM : _1;
	}

	void Machine::_negf(Instruction ins)
	{
		_FP0 = -_FP1;
	}

	void Machine::_addf(Instruction ins)
	{
		_FP0 = _FP1 + _FP2;
	}

	void Machine::_subf(Instruction ins)
	{
		_FP0 = _FP1 - _FP2;
	}

	void Machine::_mulf(Instruction ins)
	{
		_FP0 = _FP1 * _FP2;
	}

	void Machine::_divf(Instruction ins)
	{
		_FP0 = _FP1 / _FP2;
	}

	void Machine::_cgi(Instruction ins)
	{
		_graph.addInput(Input(_HEAD, _FP0));
	}

	void Machine::_cagn(Instruction ins)
	{
		if (_FLAG1)
			_HEAD = _graph.addNode((Node(_FP0, _FP1)));
		else
			_graph.getNode(_HEAD) = Node(_FP0, _FP1);
	}

	void Machine::_cage(Instruction ins)
	{
		_graph.adjustEdge(head[0], head[1], _FP0);
	}

	void Machine::_mhd(Instruction ins)
	{
		if (_FLAG0)
			_HEAD = (_HEAD == _FIRST) ? _LAST : --_HEAD;
		else
			_HEAD = (_HEAD == _LAST) ? _FIRST : ++_HEAD;
	}

	void Machine::_rdgn(Instruction ins)
	{
		Node const &node = _graph.getNode(_HEAD);

		_FP0 = node.bias;
		_FP1 = node.threshold;
		_FP2 = node.output;
	}

	void Machine::_rdge(Instruction ins)
	{
		_FP0 = _graph.edgeWeight(head[0], head[1]);
	}

	void Machine::_push(Instruction ins)
	{
		if (_FLAG0) stack.push_front(_HEAD);
		if (_FLAG1) _graph.pushUpstream(stack, _HEAD);
		if (_FLAG2) _graph.pushDownstream(stack, _HEAD);
	}

	void Machine::_pop(Instruction ins)
	{
		if (stack.empty())
			return;

		_HEAD = stack.front();
		stack.pop_front();
	}

	void Machine::_nop(Instruction ins)
	{
		// no operation
	}

	Machine::operation_t const Machine::operations[OPCODE_COUNT] =
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
