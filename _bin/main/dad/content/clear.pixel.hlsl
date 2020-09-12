#include "screen.header.hlsl"

DoublePixel main(InputPixel input)
{
	DoublePixel output = (DoublePixel)0;

	///	TBD: if different values are required, use the render context to change
	output.color = float4(0, 0, 0, 1);
	output.glow = float4(0, 0, 0, 1);

	return output;
}