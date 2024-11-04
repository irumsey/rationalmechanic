#include "utility.header.hlsl"
#include "sphere.header.hlsl"

SamplerState theSampler;

OutputPixel main(InputPS input)
{
	OutputPixel output = (OutputPixel)0;

	output.   id = input.id;
	output.color = float4(1, 1, 1, 1);
	output. glow = float4(0, 0, 0, 1);
	output.depth = input.depth;
	
	return output;
}
 