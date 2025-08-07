#pragma once

#include <lucid/core/Defines.h>
#include <rnd/_orbit/Defines.h>
#include <rnd/_orbit/Dictionary.h>
#include <rnd/_orbit/StarCatalog.h>
#include <rnd/_orbit/Ephemeris.h>

LUCID_CORE_BEGIN

class Unserializer;

LUCID_CORE_END

ORBIT_BEGIN

class Frame;

/// Mission
///
/// 
class Mission
{
public:
	Mission();

	Mission(LUCID_CORE::Unserializer &stream);

	virtual ~Mission();

	void read(LUCID_CORE::Unserializer &stream);

private:
	Dictionary _dictionary;
	StarCatalog _stars;
	Ephemeris _ephemeris;

	Frame *_root = nullptr;

	Frame *read_frame(LUCID_CORE::Unserializer &stream);

	void shutdown();

};

ORBIT_END
