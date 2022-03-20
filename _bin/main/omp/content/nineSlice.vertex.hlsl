#include "nineSlice.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;
	const float2 screenDimensions = float2(screenWidth, screenHeight);
	const float2 ratio = 0.5 * input.dimension.xz / input.dimension.yw;

	float2 position = input.position + float2(dot(input.select.xy, input.dimension.xy), dot(input.select.zw, input.dimension.zw));
	float2 texcoord = 0.5 * float2(dot(input.select.xy, float2(1, ratio.x)), dot(input.select.zw, float2(1, ratio.y))) + 0.5;

	output.ppsPosition = float4(position / screenDimensions - 1, 0, 1);
	output.texcoord = texcoord;
	output.color = input.color;

	return output;
}
