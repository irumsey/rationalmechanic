#include "sphere.header.hlsl"

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float4  texel = normalsTexture.Sample(theSampler, input.texcoord);
	float3 normal = normalize(2 * texel - 1);
	float   shade = clamp(dot(input.lightDirection, normal), 0, 1);

	output.color = float4(shade * input.diffuse.rgb, texel.a * input.diffuse.a);

	return output;
}
