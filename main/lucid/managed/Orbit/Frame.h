#pragma once

#include <lucid/orbit/Defines.h>
#include <lucid/orbit/Frame.h>

LUCID_ORBIT_BEGIN

class Frame;
class DynamicPoint;
class OrbitalBody;
class DynamicBody;
class CameraFrame;

LUCID_ORBIT_END

namespace Lucid {
namespace Math {

	ref class Vector3;

}	///	Math
}	///	Orbit

namespace Lucid {
namespace GIGL {

	ref class Camera2D;

}	///	GIGL
}	///	Lucid

namespace Lucid {
namespace Orbit {

	ref class PhysicalProperties;
	ref class RenderProperties;

	///	Frame
	///
	///
	public ref class Frame
	{
	public:
		~Frame();

		!Frame();

		property Frame ^CenterFrame { Frame ^get(); }

		property Frame ^FirstChild { Frame ^get(); }

		property Frame ^NextSibling { Frame ^get(); }

		property int32_t ID { int32_t get(); }

		property System::String ^Name { System::String ^get(); }

		property System::String ^Description { System::String ^get(); }

		property Math::Vector3 ^RelativePosition { Math::Vector3 ^get(); void set(Math::Vector3 ^value); }

		property Math::Vector3 ^AbsolutePosition { Math::Vector3 ^get(); }

		property LUCID_ORBIT::Frame &ref { LUCID_ORBIT::Frame &get() { return *_internal; } }

		property LUCID_ORBIT::Frame *ptr { LUCID_ORBIT::Frame *get() { return  _internal; } }

		static Frame ^Wrap(LUCID_ORBIT::Frame *frame);

	protected:
		Frame(LUCID_ORBIT::Frame *frame);

	private:
		LUCID_ORBIT::Frame *_internal = nullptr;

	};

	///	DynamicPoint
	///
	/// 
	public ref class DynamicPoint : public Frame
	{
	public:
		DynamicPoint(LUCID_ORBIT::DynamicPoint *body);

		DynamicPoint(int32_t id, System::String ^name, System::String ^description);

		~DynamicPoint();

		!DynamicPoint();

		property LUCID_ORBIT::DynamicPoint &ref { LUCID_ORBIT::DynamicPoint &get() { return *_internal; } }

		property LUCID_ORBIT::DynamicPoint *ptr { LUCID_ORBIT::DynamicPoint *get() { return  _internal; } }

	private:
		LUCID_ORBIT::DynamicPoint *_internal = nullptr;

	};

	///	OrbitalBody
	///
	/// 
	public ref class OrbitalBody : public Frame
	{
	public:
		OrbitalBody(LUCID_ORBIT::OrbitalBody *body);

		// OrbitalBody(...) needs a means of specifying properties

		~OrbitalBody();

		!OrbitalBody();

		property PhysicalProperties ^PhysicalProps { PhysicalProperties ^get(); }

		property RenderProperties ^RenderProps { RenderProperties ^get(); }

		property LUCID_ORBIT::OrbitalBody &ref { LUCID_ORBIT::OrbitalBody &get() { return *_internal; } }

		property LUCID_ORBIT::OrbitalBody *ptr { LUCID_ORBIT::OrbitalBody *get() { return  _internal; } }

	private:
		LUCID_ORBIT::OrbitalBody *_internal = nullptr;

	};

	///	DynamicBody
	///
	/// 
	public ref class DynamicBody : public Frame
	{
	public:
		DynamicBody(LUCID_ORBIT::DynamicBody *body);

		DynamicBody(int32_t id, System::String ^name, System::String ^description);

		~DynamicBody();

		!DynamicBody();

		property LUCID_ORBIT::DynamicBody &ref { LUCID_ORBIT::DynamicBody &get() { return *_internal; } }

		property LUCID_ORBIT::DynamicBody *ptr { LUCID_ORBIT::DynamicBody *get() { return  _internal; } }

	private:
		LUCID_ORBIT::DynamicBody *_internal = nullptr;

	};

	///	CameraFrame
	///
	///	Note: no exposing the underlying camera since it would imply
	/// the ability to alter "look at" parameters.   
	public ref class CameraFrame : public Frame
	{
	public:
		CameraFrame(int32_t id, System::String ^name, System::String ^description);

		CameraFrame(LUCID_ORBIT::CameraFrame *camera);

		~CameraFrame();

		!CameraFrame();

		// the xaxis of the camera frame in world coordinates: view right
		property Math::Vector3 ^xaxis { Math::Vector3 ^get(); }

		// the yaxis of the camera frame in world coordinates: view forward
		property Math::Vector3 ^yaxis { Math::Vector3 ^get(); }

		// the zaxis of the camera frame in world coordinates: view up
		property Math::Vector3 ^zaxis { Math::Vector3 ^get(); }

		property Frame ^Focus { Frame ^get(); void set(Frame ^value); }

		property LUCID_ORBIT::CameraFrame &ref { LUCID_ORBIT::CameraFrame &get() { return *_internal; } }

		property LUCID_ORBIT::CameraFrame *ptr { LUCID_ORBIT::CameraFrame *get() { return  _internal; } }

	private:
		LUCID_ORBIT::CameraFrame *_internal = nullptr;

	};

}	///	Orbit
}	///	Lucid
