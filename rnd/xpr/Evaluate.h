#pragma once

#include <string>
#include <unordered_map>
#include <rnd/xpr/Symbols.h>
#include <rnd/xpr/Algorithm.h>

namespace rnd
{
	namespace xpr
	{

		class Node;
		class UnaryOperation;
		class BinaryOperation;

		///	Evaluate
		///
		///	evaluates the supplied expression using the
		///	supplied symbol substitutions.
		///
		///	SEE ALSO: Symbols
		class Evaluate : public Algorithm
		{
		public:
			Evaluate() = default;

			virtual ~Evaluate() = default;

			double operator()(Node const *node, Symbols const &symbols);

			virtual void evaluate(Constant const *node) override;

			virtual void evaluate(Variable const *node) override;

			virtual void evaluate(Negate const *node) override;

			virtual void evaluate(NaturalLogarithm const *node) override;

			virtual void evaluate(Sine const *node) override;

			virtual void evaluate(Cosine const *node) override;

			virtual void evaluate(Add const *node) override;

			virtual void evaluate(Subtract const *node) override;

			virtual void evaluate(Multiply const *node) override;

			virtual void evaluate(Divide const *node) override;

			virtual void evaluate(Power const *node) override;

		private:
			Symbols symbols;
			double result = 0.0;

			template<class T> double lhs(T const *node);

			template<class T> double rhs(T const *node);

		};

		template<class T> inline double Evaluate::lhs(T const *node)
		{
			node->lhs->apply(this);
			return result;
		}

		template<class T> inline double Evaluate::rhs(T const *node)
		{
			node->rhs->apply(this);
			return result;
		}

	}	///	xpr
}		///	rnd
