#include "utility.header.hlsl"
#include "sphere.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 e2 = input.vertex;				/// normal
	float3 e0 = input.tangent;				/// tangent
	float3 e1 = normalize(cross(e2, e0));	///	bi-tangent

	float3x3 R = float3x3(e0, e1, e2);

	float3 position = input.position.xyz;
	float scale = input.position.w;

	float3 lightDirection = normalize(lightPosition - position);
	float3 viewDirection = normalize(viewPosition - position);

	float3x3 worldMatrix = rotationFromQuaterion(input.rotation);
	float3 worldPosition = mul(worldMatrix, scale * e2) + position;

	output.ppsPosition = mul(viewProjMatrix, float4(worldPosition, 1));
	output.texcoord = input.texcoord;
	output.lightDirection = mul(R, mul(lightDirection, worldMatrix));
	output.viewDirection = mul(R, mul(viewDirection, worldMatrix));
	output.id = input.id;
	output.diffuse = input.diffuse;
	output.ambient = input.ambient;

	return output;
}
  