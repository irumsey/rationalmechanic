#include "sphere.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 position = input.position.xyz;
	float     scale = input.scale;

	float2 corner = scale.xx * input.corner.xy;

	position = position + viewRight * corner.xxx + viewUp * corner.yyy;

	output.   ppsPosition = mul(viewProjMatrix, float4(position, 1));
	output.lightDirection = normalize(lightPosition - position);
	output.       diffuse = input.color;
	output.      texcoord = input.corner.zw;

	return output; 
}
