#include "utility.header.hlsl"
#include "cuboid.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 position = input.position.xyz;
	float  scale = input.position.w;
	float  lightDistance = input.channel2.x;

	float3x3 worldMatrix = rotationFromQuaterion(input.rotation);
	float3 worldPosition = mul(worldMatrix, scale * input.vertex) + position;

	float3 normal = mul(worldMatrix, input.normal);

	float3 lightPosition = lightDistance * lightDirection;
	float3 lightDirection2 = normalize(lightPosition - position);

	output.ppsPosition = mul(viewProjMatrix, float4(worldPosition, 1));
	output.shade = clamp(dot(lightDirection2, normal), 0.2, 1);
	output.id = input.id;
	output.diffuse = input.channel0;
	output.ambient = input.channel1;

	return output;
}
 