#include "utility.header.hlsl"
#include "hemisphere.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 position = input.position.xyz;
	float     scale = input.scale;
	float3   vertex = scale.xxx * input.vertex;

	position = position + viewRight * vertex.xxx + viewUp * vertex.yyy - viewForward * vertex.zzz;
	output.ppsPosition = mul(viewProjMatrix, float4(position, 1));

	float3x3 R = float3x3(viewRight, -viewForward, -viewUp);

	output.lightDirection = mul(R, lightPosition - position);;
	output.       diffuse = input.color;

	return output; 
}
