#pragma once

#include <cassert>
#include <lucid/math/Constants.h>
#include <lucid/math/Scalar.h>
#include <lucid/math/Vector.h>

///
///
///
namespace lucid {
namespace math {

	///	Matrix<T,ROWS,COLS>
	///
	///	general matrix template
	template<class T, int ROWS, int COLS> struct Matrix
	{
		static int const COUNT = ROWS * COLS;

		T elements[COUNT];

		Matrix() = default;

		T *operator[](int32_t row)
		{
			assert(row < ROWS);
			return elements + COLS * row;
		}

		T const *operator[](int32_t row) const
		{
			assert(row < ROWS);
			return elements + COLS * row;
		}
	};

	///	Matrix<T,2,2>
	///
	///	2x2 specialization of Matrix<>
	///	providing named members.
	template<class T> struct Matrix<T,2,2>
	{
		static int const COUNT = 4;

		union
		{
			T elements[COUNT];
			struct
			{
				T xx, xy;
				T yx, yy;
			};
		};

		Matrix() = default;

		Matrix
		(
			T const &xx, T const &xy,
			T const &yx, T const &yy
		)
			: xx(xx), xy(xy)
			, yx(yx), yy(yy)
		{
		}

		T *operator[](int32_t row)
		{
			assert(row < 2);
			return elements + 2 * row;
		}

		T const *operator[](int32_t row) const
		{
			assert(row < 2);
			return elements + 2 * row;
		}
	};

	///	Matrix<T,3,3>
	///
	///	3x3 specialization of Matrix<>
	///	providing named members.
	template<class T> struct Matrix<T,3,3>
	{
		static int const COUNT = 9;

		union
		{
			T elements[COUNT];
			struct
			{
				T xx, xy, xz;
				T yx, yy, yz;
				T zx, zy, zz;
			};
		};

		Matrix() = default;

		Matrix
		(
			T const &xx, T const &xy, T const &xz,
			T const &yx, T const &yy, T const &yz,
			T const &zx, T const &zy, T const &zz
		)
			: xx(xx), xy(xy), xz(xz)
			, yx(yx), yy(yy), yz(yz)
			, zx(zx), zy(zy), zz(zz)
		{
		}

		T *operator[](int32_t row)
		{
			assert(row < 3);
			return elements + 3 * row;
		}

		T const *operator[](int32_t row) const
		{
			assert(row < 3);
			return elements + 3 * row;
		}
	};

	///	Matrix<T,4,4>
	///
	///	4x4 specialization of Matrix<>
	///	providing named members.
	template<class T> struct Matrix<T,4,4>
	{
		static int const COUNT = 16;

		union
		{
			T elements[COUNT];
			struct
			{
				T xx, xy, xz, xw;
				T yx, yy, yz, yw;
				T zx, zy, zz, zw;
				T wx, wy, wz, ww;
			};
		};

		Matrix() = default;

		Matrix
		(
			T const &xx, T const &xy, T const &xz, T const &xw,
			T const &yx, T const &yy, T const &yz, T const &yw,
			T const &zx, T const &zy, T const &zz, T const &zw,
			T const &wx, T const &wy, T const &wz, T const &ww
		)
			: xx(xx), xy(xy), xz(xz), xw(xw)
			, yx(yx), yy(yy), yz(yz), yw(yw)
			, zx(zx), zy(zy), zz(zz), zw(zw)
			, wx(wx), wy(wy), wz(wz), ww(ww)
		{
		}

		T *operator[](int32_t row)
		{
			assert(row < 4);
			return elements + 4 * row;
		}

		T const *operator[](int32_t row) const
		{
			assert(row < 4);
			return elements + 4 * row;
		}
	};

}	///	math
}	///	lucid

///	vector and matrix shorthand for the following operator overloads
///
///	note: relies upon push/pop macro pragmas

#pragma push_macro("VECTOR")
#define VECTOR(T,DIM) lucid::math::Vector<T,DIM>

#pragma push_macro("MATRIX")
#define MATRIX(T,ROWS,COLS) lucid::math::Matrix<T,ROWS,COLS>

///
///
///
template<class T, int ROWS, int COLS> inline MATRIX(T,ROWS,COLS) operator-(MATRIX(T,ROWS,COLS) const &rhs)
{
	MATRIX(T,ROWS,COLS) result;

	result.elements[0] = -rhs.elements[0];
	for (int32_t i = 1; i < result.COUNT; ++i)
	{
		result.elements[i] = -rhs.elements[i];
	}

	return result;
}

