#include "utility.header.hlsl"
#include "planet.header.hlsl"

OutputVS main(InputVS input)
{
	OutputVS output = (OutputVS)0;

	Vertex vertex = input.vertex;
	Instance instance = input.instance;

	float3 sphereCenter = instance.position.xyz;
	float radius = instance.position.w;

	float3x3 worldMatrix = rotationFromQuaterion(instance.rotation);
	float3 worldPosition = mul(worldMatrix, radius * vertex.normal) + sphereCenter;

	float3 lightPosition = instance.lightPosition.xyz;
	float3 lightDirection = normalize(lightPosition - sphereCenter);

	float3 viewDirection = -normalize(worldPosition);

	float2 texOffset = float2(0.0, 0.0);

	output.ppsPosition = mul(viewProjMatrix, float4(worldPosition, 1));
	output.texcoord = vertex.texcoord + texOffset;
	output.height = vertex.height;
	output.lightDirection = lightDirection;
	output.viewDirection = viewDirection;
	output.specCoeff = instance.channel0.w;
	output.ambient = instance.channel1;
	output.id = instance.id;
	output.depth = instance.compositing.y * length(worldPosition);

	return output;
}
  