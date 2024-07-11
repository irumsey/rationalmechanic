#include "icons.header.hlsl"

Texture2D iconsTexture;

SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float4 texel = iconsTexture.Sample(theSampler, input.texcoord);

	output.color = input.color * texel;
	output.id = input.id;

	return output;
}
