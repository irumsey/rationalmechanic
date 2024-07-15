#include "star.header.hlsl"

Texture2D shapeTexture;

SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float  alpha = shapeTexture.Sample(theSampler, input.texcoord).a;

	output.color = float4(input.color.rgb, alpha);
	output.glow = float4(1, 1, 1, alpha);
	output.select = input.id;

	return output;
}
