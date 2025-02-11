#include "gui.header.hlsl"

Texture2D guiTexture;
SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float4 texel = guiTexture.Sample(theSampler, input.texcoord);	
	output.color = input.color * texel;
	output.glow = float4(0, 0, 0, input.color.a * texel.a);

	return output;
}
