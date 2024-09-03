#include "icons.header.hlsl"

Texture2D iconsTexture;
Texture2D depthTarget;

SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float4 texel = iconsTexture.Sample(theSampler, input.texcoord);

	float depth = depthTarget.Sample(theSampler, input.depthParameters.xy).r;
	float clip = (depth < input.depthParameters.z) ? 0 : 1;
	
	float3 color = input.color.rgb * texel.rgb;
	float alpha = input.color.a * texel.a * clip;

	output.   id = input.id;
	output.color = float4(color, alpha);
	output. glow = float4(0, 0, 0, 0);

	return output;
}
