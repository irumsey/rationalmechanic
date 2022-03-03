#pragma once

#include <lucid/orbit/Frame.h>

namespace lucid {
namespace orbit {

	class Frame;
	class DynamicPoint;
	class OrbitalBody;
	class DynamicBody;

}	///	orbit
}	///	lucid

namespace Lucid {
namespace Math {

	ref class Vector3;

}	///	Math
}	///	Orbit

namespace Lucid {
namespace Orbit {

	ref class PhysicalProperties;

	///	Frame
	///
	///
	public ref class Frame
	{
	public:
		static Frame ^Wrap(::lucid::orbit::Frame *frame);

		~Frame();

		!Frame();

		property Frame ^CenterFrame { Frame ^get(); }

		property Frame ^FirstChild { Frame ^get(); }

		property Frame ^NextSibling { Frame ^get(); }

		property size_t ID { size_t get(); }

		property System::String ^Name { System::String ^get(); }

		property System::String ^Description { System::String ^get(); }

		property Math::Vector3 ^RelativePosition { Math::Vector3 ^get(); void set(Math::Vector3 ^value); }

		property Math::Vector3 ^AbsolutePosition { Math::Vector3 ^get(); }

		property ::lucid::orbit::Frame &ref { ::lucid::orbit::Frame &get() { return *_internal; } }

		property ::lucid::orbit::Frame *ptr { ::lucid::orbit::Frame *get() { return  _internal; } }

	protected:
		Frame(::lucid::orbit::Frame *frame);

	private:
		::lucid::orbit::Frame *_internal = nullptr;

	};

	///	DynamicPoint
	///
	/// 
	public ref class DynamicPoint : public Frame
	{
	public:
		DynamicPoint(::lucid::orbit::DynamicPoint *body);

		DynamicPoint(size_t id, System::String ^name, System::String ^description);

		~DynamicPoint();

		!DynamicPoint();

		property ::lucid::orbit::DynamicPoint &ref { ::lucid::orbit::DynamicPoint &get() { return *_internal; } }

		property ::lucid::orbit::DynamicPoint *ptr { ::lucid::orbit::DynamicPoint *get() { return  _internal; } }

	private:
		::lucid::orbit::DynamicPoint *_internal = nullptr;

	};

	///	OrbitalBody
	///
	/// 
	public ref class OrbitalBody : public Frame
	{
	public:
		OrbitalBody(::lucid::orbit::OrbitalBody *body);

		// OrbitalBody(...) needs a means of specifying properties

		~OrbitalBody();

		!OrbitalBody();

		property PhysicalProperties ^Properties { PhysicalProperties ^get(); }

		property ::lucid::orbit::OrbitalBody &ref { ::lucid::orbit::OrbitalBody &get() { return *_internal; } }

		property ::lucid::orbit::OrbitalBody *ptr { ::lucid::orbit::OrbitalBody *get() { return  _internal; } }

	private:
		::lucid::orbit::OrbitalBody *_internal = nullptr;

	};

	///	DynamicBody
	///
	/// 
	public ref class DynamicBody : public Frame
	{
	public:
		DynamicBody(::lucid::orbit::DynamicBody *body);

		DynamicBody(size_t id, System::String ^name, System::String ^description);

		~DynamicBody();

		!DynamicBody();

		property ::lucid::orbit::DynamicBody &ref { ::lucid::orbit::DynamicBody &get() { return *_internal; } }

		property ::lucid::orbit::DynamicBody *ptr { ::lucid::orbit::DynamicBody *get() { return  _internal; } }

	private:
		::lucid::orbit::DynamicBody *_internal = nullptr;

	};

}	///	Orbit
}	///	Lucid
