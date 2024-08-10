#include "utility.header.hlsl"
#include "cuboid.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 position = input.position.xyz;
	float  scale = input.position.w;

	float3 lightDirection = normalize(lightPosition - position);

	float3x3 R = rotationFromQuaterion(input.rotation);
	position = mul(R, scale * input.vertex) + position;

	float3 normal = mul(R, input.normal);

	output.ppsPosition = mul(viewProjMatrix, float4(position, 1));
	output.shade = clamp(dot(lightDirection, normal), 0.2, 1);
	output.id = input.id;
	output.diffuse = input.diffuse;
	output.ambient = input.ambient;

	return output;
}
 