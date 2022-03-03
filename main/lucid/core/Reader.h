#pragma once

#include <string>
#include <lucid/core/Types.h>

///
///
///
namespace lucid {
namespace core {

	///
	///
	///

	class Identity;

	///	Reader
	///
	///	generic serialization facility.
	class Reader
	{
	public:
		virtual ~Reader() = default;

		virtual void read(void *data, size_t size) = 0;

		virtual void read(bool &data) = 0;

		virtual void read(int8_t &data) = 0;

		virtual void read(uint8_t &data) = 0;

		virtual void read(int16_t &data) = 0;

		virtual void read(uint16_t &data) = 0;

		virtual void read(int32_t &data) = 0;

		virtual void read(uint32_t &data) = 0;

		virtual void read(int64_t &data) = 0;

		virtual void read(uint64_t &data) = 0;

		virtual void read(float32_t &data) = 0;

		virtual void read(float64_t &data) = 0;

		virtual void read(std::string &data) = 0;

		virtual void read(Identity &data) = 0;

		template<class T> T read();

	protected:
		Reader() = default;

	};

	template<class T> inline T Reader::read()
	{
		T value = T();
		read(value);		
		return value;
	}

}	///	core
}	///	lucid
