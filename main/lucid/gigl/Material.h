#pragma once

#include <memory>
#include <string>
#include <hash_map>
#include <lucid/core/Identity.h>
#include <lucid/core/Noncopyable.h>
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
namespace gal {

	class Parameter;
	class Program;

}	///	gal
}	///	lucid

///
///
///
namespace lucid {
namespace gigl {

	class Context;

	///	Material
	///
	///	a gigl material is defined as: a "bag" of data and a
	///	gal program to interpret it.  in other words, a gal
	///	program (render state, vertex shader, and pixel shader)
	///	can require any data it likes.  a gigl material encapsulates
	///	a program and provides a mechanism to supply that data.
	///
	///	note: in the context of a material, data means shader constant
	///	data.  per vertex and per instance data is supplied via 
	///	the vertex buffers.
	///
	///	note: a material provides a program's constant data in one
	///	of two ways.  a material may contain constant data, meaning
	///	data which does not change for the life of the material.  in addition,
	///	a material may also contain "queries".  a "query" is a request for
	///	data which is supplied by the application via a rendering context.
	class Material final
	{
	public:
		Material() = delete;

		Material(std::string const &path);

		Material(core::Reader &reader);

		virtual ~Material();

		::lucid::core::Identity const &identity() const;

		std::shared_ptr<gal::Program> program() const;

		Primitive const &attribute(std::string const &name) const;

		Primitive &attribute(std::string const &name);

		void begin(Context const &context) const;

		void end();

		static Material *create(std::string const &path);

		static Material *create(core::Reader &reader);

	private:
		///	Attribute
		///
		///	a material attribute "binds" a value to a program parameter.
		struct Attribute
		{
			::lucid::gal::Parameter const *parameter = nullptr;
			Primitive value;

			Attribute() = default;

			Attribute(::lucid::gal::Parameter const *parameter, Primitive const &value)
				: parameter(parameter)
				, value(value)
			{
			}
		};

		friend struct Applicator;

		::lucid::core::Identity const _identity;

		std::shared_ptr<::lucid::gal::Program> _program;
		std::hash_map<std::string, Attribute> _attributes;

		void initialize(::lucid::core::Reader &reader);

		void shutdown();

		LUCID_PREVENT_COPY(Material);
		LUCID_PREVENT_ASSIGNMENT(Material);
	};

	inline ::lucid::core::Identity const &Material::identity() const
	{
		return _identity;
	}

	inline std::shared_ptr<gal::Program> Material::program() const
	{
		return _program;
	}

	inline Primitive const &Material::attribute(std::string const &name) const
	{
		auto const &iter = _attributes.find(name);
		LUCID_VALIDATE(iter != _attributes.end(), "unknown material attribute '" + name + "' specified");

		return iter->second.value;
	}

	inline Primitive &Material::attribute(std::string const &name)
	{
		auto &iter = _attributes.find(name);
		LUCID_VALIDATE(iter != _attributes.end(), "unknown material attribute '" + name + "' specified");

		return iter->second.value;
	}

}	///	gigl
}	///	lucid