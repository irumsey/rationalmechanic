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

	float3 lightPosition = lightDistance * lightDirection;
	float3 lightDirection2 = normalize(lightPosition - position);

	output.ppsPosition = mul(viewProjMatrix, float4(worldPosition, 1));
	output.shade = clamp(dot(lightDirection2, normal), 0.2, 1);
	output.id = instance.id;
	output.specCoeff = instance.channel0.w;
	output.ambient = instance.channel1;

	return output;
}
 