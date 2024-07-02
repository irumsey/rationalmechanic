#pragma once

#include <lucid/core/Types.h>
#include <lucid/units/Defines.h>
#include <lucid/units/System.h>
#include <lucid/math/Defines.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Quaternion.h>
#include <lucid/math/Matrix.h>
#include <lucid/math/Sphere.h>
#include <lucid/math/AABB.h>
#include <lucid/math/Frustum.h>

#pragma push_macro("PROPERTY")
#define PROPERTY(name, type) property type name { type get() { return _internal->name; } void set(type value) { _internal->name = value; } }
 
namespace Lucid {
namespace Math {

	/// 
	/// avoiding the complication of templates for this managed library
	/// 

	typedef LUCID_MATH::Scalar<float32_t, LUCID_UNITS::none> scalar_t;

	typedef LUCID_MATH::Vector<float32_t, 2, LUCID_UNITS::none> vector2_t;
	typedef LUCID_MATH::Vector<float32_t, 3, LUCID_UNITS::none> vector3_t;
	typedef LUCID_MATH::Vector<float32_t, 4, LUCID_UNITS::none> vector4_t;

	typedef LUCID_MATH::Matrix<float32_t, 2, 2, LUCID_UNITS::none> matrix2x2_t;
	typedef LUCID_MATH::Matrix<float32_t, 3, 3, LUCID_UNITS::none> matrix3x3_t;
	typedef LUCID_MATH::Matrix<float32_t, 4, 4, LUCID_UNITS::none> matrix4x4_t;

	typedef LUCID_MATH::Quaternion<float32_t> quaternion_t;

	typedef LUCID_MATH::AABB<float32_t, 3, LUCID_UNITS::none> aabb3_t;
	typedef LUCID_MATH::Sphere<float32_t, 3, LUCID_UNITS::none> sphere3_t;
	typedef LUCID_MATH::Frustum<float32_t, LUCID_UNITS::system::none, LUCID_UNITS::quantity::pure> frustum_t;

	///	Scalar
	/// 
	/// 
	public ref class Scalar
	{
	public:
		Scalar()
		{
			_internal = new scalar_t();
		}

		Scalar(float32_t value)
		{
			_internal = new scalar_t(value);
		}

		Scalar(scalar_t const &rhs)
		{
			_internal = new scalar_t(rhs);
		}

		~Scalar()
		{
			this->!Scalar();
		}

		!Scalar()
		{
			delete _internal;
		}

		static Scalar ^operator -(Scalar ^rhs)
		{
			return gcnew Scalar(-rhs->ref);
		}

		static Scalar ^operator +(Scalar ^lhs, Scalar ^rhs)
		{
			return gcnew Scalar(lhs->ref + rhs->ref);
		}

		static Scalar ^operator -(Scalar ^lhs, Scalar ^rhs)
		{
			return gcnew Scalar(lhs->ref - rhs->ref);
		}

		static Scalar ^operator *(Scalar ^lhs, Scalar ^rhs)
		{
			return gcnew Scalar(lhs->ref * rhs->ref);
		}

		static Scalar ^operator /(Scalar ^lhs, Scalar ^rhs)
		{
			return gcnew Scalar(lhs->ref / rhs->ref);
		}

		PROPERTY(value, float)

		property scalar_t const *ptr { scalar_t const *get() { return _internal; } }

		property scalar_t const &ref { scalar_t const &get() { return *_internal; } }

	private:
		scalar_t *_internal = nullptr;

	};

	///	Vector2
	///
	///
	public ref class Vector2
	{
	public:
		Vector2()
		{
			_internal = new vector2_t();
		}

		Vector2(float32_t x, float32_t y)
		{
			_internal = new vector2_t(x, y);
		}

		Vector2(vector2_t const &rhs)
		{
			_internal = new vector2_t(rhs);
		}

		~Vector2()
		{
			this->!Vector2();
		}

		!Vector2()
		{
			delete _internal;
		}

		static Vector2 ^operator -(Vector2 ^rhs)
		{
			return gcnew Vector2(-rhs->ref);
		}

		static Vector2 ^operator +(Vector2 ^lhs, Vector2 ^rhs)
		{
			return gcnew Vector2(lhs->ref + rhs->ref);
		}

		static Vector2 ^operator -(Vector2 ^lhs, Vector2 ^rhs)
		{
			return gcnew Vector2(lhs->ref - rhs->ref);
		}

		static Vector2 ^operator *(Scalar ^lhs, Vector2 ^rhs)
		{
			return gcnew Vector2(lhs->ref * rhs->ref);
		}

		static Vector2 ^operator *(Vector2 ^lhs, Scalar ^rhs)
		{
			return gcnew Vector2(lhs->ref * rhs->ref);
		}

		static Vector2 ^operator /(Vector2 ^lhs, Scalar ^rhs)
		{
			return gcnew Vector2(lhs->ref / rhs->ref);
		}

		PROPERTY(x, float)
		PROPERTY(y, float)

		property vector2_t const *ptr { vector2_t const *get() { return _internal; } }

		property vector2_t const &ref { vector2_t const &get() { return *_internal; } }

	private:
		vector2_t *_internal = nullptr;

	};

