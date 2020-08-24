#include "sphere.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 position = input.position.xyz;
	float     scale = input.position.w;

	float2 corner = scale.xx * input.corner.xy;

	position = position + corner.xxx * viewRight + corner.yyy * viewUp;
	output.ppsPosition = mul(viewProjMatrix, float4(position, 1));

	output.lightDirection = normalize(lightPosition - position);
	output.diffuse = float4(1, 1, 0, 1);
	output.texcoord = input.corner.zw;

	return output;
}
