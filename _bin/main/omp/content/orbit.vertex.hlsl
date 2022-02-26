#include "utility.header.hlsl"
#include "orbit.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float  hu = input.parameters.x;
	float  eccentricity = input.parameters.y;
	float2 domain = input.parameters.zw;
	float  theta = (domain.y - domain.x) * input.vertex.x + domain.x;
	float  dTheta = segCoeff * (domain.y - domain.x);

	float4x4 worldMatrix = matrixFromQuaternion(input.rotation, input.focus);

	float4 curvePosition0 = mul(worldMatrix, float4(computeConicPoint(hu, eccentricity, theta +      0), 0, 1));
	float4 curvePosition1 = mul(worldMatrix, float4(computeConicPoint(hu, eccentricity, theta + dTheta), 0, 1));
	float3 tangent = normalize(curvePosition1.xyz - curvePosition0.xyz);

	float3 viewDirection = viewPosition - curvePosition0.xyz;
	float   viewDistance = length(viewDirection);

	float3 e2 = normalize(cross(viewDirection / viewDistance, tangent));

	float width = input.scale * dot(input.vertex.yzw, float3(0, 1, -1));

	output.ppsPosition = mul(viewProjMatrix, float4(curvePosition0.xyz + width * e2, 1));
	output.   distance = viewDistance;
	output.         id = input.id;
	output.      width = 0.5 * (width / input.scale) + 0.5;
	output.      color = input.color;

	return output;
}
