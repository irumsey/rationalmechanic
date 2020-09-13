#include "screen.header.hlsl"

Texture2D theSource;
SamplerState theSampler;

SinglePixel main(InputPixel input)
{
	static const float weights[5] = { 0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162, };
	static const float offsets[5] = { 0, 1, 2, 3, 4, };

	SinglePixel output = (SinglePixel)0;

	float3 pixel = weights[0] * theSource.Sample(theSampler, input.texcoord).rgb;
	for (int i = 1; i < 5; ++i)
	{
		pixel += weights[i] * theSource.Sample(theSampler, input.texcoord - offsets[i] * texelSize).rgb;
		pixel += weights[i] * theSource.Sample(theSampler, input.texcoord + offsets[i] * texelSize).rgb;
	}
	output.color = float4(pixel, 1);

	return output;
}
 