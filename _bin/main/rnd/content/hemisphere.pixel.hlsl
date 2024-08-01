#include "hemisphere.header.hlsl"

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float3   shaded = clamp(dot(input.lightDirection, input.normal), 0.25, 1) * input.diffuse.rgb;
	float3 unshaded = input.diffuse.rgb;

	output.color = float4((unshaded - shaded) * input.diffuse.a + shaded, 1);

	return output;
}
 