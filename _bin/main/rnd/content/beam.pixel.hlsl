#include "beam.header.hlsl"

Texture2D diffuseTexture;

SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float4 diffuse = diffuseTexture.Sample(theSampler, input.texcoord);
	output.color = diffuse * input.diffuse;

	return output;
}
