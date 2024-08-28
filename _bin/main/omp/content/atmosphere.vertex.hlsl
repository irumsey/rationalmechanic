#include "utility.header.hlsl"
#include "atmosphere.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 normal = input.vertex;
	float3 planetCenter = input.position.xyz;
	float lightDistance = input.channel2.x;
	float thickness = input.channel2.y;
	float radii[2] = { input.position.w, thickness * input.position.w };

	float3 lightPosition = lightDirection * lightDistance;

	float3x3 worldMatrix = rotationFromQuaterion(input.rotation);
	float3 worldPosition = mul(worldMatrix, radii[1] * normal) + planetCenter;
	
	output.ppsPosition = mul(viewProjMatrix, float4(worldPosition, 1));
	output.planetCenter = planetCenter;
	output.radii = float2(radii[0], radii[1]);
	output.normal = mul(worldMatrix, normal);
	output.beta = input.channel0.xyz;
	output.viewDirection = -normalize(worldPosition);
	output.lightDirection = normalize(lightPosition - worldPosition);

	return output;
}
