#include "utility.header.hlsl"
#include "orbit.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex    output = (OutputVertex)0;

	float      hu = input.parameters.x;
	float     ecc = input.parameters.y;
	float2 domain = input.parameters.zw;
	float  theta0 = (domain.y - domain.x) * input.vertex.x + domain.x;
	float  theta1 = (domain.y - domain.x) * input.vertex.y + domain.x;

	float4x4 worldMatrix = matrixFromQuaternion(input.rotation, input.focus);

	float4 worldPosition0 = mul(worldMatrix, float4(computeConicPoint(hu, ecc, theta0), 0, 1));
	float4 psPosition0 = mul(viewProjMatrix, worldPosition0);
	psPosition0.xy = psPosition0.xy / psPosition0.w;

	float4 worldPosition1 = mul(worldMatrix, float4(computeConicPoint(hu, ecc, theta1), 0, 1));
	float4 psPosition1 = mul(viewProjMatrix, worldPosition1);
	psPosition1.xy = psPosition1.xy / psPosition1.w;

	float2 tangent = normalize(psPosition1.xy - psPosition0.xy);
	float2 normal = float2(-tangent.y, tangent.x);

	float2 width = input.vertex.z * input.scale * texelSize;

	psPosition0.xy = (width * normal + psPosition0.xy) * psPosition0.w;

	output.ppsPosition = psPosition0;
	output.id = input.id;
	output.color = input.color;
	output.blend = input.vertex.z;

	return output;
}
