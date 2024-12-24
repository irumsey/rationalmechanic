#include "Action.h"
#include "Hasher.h"
#include "Iterator.h"
#include "Clone.h"
#include "Node.h"
#include <cassert>

LUCID_XPR_BEGIN

namespace action
{

	typedef BinaryOperation const *(*binary_func_t)(Node const *, Node const *);

	/// 
	/// utility methods
	/// 

	/// debug assert is sufficient.  when the token hashing and
	/// simplification rules are correct, the cast should not fail.
	template<typename T> T const *cast(Node const *node)
	{
		T const *result = reinterpret_cast<T const *>(node);
		assert(nullptr != result);
		return result;
	}

	bool equal(Node const *lhs, Node const *rhs)
	{
		thread_local static Hasher hash;

		Iterator lhsIter = lhs;
		Iterator rhsIter = rhs;

		while ((nullptr != lhsIter) && (nullptr != rhsIter))
		{
			if (hash(lhsIter) != hash(rhsIter))
				return false;
			++lhsIter;
			++rhsIter;
		}

		return ((nullptr == lhsIter) && (nullptr == rhsIter));
	}

	Node const *copy(Node const *node)
	{
		thread_local static Clone clone;
		return clone(node);
	}

	bool not_equal(Node const *lhs, Node const *rhs)
	{
		return !equal(lhs, rhs);
	}

	float64_t value(Node const *node)
	{
		return cast<Constant>(node)->value;
	}

	uint64_t index(Node const *node)
	{
		return cast<Variable>(node)->index;
	}

	Node const *arg(Node const *node)
	{
		return cast<UnaryOperation>(node)->rhs;
	}

	Node const *lhs(Node const *node)
	{
		return cast<BinaryOperation>(node)->lhs;
	}

	Node const *rhs(Node const *node)
	{
		return cast<BinaryOperation>(node)->rhs;
	}

	Node const *swap(binary_func_t func, Node const *node)
	{
		Node const *x = copy(lhs(node));
		Node const *y = copy(rhs(node));
		delete node;

		return func(y, x);
	}

	Node const *sort(binary_func_t func, Node const *node)
	{
		if (index(rhs(node)) < index(lhs(node)))
			return swap(func, node);
		return node;
	}

	/// 
	/// the actions
	/// 

	Node const *nop(Node const *node)
	{
		return node;
	}

	Node const *zero(Node const *node)
	{
		delete node;
		return val(0);
	}

	Node const *one(Node const *node)
	{
		delete node;
		return val(1);
	}

	Node const *select_arg(Node const *node)
	{
		Node const *copied = copy(arg(node));
		delete node;

		return copied;
	}

	Node const *select_lhs(Node const *node)
	{
		Node const *copied = copy(lhs(node));
		delete node;

		return copied;
	}

	Node const *select_rhs(Node const *node)
	{
		Node const *copied = copy(rhs(node));
		delete node;

		return copied;
	}

	Node const *negate_rhs(Node const *node)
	{
		Node const *copied = copy(rhs(node));
		delete node;

		return neg(copied);
	}

	Node const *swap_add_args(Node const *node)
	{
		return swap(add, node);
	}

	Node const *sort_add_args(Node const *node)
	{
		return sort(add, node);
	}

	Node const *swap_mul_args(Node const *node)
	{
		return swap(mul, node);
	}

	Node const *sort_mul_args(Node const *node)
	{
		return sort(mul, node);
	}

	Node const *inv_and_mul(Node const *node)
	{
		Node const *x = copy(lhs(node));
		Node const *y = copy(lhs(rhs(node)));
		Node const *z = copy(rhs(rhs(node)));
		delete node;

		return mul(x, div(z, y));
	}

	Node const *factor_terms(Node const *node)
	{
		if (not_equal(lhs(node), rhs(node)))
			return node;

		Node const *result = mul(val(2), copy(lhs(node)));
		delete node;

		return result;
	}

	Node const *sub_cancel(Node const *node)
	{
		if (not_equal(lhs(node), rhs(node)))
			return node;
		delete node;

		return val(0);
	}

	Node const *compute_neg(Node const *node)
	{
		float64_t result = -value(rhs(node));
		delete node;

		return val(result);
	}

	Node const *compute_add(Node const *node)
	{
		float64_t result = value(lhs(node)) + value(rhs(node));
		delete node;

		return val(result);
	}

	Node const *collapse_addition(Node const *node)
	{
		Node const *x = copy (lhs(lhs(node)));
		float64_t   a = value(rhs(lhs(node)));
		float64_t   b = value(rhs(node));
		delete node;

		return add(x, val(a + b));
	}

	Node const *compute_sub(Node const *node)
	{
		float64_t result = value(lhs(node)) - value(rhs(node));
		delete node;

		return val(result);
	}

	Node const *compute_mul(Node const *node)
	{
		float64_t result = value(lhs(node)) * value(rhs(node));
		delete node;

		return val(result);
	}

	Node const *mul_combine_const(Node const *node)
	{
		float64_t k = value(lhs(lhs(node))) * value(rhs(node));
		Node const *D = copy(rhs(lhs(node)));
		delete node;

		return div(val(k), D);
	}

	Node const *div_combine_const(Node const *node)
	{
		float64_t k = value(rhs(lhs(node))) / value(rhs(node));
		Node const *x = copy(lhs(lhs(node)));
		delete node;

		return mul(x, val(k));
	}

	Node const *collapse_multiplication(Node const *node)
	{
		Node const *x = copy (lhs(lhs(node)));
		float64_t   a = value(rhs(lhs(node)));
		float64_t   b = value(rhs(node));
		delete node;

		return mul(x, val(a * b));
	}

	Node const *compute_div(Node const *node)
	{
		float64_t result = value(lhs(node)) / value(rhs(node));
		delete node;

		return val(result);
	}

	Node const *compute_sin(Node const *node)
	{
		float64_t  theta = value(arg(node));
		float64_t result = ::sin(theta);
		delete node;

		return val(result);
	}

	Node const *compute_cos(Node const *node)
	{
		float64_t  theta = value(arg(node));
		float64_t result = ::cos(theta);
		delete node;

		return val(result);
	}

	Node const *compute_exp(Node const *node)
	{
		float64_t      x = value(arg(node));
		float64_t result = ::exp(x);
		delete node;

		return val(result);
	}

	Node const *compute_log(Node const *node)
	{
		float64_t      x = value(arg(node));
		float64_t result = ::log(x);
		delete node;

		return val(result);
	}

	Node const *collapse_identity(Node const *node)
	{
		Node const *copied = copy(arg(arg(node)));
		delete node;

		return copied;
	}

	Node const *div_cancel(Node const *node)
	{
		if (not_equal(lhs(node), rhs(node)))
			return node;
		delete node;

		return val(1);
	}

	Node const *div_pow_cancel(Node const *node)
	{
		Node const *x = lhs(lhs(node));
		Node const *y = rhs(node);

		if (not_equal(x, y))
			return node;

		Node const *e = rhs(lhs(node));
		Node const *canceled = pow(copy(x), sub(copy(e), val(1)));
		delete node;

		return canceled;
	}

}

LUCID_XPR_END