///
///
///
template<class T, int ROWS, int COLS> inline MATRIX(T,ROWS,COLS) operator+(MATRIX(T,ROWS,COLS) const &lhs, MATRIX(T,ROWS,COLS) const &rhs)
{
	MATRIX(T,ROWS,COLS) result;

	result.elements[0] = lhs.elements[0] + rhs.elements[0];
	for (int32_t i = 1; i < result.COUNT; ++i)
	{
		result.elements[i] = lhs.elements[i] + rhs.elements[i];
	}

	return result;
}

///
///
///
template<class T, int ROWS, int COLS> inline MATRIX(T,ROWS,COLS) operator-(MATRIX(T,ROWS,COLS) const &lhs, MATRIX(T,ROWS,COLS) const &rhs)
{
	MATRIX(T,ROWS,COLS) result;

	result.elements[0] = lhs.elements[0] - rhs.elements[0];
	for (int32_t i = 1; i < result.COUNT; ++i)
	{
		result.elements[i] = lhs.elements[i] - rhs.elements[i];
	}

	return result;
}

///
///
///
template<class T, int ROWS, int COLS> inline MATRIX(T,ROWS,COLS) operator*(MATRIX(T,ROWS,COLS) const &lhs, T const &rhs)
{
	MATRIX(T,ROWS,COLS) result;

	result.elements[0] = lhs.elements[0] * rhs;
	for (int32_t i = 1; i < result.COUNT; ++i)
	{
		result.elements[i] = lhs.elements[i] * rhs;
	}

	return result;
}

///
///
///
template<class T, int ROWS, int COLS> inline MATRIX(T,ROWS,COLS) operator*(T const &lhs, MATRIX(T,ROWS,COLS) const &rhs)
{
	MATRIX(T,ROWS,COLS) result;

	result.elements[0] = lhs * rhs.elements[0];
	for (int32_t i = 1; i < result.COUNT; ++i)
	{
		result.elements[i] = lhs * rhs.elements[i];
	}

	return result;
}

///
///
///
template<class T, int ROWS, int COLS> inline MATRIX(T,ROWS,COLS) operator/(MATRIX(T,ROWS,COLS) const &lhs, T const &rhs)
{
	MATRIX(T,ROWS,COLS) result;

	T const coeff = ::lucid::math::constants::one<T>() / rhs;

	result.elements[0] = coeff * lhs.elements[0];
	for (int32_t i = 1; i < result.COUNT; ++i)
	{
		result.elements[i] = coeff * lhs.elements[i];
	}

	return result;
}

///
///
///	row vector times matrix: p = q * R
template<class T, int ROWS, int COLS> inline VECTOR(T,COLS) operator*(VECTOR(T,ROWS) const &lhs, MATRIX(T,ROWS,COLS) const &rhs)
{
	VECTOR(T,COLS) result;

	for (int32_t col = 0; col < COLS; ++col)
	{
		result[col] = lhs[0] * rhs[0][col];
		for (int32_t row = 1; row < ROWS; ++row)
		{
			result[col] += lhs[row] * rhs[row][col];
		}
	}

	return result;
}

///
///
///	matrix times column vector: p = R * q
template<class T, int ROWS, int COLS> inline VECTOR(T,ROWS) operator*(MATRIX(T,ROWS,COLS) const &lhs, VECTOR(T,COLS) const &rhs)
{
	VECTOR(T,ROWS) result;

	for (int32_t row = 0; row < ROWS; ++row)
	{
		result[row] = lhs[row][0] * rhs[0];
		for (int32_t col = 1; col < COLS; ++col)
		{
			result[row] += lhs[row][col] * rhs[col];
		}
	}

	return result;
}

///
///
///
template<class T, int ROWS, int INNER, int COLS> inline MATRIX(T,ROWS,COLS) operator*(MATRIX(T,ROWS,INNER) const &lhs, MATRIX(T,INNER,COLS) const &rhs)
{
	MATRIX(T,ROWS,COLS) result;

	for (int32_t row = 0; row < ROWS; ++row)
	{
		for (int32_t col = 0; col < COLS; ++col)
		{
			result[row][col] = lhs[row][0] * rhs[0][col];
			for (int32_t inner = 1; inner < INNER; ++inner)
			{
				result[row][col] += lhs[row][inner] * rhs[inner][col];
			}
		}
	}

	return result;
}

