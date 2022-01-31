#include "sprite.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 position = (input.position1 - input.position0) * interpolant + input.position0;
	float2   vertex = input.scale.xx * input.vertex;

	float3 e1 = normalize(viewPosition - position);
	float3 e0 = normalize(cross(viewUp, e1));
	float3 e2 = normalize(cross(e1, e0));

	position = position + e0 * vertex.xxx + e2 * vertex.yyy;

	float3x3 R = float3x3(e0, e2, e1);

	output.ppsPosition = mul(viewProjMatrix, float4(position, 1));
	output.lightDirection = mul(R, normalize(lightPosition - position));
	output.diffuse = float4(1, 1, 1, 0.1);
	output.texcoord = 0.5 * (float2(1, 1) + float2(input.vertex.x, -input.vertex.y));

	return output;
}


