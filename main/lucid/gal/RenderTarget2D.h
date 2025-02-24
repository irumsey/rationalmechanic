#pragma once

#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/gal/Defines.h>

LUCID_CORE_BEGIN

class Unserializer;

LUCID_CORE_END

LUCID_GAL_BEGIN

///	RenderTarget2D
///
///
class RenderTarget2D
{
public:
	enum FORMAT
	{
		FORMAT_UNDEFINED = -1,

		FORMAT_UNORM_R8G8B8A8,

		FORMAT_UINT_R16G16,
		FORMAT_UINT_R10G10B10A2,
			
		FORMAT_FLOAT_R16G16,
		FORMAT_FLOAT_R32,
		FORMAT_FLOAT_R32G32,

		FORMAT_UINT_R32,
	};

	virtual ~RenderTarget2D() = 0 {}

	virtual FORMAT format() const = 0;

	virtual int32_t width() const = 0;

	virtual int32_t height() const = 0;

	static RenderTarget2D *create(FORMAT format, int32_t width, int32_t height, int32_t samples = 1);

	static RenderTarget2D *create(LUCID_CORE::Unserializer &read);

protected:
	RenderTarget2D() {}

	LUCID_PREVENT_COPY(RenderTarget2D);
	LUCID_PREVENT_ASSIGNMENT(RenderTarget2D);
};

LUCID_GAL_END
