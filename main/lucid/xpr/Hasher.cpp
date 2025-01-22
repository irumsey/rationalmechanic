#include "Hasher.h"
#include "Iterator.h"
#include "Node.h"

LUCID_XPR_BEGIN

///
///
/// 

Token const Token::ANY = Token(             size_t(-1));
Token const Token:: FN = Token(TYPE<   Function>::ID());
Token const Token::DDX = Token(TYPE< Derivative>::ID());
Token const Token::NEG = Token(TYPE<     Negate>::ID());
Token const Token::ADD = Token(TYPE<        Add>::ID());
Token const Token::SUB = Token(TYPE<   Subtract>::ID());
Token const Token::MUL = Token(TYPE<   Multiply>::ID());
Token const Token::DIV = Token(TYPE<     Divide>::ID());
Token const Token::SIN = Token(TYPE<       Sine>::ID());
Token const Token::COS = Token(TYPE<     Cosine>::ID());
Token const Token::EXP = Token(TYPE<Exponential>::ID());
Token const Token::LOG = Token(TYPE<  Logarithm>::ID());

Token Token::VAL()                { return Token(TYPE<Constant>::ID()       ); }
Token Token::VAL(float64_t value) { return Token(TYPE<Constant>::ID(), value); }

Token Token::VAR()                { return Token(TYPE<Variable>::ID()       ); }
Token Token::VAR( uint64_t index) { return Token(TYPE<Variable>::ID(), index); }

/// 
/// 
/// 

Token Hasher::hash(Iterator const &iter)
{
	return hash(iter.ptr());
}

Token Hasher::hash(Node const *node)
{
	token = Token();
	node->apply(this);
	return token;
}

void Hasher::evaluate(Constant const *node)
{
	token.tid = TYPE<Constant>::ID();
	token.value = node->value;
}

void Hasher::evaluate(Variable const *node)
{
	token.tid = TYPE<Variable>::ID();
	token.index = node->index;
}

void Hasher::evaluate(Function const *node)
{
	token.tid = TYPE<Function>::ID();
	token.index = node->index;
}

void Hasher::evaluate(Derivative const *node)
{
	token.tid = TYPE<Derivative>::ID();
	token.index = node->index;
}

void Hasher::evaluate(Negate const *node)
{
	token.tid = TYPE<Negate>::ID();
}

void Hasher::evaluate(Add const *node)
{
	token.tid = TYPE<Add>::ID();
}

void Hasher::evaluate(Subtract const *node)
{
	token.tid = TYPE<Subtract>::ID();
}

void Hasher::evaluate(Multiply const *node)
{
	token.tid = TYPE<Multiply>::ID();
}

void Hasher::evaluate(Divide const *node)
{
	token.tid = TYPE<Divide>::ID();
}

void Hasher::evaluate(Sine const *node)
{
	token.tid = TYPE<Sine>::ID();
}

void Hasher::evaluate(Cosine const *node)
{
	token.tid = TYPE<Cosine>::ID();
}

void Hasher::evaluate(Exponential const *node)
{
	token.tid = TYPE<Exponential>::ID();
}

void Hasher::evaluate(Logarithm const *node)
{
	token.tid = TYPE<Logarithm>::ID();
}

LUCID_XPR_END
