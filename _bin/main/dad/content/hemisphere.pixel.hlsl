#include "hemisphere.header.hlsl"

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float shade = clamp(dot(input.normal, input.lightDirection), 0, 1);
	output.color = float4(shade * input.diffuse.rgb, 1);

	return output;
}
