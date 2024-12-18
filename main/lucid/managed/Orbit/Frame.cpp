#include "Frame.h"
#include "Properties.h"
#include <lucid/managed/Math/Types.h>
#include <lucid/orbit/Ephemeris.h>
#include <lucid/orbit/Algorithm.h>
#include <lucid/orbit/Utility.h>
#include <lucid/core/Error.h>
#include <msclr/marshal_cppstd.h>

#include <lucid/managed/GIGL/Camera2D.h>

LUCID_ANONYMOUS_BEGIN

namespace MI = msclr::interop;

LUCID_ANONYMOUS_END

namespace Lucid {
namespace Orbit {

	///	the c++ side doesn't use "switch on type" techniques because
	/// i hate switch on type code (even when i write it).
	///
	/// managed c++ does not allow a non-managed type to hold a handle
	/// to a managed type or a managed type to inherit from non-managed.
	/// therefore, the following (slightly ugly) wrapping of the frame
	/// hierarchy.  the factory is clean but the Wrapper is required
	/// because of the above reasons.
	
	/// TBD: determine how "safe" this is and improve if ness/req.
	private ref class Wrapper { public: static Frame ^wrapped = nullptr;  };

	class Factory : public LUCID_ORBIT::Algorithm
	{
	public:
		Factory() { Wrapper::wrapped = nullptr; }

		virtual ~Factory() { Wrapper::wrapped = nullptr; }

		Frame ^operator()(LUCID_ORBIT::Frame *wrapped)
		{
			wrapped->apply(this);
			return Wrapper::wrapped;
		}

		virtual void evaluate(LUCID_ORBIT::DynamicPoint *point) override
		{
			Wrapper::wrapped = gcnew DynamicPoint(point);
		}

		virtual void evaluate(LUCID_ORBIT::OrbitalBody *body) override
		{
			Wrapper::wrapped = gcnew OrbitalBody(body);
		}

		virtual void evaluate(LUCID_ORBIT::DynamicBody *body) override
		{
			Wrapper::wrapped = gcnew DynamicBody(body);
		}

		virtual void evaluate(LUCID_ORBIT::CameraFrame *camera) override
		{
			Wrapper::wrapped = gcnew CameraFrame(camera);
		}
	};

	///
	///
	///
	
	Frame::Frame(LUCID_ORBIT::Frame *frame)
		: _internal(frame)
	{
		LUCID_VALIDATE(nullptr != frame, "internal consistancy error");
	}

	Frame::~Frame()
	{
		this->!Frame();
	}

	Frame::!Frame()
	{
#if false
		/// this owns _internal iff centerFrame is null
		if (nullptr == _internal->centerFrame)
			delete _internal;
#endif
	}

	Frame ^Frame::CenterFrame::get()
	{
		return Wrap(_internal->centerFrame);
	}

	Frame ^Frame::FirstChild::get()
	{
		return Wrap(_internal->firstChild);
	}

	Frame ^Frame::NextSibling::get()
	{
		return Wrap(_internal->nextSibling);
	}

	int32_t Frame::ID::get()
	{
		return _internal->id;
	}

	System::String ^Frame::Name::get()
	{
		return MI::marshal_as<System::String ^>(_internal->name);
	}

	System::String ^Frame::Description::get()
	{
		return MI::marshal_as<System::String ^>(_internal->description);
	}

	Math::Vector3 ^Frame::RelativePosition::get()
	{
		return gcnew Math::Vector3(LUCID_ORBIT::cast(_internal->relativePosition[1]));
	}

	void Frame::RelativePosition::set(Math::Vector3 ^value)
	{
		LUCID_VALIDATE(_internal != _internal->centerFrame, "attempt to move root frame");

		_internal->relativePosition[1] = LUCID_ORBIT::cast(value->ref);
	}

	Math::Vector3 ^Frame::AbsolutePosition::get()
	{
		return gcnew Math::Vector3(LUCID_ORBIT::cast(_internal->absolutePosition[1]));
	}

	Frame ^Frame::Wrap(LUCID_ORBIT::Frame *frame)
	{
		Factory ftor;
		return (nullptr == frame) ? nullptr : ftor(frame);
	}

	///
	///
	/// 

