#pragma once

#include <lucid/xpr/Defines.h>

LUCID_XPR_BEGIN

class Node;

typedef Node const *(*action_type)(Node const *);

/// 
/// Actions
/// 
/// Note: actions to perform when a particular condition
/// is met.  
namespace action
{

	Node const *nop(Node const *node);

	Node const *zero(Node const *node);

	Node const *one(Node const *node);

	Node const *select_arg(Node const *node);

	Node const *select_lhs(Node const *node);

	Node const *select_rhs(Node const *node);

	Node const *negate_rhs(Node const *node);

	Node const *swap_add_args(Node const *node);

	Node const *sort_add_args(Node const *node);

	Node const *swap_mul_args(Node const *node);

	Node const *sort_mul_args(Node const *node);

	Node const *inv_and_mul(Node const *node);

	Node const *factor_terms(Node const *node);

	Node const *sub_cancel(Node const *node);

	Node const *compute_neg(Node const *node);

	Node const *compute_add(Node const *node);

	Node const *collapse_addition(Node const *node);

	Node const *compute_sub(Node const *node);

	Node const *compute_mul(Node const *node);

	Node const *mul_combine_1(Node const *node);

	Node const *div_combine_1(Node const *node);

	Node const *div_combine_2(Node const *node);

	Node const *collapse_multiplication(Node const *node);

	Node const *compute_div(Node const *node);

	Node const *compute_sin(Node const *node);

	Node const *compute_cos(Node const *node);

	Node const *compute_exp(Node const *node);

	Node const *compute_log(Node const *node);

	Node const *compute_pow(Node const *node);

	Node const *collapse_identity(Node const *node);

	Node const *div_cancel(Node const *node);

	Node const *div_pow_cancel(Node const *node);

}

LUCID_XPR_END
