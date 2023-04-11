#include "utility.header.hlsl"
#include "atmosphere.header.hlsl"

Texture2D scatterTexture;
Texture2D normalsTexture;

SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float4 worldPosition = mul(invViewProjMatrix, input.clipSpace);
	worldPosition = worldPosition / worldPosition.w;

	float3 direction = normalize(lightPosition - worldPosition.xyz);
	Ray ray = { worldPosition.xyz, direction };
	Sphere planet = { input.center, input.radii.x };

	RaySphereIntersection intersection = intersects(ray, planet);
	
	output.color = (intersection.intersects && (intersection.t[0] > 0)) ? float4(0, 0, 0, 0) : float4(1, 1, 1, 0.5);

	return output;
}
