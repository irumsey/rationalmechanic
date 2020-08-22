#include "orbit.header.hlsl"

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	output.color = float4(lineColor.rgb, 1);

	return output;
}

