#include "star.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 direction = computeRelativeDirection(input.parameters.y, input.parameters.z);

	float3 e1 = -direction;
	float3 e0 = cross(e1, viewUp);
	float3 e2 = cross(e0, e1);

	float2 vertex = input.parameters.w * input.vertex;

	float3 position =  starScale * (e0 * vertex.xxx + e2 * vertex.yyy) + (starFieldRadius * direction + viewPosition);

	output.ppsPosition = mul(viewProjMatrix, float4(position, 1));
	output.         id = input.id;
	output.   texcoord = 0.5 * input.vertex + 0.5;
	output.      color = input.color;

	return output;
}