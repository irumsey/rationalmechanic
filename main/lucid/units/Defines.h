#pragma once

#include <lucid/core/Defines.h>

#define LUCID_UNITS ::lucid::units

#define LUCID_UNITS_BEGIN LUCID_BEGIN namespace units {
#define LUCID_UNITS_END } LUCID_END

#define LUCID_QUANTITY_PURE ::lucid::units::quantity::pure

#define LUCID_QUANTITY_ADD(LHS, RHS) typename ::lucid::units::quantity::add<LHS, RHS>::result

#define LUCID_QUANTITY_SUB(LHS, RHS) typename ::lucid::units::quantity::sub<LHS, RHS>::result

#define LUCID_QUANTITY_NEG(RHS) typename ::lucid::units::quantity::neg<RHS>::result

#define LUCID_QUANTITY_HALF(RHS) typename ::lucid::units::quantity::half<RHS>::result
