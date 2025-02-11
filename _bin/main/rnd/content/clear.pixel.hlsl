#include "screen.header.hlsl"

QuadPixel main(InputPixel input)
{
	QuadPixel output = (QuadPixel)0;

	output.   id = 0;
	output.color = float4(0, 0, 0, 1);
	output. glow = float4(0, 0, 0, 1);
	output.depth = 1e38;

	return output;
}
