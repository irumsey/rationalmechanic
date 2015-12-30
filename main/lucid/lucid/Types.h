#pragma once

#include <lucid/core/Types.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Frustum.h>
#include <lucid/math/Algorithm.h>
#include <lucid/gal/Types.h>

#pragma push_macro("PROPERTY")
#define PROPERTY(name, type) property type name { type get() { return _internal->name; } void set(type value) { _internal->name = value; } }
 
namespace lucid {

	///	Vector2
	///
	///
	public ref class Vector2
	{
	public:
		Vector2()
		{
			_internal = new ::lucid::gal::Vector2();
		}

		Vector2(float32_t x, float32_t y)
		{
			_internal = new ::lucid::gal::Vector2(x, y);
		}

		Vector2(::lucid::gal::Vector2 const &rhs)
		{
			_internal = new ::lucid::gal::Vector2(rhs);
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

		static Vector2 ^operator *(float lhs, Vector2 ^rhs)
		{
			return gcnew Vector2(lhs * rhs->ref);
		}

		static Vector2 ^operator *(Vector2 ^lhs, float rhs)
		{
			return gcnew Vector2(lhs->ref * rhs);
		}

		static Vector2 ^operator /(Vector2 ^lhs, float rhs)
		{
			return gcnew Vector2(lhs->ref / rhs);
		}

		PROPERTY(x, float)
		PROPERTY(y, float)

		property ::lucid::gal::Vector2 const *ptr { ::lucid::gal::Vector2 const *get() { return _internal; } }

		property ::lucid::gal::Vector2 const &ref { ::lucid::gal::Vector2 const &get() { return *_internal; } }

	private:
		::lucid::gal::Vector2 *_internal = nullptr;

	};

	///	Vector3
	///
	///
	public ref class Vector3
	{
	public:
		Vector3()
		{
			_internal = new ::lucid::gal::Vector3();
		}

		Vector3(float32_t x, float32_t y, float32_t z)
		{
			_internal = new ::lucid::gal::Vector3(x, y, z);
		}

		Vector3(::lucid::gal::Vector3 const &rhs)
		{
			_internal = new ::lucid::gal::Vector3(rhs);
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

		static Vector3 ^operator *(float lhs, Vector3 ^rhs)
		{
			return gcnew Vector3(lhs * rhs->ref);
		}

		static Vector3 ^operator *(Vector3 ^lhs, float rhs)
		{
			return gcnew Vector3(lhs->ref * rhs);
		}

		static Vector3 ^operator /(Vector3 ^lhs, float rhs)
		{
			return gcnew Vector3(lhs->ref / rhs);
		}

		PROPERTY(x, float)
		PROPERTY(y, float)
		PROPERTY(z, float)

		property ::lucid::gal::Vector3 const *ptr { ::lucid::gal::Vector3 const *get() { return _internal; } }

		property ::lucid::gal::Vector3 const &ref { ::lucid::gal::Vector3 const &get() { return *_internal; } }

	private:
		::lucid::gal::Vector3 *_internal = nullptr;

	};

	///	Vector4
	///
	///
	public ref class Vector4
	{
	public:
		Vector4()
		{
			_internal = new ::lucid::gal::Vector4();
		}

		Vector4(float32_t x, float32_t y, float32_t z, float32_t w)
		{
			_internal = new ::lucid::gal::Vector4(x, y, z, w);
		}

		Vector4(::lucid::gal::Vector4 const &rhs)
		{
			_internal = new ::lucid::gal::Vector4(rhs);
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

		static Vector4 ^operator *(float lhs, Vector4 ^rhs)
		{
			return gcnew Vector4(lhs * rhs->ref);
		}

		static Vector4 ^operator *(Vector4 ^lhs, float rhs)
		{
			return gcnew Vector4(lhs->ref * rhs);
		}

		static Vector4 ^operator /(Vector4 ^lhs, float rhs)
		{
			return gcnew Vector4(lhs->ref / rhs);
		}

		PROPERTY(x, float)
		PROPERTY(y, float)
		PROPERTY(z, float)
		PROPERTY(w, float)
		
		property ::lucid::gal::Vector4 const *ptr { ::lucid::gal::Vector4 const *get() { return _internal; } }

		property ::lucid::gal::Vector4 const &ref { ::lucid::gal::Vector4 const &get() { return *_internal; } }

	private:
		::lucid::gal::Vector4 *_internal = nullptr;

	};

	///	Quaternion
	///
	///
	public ref class Quaternion
	{
	public:
		Quaternion()
		{
			_internal = new ::lucid::gal::Quaternion();
		}

		Quaternion(float32_t x, float32_t y, float32_t z, float32_t w)
		{
			_internal = new ::lucid::gal::Quaternion(x, y, z, w);
		}

		Quaternion(::lucid::gal::Quaternion const &rhs)
		{
			_internal = new ::lucid::gal::Quaternion(rhs);
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

		static Quaternion ^operator *(float lhs, Quaternion ^rhs)
		{
			return gcnew Quaternion(lhs * rhs->ref);
		}

		static Quaternion ^operator *(Quaternion ^lhs, float rhs)
		{
			return gcnew Quaternion(lhs->ref * rhs);
		}

		static Quaternion ^operator *(Quaternion ^lhs, Quaternion ^rhs)
		{
			return gcnew Quaternion(lhs->ref * rhs->ref);
		}

		static Quaternion ^operator /(Quaternion ^lhs, float rhs)
		{
			return gcnew Quaternion(lhs->ref / rhs);
		}

		PROPERTY(x, float)
		PROPERTY(y, float)
		PROPERTY(z, float)
		PROPERTY(w, float)

		property ::lucid::gal::Quaternion const *ptr { ::lucid::gal::Quaternion const *get() { return _internal; } }

		property ::lucid::gal::Quaternion const &ref { ::lucid::gal::Quaternion const &get() { return *_internal; } }

	private:
		::lucid::gal::Quaternion *_internal = nullptr;

	};

	///	Matrix2x2
	///
	///
	public ref class Matrix2x2
	{
	public:
		Matrix2x2()
		{
			_internal = new ::lucid::gal::Matrix2x2();
		}

		Matrix2x2
		(
			float32_t xx, float32_t xy,
			float32_t yx, float32_t yy
		)
		{
			_internal = new ::lucid::gal::Matrix2x2
			(
				xx, xy,
				yx, yy
			);
		}

		Matrix2x2(::lucid::gal::Matrix2x2 const &rhs)
		{
			_internal = new ::lucid::gal::Matrix2x2(rhs);
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

		static Matrix2x2 ^operator *(float lhs, Matrix2x2 ^rhs)
		{
			return gcnew Matrix2x2(lhs * rhs->ref);
		}

		static Matrix2x2 ^operator *(Matrix2x2 ^lhs, float rhs)
		{
			return gcnew Matrix2x2(lhs->ref * rhs);
		}

		static Matrix2x2 ^operator /(Matrix2x2 ^lhs, float rhs)
		{
			return gcnew Matrix2x2(lhs->ref / rhs);
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

		property ::lucid::gal::Matrix2x2 const *ptr { ::lucid::gal::Matrix2x2 const *get() { return _internal; } }

		property ::lucid::gal::Matrix2x2 const &ref { ::lucid::gal::Matrix2x2 const &get() { return *_internal; } }

	private:
		::lucid::gal::Matrix2x2 *_internal = nullptr;

	};

	///	Matrix3x3
	///
	///
	public ref class Matrix3x3
	{
	public:
		Matrix3x3()
		{
			_internal = new ::lucid::gal::Matrix3x3();
		}

		Matrix3x3
		(
			float32_t xx, float32_t xy, float32_t xz,
			float32_t yx, float32_t yy, float32_t yz,
			float32_t zx, float32_t zy, float32_t zz
		)
		{
			_internal = new ::lucid::gal::Matrix3x3
			(
				xx, xy, xz,
				yx, yy, yz,
				zx, zy, zz
			);
		}

		Matrix3x3(::lucid::gal::Matrix3x3 const &rhs)
		{
			_internal = new ::lucid::gal::Matrix3x3(rhs);
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

		static Matrix3x3 ^operator *(float lhs, Matrix3x3 ^rhs)
		{
			return gcnew Matrix3x3(lhs * rhs->ref);
		}

		static Matrix3x3 ^operator *(Matrix3x3 ^lhs, float rhs)
		{
			return gcnew Matrix3x3(lhs->ref * rhs);
		}

		static Matrix3x3 ^operator /(Matrix3x3 ^lhs, float rhs)
		{
			return gcnew Matrix3x3(lhs->ref / rhs);
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

		property ::lucid::gal::Matrix3x3 const *ptr { ::lucid::gal::Matrix3x3 const *get() { return _internal; } }

		property ::lucid::gal::Matrix3x3 const &ref { ::lucid::gal::Matrix3x3 const &get() { return *_internal; } }

	private:
		::lucid::gal::Matrix3x3 *_internal = nullptr;

	};

	///	Matrix4x4
	///
	///
	public ref class Matrix4x4
	{
	public:
		Matrix4x4()
		{
			_internal = new ::lucid::gal::Matrix4x4();
		}

		Matrix4x4
		(
			float32_t xx, float32_t xy, float32_t xz, float32_t xw,
			float32_t yx, float32_t yy, float32_t yz, float32_t yw,
			float32_t zx, float32_t zy, float32_t zz, float32_t zw,
			float32_t wx, float32_t wy, float32_t wz, float32_t ww
		)
		{
			_internal = new ::lucid::gal::Matrix4x4
			(
				xx, xy, xz, xw,
				yx, yy, yz, yw,
				zx, zy, zz, zw,
				wx, wy, wz, ww
			);
		}

		Matrix4x4(::lucid::gal::Matrix4x4 const &rhs)
		{
			_internal = new ::lucid::gal::Matrix4x4(rhs);
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

		static Matrix4x4 ^operator *(float lhs, Matrix4x4 ^rhs)
		{
			return gcnew Matrix4x4(lhs * rhs->ref);
		}

		static Matrix4x4 ^operator *(Matrix4x4 ^lhs, float rhs)
		{
			return gcnew Matrix4x4(lhs->ref * rhs);
		}

		static Matrix4x4 ^operator /(Matrix4x4 ^lhs, float rhs)
		{
			return gcnew Matrix4x4(lhs->ref / rhs);
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

		property ::lucid::gal::Matrix4x4 const *ptr { ::lucid::gal::Matrix4x4 const *get() { return _internal; } }

		property ::lucid::gal::Matrix4x4 const &ref { ::lucid::gal::Matrix4x4 const &get() { return *_internal; } }

	private:
		::lucid::gal::Matrix4x4 *_internal = nullptr;

	};

	///	Color
	///
	///
	public ref class Color
	{
	public:
		Color()
		{
			_internal = new ::lucid::gal::Color();
		}

		Color(float32_t r, float32_t g, float32_t b, float32_t a)
		{
			_internal = new ::lucid::gal::Color(r, g, b, a);
		}

		~Color()
		{
			this->!Color();
		}

		!Color()
		{
			delete _internal;
		}

		PROPERTY(r, float)
		PROPERTY(g, float)
		PROPERTY(b, float)
		PROPERTY(a, float)

		property ::lucid::gal::Color const *ptr { ::lucid::gal::Color const *get() { return _internal; } }

		property ::lucid::gal::Color const &ref { ::lucid::gal::Color const &get() { return *_internal; } }

	private:
		::lucid::gal::Color *_internal = nullptr;

	};

	///
	///
	///
	public ref class Viewport
	{
	public:
		Viewport()
		{
			_internal = new ::lucid::gal::Viewport();
		}

		Viewport(::lucid::gal::Viewport const &viewport)
		{
			_internal = new ::lucid::gal::Viewport(viewport);
		}

		Viewport(int x, int y, int width, int height, float znear, float zfar)
		{
			_internal = new ::lucid::gal::Viewport(x, y, width, height, znear, zfar);
		}

		~Viewport()
		{
			this->!Viewport();
		}

		!Viewport()
		{
			delete _internal;
		}

		PROPERTY(     x,   int)
		PROPERTY(     y,   int)
		PROPERTY( width,   int)
		PROPERTY(height,   int)
		PROPERTY( znear, float)
		PROPERTY(  zfar, float)

		property ::lucid::gal::Viewport const *ptr { ::lucid::gal::Viewport const *get() { return _internal; } }

		property ::lucid::gal::Viewport const &ref { ::lucid::gal::Viewport const &get() { return *_internal; } }

	private:
		::lucid::gal::Viewport *_internal = nullptr;

	};

	///	Sphere
	///
	///
	public ref class Sphere
	{
	public:
		Sphere()
		{
			_internal = new ::lucid::math::Sphere<float, 3>(::lucid::math::Vector<float, 3>(0, 0, 0), 0);
		}

		Sphere(Vector3 ^center, float radius)
		{
			_internal = new ::lucid::math::Sphere<float, 3>(center->ref, radius);
		}

		~Sphere()
		{
			this->!Sphere();
		}

		!Sphere()
		{
			delete _internal;
		}

		property Vector3 ^name { Vector3 ^get() { return gcnew Vector3(_internal->center); } void set(Vector3 ^value) { _internal->center = value->ref; } }
		PROPERTY(radius, float)

		property ::lucid::math::Sphere<float, 3> const *ptr { ::lucid::math::Sphere<float, 3> const *get() { return _internal; } }

		property ::lucid::math::Sphere<float, 3> const &ref { ::lucid::math::Sphere<float, 3> const &get() { return *_internal; } }

	private:
		::lucid::math::Sphere<float, 3> *_internal = nullptr;

	};

	///	AABB
	///
	///
	public ref class AABB
	{
	public:
		AABB()
		{
			_internal = new ::lucid::math::AABB<float, 3>();
		}

		AABB(Vector3 ^min, Vector3 ^max)
		{
			_internal = new ::lucid::math::AABB<float, 3>(min->ref, max->ref);
		}

		AABB(::lucid::math::AABB<float, 3> const &rhs)
		{
			_internal = new ::lucid::math::AABB<float, 3>(rhs);
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

		property ::lucid::math::AABB<float, 3> const *ptr { ::lucid::math::AABB<float, 3> const *get() { return _internal; } }

		property ::lucid::math::AABB<float, 3> const &ref { ::lucid::math::AABB<float, 3> const &get() { return *_internal; } }

	private:
		::lucid::math::AABB<float, 3> *_internal = nullptr;

	};

	///	Frustum
	///
	///
	public ref class Frustum
	{
	public:
		Frustum(Matrix4x4 ^invViewProj)
		{
			_internal = new ::lucid::math::Frustum<float>(invViewProj->ref);
		}

		~Frustum()
		{
			this->!Frustum();
		}

		!Frustum()
		{
			delete _internal;
		}

		property ::lucid::math::Frustum<float> const *ptr { ::lucid::math::Frustum<float> const *get() { return _internal; } }

		property ::lucid::math::Frustum<float> const &ref { ::lucid::math::Frustum<float> const &get() { return *_internal; } }

	private:
		::lucid::math::Frustum<float> *_internal = nullptr;

	};

}	///  lucid

#undef PROPERTY
#pragma pop_macro("PROPERTY")