#pragma once

#include <lucid/units/Quantity.h>

///
///
///

namespace lucid {
namespace units {

	///
	///
	///
	template<typename LHS, typename RHS> struct add
	{
		typedef typename Quantity
		<
			LHS::       TIME + RHS::       TIME,
			LHS::     LENGTH + RHS::     LENGTH,
			LHS::       MASS + RHS::       MASS,
			LHS::TEMPERATURE + RHS::TEMPERATURE,
			LHS::    CURRENT + RHS::    CURRENT,
			LHS::     AMOUNT + RHS::     AMOUNT,
			LHS::  INTENSITY + RHS::  INTENSITY
		> result;
	};

	///
	///
	///
	template<typename LHS, typename RHS> struct sub
	{
		typedef typename Quantity
		<
			LHS::       TIME - RHS::       TIME,
			LHS::     LENGTH - RHS::     LENGTH,
			LHS::       MASS - RHS::       MASS,
			LHS::TEMPERATURE - RHS::TEMPERATURE,
			LHS::    CURRENT - RHS::    CURRENT,
			LHS::     AMOUNT - RHS::     AMOUNT,
			LHS::  INTENSITY - RHS::  INTENSITY
		> result;
	};

	///
	///
	///
	template<typename LHS, typename RHS> struct mul
	{
		typedef typename Quantity
		<
			LHS::       TIME * RHS::       TIME,
			LHS::     LENGTH * RHS::     LENGTH,
			LHS::       MASS * RHS::       MASS,
			LHS::TEMPERATURE * RHS::TEMPERATURE,
			LHS::    CURRENT * RHS::    CURRENT,
			LHS::     AMOUNT * RHS::     AMOUNT,
			LHS::  INTENSITY * RHS::  INTENSITY
		> result;
	};

	///
	///
	///
	template<typename LHS, typename RHS> struct div
	{
		typedef typename Quantity
		<
			LHS::       TIME / RHS::       TIME,
			LHS::     LENGTH / RHS::     LENGTH,
			LHS::       MASS / RHS::       MASS,
			LHS::TEMPERATURE / RHS::TEMPERATURE,
			LHS::    CURRENT / RHS::    CURRENT,
			LHS::     AMOUNT / RHS::     AMOUNT,
			LHS::  INTENSITY / RHS::  INTENSITY
		> result;
	};

}	///	units
}	///	lucid
