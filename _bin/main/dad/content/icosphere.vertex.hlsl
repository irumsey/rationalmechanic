#include "icosphere.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 position = input.scale * input.vertex + input.position;
	output.ppsPosition = mul(viewProjMatrix, float4(position, 1));

	return output;
}