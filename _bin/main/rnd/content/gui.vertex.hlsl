#include "gui.header.hlsl"

float2 size(float4 rect)
{
	return rect.zw - rect.xy;
}

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float2 scrPosition = texelSize * (input.vertex * size(input.position) + input.position.xy);

	output.ppsPosition = float4(2 * scrPosition - 1, 0, 1);
	output.texcoord = input.vertex * size(input.texcoord) + input.texcoord.xy;
	output.color = input.color;

	return output;
}
