#pragma once

#include <string>
#include <lucid/core/Error.h>
#include <lucid/math/Algorithm.h>
#include <lucid/math/Scalar.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Matrix.h>

///
///
///

typedef lucid::math::Vector<float32_t, 3> vector3_t;
typedef lucid::math::Vector<float32_t, 4> vector4_t;
typedef lucid::math::Matrix<float32_t, 4, 4> matrix4x4_t;
typedef lucid::math::Sphere<float32_t, 3> sphere_t;

struct ray_t
{
	vector3_t origin;
	vector3_t direction;

	ray_t() = default;

	ray_t(vector3_t const &origin, vector3_t const &direction)
		: origin(origin)
		, direction(direction)
	{
	}
};

struct RaySphereIntersection
{
	bool intersects = false;
	float32_t t[2] = { 0, 0, };
	vector3_t point[2];

	RaySphereIntersection() = default;

	operator bool () const
	{
		return intersects;
	}

	bool operator()(ray_t const &ray, sphere_t const &sphere)
	{
		intersects = false;

		vector3_t r = ray.origin - sphere.center;
		float32_t c = lucid::math::dot(r, r) - sphere.radius * sphere.radius;

		float32_t a = lucid::math::dot(ray.direction, ray.direction);
		float32_t b = 2.f * lucid::math::dot(ray.direction, r);

		float32_t dd = b * b - 4.f * a * c;
		if (dd < 0.f)
			return intersects;
		intersects = true;

		float32_t d = lucid::math::sqrt(dd);

		t[0] = 0.5f * (-b - d) / a;
		t[1] = 0.5f * (-b + d) / a;

		point[0] = t[0] * ray.direction + ray.origin;
		point[1] = t[1] * ray.direction + ray.origin;

		return intersects;
	}
};

///
///
///
class Scattering
{
public:
	Scattering() = default;

	virtual ~Scattering() = default;

	virtual void execute();
	 
private:
	vector3_t _lightDirection;
	vector3_t _viewPosition;
	vector3_t _viewUp;
	vector3_t _viewDirection;

	sphere_t _atmosphere;
	sphere_t _planet;

	size_t _width = 0;
	size_t _height = 0;
	vector3_t *_image = nullptr;

	void writeImage(std::string const &path);

	float32_t phase(float32_t theta) const;

	float32_t phase(float32_t theta, float32_t g) const;
	 
	vector3_t trace(ray_t const &ray);

	vector3_t sample(ray_t const &ray, vector3_t const &point);
};

inline float32_t Scattering::phase(float32_t cos_theta) const
{
	static float32_t const coeff = 3.f / (16.f * lucid::math::constants::pi<float32_t>());
	return coeff * (1.f + cos_theta * cos_theta);
}

inline float32_t Scattering::phase(float32_t cos_theta, float32_t g) const
{
	static float32_t const coeff = 1.f / (4.f * lucid::math::constants::pi<float32_t>());
	float32_t gg = g * g;
	return coeff * (1.f - gg) / lucid::math::pow(1.f + gg - 2.f * g * cos_theta, 1.5f);
}

inline vector3_t Scattering::trace(ray_t const &ray)
{
	static float32_t const dL = 0.1f;

	RaySphereIntersection intersectsAtmosphere;
	RaySphereIntersection intersectsPlanet;

	vector3_t scattered = vector3_t(0, 0, 0);
	if (!intersectsAtmosphere(ray, _atmosphere))
		return scattered;

	///	handles the viewer inside or outside the atmosphere
	vector3_t const &p1 = (intersectsPlanet(ray, _planet) && intersectsPlanet.t[0] > 0.f) ? intersectsPlanet.point[0] : intersectsAtmosphere.point[1];
	vector3_t const &p0 = (intersectsAtmosphere.t[0] < 0.f) ? ray.origin : intersectsAtmosphere.point[0];

	float32_t D = lucid::math::len(p1 - p0);
	if (D < 0.0001f) 
		return scattered;

	float32_t d = 0.f;
	while (d < D)
	{
		vector3_t p = d * ray.direction + p0;
		scattered = scattered + sample(ray, p);
		d = d + dL;
	}

	return scattered;
}

inline vector3_t Scattering::sample(ray_t const &ray, vector3_t const &p0)
{
	float32_t altitude = (lucid::math::len(p0) - _planet.radius) / (_atmosphere.radius - _planet.radius);

	RaySphereIntersection intersectsAtmosphere;
	RaySphereIntersection intersectsPlanet;

	intersectsAtmosphere(ray_t(p0, _lightDirection), _atmosphere);
	vector3_t p1 = intersectsPlanet(ray_t(p0, _lightDirection), _planet) ? intersectsPlanet.point[0] : intersectsAtmosphere.point[1];
	float32_t D = lucid::math::len(p1 - p0);

	if (D < 0.001f)
		return vector3_t(0, 0, 0);

	float32_t cos_theta = lucid::math::dot(ray.direction, _lightDirection);
	float32_t i = lucid::math::pow(2.78f, -30.f / D) * (phase(cos_theta) + phase(cos_theta, 0.7f));

	return 0.3f * (1.f - altitude) * (D / _atmosphere.radius) * vector3_t(0.142f * i, 0.18f * i, 0.25f * i);
}
