#include "nineSlice.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;
	const float2 screenDimensions = float2(screenWidth, screenHeight);

	float2 position = input.position + float2(dot(input.select.xy, input.dimension.xy), dot(input.select.zw, input.dimension.zw));

	output.ppsPosition = float4(position / screenDimensions - 1, 0, 1);
	output.texcoord = input.texcoord;
	output.color = input.color;

	return output;
}
