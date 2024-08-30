#include "utility.header.hlsl"
#include "cuboid.header.hlsl"

OutputVS main(InputVS input)
{
	OutputVS output = (OutputVS)0;

	Vertex vertex = input.vertex;
	Instance instance = input.instance;

	float3 position = instance.position.xyz;
	float3 scale = instance.channel0.xyz;
	float  lightDistance = instance.channel2.x;

	float3x3 worldMatrix = rotationFromQuaterion(instance.rotation);
	float3 worldPosition = mul(worldMatrix, scale * vertex.position) + position;

	float3 normal = mul(worldMatrix, vertex.normal);

	float3 lightPosition = lightDistance * lightDirFromOrigin;
	float3 lightDirection = normalize(lightPosition - position);

	output.ppsPosition = mul(viewProjMatrix, float4(worldPosition, 1));
	output.shade = clamp(dot(lightDirection, normal), 0.2, 1);
	output.id = instance.id;
	output.specCoeff = instance.channel0.w;
	output.ambient = instance.channel1;

	return output;
}
 