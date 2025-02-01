#include "font.header.hlsl"

Texture2D fontTexture;

SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float4 texel = fontTexture.Sample(theSampler, input.texcoord);

	output.   id = 0;
	output.color = input.color * dot(input.channel, texel);
	output. glow = float4(0, 0, 0, 1);

	return output;
}
