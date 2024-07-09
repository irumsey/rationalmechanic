#pragma once

#include <cassert>
#include <lucid/math/Defines.h>
#include <lucid/math/Scalar.h>
#include <lucid/math/Vector.h>

LUCID_MATH_BEGIN

///	Matrix<T, ROWS, COLS, U>
///
///	general matrix template
template<typename T, size_t ROWS, size_t COLS, typename U>
struct Matrix
{
	enum { COUNT = ROWS * COLS };

	T elements[COUNT];

	Matrix() = default;

	~Matrix() = default;

	T *operator[](size_t row)
	{
		assert(row < ROWS);
		return elements + COLS * row;
	}

	T const *operator[](size_t row) const
	{
		assert(row < ROWS);
		return elements + COLS * row;
	}
};

///	Matrix<T, 2, 2, U>
///
///	2x2 specialization of Matrix<>
///	providing named members.
template<typename T, typename U>
struct Matrix<T, 2, 2, U>
{
	enum { COUNT = 4 };

	union
	{
		T elements[COUNT];
		struct
		{
			T xx, xy;
			T yx, yy;
		};
	};

	Matrix()
		: xx(), xy()
		, yx(), yy()
	{
	}

	Matrix
	(
		T const &xx, T const &xy,
		T const &yx, T const &yy
	)
		: xx(xx), xy(xy)
		, yx(yx), yy(yy)
	{
	}

	~Matrix() = default;

	T *operator[](size_t row)
	{
		assert(row < 2);
		return elements + 2 * row;
	}

	T const *operator[](size_t row) const
	{
		assert(row < 2);
		return elements + 2 * row;
	}
};

///	Matrix<T, 3, 3, U>
///
///	3x3 specialization of Matrix<>
///	providing named members.
template<typename T, typename U>
struct Matrix<T, 3, 3, U>
{
	enum { COUNT = 9 };

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

	Matrix()
		: xx(), xy(), xz()
		, yx(), yy(), yz()
		, zx(), zy(), zz()
	{
	}

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

	~Matrix() = default;

	T *operator[](size_t row)
	{
		assert(row < 3);
		return elements + 3 * row;
	}

	T const *operator[](size_t row) const
	{
		assert(row < 3);
		return elements + 3 * row;
	}
};

///	Matrix<T, 4, 4, U>
///
///	4x4 specialization of Matrix<>
///	providing named members.
template<typename T, typename U>
struct Matrix<T, 4, 4, U>
{
	enum { COUNT = 16 };

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

	Matrix()
		: xx(), xy(), xz(), xw()
		, yx(), yy(), yz(), yw()
		, zx(), zy(), zz(), zw()
		, wx(), wy(), wz(), ww()
	{
	}

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

	~Matrix() = default;

	T *operator[](size_t row)
	{
		assert(row < 4);
		return elements + 4 * row;
	}

	T const *operator[](size_t row) const
	{
		assert(row < 4);
		return elements + 4 * row;
	}
};

LUCID_MATH_END

///
///
///
template<typename T, size_t ROWS, size_t COLS, typename S, typename Q>
inline LUCID_MATRIX(T, ROWS, COLS, S, Q) operator-(LUCID_MATRIX(T, ROWS, COLS, S, Q) const &rhs)
{
	LUCID_MATRIX(T, ROWS, COLS, S, Q) result;

	result.elements[0] = -rhs.elements[0];
	for (size_t i = 1; i < result.COUNT; ++i)
		result.elements[i] = -rhs.elements[i];

	return result;
}

///
///
///
template<typename T, size_t ROWS, size_t COLS, typename S, typename Q>
inline LUCID_MATRIX(T, ROWS, COLS, S, Q) operator+(LUCID_MATRIX(T, ROWS, COLS, S, Q) const &lhs, LUCID_MATRIX(T, ROWS, COLS, S, Q) const &rhs)
{
	LUCID_MATRIX(T, ROWS, COLS, S, Q) result;

	result.elements[0] = lhs.elements[0] + rhs.elements[0];
	for (size_t i = 1; i < result.COUNT; ++i)
		result.elements[i] = lhs.elements[i] + rhs.elements[i];

	return result;
}

