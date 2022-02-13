#include "screen.header.hlsl"

Texture2D colorTarget;
Texture2D  glowTarget;

SamplerState theSampler;

SinglePixel main(InputPixel input)
{
	SinglePixel output = (SinglePixel)0;

	float4 color = colorTarget.Sample(theSampler, input.texcoord);
	float4  glow =  glowTarget.Sample(theSampler, input.texcoord);

	output.color = float4(color.rgb + glow.rgb, 1);

	return output;
}