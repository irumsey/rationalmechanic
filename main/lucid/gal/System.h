#pragma once

#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>

///
///
///
namespace lucid {
namespace gal {

	///	System
	///
	///	gal system used for initialization and basic, global, information.
	class System
	{
	public:
		virtual ~System() = 0 {}

		virtual void initialize(bool windowed, int32_t width, int32_t height, int32_t samples, void *window) = 0;

		virtual void shutdown() = 0;

		virtual bool windowed() const = 0;

		virtual int32_t width() const = 0;

		virtual int32_t height() const = 0;

		virtual float32_t aspect() const = 0;

		static System &instance();

	protected:
		System() {}

		LUCID_PREVENT_COPY(System);
		LUCID_PREVENT_ASSIGNMENT(System);
	};

}	///	gal
}	///	lucid
