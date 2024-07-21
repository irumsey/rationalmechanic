#include "nineSlice.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float2 position = input.position + float2(dot(input.select.xy, input.dimension.xy), dot(input.select.zw, input.dimension.zw));

	output.ppsPosition = float4(2 * texelSize * position - 1, 0, 1);
	output.texcoord = input.texcoord;
	output.color = input.color;
	output.id = input.id;

	return output;
}
