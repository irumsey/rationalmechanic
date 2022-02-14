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
	worldPosition = worldPosition / worldPosition.w;

	float3     direction = normalize(worldPosition.xyz - viewPosition);
	Ray              ray = { viewPosition,     direction };
	Sphere        planet = { input.center, input.radii.x };
	Sphere    atmosphere = { input.center, input.radii.y };

	RaySphereIntersection rayAtmosphere = intersects(ray, atmosphere, planet);
	if (rayAtmosphere.intersects)
	{
		float3 midPosition = 0.5 * (rayAtmosphere.position[0] + rayAtmosphere.position[1]);
		float3 lightDirection = normalize(lightPosition - worldPosition.xyz);

		Ray rays[3] = {
			{ rayAtmosphere.position[0], lightDirection },
			{               midPosition, lightDirection },
			{ rayAtmosphere.position[1], lightDirection },
		};

		RaySphereIntersection scattered[3] = {
			intersects(rays[0], atmosphere , planet),
			intersects(rays[1], atmosphere , planet),
			intersects(rays[2], atmosphere , planet),
		};

		float depth[3] = {
			length(scattered[0].position[0] - rays[0].origin) * input.radii.z,
			length(scattered[1].position[0] - rays[1].origin) * input.radii.z,
			length(scattered[2].position[0] - rays[2].origin) * input.radii.z,
		};
		 
		output.color = float4(float3(1, 1, 1), depth[1] * input.radii.z);
	}

	return output;
}
