#pragma once

#include <string>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Error.h>
#include <lucid/gal/Types.h>
#include <lucid/orbit/Defines.h>
#include <lucid/orbit/Constants.h>
#include <lucid/orbit/Types.h>
#include <lucid/orbit/Properties.h>
#include <lucid/orbit/Elements.h>

LUCID_ORBIT_BEGIN

class Algorithm;

///	Frame
///
///	A frame of reference.
/// 
///	Note: cascading delete is used.  a frame will delete its children causing those
/// children to delete their children, etc.
/// 
/// Note: position and rotation
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
	///
	/// 
	/// 
	enum SIM_STATE
	{
		SIM_STATE_IDLE = 0,			// basically, not started yet
		SIM_STATE_ERROR,			// the most likely issue, JDN out of bounds.
		SIM_STATE_STABLE,			//
	};

	/// 
	/// 
	/// 
	enum CULL_STATE
	{
		CULL_STATE_PRUNED = 0,		// whole branch, this frame and children, not in view volume
		CULL_STATE_CULLED,			// just this, the parent frame, not in view volume
		CULL_STATE_IMPERCEPTIBLE,	// too small to render
		CULL_STATE_VISIBLE,			// frame is visible
	};

	Frame *centerFrame = nullptr;	// parent frame

	Frame *firstChild = nullptr;
	Frame *nextSibling = nullptr;

	SIM_STATE simState = SIM_STATE_IDLE;
	CULL_STATE cullState = CULL_STATE_PRUNED;

	int32_t id = 0;
	std::string name;
	std::string description;

	vector3_t relativePosition[2];
	vector3_t relativeVelocity[2];

	vector3_t absolutePosition[2];
	vector3_t absoluteVelocity[2];

	matrix3x3_t rotation[2];

	aabb3_t aabbSelf[2];
	aabb3_t aabbTotal[2];

	virtual ~Frame();

	void addChild(Frame *child);

	void removeChild(Frame *child);

	virtual void apply(Algorithm *algorithm) = 0;

protected:
	Frame(int32_t id, std::string const &name, std::string const &description);

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
	/// TBD: data...

	DynamicPoint(int32_t id, std::string const &name, std::string const &description);

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

	RotationalElements rotationalElements;
	OrbitalElements orbitalElements[2];			/// can change over time

	OrbitalBody(int32_t id, std::string const &name, std::string const &description);

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

	DynamicBody(int32_t id, std::string const &name, std::string const &description);

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
	scalar_t fov = 0.25 * constants::pi;
	Frame *focus = nullptr;

	CameraFrame(int32_t id, std::string const &name, std::string const &description);

	virtual ~CameraFrame() = default;

	virtual void apply(Algorithm *algorithm) override;

	void look(Frame *frame);

	LUCID_PREVENT_COPY(CameraFrame);
	LUCID_PREVENT_ASSIGNMENT(CameraFrame);
};

LUCID_ORBIT_END