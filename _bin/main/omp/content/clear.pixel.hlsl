#include "screen.header.hlsl"

QuadPixel main(InputPixel input)
{
	QuadPixel output = (QuadPixel)0;

	///	TBD: if different values are required, use the render context to change
	output.id = 0;
	output.color = float4(0, 0, 0, 1);
	output.glow = float4(0, 0, 0, 1);
	output.depth = 1.0;

	return output;
}
