#include "utility.header.hlsl"
#include "orbit.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex    output = (OutputVertex)0;

	float      hu = input.channel2.x;
	float     ecc = input.channel2.y;
	float2 domain = input.channel2.zw;
	float  theta0 = (domain.y - domain.x) * input.vertex.x + domain.x;
	float  theta1 = (domain.y - domain.x) * input.vertex.y + domain.x;

	float3 position = input.position.xyz;
	float scale = input.position.w;

	float4x4 worldMatrix = matrixFromQuaternion(input.rotation, position);

	float4 worldPosition0 = mul(worldMatrix, float4(computeConicPoint(hu, ecc, theta0), 0, 1));
	float4 csPosition0 = mul(viewMatrix, worldPosition0);
//	csPosition0.z = min(csPosition0.z, -0.0001);

	float4 worldPosition1 = mul(worldMatrix, float4(computeConicPoint(hu, ecc, theta1), 0, 1));
	float4 csPosition1 = mul(viewMatrix, worldPosition1);
//	csPosition1.z = min(csPosition1.z, -0.0001);

	float4 psPosition0 = mul(projMatrix, csPosition0);
	psPosition0.xyz = psPosition0.xyz / psPosition0.w;
	psPosition0.z = clamp(psPosition0.z, 0.0001, 0.9999) * psPosition0.w;

	float4 psPosition1 = mul(projMatrix, csPosition1);
	psPosition1.xy = psPosition1.xy / psPosition1.w;

	float2 tangent = normalize(psPosition1.xy - psPosition0.xy);
	float2 normal = float2(-tangent.y, tangent.x);

	float2 width = input.vertex.z * scale * texelSize;

	psPosition0.xy = (width * normal + psPosition0.xy) * psPosition0.w;

	output.ppsPosition = psPosition0;
	output.id = input.id;
	output.color = input.channel0;
	output.blend = input.vertex.z;

	return output;
}
