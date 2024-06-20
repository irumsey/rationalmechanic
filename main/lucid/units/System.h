#pragma once

#include <lucid/units/Quantity.h>

///
///
///

namespace lucid {
namespace units {

	///	Unit
	///
	///	a simple type used as a tag which combines a quantity with a system of measurement.
	///	this is what one can use to define a unique type which can only combine with other, compatable, types.
	template<typename S, typename Q> struct Unit
	{
	};

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

	typedef Unit<system::none, quantity::pure> none;

	///
	///	SI
	///

	namespace si
	{

		typedef Unit<system::si, quantity::                  pure>         pure;

		typedef Unit<system::si, quantity::                  pure>       radian;

		typedef Unit<system::si, quantity::                  time>       second;
		typedef Unit<system::si, quantity::                length>        meter;
		typedef Unit<system::si, quantity::                  mass>     kilogram;
		typedef Unit<system::si, quantity::           temperature>       kelvin;
		typedef Unit<system::si, quantity::               current>       ampere;
		typedef Unit<system::si, quantity::                amount>         mole;
		typedef Unit<system::si, quantity::             intensity>      candela;

		typedef Unit<system::si, quantity::                  area>         area;
		typedef Unit<system::si, quantity::                volume>       volume;

		typedef Unit<system::si, quantity::             frequency>        hertz;
		typedef Unit<system::si, quantity::              velocity>     velocity;
		typedef Unit<system::si, quantity::          acceleration> acceleration;
		typedef Unit<system::si, quantity::                 force>       newton;
/*		typedef Unit<system::si, quantity::              pressure>       pascal;         defined as a macro which i will have to work around */
		typedef Unit<system::si, quantity::                energy>        joule;
		typedef Unit<system::si, quantity::                 power>         watt;

		typedef Unit<system::si, quantity::     electrical_charge>      coulomb;
		typedef Unit<system::si, quantity::  electrical_potential>         volt;
		typedef Unit<system::si, quantity::electrical_capacitance>        farad;
		typedef Unit<system::si, quantity:: electrical_resistance>          ohm;
		typedef Unit<system::si, quantity::electrical_conductance>       seimen;
		typedef Unit<system::si, quantity:: electrical_inductance>        henry;

		typedef Unit<system::si, quantity::         magnetic_flux>        weber;
		typedef Unit<system::si, quantity::    magnetic_induction>        tesla;

		typedef Unit<system::si, quantity::           illuminance>          lux;

		///	TBD: flesh out

	}	///	si

	///
	///	MKS
	///

	namespace mks
	{

		typedef Unit<system::mks, quantity::                  pure>         pure;

		typedef Unit<system::mks, quantity::                  pure>       radian;

		typedef Unit<system::mks, quantity::                  time>       second;
		typedef Unit<system::mks, quantity::                length>        meter;
		typedef Unit<system::mks, quantity::                  mass>     kilogram;

		typedef Unit<system::mks, quantity::                  area>         area;
		typedef Unit<system::mks, quantity::                volume>       volume;

		///	TBD: flesh out

	}

	///
	///	CGS
	///

	namespace cgs
	{

		typedef Unit<system::cgs, quantity::                  pure>         pure;

		typedef Unit<system::cgs, quantity::                  time>       second;
		typedef Unit<system::cgs, quantity::                length>   centimeter;
		typedef Unit<system::cgs, quantity::                  mass>         gram;

		typedef Unit<system::cgs, quantity::                  area>         area;
		typedef Unit<system::cgs, quantity::                volume>       volume;

		///	TBD: flesh out

	}

}	///	units
}	///	lucid