///
///	remove the shorthand
///

#undef MATRIX
#pragma pop_macro("MATRIX")

#undef VECTOR
#pragma pop_macro("VECTOR")

///
///
///
namespace lucid {
namespace math {

	///	trace
	///
	///	compute the trace of a square matrix
	template<class T, int DIM> inline T trace(Matrix<T,DIM,DIM> const &rhs)
	{
		T result = rhs[0][0];

		for (int32_t i = 1; i < DIM; ++i)
		{
			result += rhs[i][i];
		}

		return result;
	}

	///	equ and neq
	///
	///	equality tests

	template<class T, int ROWS, int COLS> inline bool equ(Matrix<T,ROWS,COLS> const &lhs, Matrix<T,ROWS,COLS> const &rhs)
	{
		for (int32_t i = 0; i < rhs.COUNT; ++i)
		{
			if (neq(lhs.elements[i], rhs.elements[i]))
			{
				return false;
			}
		}
		return true;
	}

	template<class T, int ROWS, int COLS> inline bool neq(Matrix<T,ROWS,COLS> const &lhs, Matrix<T,ROWS,COLS> const &rhs)
	{
		return !equ(lhs, rhs);
	}

	///	transpose
	///
	///	compute the transpose of a matrix
	template<class T, int ROWS, int COLS> inline Matrix<T,COLS,ROWS> transpose(Matrix<T,ROWS,COLS> const &rhs)
	{
		Matrix<T,COLS,ROWS> result;

		for (int32_t col = 0; col < COLS; ++col)
		{
			for (int32_t row = 0; row < ROWS; ++row)
			{
				result[col][row] = rhs[row][col];
			}
		}

		return result;
	}

	///	determinate
	///
	///	compute the determinate of a 2x2, 3x3, or 4x4 matrix

	template<class T> inline T determinate(Matrix<T,2,2> const &rhs)
	{
		return rhs.xx * rhs.yy - rhs.yx * rhs.xy;
	}

	template<class T> inline T determinate(Matrix<T,3,3> const &rhs)
	{
		return
			rhs.xx * (rhs.yy * rhs.zz - rhs.zy * rhs.yz) +
			rhs.xy * (rhs.zx * rhs.yz - rhs.yx * rhs.zz) +
			rhs.xz * (rhs.yx * rhs.zy - rhs.zx * rhs.yy);
	}

	template<class T> inline T determinate(Matrix<T,4,4> const &rhs)
	{
		return
			rhs.xw * rhs.yz * rhs.zy * rhs.wx - rhs.xz * rhs.yw * rhs.zy * rhs.wx - rhs.xw * rhs.yy * rhs.zz * rhs.wx + rhs.xy * rhs.yw * rhs.zz * rhs.wx +
			rhs.xz * rhs.yy * rhs.zw * rhs.wx - rhs.xy * rhs.yz * rhs.zw * rhs.wx - rhs.xw * rhs.yz * rhs.zx * rhs.wy + rhs.xz * rhs.yw * rhs.zx * rhs.wy +
			rhs.xw * rhs.yx * rhs.zz * rhs.wy - rhs.xx * rhs.yw * rhs.zz * rhs.wy - rhs.xz * rhs.yx * rhs.zw * rhs.wy + rhs.xx * rhs.yz * rhs.zw * rhs.wy +
			rhs.xw * rhs.yy * rhs.zx * rhs.wz - rhs.xy * rhs.yw * rhs.zx * rhs.wz - rhs.xw * rhs.yx * rhs.zy * rhs.wz + rhs.xx * rhs.yw * rhs.zy * rhs.wz +
			rhs.xy * rhs.yx * rhs.zw * rhs.wz - rhs.xx * rhs.yy * rhs.zw * rhs.wz - rhs.xz * rhs.yy * rhs.zx * rhs.ww + rhs.xy * rhs.yz * rhs.zx * rhs.ww +
			rhs.xz * rhs.yx * rhs.zy * rhs.ww - rhs.xx * rhs.yz * rhs.zy * rhs.ww - rhs.xy * rhs.yx * rhs.zz * rhs.ww + rhs.xx * rhs.yy * rhs.zz * rhs.ww;
	}

	///	inverse
	///
	///	compute the inverse of a 2x2, 3x3, or 4x4 matrix

