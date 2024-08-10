#include "utility.header.hlsl"
#include "sphere.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 e2 = input.vertex;
	float3 e0 = cross(float3(0, 0, 1), e2);
	float3 e1 = normalize(cross(e2, e0));

	e0 = normalize(cross(e1, e2));

	float3x3 R = float3x3(e0, e1, e2);

	float3 position = input.position.xyz;
	float scale = input.position.w;

	float3 lightDirection = normalize(lightPosition - position);
	float3 viewDirection = normalize(viewPosition - position);

	float4x4 worldMatrix = matrixFromQuaternion(input.rotation, position);
	float4 worldPosition = mul(worldMatrix, float4(scale * e2, 1));

	output.ppsPosition = mul(viewProjMatrix, worldPosition);
	output.texcoord = input.texcoord;
	output.lightDirection = mul(R, lightDirection);
	output.viewDirection = mul(R, viewDirection);
	output.id = input.id;
	output.diffuse = input.diffuse;
	output.ambient = input.ambient;

	return output;
}
  