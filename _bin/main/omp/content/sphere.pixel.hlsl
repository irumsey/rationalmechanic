#include "sphere.header.hlsl"

Texture2D diffuseTexture;
Texture2D normalsTexture;

SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float4 diffuse = diffuseTexture.Sample(theSampler, input.texcoord);
	float3 normal = normalize(2 * normalsTexture.Sample(theSampler, input.texcoord).rgb - 1);

	float shade = clamp(dot(normal, input.lightDirection), 0.1, 1);

	output.color = float4(shade * diffuse.rgb, 1);
	output. glow = float4(0, 0, 0, 1);
	output.   id = input.id;

	return output;
}
