#include "hemisphere.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 position = input.position;
	float3   normal = input.vertex;
	float3   vertex = input.scale.xxx * normal;

	float3 e1 = normalize(viewPosition - position);
	float3 e0 = normalize(cross(e1, viewUp));
	float3 e2 = normalize(cross(e0, e1));

	position = position + e0 * vertex.xxx + e2 * vertex.yyy + e1 * vertex.zzz;

	float3x3 R = float3x3(e0, e2, e1);

	output.ppsPosition = mul(viewProjMatrix, float4(position, 1));
	output.normal = normal;
	output.lightDirection = mul(R, normalize(lightPosition - position));
	output.diffuse = input.color;

	return output;
}
