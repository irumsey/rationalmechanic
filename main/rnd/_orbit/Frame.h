#pragma once

#include <string>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Error.h>
#include <rnd/_orbit/Defines.h>
#include <rnd/_orbit/Types.h>
#include <rnd/_orbit/Properties.h>
#include <rnd/_orbit/Elements.h>

LUCID_CORE_BEGIN

class Writer;
class Reader;

LUCID_CORE_END

ORBIT_BEGIN

class Algorithm;

///	Frame
///
///	A frame of reference.
/// 
///	Note: cascading delete is used.  a frame will delete its children causing those
/// children to delete their children, etc.
/// 
/// Note: position and rotation
/// 
///		0)	absolute positions and velocites are wrt BECS (Barycentric Ecliptic Coordinate System)
///		1)	relative positions and velocites are wrt the parent's frame
///		2)	rotations are wrt BECS
/// 
/// Note: ownership rules
///
///		0)	a parent frame always owns its children and will delete them when
///			it is deleted.
///		1)	if the parent, centerFrame, is null then the frame is detached from
///			a hierarchy and ownership passes to whatever "detached" it.
///		2)	if the parent is not-null and is equal to itself, it is a root
///			frame and the owner is the one which made it a root frame.
///		3)	if the parent is not-null and not equal to itself, it is a child frame
///			and the owner is its parent (as per rule 0).
///  
class Frame
{
public:
	///	TYPE
	/// 
	/// 
	enum TYPE
	{
		TYPE_UNDEFINED		= 0,
		TYPE_DYNAMIC_POINT	= 1,
		TYPE_ORBITAL_BODY,
		TYPE_DYNAMIC_BODY,
		TYPE_CAMERA,
	};
	enum { TYPE_COUNT = TYPE_CAMERA + 1 };

	///	SIM_STATE
	/// 
	/// 
	enum SIM_STATE
	{
		SIM_STATE_IDLE = 0,			// not started yet
		SIM_STATE_ERROR,			//
		SIM_STATE_STABLE,			//
	};

	/// CULL_STATE
	/// 
	/// 
	enum CULL_STATE
	{
		CULL_STATE_PRUNED = 0,		// whole branch, this frame and children, not in view volume
		CULL_STATE_CULLED,			// just this, the parent frame, not in view volume
		CULL_STATE_IMPERCEPTIBLE,	// too small to render
		CULL_STATE_VISIBLE,			// frame is visible
	};

	TYPE const type = TYPE_UNDEFINED;
	std::string name;
	std::string description;

	Frame *centerFrame = nullptr;	// parent frame

	Frame *firstChild = nullptr;
	Frame *nextSibling = nullptr;

	SIM_STATE simState = SIM_STATE_IDLE;
	CULL_STATE cullState = CULL_STATE_PRUNED;

	vector3_t relativePosition[2];
	vector3_t relativeVelocity[2];

	vector3_t absolutePosition[2];
	vector3_t absoluteVelocity[2];

	quaternion_t rotation[2];

	aabb3_t aabbSelf[2];
	aabb3_t aabbTotal[2];

	virtual ~Frame();

	void addChild(Frame *child);

	void removeChild(Frame *child);

	virtual void apply(Algorithm *algorithm) = 0;

	virtual void write(LUCID_CORE::Writer &writer) const;

	virtual void read(LUCID_CORE::Reader &reader);

protected:
	Frame(TYPE type, std::string const &name, std::string const &description);

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
///	General point of reference such as a barycenter, etc.
class DynamicPoint : public Frame
{
public:
	RenderProperties renderProperties;

	DynamicPoint(std::string const &name, std::string const &description);

	virtual ~DynamicPoint() = default;

	virtual void apply(Algorithm *algorithm) override;

	virtual void write(LUCID_CORE::Writer &writer) const override;

	virtual void read(LUCID_CORE::Reader &reader) override;

	LUCID_PREVENT_COPY(DynamicPoint);
	LUCID_PREVENT_ASSIGNMENT(DynamicPoint);
};

///	OrbitalBody
///
///	Body with an orbit which can be specified using orbital elements.
/// 
///	note: not a distinction between natural vs manmade.
class OrbitalBody : public Frame
{
public:
	PhysicalProperties physicalProperties;
	RenderProperties renderProperties;

	RotationalElements rotationalElements;
	OrbitalElements orbitalElements[2];

	OrbitalBody(std::string const &name, std::string const &description);

	virtual ~OrbitalBody() = default;

	virtual void apply(Algorithm *algorithm) override;

	virtual void write(LUCID_CORE::Writer &writer) const override;

	virtual void read(LUCID_CORE::Reader &reader) override;

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
	RenderProperties renderProperties;

	DynamicBody(std::string const &name, std::string const &description);

	virtual ~DynamicBody() = default;

	virtual void apply(Algorithm *algorithm) override;

	virtual void write(LUCID_CORE::Writer &writer) const override;

	virtual void read(LUCID_CORE::Reader &reader) override;

	LUCID_PREVENT_COPY(DynamicBody);
	LUCID_PREVENT_ASSIGNMENT(DynamicBody);
};

///	Camera
///
///	
class Camera : public Frame
{
public:
	RenderProperties renderProperties;

	Frame *focus = nullptr;

	Camera(std::string const &name, std::string const &description);

	virtual ~Camera() = default;

	virtual void apply(Algorithm *algorithm) override;

	virtual void write(LUCID_CORE::Writer &writer) const override;

	virtual void read(LUCID_CORE::Reader &reader) override;

	void look(Frame *frame);

	LUCID_PREVENT_COPY(Camera);
	LUCID_PREVENT_ASSIGNMENT(Camera);
};

ORBIT_END