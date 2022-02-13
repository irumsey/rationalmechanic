
///
///
///
float3x3 rotationFromQuaterion(float4 q)
{
	float xx = q.x * q.x;
	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float xw = q.x * q.w;
	float yy = q.y * q.y;
	float yz = q.y * q.z;
	float yw = q.y * q.w;
	float zz = q.z * q.z;
	float zw = q.z * q.w;

	return float3x3(
		1 - 2 * ( yy + zz ),     2 * ( xy - zw ),     2 * ( xz + yw ),
		    2 * ( xy + zw ), 1 - 2 * ( xx + zz ),     2 * ( yz - xw ),
		    2 * ( xz - yw ),     2 * ( yz + xw ), 1 - 2 * ( xx + yy )
	);
}

///
///
///
float4x4 matrixFromQuaternion(float4 q)
{
	float xx = q.x * q.x;
	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float xw = q.x * q.w;
	float yy = q.y * q.y;
	float yz = q.y * q.z;
	float yw = q.y * q.w;
	float zz = q.z * q.z;
	float zw = q.z * q.w;

	return float4x4(
		1 - 2 * ( yy + zz ),     2 * ( xy - zw ),     2 * ( xz + yw ), 0,
		    2 * ( xy + zw ), 1 - 2 * ( xx + zz ),     2 * ( yz - xw ), 0,
		    2 * ( xz - yw ),     2 * ( yz + xw ), 1 - 2 * ( xx + yy ), 0,
		                  0,                   0,                   0, 1
	);
}

///
///
///
float4x4 matrixFromQuaternion(float4 q, float3 p)
{
	float xx = q.x * q.x;
	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float xw = q.x * q.w;
	float yy = q.y * q.y;
	float yz = q.y * q.z;
	float yw = q.y * q.w;
	float zz = q.z * q.z;
	float zw = q.z * q.w;

	return float4x4(
		1 - 2 * ( yy + zz ),     2 * ( xy - zw ),     2 * ( xz + yw ), p.x,
		    2 * ( xy + zw ), 1 - 2 * ( xx + zz ),     2 * ( yz - xw ), p.y,
		    2 * ( xz - yw ),     2 * ( yz + xw ), 1 - 2 * ( xx + yy ), p.z,
		                  0,                   0,                   0,   1
		);
}

///
///
///

struct Ray
{
	float3 origin;
	float3 direction;
};

struct Sphere
{
	float3 center;
	float radius;
};

struct RaySphereIntersection
{
	bool intersects;
	float t[2];
	float3 position[2];
};

RaySphereIntersection intersects(Ray ray, Sphere sphere)
{
	RaySphereIntersection result = { false, { 0, 0}, { float3(0, 0, 0), float3(0, 0, 0) }};

	float3 r = ray.origin - sphere.center;

	float  c = dot(r, r) - sphere.radius * sphere.radius;
	float  a = dot(ray.direction, ray.direction);
	float  b = 2 * dot(ray.direction, r);

	float dsq = b * b - 4 * a * c;
	if (dsq < 0) return result;

	result.intersects = true;

	float d = sqrt(dsq);
	float coeff = 0.5 / a;

	result.t[0] = coeff * (-b - d);
	result.t[1] = coeff * (-b + d);

	result.position[0] = result.t[0] * ray.direction + ray.origin;
	result.position[1] = result.t[1] * ray.direction + ray.origin;

	return result;
}

///	viewer outside atmosphere only
RaySphereIntersection intersects(Ray ray, Sphere atmosphere, Sphere planet)
{
	RaySphereIntersection rayAtmosphere = intersects(ray, atmosphere);
	if (!rayAtmosphere.intersects) return rayAtmosphere;

	RaySphereIntersection rayPlanet = intersects(ray, planet);
	if (rayPlanet.intersects)
	{
		rayAtmosphere.t[1] = rayPlanet.t[0];
		rayAtmosphere.position[1] = rayPlanet.position[0];
	}

	return rayAtmosphere; 
}