///
///
///
template<typename T, size_t ROWS, size_t COLS, typename S, typename Q>
inline LUCID_MATRIX(T, ROWS, COLS, S, Q) operator-(LUCID_MATRIX(T, ROWS, COLS, S, Q) const &lhs, LUCID_MATRIX(T, ROWS, COLS, S, Q) const &rhs)
{
	LUCID_MATRIX(T, ROWS, COLS, S, Q) result;

	result.elements[0] = lhs.elements[0] - rhs.elements[0];
	for (size_t i = 1; i < result.COUNT; ++i)
		result.elements[i] = lhs.elements[i] - rhs.elements[i];

	return result;
}

///
///
///
template<typename T, size_t ROWS, size_t COLS, typename S, typename LQ, typename RQ>
inline LUCID_MATRIX(T, ROWS, COLS, S, LUCID_QUANTITY_ADD(LQ, RQ)) operator*(LUCID_MATRIX(T, ROWS, COLS, S, LQ) const &lhs, LUCID_SCALAR(T, S, RQ) const &rhs)
{
	LUCID_MATRIX(T, ROWS, COLS, S, LUCID_QUANTITY_ADD(LQ, RQ)) result;

	result.elements[0] = lhs.elements[0] * rhs;
	for (size_t i = 1; i < result.COUNT; ++i)
		result.elements[i] = lhs.elements[i] * rhs;

	return result;
}

///
///
///
template<typename T, size_t ROWS, size_t COLS, typename S, typename LQ, typename RQ>
inline LUCID_MATRIX(T, ROWS, COLS, S, LUCID_QUANTITY_ADD(LQ, RQ)) operator*(LUCID_SCALAR(T, S, LQ) const &lhs, LUCID_MATRIX(T, ROWS, COLS, S, RQ) const &rhs)
{
	LUCID_MATRIX(T, ROWS, COLS, S, LUCID_QUANTITY_ADD(LQ, RQ)) result;

	result.elements[0] = lhs * rhs.elements[0];
	for (size_t i = 1; i < result.COUNT; ++i)
		result.elements[i] = lhs * rhs.elements[i];

	return result;
}

///
///
///
template<typename T, size_t ROWS, size_t COLS, typename S, typename LQ, typename RQ>
inline LUCID_MATRIX(T, ROWS, COLS, S, LUCID_QUANTITY_SUB(LQ, RQ)) operator/(LUCID_MATRIX(T, ROWS, COLS, S, LQ) const &lhs, LUCID_SCALAR(T, S, RQ) const &rhs)
{
	LUCID_MATRIX(T, ROWS, COLS, S, LUCID_QUANTITY_SUB(LQ, RQ)) result;

	T const coeff = T(1) / rhs;

	result.elements[0] = coeff * lhs.elements[0];
	for (size_t i = 1; i < result.COUNT; ++i)
		result.elements[i] = coeff * lhs.elements[i];

	return result;
}

///
///
///	row vector times matrix: p = q * R
template<typename T, size_t ROWS, size_t COLS, typename S, typename LQ, typename RQ>
inline LUCID_VECTOR(T, COLS, S, LUCID_QUANTITY_ADD(LQ, RQ)) operator*(LUCID_VECTOR(T, ROWS, S, LQ) const &lhs, LUCID_MATRIX(T, ROWS, COLS, S, RQ) const &rhs)
{
	LUCID_VECTOR(T, COLS, S, LUCID_QUANTITY_ADD(LQ, RQ)) result;

	for (size_t col = 0; col < COLS; ++col)
	{
		result[col] = lhs[0] * rhs[0][col];
		for (size_t row = 1; row < ROWS; ++row)
		{
			result[col] = result[col] + lhs[row] * rhs[row][col];
		}
	}

	return result;
}

///
///
///	matrix times column vector: p = R * q
template<typename T, size_t ROWS, size_t COLS, typename S, typename LQ, typename RQ>
inline LUCID_VECTOR(T, ROWS, S, LUCID_QUANTITY_ADD(LQ, RQ)) operator*(LUCID_MATRIX(T, ROWS, COLS, S, LQ) const &lhs, LUCID_VECTOR(T, COLS, S, RQ) const &rhs)
{
	LUCID_VECTOR(T, ROWS, S, LUCID_QUANTITY_ADD(LQ, RQ)) result;

	for (size_t row = 0; row < ROWS; ++row)
	{
		result[row] = lhs[row][0] * rhs[0];
		for (size_t col = 1; col < COLS; ++col)
		{
			result[row] = result[row] + lhs[row][col] * rhs[col];
		}
	}

	return result;
}

