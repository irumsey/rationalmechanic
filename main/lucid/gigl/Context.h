#pragma once

#include <string>
#include <unordered_map>
#include <lucid/core/Error.h>
#include <lucid/gigl/Primitive.h>

///
///
///
namespace lucid {
namespace core {

	class Reader;

}	///	core
}	///	lucid

///
///
///
namespace lucid {
namespace gigl {

	///	Context
	///
	///
	class Context final
	{
	public:
		Context() = default;

		~Context() = default;

		Context(std::string const &path);

		Context(::lucid::core::Reader &reader);

		Primitive &operator[](std::string const &name);

		Primitive const &operator[](std::string const &name) const;

		Primitive &lookup(std::string const &name);

		Primitive const &lookup(std::string const &name) const;

		void add(std::string const &name, Primitive const &parameter);

	private:
		std::unordered_map<std::string, Primitive> _values;

		void initialize(::lucid::core::Reader &reader);

	};

	inline Primitive &Context::operator[](std::string const &name)
	{
		return lookup(name);
	}

	inline Primitive const &Context::operator[](std::string const &name) const
	{
		return lookup(name);
	}

	inline Primitive &Context::lookup(std::string const &name)
	{
		auto iter = _values.find(name);
		LUCID_VALIDATE(iter != _values.end(), "unknown parameter, " + name + ", requested");

		return _values[name];
	}

	inline Primitive const &Context::lookup(std::string const &name) const
	{
		auto iter = _values.find(name);
		LUCID_VALIDATE(iter != _values.end(), "unknown parameter, " + name + ", requested");

		return iter->second;
	}

	inline void Context::add(std::string const &name, Primitive const &parameter)
	{
		LUCID_VALIDATE(_values.end() == _values.find(name), "duplicate parameter, " + name + ", specified");

		_values[name] = parameter;
	}

}	///	gigl
}	///	lucid
