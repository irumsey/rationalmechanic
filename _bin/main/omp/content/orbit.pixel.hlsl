#include "utility.header.hlsl"
#include "orbit.header.hlsl"
 
Texture2D orbitLookup;

SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	output.color = float4(0.7 * input.color.rgb, 1);
	output.glow = float4(0.3 * input.color.rgb, 1);
	output.id = input.id;

	return output; 
}