	///	Vector3
	///
	///
	public ref class Vector3
	{
	public:
		Vector3()
		{
			_internal = new vector3_t();
		}

		Vector3(float32_t x, float32_t y, float32_t z)
		{
			_internal = new vector3_t(x, y, z);
		}

		Vector3(vector3_t const &rhs)
		{
			_internal = new vector3_t(rhs);
		}

		~Vector3()
		{
			this->!Vector3();
		}

		!Vector3()
		{
			delete _internal;
		}

		static Vector3 ^operator -(Vector3 ^rhs)
		{
			return gcnew Vector3(-rhs->ref);
		}

		static Vector3 ^operator +(Vector3 ^lhs, Vector3 ^rhs)
		{
			return gcnew Vector3(lhs->ref + rhs->ref);
		}

		static Vector3 ^operator -(Vector3 ^lhs, Vector3 ^rhs)
		{
			return gcnew Vector3(lhs->ref - rhs->ref);
		}

		static Vector3 ^operator *(Scalar ^lhs, Vector3 ^rhs)
		{
			return gcnew Vector3(lhs->ref * rhs->ref);
		}

		static Vector3 ^operator *(Vector3 ^lhs, Scalar ^rhs)
		{
			return gcnew Vector3(lhs->ref * rhs->ref);
		}

		static Vector3 ^operator /(Vector3 ^lhs, Scalar ^rhs)
		{
			return gcnew Vector3(lhs->ref / rhs->ref);
		}

		PROPERTY(x, float)
		PROPERTY(y, float)
		PROPERTY(z, float)

		property vector3_t const *ptr { vector3_t const *get() { return _internal; } }

		property vector3_t const &ref { vector3_t const &get() { return *_internal; } }

	private:
		vector3_t *_internal = nullptr;

	};

	///	Vector4
	///
	///
	public ref class Vector4
	{
	public:
		Vector4()
		{
			_internal = new vector4_t();
		}

		Vector4(float32_t x, float32_t y, float32_t z, float32_t w)
		{
			_internal = new vector4_t(x, y, z, w);
		}

		Vector4(vector4_t const &rhs)
		{
			_internal = new vector4_t(rhs);
		}

		~Vector4()
		{
			this->!Vector4();
		}

		!Vector4()
		{
			delete _internal;
		}

		static Vector4 ^operator -(Vector4 ^rhs)
		{
			return gcnew Vector4(-rhs->ref);
		}

		static Vector4 ^operator +(Vector4 ^lhs, Vector4 ^rhs)
		{
			return gcnew Vector4(lhs->ref + rhs->ref);
		}

		static Vector4 ^operator -(Vector4 ^lhs, Vector4 ^rhs)
		{
			return gcnew Vector4(lhs->ref - rhs->ref);
		}

		static Vector4 ^operator *(Scalar ^lhs, Vector4 ^rhs)
		{
			return gcnew Vector4(lhs->ref * rhs->ref);
		}

		static Vector4 ^operator *(Vector4 ^lhs, Scalar ^rhs)
		{
			return gcnew Vector4(lhs->ref * rhs->ref);
		}

		static Vector4 ^operator /(Vector4 ^lhs, Scalar ^rhs)
		{
			return gcnew Vector4(lhs->ref / rhs->ref);
		}

		PROPERTY(x, float)
		PROPERTY(y, float)
		PROPERTY(z, float)
		PROPERTY(w, float)

		property vector4_t const *ptr { vector4_t const *get() { return _internal; } }

		property vector4_t const &ref { vector4_t const &get() { return *_internal; } }

	private:
		vector4_t *_internal = nullptr;

	};