///
///
///
template<typename T, size_t ROWS, size_t INNER, size_t COLS, typename S, typename LQ, typename RQ>
inline LUCID_MATRIX(T, ROWS, COLS, S, LUCID_QUANTITY_ADD(LQ, RQ)) operator*(LUCID_MATRIX(T, ROWS, INNER, S, LQ) const &lhs, LUCID_MATRIX(T, INNER, COLS, S, RQ) const &rhs)
{
	LUCID_MATRIX(T, ROWS, COLS, S, LUCID_QUANTITY_ADD(LQ, RQ)) result;

	for (size_t row = 0; row < ROWS; ++row)
	{
		for (size_t col = 0; col < COLS; ++col)
		{
			result[row][col] = lhs[row][0] * rhs[0][col];
			for (size_t inner = 1; inner < INNER; ++inner)
			{
				result[row][col] = result[row][col] + lhs[row][inner] * rhs[inner][col];
			}
		}
	}

	return result;
}

LUCID_MATH_BEGIN

///	trace
///
///	compute the trace of a square matrix
template<typename T, size_t DIM, typename S, typename Q>
inline LUCID_SCALAR(T, S, Q) trace(LUCID_MATRIX(T, DIM, DIM, S, Q) const &rhs)
{
	LUCID_SCALAR(T, S, Q) result = rhs[0][0];

	for (size_t i = 1; i < DIM; ++i)
		result = result + rhs[i][i];

	return result;
}

///	transpose
///
///	compute the transpose of a matrix
template<typename T, size_t ROWS, size_t COLS, typename S, typename Q>
inline LUCID_MATRIX(T, COLS, ROWS, S, Q) transpose(LUCID_MATRIX(T, ROWS, COLS, S, Q) const &rhs)
{
	LUCID_MATRIX(T, COLS, ROWS, S, Q) result;

	for (size_t col = 0; col < COLS; ++col)
		for (size_t row = 0; row < ROWS; ++row)
			result[col][row] = rhs[row][col];

	return result;
}

///	determinate
///
///	compute the determinate of a 2x2, 3x3, or 4x4 matrix

template<typename T, typename S, typename Q>
inline LUCID_SCALAR(T, S, LUCID_QUANTITY_ADD(Q, Q)) determinate(LUCID_MATRIX(T, 2, 2, S, Q) const &rhs)
{
	return LUCID_SCALAR(T, S, LUCID_QUANTITY_ADD(Q, Q))(rhs.xx * rhs.yy - rhs.yx * rhs.xy);
}

template<typename T, typename S, typename Q>
inline LUCID_SCALAR(T, S, LUCID_QUANTITY_ADD(Q, LUCID_QUANTITY_ADD(Q, Q))) determinate(LUCID_MATRIX(T, 3, 3, S, Q) const &rhs)
{
	return
		rhs.xx * (rhs.yy * rhs.zz - rhs.zy * rhs.yz) +
		rhs.xy * (rhs.zx * rhs.yz - rhs.yx * rhs.zz) +
		rhs.xz * (rhs.yx * rhs.zy - rhs.zx * rhs.yy);
}

template<typename T, typename S, typename Q>
inline LUCID_SCALAR(T, S, LUCID_QUANTITY_ADD(Q, LUCID_QUANTITY_ADD(Q, LUCID_QUANTITY_ADD(Q, Q)))) determinate(LUCID_MATRIX(T, 4, 4, S, Q) const &rhs)
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

template<typename T, typename S, typename Q>
inline LUCID_MATRIX(T, 2, 2, S, LUCID_QUANTITY_NEG(Q)) inverse(LUCID_MATRIX(T, 2, 2, S, Q) const &rhs)
{
	LUCID_MATRIX(T, 2, 2, S, LUCID_QUANTITY_NEG(Q)) lval;

	T const coeff = T(1) / determinate(rhs);

	lval.xx =  coeff * rhs.yy;
	lval.xy = -coeff * rhs.xy;

	lval.yx = -coeff * rhs.yx;
	lval.yy =  coeff * rhs.xx;

	return lval;
}

