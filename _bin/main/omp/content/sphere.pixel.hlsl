#include "utility.header.hlsl"
#include "sphere.header.hlsl"

Texture2D diffuseTexture;
Texture2D normalsTexture;

SamplerState theSampler;

OutputPixel main(InputPS input)
{
	OutputPixel output = (OutputPixel)0;

	float4 diffuseTexel = diffuseTexture.Sample(theSampler, input.texcoord);
	float3 normalTexel = normalsTexture.Sample(theSampler, input.texcoord).rgb;

	float3 diffuse = diffuseTexel.rgb;
	float3 ambient = input.ambient.rgb * diffuseTexel.rgb;

	float3 normal = normalize(2 * normalTexel - 1);
	normal.y = -normal.y;

	float3 lightDirectionN = normalize(input.lightDirection);
	float3 viewDirectionN = normalize(input.viewDirection);

	float shade = clamp(dot(normal, lightDirectionN), 0, 1);
	float3 color = (diffuse - ambient) * shade + ambient;

	float specMask = (input.specCoeff - input.ambient.a) * shade + input.ambient.a;
	specMask = diffuseTexel.a * specMask;

	float spec = clamp(dot(reflect(-viewDirectionN, normal), lightDirectionN), 0, 1);
	spec = specMask * pow(spec, 16);

	output.   id = input.id;
	output.color = float4(spec.rrr + color, 1);
	output. glow = float4(0, 0, 0, 1);
	output.depth = input.depth;
	
	output.color = float4(diffuse,  1);

	return output;
}
 