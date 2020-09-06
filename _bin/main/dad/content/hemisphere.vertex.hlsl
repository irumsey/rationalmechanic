#include "utility.header.hlsl"
#include "hemisphere.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 position = input.position.xyz;
	float3   normal = input.normal;
	float     scale = input.scale;
	float3   vertex = scale.xxx * input.vertex;

	position = position + viewRight * vertex.xxx + viewUp * vertex.yyy - viewForward * vertex.zzz;
	normal = viewRight * normal.xxx + viewUp * normal.yyy - viewForward * normal.zzz;

	output.   ppsPosition = mul(viewProjMatrix, float4(position, 1));
	output.        normal = normal;
	output.       diffuse = input.color;
	output.lightDirection = normalize(lightPosition - position);

	return output; 
}
