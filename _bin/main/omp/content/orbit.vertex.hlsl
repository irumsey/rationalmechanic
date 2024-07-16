#include "utility.header.hlsl"
#include "orbit.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float hu = input.parameters.x;
	float ecc = input.parameters.y;
	float2 domain = input.parameters.zw;

	float theta0 = (domain.y - domain.x) * input.vertex.x + domain.x;
	float theta1 = (domain.y - domain.x) * input.vertex.y + domain.x;
	 
	float4x4 worldMatrix = matrixFromQuaternion(input.rotation, input.focus);

	float4 worldPosition0 = mul(worldMatrix, float4(computeConicPoint(hu, ecc, theta0), 0, 1));
	float4 worldPosition1 = mul(worldMatrix, float4(computeConicPoint(hu, ecc, theta1), 0, 1));

	float4 ppsPosition0 = mul(viewProjMatrix, worldPosition0);
	float4 ppsPosition1 = mul(viewProjMatrix, worldPosition1);

	if ((ppsPosition0.z > 0) && (ppsPosition1.z > 0))
	{
		ppsPosition0 = ppsPosition0 / ppsPosition0.w;
		ppsPosition1 = ppsPosition1 / ppsPosition1.w;

		float2 tangent = ppsPosition1.xy - ppsPosition0.xy;
		float T = length(tangent);
		tangent = (T > 0) ? tangent / T : float2(0, 1);

		float2 normal = input.vertex.z * float2(-tangent.y, tangent.x);
		float2 width = input.scale * texelSize;

		ppsPosition0.xy = width * normal + ppsPosition0.xy;
		ppsPosition0.zw = float2(min(ppsPosition0.z, 0.99), 1);
	}

	output.ppsPosition = ppsPosition0;
	output.         id = input.id;
	output.      color = input.color;
	output.      blend = input.vertex.z;

	return output;
}
