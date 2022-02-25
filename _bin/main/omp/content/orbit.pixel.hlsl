#include "utility.header.hlsl"
#include "orbit.header.hlsl"

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	output.color = float4(1, 1, 1, 1);
	output.glow = float4(0, 0, 0, 0);

	return output;
}
