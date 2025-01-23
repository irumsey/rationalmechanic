#include "utility.header.hlsl"
#include "sphere.header.hlsl"

OutputVS main(InputVS input)
{
	OutputVS output = (OutputVS)0;

	Vertex vertex = input.vertex;
	Instance instance = input.instance;

	float3 e2 = vertex.normal;				/// normal
	float3 e0 = vertex.tangent;				/// tangent
	float3 e1 = normalize(cross(e2, e0));	///	bi-tangent

	float3x3 surfaceMatrix = float3x3(e0, e1, e2);

	float3 sphereCenter = instance.position.xyz;
	float radius = instance.position.w;

	float3x3 worldMatrix = rotationFromQuaterion(instance.rotation);
	float3 worldPosition = mul(worldMatrix, radius * e2) + sphereCenter;

	float3 lightPosition = instance.lightPosition.xyz;
	float3 lightDirection = normalize(lightPosition - sphereCenter);

	float3 viewDirection = -normalize(worldPosition);

	float2 texOffset = float2(0.5, 0.0);

	output.ppsPosition = mul(viewProjMatrix, float4(worldPosition, 1));
	output.texcoord = vertex.texcoord + texOffset;
	output.lightDirection = mul(surfaceMatrix, mul(lightDirection, worldMatrix));
	output.viewDirection = mul(surfaceMatrix, mul(viewDirection, worldMatrix));
	output.specCoeff = instance.channel0.w;
	output.ambient = instance.channel1;
	output.id = instance.id;
	output.depth = instance.compositing.y * length(worldPosition);

	return output;
}
  