#include "screen.header.hlsl"

Texture2D theSource;
SamplerState theSampler;

SinglePixel main(InputPixel input)
{
	SinglePixel output = (SinglePixel)0;

	output.color = theSource.Sample(theSampler, input.texcoord);

	return output;
}