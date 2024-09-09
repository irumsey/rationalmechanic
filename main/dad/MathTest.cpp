#include "MathTest.h"
#include "Utility.h"
#include <lucid/gal/Pipeline.h>
#include <lucid/gal/Types.h>
#include <lucid/math/Matrix.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Fixed.h>
#include <lucid/math/Integer.h>
#include <lucid/math/Defines.h>
#include <lucid/math/Algorithm.h>
#include <lucid/core/Profiler.h>
#include <lucid/core/Numbers.h>
#include <lucid/core/Defines.h>

///
///
///

LUCID_ANONYMOUS_BEGIN

typedef LUCID_MATH::Integer<256> int256_t;
typedef LUCID_MATH::Fixed<256, 16> fixed256_t;
typedef LUCID_MATH::Vector<float32_t, 3> vector3_t;

template<typename T> inline bool validate(std::string const &test, T const &value, T const &target)
{
	return false;
}

template<> inline bool validate<vector3_t>(std::string const &test, vector3_t const &value, vector3_t const &target)
{
	return false;
}

LUCID_ANONYMOUS_END

///
///
///

void MathTest::begin(float64_t t)
{
	LUCID_CORE::log("INFO", "starting math test...");
}

bool MathTest::update(float64_t t, float64_t dt)
{
	_passed = true;

	float32_t const theta = 37.3f * LUCID_CORE_NUMBERS::pi<float32_t> / 180.f;
	float32_t const half_pi = 0.5f * LUCID_CORE_NUMBERS::pi<float32_t>;
	float32_t const quarter_pi = 0.25f * LUCID_CORE_NUMBERS::pi<float32_t>;

	///
	///	large integer number tests...
	///
	{
		LUCID_PROFILE_BEGIN("large integer test");

		int256_t a = "1,000,000,000,000,000";
		int256_t b =                  "-250";
		int256_t c =                     "3";

		_passed &= validate(  "large integer negate",    -a, int256_t(  "-1,000,000,000,000,000"));
		_passed &= validate(     "large integer add", a + b, int256_t(     "999,999,999,999,750"));
		_passed &= validate("large integer subtract", a - b, int256_t(   "1,000,000,000,000,250"));
		_passed &= validate("large integer multiply", a * b, int256_t("-250,000,000,000,000,000"));
		_passed &= validate(  "large integer divide", a / b, int256_t(      "-4,000,000,000,000"));
		_passed &= validate(  "large integer divide", a / c, int256_t(     "333,333,333,333,333"));
		_passed &= validate(  "large integer modulo", a % c, int256_t(                       "1"));

		LUCID_PROFILE_END();
	}

	///
	///	Fixed point number tests...
	///
	{
		LUCID_PROFILE_BEGIN("large fixed point test");

		fixed256_t two = "2.0";
		fixed256_t  pi = "3.14159";
		fixed256_t   r = "5,999,400,000,000.000";

		LUCID_PROFILE_BEGIN("simple arithmetic");
			fixed256_t circ = two * pi * r;
			fixed256_t area = pi * r * r;
		LUCID_PROFILE_END();

		_passed &= validate("large fixed point", area, fixed256_t( "113074488643178100585937500.0"));

		LUCID_PROFILE_END();
	}

	///
	///	Vector tests...
	///
	vector3_t e0 = LUCID_MATH::normalize(vector3_t(1, 1, 0));
	vector3_t e2(0, 0, 1);
	vector3_t e1 = LUCID_MATH::cross(e2, e0);

	_passed &= validate("vector dot product", dot(e0, e1), LUCID_MATH::cos(half_pi));

	///
	///	quaternion tests...
	///
	typedef LUCID_MATH::Quaternion<float32_t> Quaternion;

	Quaternion q = LUCID_MATH::rotateUsingAxis(vector3_t(0, 0, 1), theta);
	_passed &= validate("rotate using quaternion", LUCID_MATH::transformDirection(q, vector3_t(1, 0, 0)), LUCID_MATH::rotateAboutZ(theta) * vector3_t(1, 0, 0));

	q =  LUCID_MATH::rotateUsingAxis(vector3_t(0, 0, 1), quarter_pi);
	_passed &= validate("rotate using quaternion", LUCID_MATH::transformDirection(q, vector3_t(1, 0, 0)), LUCID_MATH::rotateAboutZ(quarter_pi) * vector3_t(1, 0, 0));

	q =  LUCID_MATH::rotateUsingAxis(vector3_t(0, 0, 1), half_pi);
	_passed &= validate("rotate using quaternion", LUCID_MATH::transformDirection(q, vector3_t(1, 0, 0)), LUCID_MATH::rotateAboutZ(half_pi) * vector3_t(1, 0, 0));

	///
	///	Matrix tests...
	///
	typedef LUCID_MATH::Matrix<float32_t, 3, 3> Matrix3x3;

	Matrix3x3 const I = Matrix3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
	Matrix3x3 R(e0.x, e1.x, e2.x, e0.y, e1.y, e2.y, e0.z, e1.z, e2.z);

	_passed &= validate("rotate using matrix", R, LUCID_MATH::rotateAboutZ(quarter_pi));
	_passed &= validate("matrix inverse", R * LUCID_MATH::inverse(R), I);
	_passed &= validate("matrix transpose", R * LUCID_MATH::transpose(R), I);

	return true;
}

void MathTest::render(float32_t time, float32_t interpolant)
{
	LUCID_GAL_PIPELINE.clear(true, true, true, LUCID_GAL::Color(0, 1, 0, 1));;
}
