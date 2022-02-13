#include "beam.header.hlsl"

// this needs to be passed in per-instance
static float1 width = 0.125;

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3  b = input.position1 - input.position0;
	float3 v0 = input.position0 - viewPosition;
	float3 v1 = input.position1 - viewPosition;
	float3  u = width * normalize(cross(b, v0));
	float3 r0 = width * normalize(cross(u, v0));
	float3 r1 = width * normalize(cross(v1, u));

	float3 position =
		input.select0.x * input.position0 +
		input.select1.x * input.position1 +
		input.select0.z * r0 +
		input.select1.z * r1 +
		(input.select0.y + input.select1.y) * u;

	output.ppsPosition = mul(viewProjMatrix, float4(position, 1));
	output.diffuse = input.diffuse;
	output.texcoord = input.texcoord;

	return output;
}