	DynamicPoint::DynamicPoint(LUCID_ORBIT::DynamicPoint *body)
		: Frame(body)
		, _internal(body)
	{
	}

	DynamicPoint::DynamicPoint(int32_t id, System::String ^name, System::String ^description)
		: _internal(new LUCID_ORBIT::DynamicPoint(id, MI::marshal_as<std::string>(name), MI::marshal_as<std::string>(description)))
		, Frame(_internal)
	{
	}

	DynamicPoint::~DynamicPoint()
	{
		this->!DynamicPoint();
	}

	DynamicPoint::!DynamicPoint()
	{
	}

	///
	///
	/// 

	OrbitalBody::OrbitalBody(LUCID_ORBIT::OrbitalBody *body)
		: Frame(body)
		, _internal(body)
	{
	}

	OrbitalBody::~OrbitalBody()
	{
		this->!OrbitalBody();
	}

	OrbitalBody::!OrbitalBody()
	{
	}

	PhysicalProperties ^OrbitalBody::PhysicalProps::get()
	{
		return gcnew PhysicalProperties(&(_internal->physicalProperties));
	}

	RenderProperties ^OrbitalBody::RenderProps::get()
	{
		return gcnew RenderProperties(&(_internal->renderProperties));
	}

	///
	///
	/// 

	DynamicBody::DynamicBody(LUCID_ORBIT::DynamicBody *body)
		: Frame(body)
		, _internal(body)
	{
	}

	DynamicBody::DynamicBody(int32_t id, System::String ^name, System::String ^description)
		: _internal(new LUCID_ORBIT::DynamicBody(id, MI::marshal_as<std::string>(name), MI::marshal_as<std::string>(description)))
		, Frame(_internal)
	{
	}

	DynamicBody::~DynamicBody()
	{
		this->!DynamicBody();
	}

	DynamicBody::!DynamicBody()
	{
	}

	///
	///
	/// 

	CameraFrame::CameraFrame(int32_t id, System::String ^name, System::String ^description)
		: _internal(new LUCID_ORBIT::CameraFrame(id, MI::marshal_as<std::string>(name), MI::marshal_as<std::string>(description)))
		, Frame(_internal)
	{
	}

	CameraFrame::CameraFrame(LUCID_ORBIT::CameraFrame *body)
		: Frame(body)
		, _internal(body)
	{
	}

	CameraFrame::~CameraFrame()
	{
		this->!CameraFrame();
	}

	CameraFrame::!CameraFrame()
	{
	}

	Math::Vector3 ^CameraFrame::xaxis::get()
	{
		LUCID_ORBIT::vector3_t y = LUCID_MATH::normalize(_internal->focus->absolutePosition[1] - _internal->absolutePosition[1]);
		LUCID_ORBIT::vector3_t z(0, 0, 1);
		LUCID_ORBIT::vector3_t x = LUCID_MATH::cross(y, z);

		return gcnew Math::Vector3(float32_t(x.x), float32_t(x.y), float32_t(x.z));
	}

	Math::Vector3 ^CameraFrame::yaxis::get()
	{
		LUCID_ORBIT::vector3_t y = LUCID_MATH::normalize(_internal->focus->absolutePosition[1] - _internal->absolutePosition[1]);

		return gcnew Math::Vector3(float32_t(y.x), float32_t(y.y), float32_t(y.z));
	}

	Math::Vector3 ^CameraFrame::zaxis::get()
	{
		LUCID_ORBIT::vector3_t y = LUCID_MATH::normalize(_internal->focus->absolutePosition[1] - _internal->absolutePosition[1]);
		LUCID_ORBIT::vector3_t z(0, 0, 1);
		LUCID_ORBIT::vector3_t x = LUCID_MATH::cross(y, z);

		z = LUCID_MATH::cross(x, y);

		return gcnew Math::Vector3(float32_t(z.x), float32_t(z.y), float32_t(z.z));
	}

	Frame ^CameraFrame::Focus::get()
	{
		return Wrap(_internal->focus);
	}

	void CameraFrame::Focus::set(Frame ^frame)
	{
		_internal->focus = frame->ptr;
	}

}	///	Orbit
}	///	Lucid
