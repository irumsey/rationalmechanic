#pragma once

///
///
///

namespace lucid {
namespace units {

	///	Quantity
	///
	///	an aspect of something to be measured
	template<int TI ,int LE, int MA, int TE, int CU, int AM, int IN> struct Quantity
	{
		enum {        TIME = TI };
		enum {      LENGTH = LE };
		enum {        MASS = MA };
		enum { TEMPERATURE = TE };
		enum {     CURRENT = CU };
		enum {      AMOUNT = AM };
		enum {   INTENSITY = IN };
	};

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

	typedef Quantity< 2, 0, 0, 0, 0, 0, 0> area;
	typedef Quantity< 3, 0, 0, 0, 0, 0, 0> volume;
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

}	///	units
}	///	lucid
