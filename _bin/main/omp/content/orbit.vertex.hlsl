#include "utility.header.hlsl"
#include "orbit.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float  hu = input.parameters.x;
	float  eccentricity = input.parameters.y;
	float2 domain = input.parameters.zw;
	float  theta = (domain.y - domain.x) * input.vertex.x + domain.x;

	float3 conics = computeConics(hu, eccentricity, theta);
	float2 conicPoint = conics.z * conics.xy;
	float2 conicDirection = conics.xy;
	float3 vertexPosition = float3(conicPoint, 0) + input.scale * float3(input.vertex.y * conicDirection, input.vertex.z);

	float4x4 worldMatrix = matrixFromQuaternion(input.rotation, input.focus);
	float4 worldPosition = mul(worldMatrix, float4(vertexPosition, 1));

	output.ppsPosition = mul(viewProjMatrix, worldPosition);
	output.   distance = length(viewPosition - worldPosition.xyz);
	output.         id = input.id;
	output.      color = input.color;

	return output;
}
