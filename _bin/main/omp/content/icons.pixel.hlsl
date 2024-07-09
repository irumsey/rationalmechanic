#include "icons.header.hlsl"

Texture2D iconsTexture;

SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float4 texel = iconsTexture.Sample(theSampler, input.texcoord);

	output.color = float4(texel.rgb * input.color.rgb, texel.a);
	output.glow = float4(0, 0, 0, 1);
	output.id = input.id;

	return output;
}
