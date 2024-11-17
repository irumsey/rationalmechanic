#include "icons.header.hlsl"

Texture2D iconsTexture;
Texture2D depthTarget;

SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float alpha = iconsTexture.Sample(theSampler, input.texcoord).a;

	float depth = depthTarget.Sample(theSampler, input.depthParameters.xy).r;
	float clip = (depth < input.depthParameters.z) ? 0 : 1;
	
	output.   id = input.id;
	output.color = float4(input.color.rgb, clip * alpha);
	output. glow = float4(0, 0, 0, 0);

	return output;
}
