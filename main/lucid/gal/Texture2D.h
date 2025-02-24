#pragma once

#include <string>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/gal/Defines.h>

LUCID_CORE_BEGIN

class Unserializer;

LUCID_CORE_END

LUCID_GAL_BEGIN

///	Texture2D
///
///
class Texture2D
{
public:
	virtual ~Texture2D() = 0 {}

	virtual int32_t width() const = 0;

	virtual int32_t height() const = 0;

	static Texture2D *create(std::string const &path);

	static Texture2D *create(LUCID_CORE::Unserializer &reader);

protected:
	Texture2D() {}

	LUCID_PREVENT_COPY(Texture2D);
	LUCID_PREVENT_ASSIGNMENT(Texture2D);
};

LUCID_GAL_END