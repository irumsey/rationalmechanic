#include "Frame.h"
#include "Properties.h"
#include <lucid/managed/Math/Types.h>
#include <lucid/orbit/Ephemeris.h>
#include <lucid/orbit/Algorithm.h>
#include <lucid/orbit/Utility.h>
#include <lucid/core/Error.h>
#include <msclr/marshal_cppstd.h>

#include <lucid/managed/GIGL/Camera2D.h>

namespace /* anonymous */ {

	namespace    MI = msclr::interop;
	namespace orbit = ::lucid::orbit;

}	/// anonymous

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

	class Factory : public orbit::Algorithm
	{
	public:
		Factory() { Wrapper::wrapped = nullptr; }

		virtual ~Factory() { Wrapper::wrapped = nullptr; }

		Frame ^operator()(orbit::Frame *wrapped)
		{
			wrapped->apply(this);
			return Wrapper::wrapped;
		}

		virtual void evaluate(orbit::DynamicPoint *point) override
		{
			Wrapper::wrapped = gcnew DynamicPoint(point);
		}

		virtual void evaluate(orbit::OrbitalBody *body) override
		{
			Wrapper::wrapped = gcnew OrbitalBody(body);
		}

		virtual void evaluate(orbit::DynamicBody *body) override
		{
			Wrapper::wrapped = gcnew DynamicBody(body);
		}

		virtual void evaluate(orbit::CameraFrame *camera) override
		{
			Wrapper::wrapped = gcnew CameraFrame(camera);
		}
	};

	///
	///
	///
	
	Frame::Frame(orbit::Frame *frame)
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
		/// this owns _internal iff centerFrame is null
		if (nullptr == _internal->centerFrame)
			delete _internal;
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

	size_t Frame::ID::get()
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
		return gcnew Math::Vector3(orbit::scale(_internal->relativePosition[1]));
	}

	void Frame::RelativePosition::set(Math::Vector3 ^value)
	{
		LUCID_VALIDATE(_internal != _internal->centerFrame, "attempt to move root frame");

		/// this is a "warp"
		_internal->relativePosition[0] = orbit::inv_scale(value->ref);
		_internal->relativePosition[1] = _internal->relativePosition[0];
	}

	Math::Vector3 ^Frame::AbsolutePosition::get()
	{
		return gcnew Math::Vector3(orbit::scale(_internal->absolutePosition[1]));
	}

	Frame ^Frame::Wrap(::lucid::orbit::Frame *frame)
	{
		Factory ftor;
		return (nullptr == frame) ? nullptr : ftor(frame);
	}

	///
	///
	/// 

	DynamicPoint::DynamicPoint(orbit::DynamicPoint *body)
		: Frame(body)
		, _internal(body)
	{
	}

	DynamicPoint::DynamicPoint(size_t id, System::String ^name, System::String ^description)
		: _internal(new orbit::DynamicPoint(id, MI::marshal_as<std::string>(name), MI::marshal_as<std::string>(description)))
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

	OrbitalBody::OrbitalBody(orbit::OrbitalBody *body)
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

	PhysicalProperties ^OrbitalBody::Properties::get()
	{
		return gcnew PhysicalProperties(_internal->physicalProperties);
	}

	///
	///
	/// 

	DynamicBody::DynamicBody(orbit::DynamicBody *body)
		: Frame(body)
		, _internal(body)
	{
	}

	DynamicBody::DynamicBody(size_t id, System::String ^name, System::String ^description)
		: _internal(new orbit::DynamicBody(id, MI::marshal_as<std::string>(name), MI::marshal_as<std::string>(description)))
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

	CameraFrame::CameraFrame(orbit::CameraFrame *body)
		: Frame(body)
		, _internal(body)
	{
	}

	CameraFrame::CameraFrame(size_t id, System::String ^name, System::String ^description)
		: _internal(new orbit::CameraFrame(id, MI::marshal_as<std::string>(name), MI::marshal_as<std::string>(description)))
		, Frame(_internal)
	{
	}

	CameraFrame::~CameraFrame()
	{
		this->!CameraFrame();
	}

	CameraFrame::!CameraFrame()
	{
	}

	void CameraFrame::InitPerspective(float fov, float aspect, float znear, float zfar)
	{
		_internal->initPerspective(fov, aspect, znear, zfar);
	}

	void CameraFrame::InitOrthographic(float width, float height, float znear, float zfar)
	{
		_internal->initOrthographic(width, height, znear, zfar);
	}

	GIGL::Camera2D ^CameraFrame::Camera::get()
	{
		return gcnew GIGL::Camera2D(_internal->camera);
	}

	void CameraFrame::Camera::set(GIGL::Camera2D ^value)
	{
		_internal->camera = value->ref;
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
