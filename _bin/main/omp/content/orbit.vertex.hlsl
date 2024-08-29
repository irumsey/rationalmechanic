#include "utility.header.hlsl"
#include "orbit.header.hlsl"

OutputVS main(InputVS input)
{
	OutputVS output = (OutputVS)0;

	Vertex vertex = input.vertex;
	Instance instance = input.instance;

	float      hu = instance.channel2.x;
	float     ecc = instance.channel2.y;
	float2 domain = instance.channel2.zw;
	float  theta0 = (domain.y - domain.x) * vertex.parameters.x + domain.x;
	float  theta1 = (domain.y - domain.x) * vertex.parameters.y + domain.x;

	float3 position = instance.position.xyz;
	float scale = instance.position.w;

	float4x4 worldMatrix = matrixFromQuaternion(instance.rotation, position);

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

	float2 width = vertex.parameters.z * scale * texelSize;

	psPosition0.xy = (width * normal + psPosition0.xy) * psPosition0.w;

	output.ppsPosition = psPosition0;
	output.id = instance.id;
	output.color = instance.channel0;
	output.blend = vertex.parameters.z;

	return output;
}
