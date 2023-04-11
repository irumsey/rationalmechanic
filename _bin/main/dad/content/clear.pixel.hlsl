#include "screen.header.hlsl"

TriplePixel main(InputPixel input)
{
	TriplePixel output = (TriplePixel)0;

	///	TBD: if different values are required, use the render context to change
	output.color = float4(0, 0, 0, 1);
	output.glow = float4(0, 0, 0, 1);
	output.id = 0;

	return output;
}
