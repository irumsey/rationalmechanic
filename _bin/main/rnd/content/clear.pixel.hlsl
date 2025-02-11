#include "screen.header.hlsl"

TriplePixel main(InputPixel input)
{
	TriplePixel output = (TriplePixel)0;

	output.   id = 0;
	output.color = float4(0, 0, 0, 1);
	output. glow = float4(0, 0, 0, 1);

	return output;
}
