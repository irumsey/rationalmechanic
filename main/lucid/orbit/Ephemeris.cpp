#include "Ephemeris.h"

///
///
///

namespace lucid {
namespace orbit {

	Ephemeris::Ephemeris()
	{
	}

	Ephemeris::~Ephemeris()
	{
	}

	void Ephemeris::initialize(std::string const &path)
	{
		///	TBD: implement
	}

	Ephemeris &Ephemeris::instance()
	{
		static Ephemeris theInstance;
		return theInstance;
	}

}	///	orbit
}	///	lucid