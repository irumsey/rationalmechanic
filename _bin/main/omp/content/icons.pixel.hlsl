#include "icons.header.hlsl"

Texture2D iconsTexture;

SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float4 texel = iconsTexture.Sample(theSampler, input.texcoord);

	output.   id = input.id;
	output.color = input.color * texel;
	output. glow = float4(0, 0, 0, 0);

	return output;
}
