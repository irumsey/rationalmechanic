#pragma once

#include <string>
#include <lucid/core/Noncopyable.h>
#include <lucid/orbit/Types.h>
#include <lucid/orbit/Elements.h>

///
///
///

namespace lucid {
namespace orbit {

	class Algorithm;

	///	Frame
	///
	///	a frame of reference
	///	note: position/velocity are relative to parent
	class Frame
	{
	public:
		Frame *centerFrame = nullptr;
		Frame *firstChild = nullptr;
		Frame *nextSibling = nullptr;

		size_t id = 0;
		std::string name;
		std::string description;

		vector3_t position[2];
		vector3_t velocity[2];

		virtual ~Frame();

		void addChild(Frame *child);

		void addSibling(Frame *sibling);

		virtual void apply(Algorithm *algorithm) = 0;

	protected:
		Frame(size_t id, std::string const &name, std::string const &description);

		LUCID_PREVENT_COPY(Frame);
		LUCID_PREVENT_ASSIGNMENT(Frame);
	};

	inline void Frame::addChild(Frame *child)
	{
		child->centerFrame = this;

		if (nullptr == firstChild)
			firstChild = child;
		else
			firstChild->addSibling(child);
	}

	inline void Frame::addSibling(Frame *sibling)
	{
		if (nullptr == nextSibling)
			nextSibling = sibling;
		else
			nextSibling->addSibling(sibling);
	}

	///	Dynamic point
	///
	///	Point of reference such as solar system barycenter, earth/moon barycenter, etc
	class DynamicPoint : public Frame
	{
	public:
		DynamicPoint(size_t id, std::string const &name, std::string const &description);

		virtual ~DynamicPoint();

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
		Elements elements[2];

		OrbitalBody(size_t id, std::string const &name, std::string const &description);

		virtual ~OrbitalBody();

		virtual void apply(Algorithm *algorithm) override;

		LUCID_PREVENT_COPY(OrbitalBody);
		LUCID_PREVENT_ASSIGNMENT(OrbitalBody);
	};

	///	DynamicBody
	///
	///	Body with complex orbital characteristics: long term thrust, perturbations via
	///	multiple attractors, etc.
	class DynamicBody : public Frame
	{
	public:
		DynamicBody(size_t id, std::string const &name, std::string const &description);

		virtual ~DynamicBody();

		virtual void apply(Algorithm *algorithm) override;

		LUCID_PREVENT_COPY(DynamicBody);
		LUCID_PREVENT_ASSIGNMENT(DynamicBody);
	};

}	///	orbit
}	///	lucid
