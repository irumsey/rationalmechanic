#include "nineSlice.header.hlsl"

Texture2D theTexture;
SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float4 texel = theTexture.Sample(theSampler, input.texcoord);
	output.glow = float4(0, 0, 0, 1);
	output.id = 0;

	output.color = input.color * texel;

	return output;
}
