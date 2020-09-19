#include "atmosphere.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 position = input.position;
	float2   vertex = 1.05 * input.scale.xx * input.vertex;

	float3 e1 = normalize(viewPosition - position);
	float3 e0 = normalize(cross(e1, viewUp));
	float3 e2 = normalize(cross(e0, e1));

	position = position + e0 * vertex.xxx + e2 * vertex.yyy + 0.001 * e1;;

	float3x3 R = float3x3(e0, e2, e1);

	output.   ppsPosition = mul(viewProjMatrix, float4(position, 1));
	output.lightDirection = mul(R, normalize(lightPosition - position));
	output. viewDirection = mul(R, e1);
	output.      texcoord = 0.5 * input.vertex + 0.5;

	return output; 
}
