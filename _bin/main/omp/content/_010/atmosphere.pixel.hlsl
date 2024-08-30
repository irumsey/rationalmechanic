#include "../utility.header.hlsl"
#include "../atmosphere.header.hlsl"

Texture2D scatterTexture;

SamplerState theSampler;

OutputPixel main(InputPS input)
{
	OutputPixel output = (OutputPixel)0;

	float radii[2] = { input.radii.x, input.radii.y };

	float3 normal = normalize(input.normal);

	// position[0] is quaranteed to be the intersection of the atmosphere
	// position[1] requires a test if it hits the planet or atmosphere
	float3 position[2] = {
		radii[1] * normal + input.planetCenter,
		radii[1] * normal + input.planetCenter
	};

	Ray ray = { float3(0, 0, 0), position[0] };
	Sphere planet = { input.planetCenter, radii[0] };
	Sphere atmosphere = { input.planetCenter, radii[1] };

	RaySphereIntersection result = intersects(ray, planet);
	if (result.intersects)
	{
		position[1] = result.position[0];
	}
	else
	{
		// failing the above test the ray must hit
		// the atmosphere somewhere
		result = intersects(ray, atmosphere);
		position[1] = result.position[1];
	}

	float h = length(0.5 * (position[1] + position[0]) - input.planetCenter);
	h = 1 - input.radii.z * (h - radii[0]);

	float d = input.radii.w * length(position[1] - position[0]);

	float I = h * h * d * d;

	output.color = float4(0, 0, 0, 0);
	output.glow = float4(input.beta, I);

	return output;
}