	///	Quaternion
	///
	///
	public ref class Quaternion
	{
	public:
		Quaternion()
		{
			_internal = new quaternion_t();
		}

		Quaternion(float32_t x, float32_t y, float32_t z, float32_t w)
		{
			_internal = new quaternion_t(x, y, z, w);
		}

		Quaternion(quaternion_t const &rhs)
		{
			_internal = new quaternion_t(rhs);
		}

		~Quaternion()
		{
			this->!Quaternion();
		}

		!Quaternion()
		{
			delete _internal;
		}

		static Quaternion ^operator -(Quaternion ^rhs)
		{
			return gcnew Quaternion(-rhs->ref);
		}

		static Quaternion ^operator +(Quaternion ^lhs, Quaternion ^rhs)
		{
			return gcnew Quaternion(lhs->ref + rhs->ref);
		}

		static Quaternion ^operator -(Quaternion ^lhs, Quaternion ^rhs)
		{
			return gcnew Quaternion(lhs->ref - rhs->ref);
		}

		static Quaternion ^operator *(Scalar ^lhs, Quaternion ^rhs)
		{
			return gcnew Quaternion(lhs->value * rhs->ref);
		}

		static Quaternion ^operator *(Quaternion ^lhs, Scalar ^rhs)
		{
			return gcnew Quaternion(lhs->ref * rhs->value);
		}

		static Quaternion ^operator *(Quaternion ^lhs, Quaternion ^rhs)
		{
			return gcnew Quaternion(lhs->ref * rhs->ref);
		}

		static Quaternion ^operator /(Quaternion ^lhs, Scalar ^rhs)
		{
			return gcnew Quaternion(lhs->ref / rhs->value);
		}

		PROPERTY(x, float)
		PROPERTY(y, float)
		PROPERTY(z, float)
		PROPERTY(w, float)

		property quaternion_t const *ptr { quaternion_t const *get() { return _internal; } }

		property quaternion_t const &ref { quaternion_t const &get() { return *_internal; } }

	private:
		quaternion_t *_internal = nullptr;

	};

	///	Matrix2x2
	///
	///
	public ref class Matrix2x2
	{
	public:
		Matrix2x2()
		{
			_internal = new matrix2x2_t();
		}

		Matrix2x2
		(
			float32_t xx, float32_t xy,
			float32_t yx, float32_t yy
		)
		{
			_internal = new matrix2x2_t
			(
				xx, xy,
				yx, yy
			);
		}

		Matrix2x2(matrix2x2_t const &rhs)
		{
			_internal = new matrix2x2_t(rhs);
		}

		~Matrix2x2()
		{
			this->!Matrix2x2();
		}

		!Matrix2x2()
		{
			delete _internal;
		}

		static Matrix2x2 ^operator -(Matrix2x2 ^rhs)
		{
			return gcnew Matrix2x2(-rhs->ref);
		}

		static Matrix2x2 ^operator +(Matrix2x2 ^lhs, Matrix2x2 ^rhs)
		{
			return gcnew Matrix2x2(lhs->ref + rhs->ref);
		}

		static Matrix2x2 ^operator -(Matrix2x2 ^lhs, Matrix2x2 ^rhs)
		{
			return gcnew Matrix2x2(lhs->ref - rhs->ref);
		}

		static Matrix2x2 ^operator *(Scalar ^lhs, Matrix2x2 ^rhs)
		{
			return gcnew Matrix2x2(lhs->ref * rhs->ref);
		}

		static Matrix2x2 ^operator *(Matrix2x2 ^lhs, Scalar ^rhs)
		{
			return gcnew Matrix2x2(lhs->ref * rhs->ref);
		}

		static Matrix2x2 ^operator /(Matrix2x2 ^lhs, Scalar ^rhs)
		{
			return gcnew Matrix2x2(lhs->ref / rhs->ref);
		}

		static Vector2 ^operator *(Vector2 ^lhs, Matrix2x2 ^rhs)
		{
			return gcnew Vector2(lhs->ref * rhs->ref);
		}

		static Vector2 ^operator *(Matrix2x2 ^lhs, Vector2 ^rhs)
		{
			return gcnew Vector2(lhs->ref * rhs->ref);
		}

		static Matrix2x2 ^operator *(Matrix2x2 ^lhs, Matrix2x2 ^rhs)
		{
			return gcnew Matrix2x2(lhs->ref * rhs->ref);
		}

		PROPERTY(xx, float) PROPERTY(xy, float)
		PROPERTY(yx, float) PROPERTY(yy, float)

		property matrix2x2_t const *ptr { matrix2x2_t const *get() { return _internal; } }

		property matrix2x2_t const &ref { matrix2x2_t const &get() { return *_internal; } }

	private:
		matrix2x2_t *_internal = nullptr;

	};

