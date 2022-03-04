#pragma once

#include <string>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Error.h>
#include <lucid/gigl/Camera2D.h>
#include <lucid/orbit/Types.h>
#include <lucid/orbit/Properties.h>
#include <lucid/orbit/Elements.h>

///
///
///

namespace lucid {
namespace orbit {

	class Algorithm;

	///	Frame
	///
	///	A frame of reference.
	/// 
	///	Note: cascading delete is used.  a frame will delete its children causing those
	/// children to delete their children, etc.
	/// 
	/// Note: ownership rules
	///
	///		0)	a parent frame always owns its children and will delete them when
	///			it is deleted.
	///		1)	if the parent, centerFrame, is null then the frame is detached from
	///			a hierarchy and ownership passes to whatever "detached" the frame.
	///		2)	if the parent is not-null and is equal to itself, it is a root
	///			frame and the owner is the one which made it a root frame.
	///		3)	if the parent is not-null and not equal to itself, it is a child frame
	///			and the owner is its parent.
	///  
	class Frame
	{
	public:
		Frame *centerFrame = nullptr;

		Frame *firstChild = nullptr;
		Frame *nextSibling = nullptr;

		size_t id = 0;
		std::string name;
		std::string description;

		vector3_t relativePosition[2];
		vector3_t relativeVelocity[2];

		vector3_t absolutePosition[2];

		virtual ~Frame();

		void addChild(Frame *child);

		void removeChild(Frame *child);

		virtual void apply(Algorithm *algorithm) = 0;

	protected:
		Frame(size_t id, std::string const &name, std::string const &description);

	private:
		/// TBD: remove this eventually.  right now, just paranoid about the new ownership rules.
		static size_t _instances;

		LUCID_PREVENT_COPY(Frame);
		LUCID_PREVENT_ASSIGNMENT(Frame);
	};

	inline void Frame::addChild(Frame *child)
	{
		LUCID_VALIDATE(nullptr == child->centerFrame, "internal frame consistency error");
		LUCID_VALIDATE(nullptr == child->nextSibling, "internal frame consistency error");

		child->centerFrame = this;

		child->nextSibling = firstChild;
		firstChild = child;
	}

	inline void Frame::removeChild(Frame *child)
	{
		LUCID_VALIDATE(this == child->centerFrame, "internal frame consistency error");
		child->centerFrame = nullptr;

		if (child == firstChild)
		{
			firstChild = child->nextSibling;
		}
		else
		{
			Frame *scan = firstChild;
			while (child != scan->nextSibling) scan = scan->nextSibling;
			LUCID_VALIDATE(child == scan->nextSibling, "frame was not a parent to specified child");

			scan->nextSibling = child->nextSibling;
		}

		child->nextSibling = nullptr;
	}

	///	Dynamic point
	///
	///	General point of reference such as solar system barycenter, earth/moon barycenter, etc.
	class DynamicPoint : public Frame
	{
	public:
		/// TBD: data...

		DynamicPoint(size_t id, std::string const &name, std::string const &description);

		virtual ~DynamicPoint() = default;

		virtual void apply(Algorithm *algorithm) override;

		LUCID_PREVENT_COPY(DynamicPoint);
		LUCID_PREVENT_ASSIGNMENT(DynamicPoint);
	};

	///	OrbitalBody
	///
	///	Body with a "simple" orbit (ie one of the conic sections)
	///	note: not a distinction between natural vs manmade.
	class OrbitalBody : public Frame
	{
	public:
		Elements elements[2];	/// used during simulation (changes over time)

		PhysicalProperties physicalProperties;
		RenderProperties renderProperties;

		OrbitalBody(size_t id, std::string const &name, std::string const &description);

		virtual ~OrbitalBody() = default;

		virtual void apply(Algorithm *algorithm) override;

		LUCID_PREVENT_COPY(OrbitalBody);
		LUCID_PREVENT_ASSIGNMENT(OrbitalBody);
	};

	///	DynamicBody
	///
	///	Body with complex orbital characteristics: long term thrust, perturbations via
	///	multiple attractors, firing thrusters all over the place, etc.
	class DynamicBody : public Frame
	{
	public:
		/// TBD: data...

		DynamicBody(size_t id, std::string const &name, std::string const &description);

		virtual ~DynamicBody() = default;

		virtual void apply(Algorithm *algorithm) override;

		LUCID_PREVENT_COPY(DynamicBody);
		LUCID_PREVENT_ASSIGNMENT(DynamicBody);
	};

	///
	///
	///
	class CameraFrame : public Frame
	{
	public:
		Frame *focus = nullptr;
		gigl::Camera2D camera;

		CameraFrame(size_t id, std::string const &name, std::string const &description);

		virtual ~CameraFrame() = default;

		virtual void apply(Algorithm *algorithm) override;

		void initPerspective(float fov, float aspect, float znear, float zfar);

		void initOrthographic(float width, float height, float znear, float zfar);

		void look(Frame *frame);

		LUCID_PREVENT_COPY(CameraFrame);
		LUCID_PREVENT_ASSIGNMENT(CameraFrame);
	};

}	///	orbit
}	///	lucid
