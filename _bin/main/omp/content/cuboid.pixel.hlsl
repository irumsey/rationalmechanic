#include "cuboid.header.hlsl"

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	// could be done in vertex shader, however, 
	// placeholding code for future...
	float3 color = (input.diffuse.rgb - input.ambient.rgb) * input.shade + input.ambient.rgb;

	output.color = float4(color, 1);
	output. glow = float4(0, 0, 0, 1);
	output.   id = input.id;

	return output;
}
