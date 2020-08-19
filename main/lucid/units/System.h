#pragma once

#include <lucid/units/Quantity.h>

///
///
///

namespace lucid {
namespace units {

	///
	///	simple types to be used as "tags" to define unique types
	///

	namespace system
	{

		///
		///	the "no system" system
		///
		struct none {};

		///
		///	Système International (SI)
		///
		struct si {};

		///
		///	meter-kilogram-second (mks)
		///
		struct mks {};

		///
		///	centimeter-gram-second (cgs)
		///
		struct cgs {};

	}

	///	Units
	///
	///	a simple type used a a tag which combines a quantity and a system of measurement.
	///	this is what one can use to define a unique type which can only combine with other, compatable, types.
	template<typename S, typename Q> struct Units
	{
		typedef typename S system_t;
		typedef typename Q quantity_t;
	};

	///
	///	SI
	///

	namespace si
	{

		typedef Units<system::si, quantity::                  pure>         pure;

		typedef Units<system::si, quantity::                  pure>       radian;

		typedef Units<system::si, quantity::                  time>       second;
		typedef Units<system::si, quantity::                length>        meter;
		typedef Units<system::si, quantity::                  mass>     kilogram;
		typedef Units<system::si, quantity::           temperature>       kelvin;
		typedef Units<system::si, quantity::               current>       ampere;
		typedef Units<system::si, quantity::                amount>         mole;
		typedef Units<system::si, quantity::             intensity>      candela;

		typedef Units<system::si, quantity::                  area>         area;
		typedef Units<system::si, quantity::                volume>       volume;

		typedef Units<system::si, quantity::             frequency>        hertz;
		typedef Units<system::si, quantity::              velocity>     velocity;
		typedef Units<system::si, quantity::          acceleration> acceleration;
		typedef Units<system::si, quantity::                 force>       newton;
		typedef Units<system::si, quantity::              pressure>       pascal;
		typedef Units<system::si, quantity::                energy>        joule;
		typedef Units<system::si, quantity::                 power>         watt;

		typedef Units<system::si, quantity::     electrical_charge>      coulomb;
		typedef Units<system::si, quantity::  electrical_potential>         volt;
		typedef Units<system::si, quantity::electrical_capacitance>        farad;
		typedef Units<system::si, quantity:: electrical_resistance>          ohm;
		typedef Units<system::si, quantity::electrical_conductance>       seimen;
		typedef Units<system::si, quantity:: electrical_inductance>        henry;

		typedef Units<system::si, quantity::         magnetic_flux>        weber;
		typedef Units<system::si, quantity::    magnetic_induction>        tesla;

		typedef Units<system::si, quantity::           illuminance>          lux;

		///	TBD: flesh out

	}	///	si

	///
	///	MKS
	///

	namespace mks
	{

		typedef Units<system::mks, quantity::                  pure>         pure;

		typedef Units<system::mks, quantity::                  pure>       radian;

		typedef Units<system::mks, quantity::                  time>       second;
		typedef Units<system::mks, quantity::                length>        meter;
		typedef Units<system::mks, quantity::                  mass>     kilogram;

		typedef Units<system::mks, quantity::                  area>         area;
		typedef Units<system::mks, quantity::                volume>       volume;

		///	TBD: flesh out

	}

	///
	///	CGS
	///

	namespace cgs
	{

		typedef Units<system::cgs, quantity::                  pure>         pure;

		typedef Units<system::cgs, quantity::                  time>       second;
		typedef Units<system::cgs, quantity::                length>   centimeter;
		typedef Units<system::cgs, quantity::                  mass>         gram;

		typedef Units<system::cgs, quantity::                  area>         area;
		typedef Units<system::cgs, quantity::                volume>       volume;

		///	TBD: flesh out

	}

}	///	units
}	///	lucid
