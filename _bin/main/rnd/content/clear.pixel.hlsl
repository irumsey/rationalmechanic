#include "screen.header.hlsl"

DoublePixel main(InputPixel input)
{
	DoublePixel output = (DoublePixel)0;

	output.color = float4(0, 0, 0, 1);
	output.glow = float4(0, 0, 0, 1);

	return output;
}
