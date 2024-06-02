#include "utility.header.hlsl"
#include "orbit.header.hlsl"
 
Texture2D orbitLookup;

SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float fade = clamp((fadeDistance.y - input.distance) / (fadeDistance.y - fadeDistance.x), 0, 1);

	output.color = float4(input.color.rgb, 1);
	output.glow = float4(0, 0, 0, 0);
	output.id = input.id;

	return output; 
}
