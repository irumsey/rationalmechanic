#include "utility.header.hlsl"
#include "sphere.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 e2 = input.vertex;
	float3 e0 = normalize(cross(float3(0, 0, 1), e2));
	float3 e1 = normalize(cross(e2, e0));

	float3 lightDirection = normalize(lightPosition - input.position);

	float4x4 worldMatrix = matrixFromQuaternion(input.rotation, input.position);
	float4 worldPosition = mul(worldMatrix, float4(input.scale * e2, 1));

	output.ppsPosition = mul(viewProjMatrix, worldPosition);
	output.texcoord = input.texcoord;
	output.lightDirection = mul(float3x3(e0, e1, e2), lightDirection);
	output.id = input.id;
	output.diffuse = input.color;

	return output;
}
 