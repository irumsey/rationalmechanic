#include "sprite.header.hlsl"

Texture2D normalsTexture;

SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float4  texel = normalsTexture.Sample(theSampler, input.texcoord);
	float3 normal = normalize(2 * texel.rgb - 1);
	float   shade = clamp(dot(input.lightDirection, normal), 0, 1) + input.diffuse.a;

	output.id = input.id;
	output.color = float4(shade * input.diffuse.rgb, texel.a);
	output.glow = float4(0, 0, 0, 1);

	return output;
}
