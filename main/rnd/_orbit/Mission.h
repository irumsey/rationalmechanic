#pragma once

#include <lucid/core/Defines.h>
#include <rnd/_orbit/Defines.h>
#include <rnd/_orbit/Dictionary.h>
#include <rnd/_orbit/Ephemeris.h>

LUCID_CORE_BEGIN

class Writer;
class Reader;

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

	Mission(LUCID_CORE::Reader &reader);

	virtual ~Mission();

	void write(LUCID_CORE::Writer &writer) const;

	void read(LUCID_CORE::Reader &reader);

private:
	Dictionary _dictionary;
	Ephemeris _ephemeris;

	Frame *_root = nullptr;

	void write(LUCID_CORE::Writer &writer, Frame const *frame) const;

	Frame *read_frame(LUCID_CORE::Reader &reader);

	void shutdown();

};

ORBIT_END
