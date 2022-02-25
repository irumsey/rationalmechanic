#include "utility.header.hlsl"
#include "orbit.header.hlsl"

Texture2D orbitLookup;

SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float fade = clamp((fadeDistance.y - input.distance) / (fadeDistance.y - fadeDistance.x), 0, 1);
	float alpha = orbitLookup.Sample(theSampler, float2(input.width, 0.5)).a;

	output.color = fade * float4(input.color.rgb, alpha) + (1 - fade) * float4(input.color.rgb, 0);
	output.glow = float4(0, 0, 0, alpha);

	return output; 
}