template<typename T, typename S, typename Q>
inline LUCID_MATRIX(T, 3, 3, S, LUCID_QUANTITY_NEG(Q)) inverse(LUCID_MATRIX(T, 3, 3, S, Q) const &rhs)
{
	LUCID_MATRIX(T, 3, 3, S, LUCID_QUANTITY_NEG(Q)) lval;

	T const coeff = T(1) / determinate(rhs);

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

template<typename T, typename S, typename Q>
inline LUCID_MATRIX(T, 4, 4, S, LUCID_QUANTITY_NEG(Q)) inverse(LUCID_MATRIX(T, 4, 4, S, Q) const &rhs)
{
	LUCID_MATRIX(T, 4, 4, S, LUCID_QUANTITY_NEG(Q)) lval;

	T const coeff = T(1) / determinate(rhs);

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
template<typename T, typename S, typename Q>
inline LUCID_MATRIX(T, 3, 3, S, LUCID_QUANTITY_PURE) rotateAboutX(LUCID_SCALAR(T, S, Q) const &theta)
{
	LUCID_MATRIX(T, 3, 3, S, LUCID_QUANTITY_PURE) lval;

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
template<typename T, typename S, typename Q>
inline LUCID_MATRIX(T, 3, 3, S, LUCID_QUANTITY_PURE) rotateAboutY(LUCID_SCALAR(T, S, Q) const &theta)
{
	LUCID_MATRIX(T, 3, 3, S, LUCID_QUANTITY_PURE) lval;

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
template<typename T, typename S, typename Q>
inline LUCID_MATRIX(T, 3, 3, S, LUCID_QUANTITY_PURE) rotateAboutZ(LUCID_SCALAR(T, S, Q) const &theta)
{
	LUCID_MATRIX(T, 3, 3, S, LUCID_QUANTITY_PURE) lval;

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

///	rotateUsingDirection (and an up vector)
///
///
template<typename T, typename S, typename Q>
inline LUCID_MATRIX(T, 3, 3, S, LUCID_QUANTITY_PURE) rotateUsingDirection(LUCID_VECTOR(T, 3, S, Q) const &direction, LUCID_VECTOR(T, 3, S, Q) const &up)
{
	LUCID_VECTOR(T, 3, S, LUCID_QUANTITY_PURE) xaxis = normalize(direction);
	LUCID_VECTOR(T, 3, S, LUCID_QUANTITY_PURE) yaxis = normalize(cross(up, xaxis));
	LUCID_VECTOR(T, 3, S, LUCID_QUANTITY_PURE) zaxis = cross(xaxis, yaxis);

	return LUCID_MATRIX(T, 3, 3, S, LUCID_QUANTITY_PURE)
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

template<typename T, typename S, typename Q>
inline LUCID_MATRIX(T, 4, 4, S, LUCID_QUANTITY_PURE) orthographic(LUCID_SCALAR(T, S, Q) const &width, LUCID_SCALAR(T, S, Q) const &height, LUCID_SCALAR(T, S, Q) const &znear, LUCID_SCALAR(T, S, Q) const &zfar)
{
	T const w = width;
	T const h = height;
	T const z = znear;
	T const depth = znear - zfar;

	return LUCID_MATRIX(T, 4, 4, S, LUCID_QUANTITY_PURE)
	(
		T(2) / w,        0,            0,         0,
		       0, T(2) / h,            0,         0,
		       0,        0, T(1) / depth, z / depth,
		       0,        0,            0,         1
	);
}

template<typename T, typename S, typename Q>
inline LUCID_MATRIX(T, 4, 4, S, LUCID_QUANTITY_PURE) orthographic(LUCID_SCALAR(T, S, Q) const &left, LUCID_SCALAR(T, S, Q) const &right, LUCID_SCALAR(T, S, Q) const &bottom, LUCID_SCALAR(T, S, Q) const &top, LUCID_SCALAR(T, S, Q) const &znear, LUCID_SCALAR(T, S, Q) const &zfar)
{
	return LUCID_MATRIX(T, 4, 4, S, LUCID_QUANTITY_PURE)
	(
		T(2) / (right - left),                     0,                     0, (left + right) / (left - right),
	                        0, T(2) / (top - bottom),                     0, (top + bottom) / (bottom - top),
	                        0,                     0, T(1) / (znear - zfar),          znear / (znear - zfar),
	                        0,                     0,                     0,                               1
);
}

///	perspective
///
///	compute a perspective projection matrix
///
///	note: assumes column vector, p = P * q

template<typename T, typename S, typename Q>
inline LUCID_MATRIX(T, 4, 4, S, LUCID_QUANTITY_PURE) perspective(LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE) const &fov, LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE) const &aspect, LUCID_SCALAR(T, S, Q) const &znear, LUCID_SCALAR(T, S, Q) const &zfar)
{
	T const yscale = T(1) / tan(LUCID_SCALAR(T, S, Q)(0.5f) * fov);
	T const xscale = yscale / aspect;

	return LUCID_MATRIX(T, 4, 4, S, LUCID_QUANTITY_PURE)
	(
		xscale,      0,                     0,                             0,
             0, yscale,                     0,                             0,
             0,      0, zfar / (znear - zfar), znear * zfar / (znear - zfar),
             0,      0,                    -1,                             0
	);
}

template<typename T, typename S, typename Q>
inline LUCID_MATRIX(T, 4, 4, S, LUCID_QUANTITY_PURE) perspective(LUCID_SCALAR(T, S, Q) const &left, LUCID_SCALAR(T, S, Q) const &right, LUCID_SCALAR(T, S, Q) const &bottom, LUCID_SCALAR(T, S, Q) const &top, LUCID_SCALAR(T, S, Q) const &znear, LUCID_SCALAR(T, S, Q) const &zfar)
{
	return LUCID_MATRIX(T, 4, 4, S, LUCID_QUANTITY_PURE)
	(
		T(2) * znear / (right - left),                             0, (left + right) / (right - left),                             0,
		                            0, T(2) * znear / (top - bottom), (top + bottom) / (top - bottom),                             0,
		                            0,                             0,           zfar / (znear - zfar), znear * zfar / (znear - zfar),
		                            0,                             0,                              -1,                             0
	);
}

///	look
///
///	compute a "look at" transformation matrix
///
///	note: assumes column vector, p = V * q
template<typename T, typename S, typename Q>
inline LUCID_MATRIX(T, 4, 4, S, LUCID_QUANTITY_PURE) look(LUCID_VECTOR(T, 3, S, Q) const &eye, LUCID_VECTOR(T, 3, S, Q) const &focus, LUCID_VECTOR(T, 3, S, Q) const &up)
{
	LUCID_VECTOR(T, 3, S, LUCID_QUANTITY_PURE) const zaxis = normalize(eye - focus);
	LUCID_VECTOR(T, 3, S, LUCID_QUANTITY_PURE) const xaxis = normalize(cross(up, zaxis));
	LUCID_VECTOR(T, 3, S, LUCID_QUANTITY_PURE) const yaxis = cross(zaxis, xaxis);

	return LUCID_MATRIX(T, 4, 4, S, LUCID_QUANTITY_PURE)
	(
		xaxis.x, xaxis.y, xaxis.z, -dot(xaxis, eye),
		yaxis.x, yaxis.y, yaxis.z, -dot(yaxis, eye),
		zaxis.x, zaxis.y, zaxis.z, -dot(zaxis, eye),
		   T(0),    T(0),    T(0),             T(1)
	);
}

template<typename T, typename S, typename Q>
inline LUCID_VECTOR(T, 3, S, LUCID_QUANTITY_PURE) extractViewForward(LUCID_MATRIX(T, 4, 4, S, Q) const &viewMatrix)
{
	return LUCID_VECTOR(T, 3, S, LUCID_QUANTITY_PURE)(-viewMatrix.zx, -viewMatrix.zy, -viewMatrix.zz);
}

template<typename T, typename S, typename Q>
inline LUCID_VECTOR(T, 3, S, LUCID_QUANTITY_PURE) extractViewRight(LUCID_MATRIX(T, 4, 4, S, Q) const &viewMatrix)
{
	return LUCID_VECTOR(T, 3, S, LUCID_QUANTITY_PURE)(viewMatrix.xx, viewMatrix.xy, viewMatrix.xz);
}

template<typename T, typename S, typename Q>
inline LUCID_VECTOR(T, 3, S, LUCID_QUANTITY_PURE) extractViewUp(LUCID_MATRIX(T, 4, 4, S, Q) const &viewMatrix)
{
	return LUCID_VECTOR(T, 3, S, LUCID_QUANTITY_PURE)(viewMatrix.yx, viewMatrix.yy, viewMatrix.yz);
}

///
///
///
template<typename T, typename S, typename Q>
inline LUCID_VECTOR(T, 3, S, Q) transform(LUCID_MATRIX(T, 4, 4, S, LUCID_QUANTITY_PURE) const &lhs, LUCID_VECTOR(T, 3, S, Q) const &rhs)
{
	LUCID_VECTOR(T, 4, S, Q) result = lhs * LUCID_VECTOR(T, 4, S, Q)(rhs.x, rhs.y, rhs.z, T(1));
	return LUCID_VECTOR(T, 3, S, Q)(result.x, result.y, result.z) / LUCID_SCALAR(T, S, Q)(result.w);
}

LUCID_MATH_END
