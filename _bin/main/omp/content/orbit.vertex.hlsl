#include "utility.header.hlsl"
#include "orbit.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float hu = input.parameters.x;
	float ecc = input.parameters.y;
	float2 domain = input.parameters.zw;
	
	float4x4 worldMatrix = matrixFromQuaternion(input.rotation, input.focus);

	float theta0 = (domain.y - domain.x) * input.vertex.x + domain.x;
	float theta1 = (domain.y - domain.x) * input.vertex.y + domain.x;

	float4 worldPosition0 = mul(worldMatrix, float4(computeConicPoint(hu, ecc, theta0), 0, 1));
	float4 ppsPosition0 = mul(viewProjMatrix, worldPosition0);
	ppsPosition0.xyz = ppsPosition0.xyz / ppsPosition0.w;
	ppsPosition0.z = clamp(ppsPosition0.z, 0.0001, 0.9999);

	float4 worldPosition1 = mul(worldMatrix, float4(computeConicPoint(hu, ecc, theta1), 0, 1));
	float4 ppsPosition1 = mul(viewProjMatrix, worldPosition1);
	ppsPosition1.xyz = ppsPosition1.xyz / ppsPosition1.w;
	ppsPosition1.z = clamp(ppsPosition1.z, 0.0001, 0.9999);

	float2 tangent = normalize(ppsPosition1.xy - ppsPosition0.xy);
	float2 normal = float2(-tangent.y, tangent.x);

	float2 width = input.vertex.z * input.scale * texelSize;

	ppsPosition0.xy = (width * normal + ppsPosition0.xy) * ppsPosition0.w;
	ppsPosition0.z = ppsPosition0.z * ppsPosition0.w;

	output.ppsPosition = ppsPosition0;
	output.id = input.id;
	output.color = input.color;
	output.blend = input.vertex.z;

	return output;
}