	template<class T> inline Matrix<T,2,2> inverse(Matrix<T,2,2> const &rhs)
	{
		Matrix<T,2,2> lval;

		T coeff = constants::one<T>() / determinate(rhs);

		lval.xx =  coeff * rhs.yy;
		lval.xy = -coeff * rhs.xy;

		lval.yx = -coeff * rhs.yx;
		lval.yy =  coeff * rhs.xx;

		return lval;
	}

	template<class T> inline Matrix<T,3,3> inverse(Matrix<T,3,3> const &rhs)
	{
		Matrix<T,3,3> lval;

		T coeff = constants::one<T>() / determinate(rhs);

		lval.xx = coeff * (rhs.zz * rhs.yy - rhs.yz * rhs.zy);
		lval.xy = coeff * (rhs.xz * rhs.zy - rhs.zz * rhs.xy);
		lval.xz = coeff * (rhs.xy * rhs.yz - rhs.xz * rhs.yy);

		lval.yx = coeff * (rhs.yz * rhs.zx - rhs.zz * rhs.yx);
		lval.yy = coeff * (rhs.zz * rhs.xx - rhs.xz * rhs.zx);
		lval.yz = coeff * (rhs.xz * rhs.yx - rhs.xx * rhs.yz);

		lval.zx = coeff * (rhs.yx * rhs.zy - rhs.yy * rhs.zx);
		lval.zy = coeff * (rhs.xy * rhs.zx - rhs.xx * rhs.zy);
		lval.zz = coeff * (rhs.xx * rhs.yy - rhs.xy * rhs.yx);

		return lval;
	}

