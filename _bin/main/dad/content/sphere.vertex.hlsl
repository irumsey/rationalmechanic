#include "sphere.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 position = input.position.xyz;
	float     scale = input.position.w;

	float2 corner = scale.xx * input.corner.xy;

	position = position + viewRight * corner.xxx + viewUp * corner.yyy;
	output.ppsPosition = mul(viewProjMatrix, float4(position, 1));

	///	create a rotation which transforms the light direction from
	///	world space to texture space
	float3x3 R = float3x3(viewRight, -viewForward, -viewUp);

	output.lightDirection = mul(R, lightPosition - position);;
	output.       diffuse = float4(1, 1, 0, 1);
	output.      texcoord = input.corner.zw;

	return output; 
}
