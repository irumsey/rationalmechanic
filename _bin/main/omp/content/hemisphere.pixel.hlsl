#include "hemisphere.header.hlsl"

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;
	output.color = input.diffuse;
	return output;
}
