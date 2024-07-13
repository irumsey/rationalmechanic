#include "fonts.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float2 screenPosition = texelSize * (input.position.zw * input.vertex.xy + input.position.xy);

	output.ppsPosition = float4(screenPosition, 0.1, 1);
	output.texcoord = input.texcoord.zw * input.vertex.zw + input.texcoord.xy;
	output.channel = input.channel;
	output.color = input.color;

	return output;
}


