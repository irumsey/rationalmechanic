#include "atmosphere.header.hlsl"

Texture2D scatterTexture;
Texture2D normalsTexture;

SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float3    normal = normalize(2 * normalsTexture.Sample(theSampler, input.texcoord).gbr - 1);

	float    n_dot_l = dot(input.lightDirection, normal);
	float3 reflected = normalize(2 * n_dot_l * normal - input.lightDirection);
	float   specular = 0.5 * dot(input.viewDirection, reflected) + 0.5;
	float      shade = 1.0 - (0.5 * n_dot_l + 0.5);
	float3 scattered = scatterTexture.Sample(theSampler, float2(shade, 0.5)).rgb;

	output.color = float4(0, 0, 0, 0);
	output. glow = float4(0, 0, 0, 0);

	return output;
}
