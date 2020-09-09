#pragma once

#include <string>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Error.h>
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
	///	Note: the "cascade delete behaviour" has been removed.  orbit::System, or other creator, now
	///	takes complete ownership of its frames and must delete them.
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

		virtual ~Frame() = default;

		void addChild(Frame *child);

		void removeChild(Frame *child);

		virtual void apply(Algorithm *algorithm) = 0;

	protected:
		Frame(size_t id, std::string const &name, std::string const &description);

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
		PhysicalProperties physicalProperties;
		RenderProperties renderProperties;
		Elements elements[2];

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
		DynamicBody(size_t id, std::string const &name, std::string const &description);

		virtual ~DynamicBody() = default;

		virtual void apply(Algorithm *algorithm) override;

		LUCID_PREVENT_COPY(DynamicBody);
		LUCID_PREVENT_ASSIGNMENT(DynamicBody);
	};

}	///	orbit
}	///	lucid
