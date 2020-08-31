#include "sphere.header.hlsl"

Texture2D diffuseTexture;
SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float4 texel = diffuseTexture.Sample(theSampler, input.texcoord);
	output.color = input.diffuse * texel;

	return output;
}
