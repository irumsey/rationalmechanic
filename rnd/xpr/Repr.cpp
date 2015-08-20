#include "Repr.h"
#include "Node.h"

namespace /* anonymous */
{
	double const TOL = 1e-12;
	double const TOLSQ = TOL * TOL;
}

namespace rnd
{
	namespace xpr
	{

		Repr::Repr(std::ostream &stream)
			: stream(stream)
		{
		}

		void Repr::operator()(Node const *node)
		{
			node->apply(this);
		}

		void Repr::evaluate(Constant const *node)
		{
			double diffZero = 0.0 - node->value;
			double diffOne = 1.0 - node->value;

			if ((diffZero * diffZero) < TOLSQ)
			{
				stream << "val(0)";
			}
			else if ((diffOne * diffOne) < TOLSQ)
			{
				stream << "val(1)";
			}
			else
			{
				stream << "val(k)";
			}
		}

		void Repr::evaluate(Variable const *node)
		{
			stream << "var(" << node->symbol << ")";
		}

		void Repr::evaluate(Negate const *node)
		{
			evaluateOperation("neg", node);
		}

		void Repr::evaluate(NaturalLogarithm const *node)
		{
			evaluateOperation("ln", node);
		}

		void Repr::evaluate(Sine const *node)
		{
			evaluateOperation("sin", node);
		}

		void Repr::evaluate(Cosine const *node)
		{
			evaluateOperation("cos", node);
		}

		void Repr::evaluate(Add const *node)
		{
			evaluateOperation("add", node);
		}

		void Repr::evaluate(Subtract const *node)
		{
			evaluateOperation("sub", node);
		}

		void Repr::evaluate(Multiply const *node)
		{
			evaluateOperation("mul", node);
		}

		void Repr::evaluate(Divide const *node)
		{
			evaluateOperation("div", node);
		}

		void Repr::evaluate(Power const *node)
		{
			evaluateOperation("pow", node);
		}

		void Repr::evaluateOperation(std::string const &label, UnaryOperation const *oper)
		{
			stream << label << "(";
			oper->rhs->apply(this);
			stream << ")";
		}

		void Repr::evaluateOperation(std::string const &label, BinaryOperation const *oper)
		{
			stream << label << "(";
			oper->lhs->apply(this);
			stream << ", ";
			oper->rhs->apply(this);
			stream << ")";
		}

	}	///	xpr
}		///	rnd