#include "sprite.header.hlsl"

Texture2D normalsTexture;

SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float3 normal = normalize(2 * normalsTexture.Sample(theSampler, input.texcoord).rgb - 1);
	float   shade = clamp(dot(input.lightDirection, normal), 0, 1) + input.diffuse.a;

	output.color = float4(shade * input.diffuse.rgb, 1);

	return output;
}
