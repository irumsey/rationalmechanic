#pragma once

#include <lucid/math/Math.h>
#include <lucid/managed/Math/Types.h>

namespace Lucid {
namespace Math {

	///	Util (placeholder name)
	///
	///	utility class for mathematical operations
	public ref class Util
	{
	public:

		///
		///
		///

		static float dot(Vector2 ^lhs, Vector2 ^rhs)
		{
			return LUCID_MATH::dot(lhs->ref, rhs->ref);
		}

		static float lsq(Vector2 ^rhs)
		{
			return LUCID_MATH::lsq(rhs->ref);
		}

		static float len(Vector2 ^rhs)
		{
			return LUCID_MATH::len(rhs->ref);
		}

		static Vector2 ^normalize(Vector2 ^rhs)
		{
			return gcnew Vector2(LUCID_MATH::normalize(rhs->ref));
		}

		///
		///
		///

		static float dot(Vector3 ^lhs, Vector3 ^rhs)
		{
			return LUCID_MATH::dot(lhs->ref, rhs->ref);
		}

		static float lsq(Vector3 ^rhs)
		{
			return LUCID_MATH::lsq(rhs->ref);
		}

		static float len(Vector3 ^rhs)
		{
			return LUCID_MATH::len(rhs->ref);
		}

		static Vector3 ^normalize(Vector3 ^rhs)
		{
			return gcnew Vector3(LUCID_MATH::normalize(rhs->ref));
		}

		static Vector3 ^cross(Vector3 ^lhs, Vector3 ^rhs)
		{
			return gcnew Vector3(LUCID_MATH::cross(lhs->ref, rhs->ref));
		}

		///
		///
		///

		static float dot(Vector4 ^lhs, Vector4 ^rhs)
		{
			return LUCID_MATH::dot(lhs->ref, rhs->ref);
		}

		static float lsq(Vector4 ^rhs)
		{
			return LUCID_MATH::lsq(rhs->ref);
		}

		static float len(Vector4 ^rhs)
		{
			return LUCID_MATH::len(rhs->ref);
		}

		static Vector4 ^normalize(Vector4 ^rhs)
		{
			return gcnew Vector4(LUCID_MATH::normalize(rhs->ref));
		}

		///
		///
		///

		static float dot(Quaternion ^lhs, Quaternion ^rhs)
		{
			return LUCID_MATH::dot(lhs->ref, rhs->ref);
		}

		static float lsq(Quaternion ^rhs)
		{
			return LUCID_MATH::lsq(rhs->ref);
		}

		static float len(Quaternion ^rhs)
		{
			return LUCID_MATH::len(rhs->ref);
		}

		static Quaternion ^normalize(Quaternion ^rhs)
		{
			return gcnew Quaternion(LUCID_MATH::normalize(rhs->ref));
		}

		static Quaternion ^conjugate(Quaternion ^rhs)
		{
			return gcnew Quaternion(LUCID_MATH::conjugate(rhs->ref));
		}

		static Vector3 ^transformDirection(Quaternion ^lhs, Vector3 ^rhs)
		{
			return gcnew Vector3(LUCID_MATH::transformDirection(lhs->ref, rhs->ref));
		}

		///
		///
		///

		static float trace(Matrix2x2 ^rhs)
		{
			return LUCID_MATH::trace(rhs->ref);
		}

		static Matrix2x2 ^transpose(Matrix2x2 ^rhs)
		{
			return gcnew Matrix2x2(LUCID_MATH::transpose(rhs->ref));
		}

		static float determinate(Matrix2x2 ^rhs)
		{
			return LUCID_MATH::determinate(rhs->ref);
		}

		static Matrix2x2 ^inverse(Matrix2x2 ^rhs)
		{
			return gcnew Matrix2x2(LUCID_MATH::inverse(rhs->ref));
		}

		///
		///
		///

		static float trace(Matrix3x3 ^rhs)
		{
			return LUCID_MATH::trace(rhs->ref);
		}

		static Matrix3x3 ^transpose(Matrix3x3 ^rhs)
		{
			return gcnew Matrix3x3(LUCID_MATH::transpose(rhs->ref));
		}

		static float determinate(Matrix3x3 ^rhs)
		{
			return LUCID_MATH::determinate(rhs->ref);
		}

		static Matrix3x3 ^inverse(Matrix3x3 ^rhs)
		{
			return gcnew Matrix3x3(LUCID_MATH::inverse(rhs->ref));
		}

		static Matrix3x3 ^rotateAboutX(float32_t theta)
		{
			return gcnew Matrix3x3(LUCID_MATH::rotateAboutX(theta));
		}

		static Matrix3x3 ^rotateAboutY(float32_t theta)
		{
			return gcnew Matrix3x3(LUCID_MATH::rotateAboutY(theta));
		}

		static Matrix3x3 ^rotateAboutZ(float32_t theta)
		{
			return gcnew Matrix3x3(LUCID_MATH::rotateAboutZ(theta));
		}

		///
		///
		/// 
		
		static Quaternion^ rotateUsingAxis(Vector3^ axis, float32_t theta)
		{
			return gcnew Quaternion(LUCID_MATH::rotateUsingAxis(axis->ref, theta));
		}

		static Matrix3x3 ^matrixFromQuaternion(Quaternion ^rhs)
		{
			return gcnew Matrix3x3(LUCID_MATH::matrixFromQuaternion(quaternion_t(rhs->ref)));
		}

		///
		///
		///

		static float trace(Matrix4x4 ^rhs)
		{
			return LUCID_MATH::trace(rhs->ref);
		}

		static Matrix4x4 ^transpose(Matrix4x4 ^rhs)
		{
			return gcnew Matrix4x4(LUCID_MATH::transpose(rhs->ref));
		}

		static float determinate(Matrix4x4 ^rhs)
		{
			return LUCID_MATH::determinate(rhs->ref);
		}

		static Matrix4x4 ^inverse(Matrix4x4 ^rhs)
		{
			return gcnew Matrix4x4(LUCID_MATH::inverse(rhs->ref));
		}

		static Matrix4x4 ^orthographic(float32_t width, float32_t height, float32_t znear, float32_t zfar)
		{
			return gcnew Matrix4x4(LUCID_MATH::orthographic(width, height, znear, zfar));
		}

		static Matrix4x4 ^perspective(float32_t fov, float32_t aspect, float32_t znear, float32_t zfar)
		{
			return gcnew Matrix4x4(LUCID_MATH::perspective(fov, aspect, znear, zfar));
		}

		static Matrix4x4 ^look(Vector3 ^position, Vector3 ^target, Vector3 ^up)
		{
			return gcnew Matrix4x4(LUCID_MATH::look(position->ref, target->ref, up->ref));
		}

		///
		///
		///

		static float32_t areaProjected(Matrix4x4 ^viewProjMatrix, float32_t znear, AABB ^aabb)
		{
			return LUCID_MATH::areaProjected(viewProjMatrix->ref, znear, aabb->ref);
		}

		///
		///
		///

		static bool intersects(Sphere ^sphere, AABB ^aabb)
		{
			return LUCID_MATH::intersects(sphere->ref, aabb->ref);
		}

		static bool intersects(Frustum ^frustum, AABB ^aabb)
		{
			return LUCID_MATH::intersects(frustum->ref, aabb->ref);
		}

		static bool contains(Frustum ^frustum, AABB ^aabb)
		{
			return LUCID_MATH::contains(frustum->ref, aabb->ref);
		}

	private:
		Util() {}

		~Util() {}

		!Util() {}

	};

} /// Math
} /// Lucid
