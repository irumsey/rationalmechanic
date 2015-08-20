#pragma once

#include <cstdint>
#include <memory>

///
///
///
namespace lucid {
namespace core {

	///	Identity
	///
	///	uuid
	class Identity
	{
	public:
		enum { SIZE = 16 };

		static Identity const null;

		Identity();

		Identity(uint8_t const *data);

		operator size_t () const;

		virtual ~Identity();

		uint8_t *data();

		uint8_t const *data() const;

		bool operator==(Identity const &rhs) const;

		bool operator!=(Identity const &rhs) const;

		bool operator<(Identity const &rhs) const;

		bool operator>(Identity const &rhs) const;

	private:
		size_t _hash = 0;
		uint8_t _data[SIZE];

	};

	inline Identity::operator size_t () const
	{
		return _hash;
	}

	inline uint8_t *Identity::data()
	{
		return _data;
	}

	inline uint8_t const *Identity::data() const
	{
		return _data;
	}

	inline bool Identity::operator==(Identity const &rhs) const
	{
		return (0 == ::memcmp(_data, rhs._data, SIZE));
	}

	inline bool Identity::operator!=(Identity const &rhs) const
	{
		return (0 != ::memcmp(_data, rhs._data, SIZE));
	}

	inline bool Identity::operator<(Identity const &rhs) const
	{
		return (0 > ::memcmp(_data, rhs._data, SIZE));
	}

	inline bool Identity::operator>(Identity const &rhs) const
	{
		return (0 < ::memcmp(_data, rhs._data, SIZE));
	}

}	///	core
}	///	lucid