#include "gui.header.hlsl"

Texture2D guiTexture;
SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float4 texel = guiTexture.Sample(theSampler, input.texcoord);	
	output.color = input.color * texel;

	return output;
}
