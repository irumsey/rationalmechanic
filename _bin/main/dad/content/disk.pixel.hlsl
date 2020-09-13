#include "disk.header.hlsl"

Texture2D diffuseTexture;
Texture2D    glowTexture;
Texture2D normalsTexture;

SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float4 diffuse = input.diffuse * diffuseTexture.Sample(theSampler, float2(1, 1) - float2(0.5, 1) * input.texcoord);
	float4    glow = glowTexture.Sample(theSampler, float2(1, 1) - float2(0.5, 1) * input.texcoord);
	float3 normal = normalize(2 * normalsTexture.Sample(theSampler, input.texcoord).gbr - 1);
	float   shade = clamp(dot(input.lightDirection, normal), 0, 1) + input.diffuse.a;

	glow.rgb = (1 - shade) * glow.rgb;

	output.color = float4(shade * diffuse.rgb, 1);
	output. glow = float4(glow.rgb, 1);

	return output;
}