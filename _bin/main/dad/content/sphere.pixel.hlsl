#include "sphere.header.hlsl"

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float4          texel = normalsTexture.Sample(theSampler, input.texcoord).grba;
	float3 lightDirection = normalize(input.lightDirection);
	float3         normal = normalize(2 * texel.xyz - 1);
	float           shade = clamp(dot(lightDirection, normal), 0, 1) + input.diffuse.a;

	output.color = float4(shade * input.diffuse.rgb, texel.a);

	return output;
}
