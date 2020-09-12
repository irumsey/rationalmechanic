#include "screen.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	output.ppsPosition = float4(input.vertex, 1, 1);
	output.texcoord = input.texcoord;

	return output;
}