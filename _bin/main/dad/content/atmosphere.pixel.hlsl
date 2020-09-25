#include "utility.header.hlsl"
#include "atmosphere.header.hlsl"

Texture2D scatterTexture;
Texture2D normalsTexture;

SamplerState theSampler;

///	Note: for now, viewer location must be outside atmosphere
OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float4 worldPosition = mul(invViewProjMatrix, input.clipSpace);
	float3     direction = normalize(worldPosition.xyz / worldPosition.www - viewPosition);
	Ray              ray = { viewPosition,     direction };
	Sphere        planet = { input.center, input.radii.x };
	Sphere    atmosphere = { input.center, input.radii.y };

	RaySphereIntersection rayAtmosphere = intersects(ray, atmosphere, planet);
	if (rayAtmosphere.intersects)
	{
		float3 midPosition = 0.5 * (rayAtmosphere.position[0] + rayAtmosphere.position[1]);

		Ray rays[3] = {
			{ rayAtmosphere.position[0], input.lightDirection },
			{               midPosition, input.lightDirection },
			{ rayAtmosphere.position[1], input.lightDirection },
		};

		RaySphereIntersection scattered[3] = {
			intersects(rays[0], atmosphere , planet),
			intersects(rays[1], atmosphere , planet),
			intersects(rays[2], atmosphere , planet),
		};

		float depth[3] = {
			length(scattered[0].position[1] - rays[0].origin) * input.radii.z,
			length(scattered[1].position[1] - rays[1].origin) * input.radii.z,
			length(scattered[2].position[1] - rays[2].origin) * input.radii.z,
		};
		 
		// float i = 1 - clamp(0.5 * (depth[0] + depth[1] + depth[2]), 0, 1);
		float i = depth[2];

		output.color = float4(i.xxx, 1);
	}

	return output;
}
