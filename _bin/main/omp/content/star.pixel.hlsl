#include "star.header.hlsl"

Texture2D shapeTexture;
Texture2D spectralTexture;

SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float  alpha = shapeTexture.Sample(theSampler, input.texcoord).r;
	float4 color = spectralTexture.Sample(theSampler, float2(input.specoord, 0.5));

	output.color = float4(color.rgb, alpha * alpha);
	output.glow = float4(color.rgb, alpha);

	return output;
}
