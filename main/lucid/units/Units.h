#pragma once

#include <lucid/units/Quantity.h>
#include <lucid/units/Meta.h>
#include <lucid/units/Measure.h>

namespace lucid {
namespace units {

	///
	///	Système International (SI)
	///

	namespace si
	{

		struct SI {};

		typedef Measure<SI,                   pure, float>         pure;

		typedef Measure<SI,                   pure, float>       radian;

		typedef Measure<SI,                   time, float>       second;
		typedef Measure<SI,                 length, float>        meter;
		typedef Measure<SI,                   mass, float>     kilogram;
		typedef Measure<SI,            temperature, float>       kelvin;
		typedef Measure<SI,                current, float>       ampere;
		typedef Measure<SI,                 amount, float>         mole;
		typedef Measure<SI,              intensity, float>      candela;
									  
		typedef Measure<SI,                   area, float>         area;
		typedef Measure<SI,                 volume, float>       volume;
		typedef Measure<SI,              frequency, float>        hertz;
		typedef Measure<SI,               velocity, float>     velocity;
		typedef Measure<SI,           acceleration, float> acceleration;
		typedef Measure<SI,                  force, float>       newton;
		typedef Measure<SI,               pressure, float>       pascal;
		typedef Measure<SI,                 energy, float>        joule;
		typedef Measure<SI,                  power, float>         watt;

		typedef Measure<SI,      electrical_charge, float>      coulomb;
		typedef Measure<SI,   electrical_potential, float>         volt;
		typedef Measure<SI, electrical_capacitance, float>        farad;
		typedef Measure<SI,  electrical_resistance, float>          ohm;
		typedef Measure<SI, electrical_conductance, float>       seimen;
		typedef Measure<SI,  electrical_inductance, float>        henry;

		typedef Measure<SI,          magnetic_flux, float>        weber;
		typedef Measure<SI,     magnetic_induction, float>        tesla;

		typedef Measure<SI,            illuminance, float>          lux;

		///	TBD: flesh out
	}

	///
	///
	///

	namespace cgs
	{

		struct CGS {};

		typedef Measure<CGS,             frequency, float>        hertz;
		typedef Measure<CGS,                  time, float>       second;
		typedef Measure<CGS,                length, float>   centimeter;
		typedef Measure<CGS,                  mass, float>         gram;
		typedef Measure<CGS,           temperature, float>       kelvin;
		typedef Measure<CGS,               current, float>   statampere;
		typedef Measure<CGS,                 force, float>         dyne;
		typedef Measure<CGS,                energy, float>          erg;

		///	TBD: flesh out
	}

}	///	units
}	///	lucid
