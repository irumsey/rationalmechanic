#include "utility.header.hlsl"
#include "orbit.header.hlsl"

Texture2D depthTarget;
SamplerState theSampler;

OutputPixel main(InputPS input)
{
	OutputPixel output = (OutputPixel)0;

	float depth = depthTarget.Sample(theSampler, input.texcoord.xy).r;

	// do not move this to the vertex shader, you should know why.
	float blend = 1 - input.blend * input.blend;
	float clip = (depth < input.texcoord.z) ? 0 : 1;

	output.   id = input.id;
	output.color = float4(input.color.rgb, clip * blend);
	output. glow = float4(0, 0, 0, 0);

	return output; 
}