	///	Matrix3x3
	///
	///
	public ref class Matrix3x3
	{
	public:
		Matrix3x3()
		{
			_internal = new matrix3x3_t();
		}

		Matrix3x3
		(
			float32_t xx, float32_t xy, float32_t xz,
			float32_t yx, float32_t yy, float32_t yz,
			float32_t zx, float32_t zy, float32_t zz
		)
		{
			_internal = new matrix3x3_t
			(
				xx, xy, xz,
				yx, yy, yz,
				zx, zy, zz
			);
		}

		Matrix3x3(matrix3x3_t const &rhs)
		{
			_internal = new matrix3x3_t(rhs);
		}

		~Matrix3x3()
		{
			this->!Matrix3x3();
		}

		!Matrix3x3()
		{
			delete _internal;
		}

		static Matrix3x3 ^operator -(Matrix3x3 ^rhs)
		{
			return gcnew Matrix3x3(-rhs->ref);
		}

		static Matrix3x3 ^operator +(Matrix3x3 ^lhs, Matrix3x3 ^rhs)
		{
			return gcnew Matrix3x3(lhs->ref + rhs->ref);
		}

		static Matrix3x3 ^operator -(Matrix3x3 ^lhs, Matrix3x3 ^rhs)
		{
			return gcnew Matrix3x3(lhs->ref - rhs->ref);
		}

		static Matrix3x3 ^operator *(Scalar ^lhs, Matrix3x3 ^rhs)
		{
			return gcnew Matrix3x3(lhs->ref * rhs->ref);
		}

		static Matrix3x3 ^operator *(Matrix3x3 ^lhs, Scalar ^rhs)
		{
			return gcnew Matrix3x3(lhs->ref * rhs->ref);
		}

		static Matrix3x3 ^operator /(Matrix3x3 ^lhs, Scalar ^rhs)
		{
			return gcnew Matrix3x3(lhs->ref / rhs->ref);
		}

		static Vector3 ^operator *(Vector3 ^lhs, Matrix3x3 ^rhs)
		{
			return gcnew Vector3(lhs->ref * rhs->ref);
		}

		static Vector3 ^operator *(Matrix3x3 ^lhs, Vector3 ^rhs)
		{
			return gcnew Vector3(lhs->ref * rhs->ref);
		}

		static Matrix3x3 ^operator *(Matrix3x3 ^lhs, Matrix3x3 ^rhs)
		{
			return gcnew Matrix3x3(lhs->ref * rhs->ref);
		}

		PROPERTY(xx, float) PROPERTY(xy, float) PROPERTY(xz, float)
		PROPERTY(yx, float) PROPERTY(yy, float) PROPERTY(yz, float)
		PROPERTY(zx, float) PROPERTY(zy, float) PROPERTY(zz, float)

		property matrix3x3_t const *ptr { matrix3x3_t const *get() { return _internal; } }

		property matrix3x3_t const &ref { matrix3x3_t const &get() { return *_internal; } }

	private:
		matrix3x3_t *_internal = nullptr;

	};

