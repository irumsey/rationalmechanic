#include "cuboid.header.hlsl"

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	output.color = float4(input.shade * input.diffuse.rgb, 1);
	output. glow = float4(0, 0, 0, 1);
	output.   id = input.id;

	return output;
}
