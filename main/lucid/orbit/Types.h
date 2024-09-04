#pragma once

#include <lucid/core/Types.h>
#include <lucid/math/Defines.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Matrix.h>
#include <lucid/math/Quaternion.h>
#include <lucid/math/AABB.h>
#include <lucid/math/Frustum.h>
#include <lucid/gal/Types.h>
#include <lucid/orbit/Defines.h>

LUCID_ORBIT_BEGIN

/// 
///	Initial, naive, attempt at avoiding preceision errors
/// within the simulation.
/// 

typedef float64_t scalar_t;

typedef LUCID_MATH::Vector<scalar_t, 2>  vector2_t;
typedef LUCID_MATH::Vector<scalar_t, 3>  vector3_t;
typedef LUCID_MATH::Vector<scalar_t, 4>  vector4_t;

typedef LUCID_MATH::Matrix<scalar_t, 3, 3> matrix3x3_t;
typedef LUCID_MATH::Matrix<scalar_t, 4, 4> matrix4x4_t;

typedef LUCID_MATH::Quaternion<scalar_t> quaternion_t;

typedef LUCID_MATH::AABB<scalar_t, 3> aabb3_t;

typedef LUCID_MATH::Frustum<scalar_t> frustum_t;

template<typename T> struct NullSort
{
	bool operator()(T const &, T const &) const
	{
		return false;
	}
};

template<typename T> struct Back2Front
{
	bool operator()(T const &lhs, T const &rhs) const
	{
		return lhs.distance > rhs.distance;
	}
};

///
/// 
/// 

#	pragma pack(push)
#	pragma pack(4)

struct StarInstance
{
	uint32_t                   id = 0;
	LUCID_GAL::Vector4 parameters;
	LUCID_GAL::Color        color;
};

struct MeshInstance
{
	uint32_t                         id = 0;
	LUCID_GAL::Vector3         position;		//	
	float32_t                     scale = 0;	//	
	LUCID_GAL::Quaternion      rotation;
	LUCID_GAL::Vector4    lightPosition;
	LUCID_GAL::Vector4      compositing;
	LUCID_GAL::Vector4         channel0;
	LUCID_GAL::Vector4         channel1;
	LUCID_GAL::Vector4         channel2;
};

#	pragma pack(pop)

#	pragma pack(push)
#	pragma pack(2)

struct IconInstance
{
	uint32_t                 id = 0;
	LUCID_GAL::Vector4 position;
	LUCID_GAL::Vector4 texcoord;
	LUCID_GAL::Vector2    scale;
	LUCID_GAL::Color      color;
};

#	pragma pack(pop)

LUCID_ORBIT_END