	///	Matrix4x4
	///
	///
	public ref class Matrix4x4
	{
	public:
		Matrix4x4()
		{
			_internal = new matrix4x4_t();
		}

		Matrix4x4
		(
			float32_t xx, float32_t xy, float32_t xz, float32_t xw,
			float32_t yx, float32_t yy, float32_t yz, float32_t yw,
			float32_t zx, float32_t zy, float32_t zz, float32_t zw,
			float32_t wx, float32_t wy, float32_t wz, float32_t ww
		)
		{
			_internal = new matrix4x4_t
			(
				xx, xy, xz, xw,
				yx, yy, yz, yw,
				zx, zy, zz, zw,
				wx, wy, wz, ww
			);
		}

		Matrix4x4(matrix4x4_t const &rhs)
		{
			_internal = new matrix4x4_t(rhs);
		}

		~Matrix4x4()
		{
			this->!Matrix4x4();
		}

		!Matrix4x4()
		{
			delete _internal;
		}

		static Matrix4x4 ^operator -(Matrix4x4 ^rhs)
		{
			return gcnew Matrix4x4(-rhs->ref);
		}

		static Matrix4x4 ^operator +(Matrix4x4 ^lhs, Matrix4x4 ^rhs)
		{
			return gcnew Matrix4x4(lhs->ref + rhs->ref);
		}

		static Matrix4x4 ^operator -(Matrix4x4 ^lhs, Matrix4x4 ^rhs)
		{
			return gcnew Matrix4x4(lhs->ref - rhs->ref);
		}

		static Matrix4x4 ^operator *(Scalar ^lhs, Matrix4x4 ^rhs)
		{
			return gcnew Matrix4x4(lhs->ref * rhs->ref);
		}

		static Matrix4x4 ^operator *(Matrix4x4 ^lhs, Scalar ^rhs)
		{
			return gcnew Matrix4x4(lhs->ref * rhs->ref);
		}

		static Matrix4x4 ^operator /(Matrix4x4 ^lhs, Scalar ^rhs)
		{
			return gcnew Matrix4x4(lhs->ref / rhs->ref);
		}

		static Vector4 ^operator *(Vector4 ^lhs, Matrix4x4 ^rhs)
		{
			return gcnew Vector4(lhs->ref * rhs->ref);
		}

		static Vector4 ^operator *(Matrix4x4 ^lhs, Vector4 ^rhs)
		{
			return gcnew Vector4(lhs->ref * rhs->ref);
		}

		static Matrix4x4 ^operator *(Matrix4x4 ^lhs, Matrix4x4 ^rhs)
		{
			return gcnew Matrix4x4(lhs->ref * rhs->ref);
		}

		PROPERTY(xx, float) PROPERTY(xy, float) PROPERTY(xz, float) PROPERTY(xw, float)
		PROPERTY(yx, float) PROPERTY(yy, float) PROPERTY(yz, float) PROPERTY(yw, float)
		PROPERTY(zx, float) PROPERTY(zy, float) PROPERTY(zz, float) PROPERTY(zw, float)
		PROPERTY(wx, float) PROPERTY(wy, float) PROPERTY(wz, float) PROPERTY(ww, float)

		property matrix4x4_t const *ptr { matrix4x4_t const *get() { return _internal; } }

		property matrix4x4_t const &ref { matrix4x4_t const &get() { return *_internal; } }

	private:
		matrix4x4_t *_internal = nullptr;

	};

	///	Sphere
	///
	///
	public ref class Sphere
	{
	public:
		Sphere()
		{
			_internal = new sphere3_t(vector3_t(0, 0, 0), 0);
		}

		Sphere(Vector3 ^center, float radius)
		{
			_internal = new sphere3_t(center->ref, radius);
		}

		~Sphere()
		{
			this->!Sphere();
		}

		!Sphere()
		{
			delete _internal;
		}

		property Vector3 ^center { Vector3 ^get() { return gcnew Vector3(_internal->center); } void set(Vector3 ^value) { _internal->center = value->ref; } }
		PROPERTY(radius, float)

		property sphere3_t const *ptr { sphere3_t const *get() { return _internal; } }

		property sphere3_t const &ref { sphere3_t const &get() { return *_internal; } }

	private:
		sphere3_t *_internal = nullptr;

	};

	///	AABB
	///
	///
	public ref class AABB
	{
	public:
		AABB()
		{
			_internal = new aabb3_t();
		}

		AABB(Vector3 ^min, Vector3 ^max)
		{
			_internal = new aabb3_t(min->ref, max->ref);
		}

		AABB(aabb3_t const &rhs)
		{
			_internal = new aabb3_t(rhs);
		}

		~AABB()
		{
			this->!AABB();
		}

		!AABB()
		{
			delete _internal;
		}

		property Vector3 ^min { Vector3 ^get() { return gcnew Vector3(_internal->min); } void set(Vector3 ^value) { _internal->min = value->ref; }}

		property Vector3 ^max { Vector3 ^get() { return gcnew Vector3(_internal->max); } void set(Vector3 ^value) { _internal->max = value->ref; }}

		property aabb3_t const *ptr { aabb3_t const *get() { return _internal; } }

		property aabb3_t const &ref { aabb3_t const &get() { return *_internal; } }

	private:
		aabb3_t *_internal = nullptr;

	};

	///	Frustum
	///
	///
	public ref class Frustum
	{
	public:
		Frustum(Matrix4x4 ^invViewProj)
		{
			_internal = new frustum_t(invViewProj->ref);
		}

		~Frustum()
		{
			this->!Frustum();
		}

		!Frustum()
		{
			delete _internal;
		}

		property frustum_t const *ptr { frustum_t const *get() { return _internal; } }

		property frustum_t const &ref { frustum_t const &get() { return *_internal; } }

	private:
		frustum_t *_internal = nullptr;

	};

} /// Math
} /// Lucid

#undef PROPERTY
#pragma pop_macro("PROPERTY")
