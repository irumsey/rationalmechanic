#include "sphere.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 position = input.position.xyz;
	float     scale = input.position.w;

	float2 corner = scale.xx * input.corner.xy;

	position = position + viewRight * corner.xxx + viewUp * corner.yyy;
	output.ppsPosition = mul(viewProjMatrix, float4(position, 1));

	float3x3 R = float3x3(viewRight, -viewForward, -viewUp);

	output.lightDirection = mul(R, lightPosition - position);;
	output.       diffuse = input.color;
	output.      texcoord = input.corner.zw;
	 
	return output; 
}