	template<class T> inline Matrix<T,4,4> inverse(Matrix<T,4,4> const &rhs)
	{
		Matrix<T,4,4> lval;

		T coeff = constants::one<T>() / determinate(rhs);

		lval.xx = coeff * (rhs.yz * rhs.zw * rhs.wy - rhs.yw * rhs.zz * rhs.wy + rhs.yw * rhs.zy * rhs.wz - rhs.yy * rhs.zw * rhs.wz - rhs.yz * rhs.zy * rhs.ww + rhs.yy * rhs.zz * rhs.ww);
		lval.xy = coeff * (rhs.xw * rhs.zz * rhs.wy - rhs.xz * rhs.zw * rhs.wy - rhs.xw * rhs.zy * rhs.wz + rhs.xy * rhs.zw * rhs.wz + rhs.xz * rhs.zy * rhs.ww - rhs.xy * rhs.zz * rhs.ww);
		lval.xz = coeff * (rhs.xz * rhs.yw * rhs.wy - rhs.xw * rhs.yz * rhs.wy + rhs.xw * rhs.yy * rhs.wz - rhs.xy * rhs.yw * rhs.wz - rhs.xz * rhs.yy * rhs.ww + rhs.xy * rhs.yz * rhs.ww);
		lval.xw = coeff * (rhs.xw * rhs.yz * rhs.zy - rhs.xz * rhs.yw * rhs.zy - rhs.xw * rhs.yy * rhs.zz + rhs.xy * rhs.yw * rhs.zz + rhs.xz * rhs.yy * rhs.zw - rhs.xy * rhs.yz * rhs.zw);

		lval.yx = coeff * (rhs.yw * rhs.zz * rhs.wx - rhs.yz * rhs.zw * rhs.wx - rhs.yw * rhs.zx * rhs.wz + rhs.yx * rhs.zw * rhs.wz + rhs.yz * rhs.zx * rhs.ww - rhs.yx * rhs.zz * rhs.ww);
		lval.yy = coeff * (rhs.xz * rhs.zw * rhs.wx - rhs.xw * rhs.zz * rhs.wx + rhs.xw * rhs.zx * rhs.wz - rhs.xx * rhs.zw * rhs.wz - rhs.xz * rhs.zx * rhs.ww + rhs.xx * rhs.zz * rhs.ww);
		lval.yz = coeff * (rhs.xw * rhs.yz * rhs.wx - rhs.xz * rhs.yw * rhs.wx - rhs.xw * rhs.yx * rhs.wz + rhs.xx * rhs.yw * rhs.wz + rhs.xz * rhs.yx * rhs.ww - rhs.xx * rhs.yz * rhs.ww);
		lval.yw = coeff * (rhs.xz * rhs.yw * rhs.zx - rhs.xw * rhs.yz * rhs.zx + rhs.xw * rhs.yx * rhs.zz - rhs.xx * rhs.yw * rhs.zz - rhs.xz * rhs.yx * rhs.zw + rhs.xx * rhs.yz * rhs.zw);

		lval.zx = coeff * (rhs.yy * rhs.zw * rhs.wx - rhs.yw * rhs.zy * rhs.wx + rhs.yw * rhs.zx * rhs.wy - rhs.yx * rhs.zw * rhs.wy - rhs.yy * rhs.zx * rhs.ww + rhs.yx * rhs.zy * rhs.ww);
		lval.zy = coeff * (rhs.xw * rhs.zy * rhs.wx - rhs.xy * rhs.zw * rhs.wx - rhs.xw * rhs.zx * rhs.wy + rhs.xx * rhs.zw * rhs.wy + rhs.xy * rhs.zx * rhs.ww - rhs.xx * rhs.zy * rhs.ww);
		lval.zz = coeff * (rhs.xy * rhs.yw * rhs.wx - rhs.xw * rhs.yy * rhs.wx + rhs.xw * rhs.yx * rhs.wy - rhs.xx * rhs.yw * rhs.wy - rhs.xy * rhs.yx * rhs.ww + rhs.xx * rhs.yy * rhs.ww);
		lval.zw = coeff * (rhs.xw * rhs.yy * rhs.zx - rhs.xy * rhs.yw * rhs.zx - rhs.xw * rhs.yx * rhs.zy + rhs.xx * rhs.yw * rhs.zy + rhs.xy * rhs.yx * rhs.zw - rhs.xx * rhs.yy * rhs.zw);

		lval.wx = coeff * (rhs.yz * rhs.zy * rhs.wx - rhs.yy * rhs.zz * rhs.wx - rhs.yz * rhs.zx * rhs.wy + rhs.yx * rhs.zz * rhs.wy + rhs.yy * rhs.zx * rhs.wz - rhs.yx * rhs.zy * rhs.wz);
		lval.wy = coeff * (rhs.xy * rhs.zz * rhs.wx - rhs.xz * rhs.zy * rhs.wx + rhs.xz * rhs.zx * rhs.wy - rhs.xx * rhs.zz * rhs.wy - rhs.xy * rhs.zx * rhs.wz + rhs.xx * rhs.zy * rhs.wz);
		lval.wz = coeff * (rhs.xz * rhs.yy * rhs.wx - rhs.xy * rhs.yz * rhs.wx - rhs.xz * rhs.yx * rhs.wy + rhs.xx * rhs.yz * rhs.wy + rhs.xy * rhs.yx * rhs.wz - rhs.xx * rhs.yy * rhs.wz);
		lval.ww = coeff * (rhs.xy * rhs.yz * rhs.zx - rhs.xz * rhs.yy * rhs.zx + rhs.xz * rhs.yx * rhs.zy - rhs.xx * rhs.yz * rhs.zy - rhs.xy * rhs.yx * rhs.zz + rhs.xx * rhs.yy * rhs.zz);

		return lval;
	}

	///	rotateAboutX
	///
	///	compute a rotation matrix about the x axis
	///
	///	note: counter clockwise rotation assuming column vector, p = R * q
	template<class T> inline Matrix<T,3,3> rotateAboutX(T theta)
	{
		Matrix<T,3,3> lval;

		T c = cos(theta);
		T s = sin(theta);

		lval.xx =  1;
		lval.xy =  0;
		lval.xz =  0;

		lval.yx =  0;
		lval.yy =  c;
		lval.yz = -s;

		lval.zx =  0;
		lval.zy =  s;
		lval.zz =  c;

		return lval;
	}

	///	rotateAboutY
	///
	///	compute a rotation matrix about the y axis
	///
	///	note: counter clockwise rotation assuming column vector, p = R * q
	template<class T> inline Matrix<T,3,3> rotateAboutY(T theta)
	{
		Matrix<T,3,3> lval;

		T c = cos(theta);
		T s = sin(theta);

		lval.xx =  c;
		lval.xy =  0;
		lval.xz =  s;

		lval.yx =  0;
		lval.yy =  1;
		lval.yz =  0;

		lval.zx = -s;
		lval.zy =  0;
		lval.zz =  c;

		return lval;
	}

