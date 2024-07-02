#pragma once

#include <lucid/units/Defines.h>

LUCID_UNITS_BEGIN

///	Quantity
///
///	an aspect of something to be measured
template<int TME ,int LNG, int MSS, int TMP, int CRR, int AMN, int INT> struct Quantity
{
	enum {        TIME = TME };
	enum {      LENGTH = LNG };
	enum {        MASS = MSS };
	enum { TEMPERATURE = TMP };
	enum {     CURRENT = CRR };
	enum {      AMOUNT = AMN };
	enum {   INTENSITY = INT };
};

namespace quantity
{
	///
	///	dimensionless (pure) number
	///

	typedef Quantity< 0, 0, 0, 0, 0, 0, 0> pure;

	///
	///	base quantities
	///

	typedef Quantity< 1, 0, 0, 0, 0, 0, 0> time;
	typedef Quantity< 0, 1, 0, 0, 0, 0, 0> length;
	typedef Quantity< 0, 0, 1, 0, 0, 0, 0> mass;
	typedef Quantity< 0, 0, 0, 1, 0, 0, 0> temperature;
	typedef Quantity< 0, 0, 0, 0, 1, 0, 0> current;
	typedef Quantity< 0, 0, 0, 0, 0, 1, 0> amount;
	typedef Quantity< 0, 0, 0, 0, 0, 0, 1> intensity;

	///
	///	derived quantities
	///

	typedef Quantity< 0, 2, 0, 0, 0, 0, 0> area;
	typedef Quantity< 0, 3, 0, 0, 0, 0, 0> volume;
	typedef Quantity<-1, 0, 0, 0, 0, 0, 0> frequency;
	typedef Quantity<-1, 1, 0, 0, 0, 0, 0> velocity;	
	typedef Quantity<-2, 1, 0, 0, 0, 0, 0> acceleration;
	typedef Quantity<-2, 1, 1, 0, 0, 0, 0> force;
	typedef Quantity<-2,-1, 1, 0, 0, 0, 0> pressure;
	typedef Quantity<-1, 1, 1, 0, 0, 0, 0> momentum;
	typedef Quantity<-1, 2, 1, 0, 0, 0, 0> angular_momentum;
	typedef Quantity<-2, 2, 1, 0, 0, 0, 0> torque;
	typedef Quantity<-2, 2, 1, 0, 0, 0, 0> energy;
	typedef Quantity<-3, 2, 1, 0, 0, 0, 0> power;
	typedef Quantity<-1, 2, 1, 0, 0, 0, 0> action;
	typedef Quantity< 1, 0, 0, 0, 1, 0, 0> electrical_charge;
	typedef Quantity<-3, 2, 1, 0,-1, 0, 0> electrical_potential;
	typedef Quantity< 4,-2,-1, 0, 2, 0, 0> electrical_capacitance;
	typedef Quantity<-3, 2, 1, 0,-2, 0, 0> electrical_resistance;
	typedef Quantity< 3,-2,-1, 0, 2, 0, 0> electrical_conductance;
	typedef Quantity<-2, 2, 1, 0,-2, 0, 0> electrical_inductance;
	typedef Quantity<-2, 2, 1, 0,-1, 0, 0> magnetic_flux;
	typedef Quantity<-2, 2, 1, 0,-1, 0, 0> magnetic_induction;
	typedef Quantity< 0,-2, 0, 0, 0, 0, 1> illuminance;

	///	add
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

	///	sub
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

	///	half
	///
	///	TBD: figure out if i can error on odd quantities
	template<typename RHS> struct half
	{
		typedef typename Quantity
		<
			RHS::       TIME / 2,
			RHS::     LENGTH / 2,
			RHS::       MASS / 2,
			RHS::TEMPERATURE / 2,
			RHS::    CURRENT / 2,
			RHS::     AMOUNT / 2,
			RHS::  INTENSITY / 2
		> result;
	};

	///	neg
	///
	/// 
	template<typename RHS> struct neg
	{
		typedef typename Quantity
		<
			-RHS::       TIME,
			-RHS::     LENGTH,
			-RHS::       MASS,
			-RHS::TEMPERATURE,
			-RHS::    CURRENT,
			-RHS::     AMOUNT,
			-RHS::  INTENSITY
		> result;
	};

}	///	quantity

LUCID_UNITS_END
