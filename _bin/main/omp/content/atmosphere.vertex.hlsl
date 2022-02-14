#include "atmosphere.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 position = input.position;
	float2   vertex = 1.05 * input.scale.xx * input.vertex;

	float3   e1 = normalize(viewPosition - position);
	float3   e0 = normalize(cross(e1, viewUp));
	float3   e2 = normalize(cross(e0, e1));

	position = position + e0 * vertex.xxx + e2 * vertex.yyy;
	position = position + 0.001 * e1; // shift towards the viewer to previent z-fighting with planet

	output.     clipSpace = mul(viewProjMatrix, float4(position, 1));
	output.   ppsPosition = output.clipSpace;
	output.        center = input.position;
	output.         radii = float3(input.scale * float2(0.975, 1.05), 1.0 / (1.05 * input.scale));
	output.      texcoord = 0.5 * (float2(1, 1) + float2(input.vertex.x, -input.vertex.y));

	return output; 
}