	///	rotateAboutZ
	///
	///	compute a rotation matrix about the z axis
	///
	///	note: counter clockwise rotation assuming column vector, p = R * q
	template<class T> inline Matrix<T,3,3> rotateAboutZ(T theta)
	{
		Matrix<T,3,3> lval;

		T c = cos(theta);
		T s = sin(theta);

		lval.xx =  c;
		lval.xy = -s;
		lval.xz =  0;

		lval.yx =  s;
		lval.yy =  c;
		lval.yz =  0;

		lval.zx =  0;
		lval.zy =  0;
		lval.zz =  1;

		return lval;
	}

	///	rotateUsingDirection (and up vector)
	///
	///
	template<class T> inline Matrix<T,3,3> rotateUsingDirection(Vector<T,3> const &direction, Vector<T,3> const &up)
	{
		Vector<T,3> xaxis = normalize(direction);
		Vector<T,3> yaxis = normalize(cross(up, xaxis));
		Vector<T,3> zaxis = cross(xaxis, yaxis);

		return Matrix<T,3,3>
		(
			xaxis.x, xaxis.y, xaxis.z,
			yaxis.x, yaxis.y, yaxis.z,
			zaxis.x, zaxis.y, zaxis.z
		);
	}

	///	orthographic
	///
	///	compute an orthographic projection matrix
	///
	///	note: assumes column vector, p = P * q

	template<class T> inline Matrix<T,4,4> orthographic(T width, T height, T znear, T zfar)
	{
		return Matrix<T,4,4>
		(
			2 / width,          0,                  0,                      0,
		            0, 2 / height,                  0,                      0,
		            0,          0, 1 / (znear - zfar), znear / (znear - zfar),
		            0,          0,                  0,                      1
		);
	}

	template<class T> inline Matrix<T,4,4> orthographic(T left, T right, T bottom, T top, T znear, T zfar)
	{
		return Matrix<T,4,4>
		(
			2 / (right - left),                  0,                  0, (left + right) / (left - right),
			                 0, 2 / (top - bottom),                  0, (top + bottom) / (bottom - top),
			                 0,                  0, 1 / (znear - zfar),          znear / (znear - zfar),
			                 0,                  0,                  0,                               1
		);
	}

	///	perspective
	///
	///	compute a perspective projection matrix
	///
	///	note: assumes column vector, p = P * q

	template<class T> inline Matrix<T,4,4> perspective(T fov, T aspect, T znear, T zfar)
	{
		T yscale = constants::one<T>() / tan(constants::half<T>() * fov);
		T xscale = yscale / aspect;

		return Matrix<T,4,4>
		(
			xscale,      0,                     0,                             0,
			     0, yscale,                     0,                             0,
			     0,      0, zfar / (znear - zfar), znear * zfar / (znear - zfar),
			     0,      0,                    -1,                             0
		);
	}

	template<class T> inline Matrix<T,4,4> perspective(T left, T right, T bottom, T top, T znear, T zfar)
	{
		return Matrix<T,4,4>
		(
			2 * znear / (right - left),                          0, (left + right) / (right - left),                             0,
			                         0, 2 * znear / (top - bottom), (top + bottom) / (top - bottom),                             0,
			                         0,                          0,           zfar / (znear - zfar), znear * zfar / (znear - zfar),
			                         0,                          0,                              -1,                             0
		);
	}

	///	look
	///
	///	compute a "look at" transformation matrix
	///
	///	note: assumes column vector, p = V * q
	template<class T> inline Matrix<T,4,4> look(Vector<T,3> const &eye, Vector<T,3> const &focus, Vector<T,3> const &up)
	{
		Vector<T,3> zaxis = normalize(eye - focus);
		Vector<T,3> xaxis = normalize(cross(up, zaxis));
		Vector<T,3> yaxis = cross(zaxis, xaxis);

		return Matrix<T,4,4>
		(
			xaxis.x, xaxis.y, xaxis.z, -dot(xaxis, eye),
			yaxis.x, yaxis.y, yaxis.z, -dot(yaxis, eye),
			zaxis.x, zaxis.y, zaxis.z, -dot(zaxis, eye),
			    0.f,     0.f,     0.f,              1.f
		);
	}

	///
	///
	///
	template<class T> inline Vector<T, 3> transform(Matrix<T, 4, 4> const &lhs, Vector<T, 3> const &rhs)
	{
		Vector<T, 4> result = lhs * Vector<T, 4>(rhs.x, rhs.y, rhs.z, constants::one<T>());
		return Vector<T, 3>(result.x, result.y, result.z) / result.w;
	}

}	///	math
}	///	lucid