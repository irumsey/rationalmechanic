#include "nineSlice.header.hlsl"

Texture2D theTexture;
SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	output.color = theTexture.Sample(theSampler, input.texcoord);
	output.glow = float4(0, 0, 0, 1);
	output.id = 0;

	output.color = input.color * float4(output.color.aaa, 1);

	return output;
}
