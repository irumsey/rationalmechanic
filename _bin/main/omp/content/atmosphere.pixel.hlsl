#include "utility.header.hlsl"
#include "atmosphere.header.hlsl"

Texture2D scatterTexture;

SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float radii[2] = { input.radii.x, input.radii.y };

	float3 normal = normalize(input.normal);

	// position[0] is quaranteed to be the intersection of the atmosphere
	// position[1] requires a test if it hits the planet
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
		result = intersects(ray, atmosphere);
		position[1] = result.position[1];
	}

	float h = length(0.5 * (position[1] + position[0]) - input.planetCenter);
	h = 1 - (h - radii[0]) / (radii[1] - radii[0]);

	float d = length(position[1] - position[0]);
	float D = 2 * sqrt(radii[1] * radii[1] - radii[0] * radii[0]);

	float cosTheta = dot(input.viewDirection, input.lightDirection);
	float cosPhi = dot(normal, input.lightDirection);

	float F = 5 * (1 + cosTheta * cosTheta);
	float I = 0.3 * (1 + cosPhi) * h * F * (d / D);

	output.color = float4(0, 0, 0, 0);
	output.glow = float4(input.beta, I);

	return output;
}
