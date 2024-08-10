#include "../sphere.header.hlsl"

float time;

Texture2D diffuseTexture;
Texture2D noiseTexture;

SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float4 diffuseTexel = diffuseTexture.Sample(theSampler, input.texcoord);

	float noise0 = noiseTexture.Sample(theSampler, 1.1 * input.texcoord - time * float2(0.033,  0.003)).r;
	float noise1 = noiseTexture.Sample(theSampler, 1.7 * input.texcoord - time * float2(0.011, -0.001)).g;
	float noise2 = noiseTexture.Sample(theSampler, 2.1 * input.texcoord + time * float2(0.007,  0.001)).b;
	float noise3 = noiseTexture.Sample(theSampler, 3.3 * input.texcoord + time * float2(0.005, -0.003)).a;

	float modulation = clamp(0.5 * (noise2 + noise1 + noise0), 0.3, 1);
	float additive = 0.333 * (noise3 + noise2 + noise1);

	output.color = float4(modulation * diffuseTexel.rgb, 1);
	output. glow = float4(additive.rr, 0, 1);
	output.   id = input.id;

	return output;
}
 