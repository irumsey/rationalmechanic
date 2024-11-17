#include "utility.header.hlsl"
#include "planet.header.hlsl"

SamplerState theSampler;

OutputPixel main(InputPS input)
{
	OutputPixel output = (OutputPixel)0;

	float4  land = float4(0, 1, 0, 1);
	float4 shore = float4(1, 0, 0, 1);
	float4 water = float4(0, 0, 1, 1);

	float4 diffuse = (input.height < 0.25) ? water : ((input.height < 0.75) ? shore : land);

	output.   id = input.id;
	output.color = diffuse;
	output. glow = float4(0, 0, 0, 1);
	output.depth = input.depth;
	
	return output;
}
 