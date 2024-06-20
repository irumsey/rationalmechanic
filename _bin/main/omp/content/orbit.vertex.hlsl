#include "utility.header.hlsl"
#include "orbit.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float hu = input.parameters.x;
	float eccentricity = input.parameters.y;
	
	float2 domain = input.parameters.zw;

	float theta0 = (domain.y - domain.x) * input.vertex.x + domain.x;
	float theta1 = (domain.y - domain.x) * input.vertex.y + domain.x;

	float4x4 worldMatrix = matrixFromQuaternion(input.rotation, input.focus);

	float4 worldPosition0 = mul(worldMatrix, float4(computeConicPoint(hu, eccentricity, theta0), 0, 1));
	float4 worldPosition1 = mul(worldMatrix, float4(computeConicPoint(hu, eccentricity, theta1), 0, 1));

	float4 ppsPosition0 = mul(viewProjMatrix, worldPosition0);
	float4 ppsPosition1 = mul(viewProjMatrix, worldPosition1);

	ppsPosition0 = (ppsPosition0.w > 0.000001) ? ppsPosition0 / ppsPosition0.w : 100000.f * ppsPosition0;
	ppsPosition1 = (ppsPosition1.w > 0.000001) ? ppsPosition1 / ppsPosition1.w : 100000.f * ppsPosition1;

	float2 ppsTangent = ppsPosition1.xy - ppsPosition0.xy;

	float ppsLength = length(ppsTangent);
	ppsLength = (ppsLength > 0.000001) ? ppsLength : 1;

	float2 ppsNormal = input.scale * texelSize * input.vertex.z * float2(-ppsTangent.y, ppsTangent.x) / ppsLength;

	output.ppsPosition = float4(ppsPosition0.xy + ppsNormal, ppsPosition1.zw);
	output.         id = input.id;
	output.      color = input.color;
	 
	return output;
}
