#include "utility.header.hlsl"
#include "atmosphere.header.hlsl"

OutputVS main(InputVS input)
{
	OutputVS output = (OutputVS)0;

	Vertex vertex = input.vertex;
	Instance instance = input.instance;

	float3 normal = vertex.normal;
	float3 planetCenter = instance.position.xyz;
	float lightDistance = instance.channel2.x;
	float thickness = instance.channel2.y;
	float radii[2] = { instance.position.w, thickness * instance.position.w };

	float3 lightPosition = lightDirection * lightDistance;

	float3x3 worldMatrix = rotationFromQuaterion(instance.rotation);
	float3 worldPosition = mul(worldMatrix, radii[1] * normal) + planetCenter;
	
	// thickness of atmosphere
	float H = radii[1] - radii[0];

	// D is the maximum possible ray length through the atmosphere.
	float D = 2.0 * sqrt(radii[1] * radii[1] - radii[0] * radii[0]);

	output.ppsPosition = mul(viewProjMatrix, float4(worldPosition, 1));
	output.planetCenter = planetCenter;
	output.radii = float4(radii[0], radii[1], 1.0 / H, 1.0 / D);
	output.normal = mul(worldMatrix, normal);
	output.beta = instance.channel0.xyz;
	output.viewDirection = -normalize(worldPosition);
	output.lightDirection = normalize(lightPosition - worldPosition);

	return output;
}
