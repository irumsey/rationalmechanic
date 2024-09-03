#include "utility.header.hlsl"
#include "orbit.header.hlsl"

OutputVS main(InputVS input)
{
	OutputVS output = (OutputVS)0;

	Vertex vertex = input.vertex;
	Instance instance = input.instance;

	float3 position = instance.position.xyz;
	float scale = instance.position.w;

	float4x4 worldMatrix = matrixFromQuaternion(instance.rotation, position);

	float      hu = instance.channel2.x;
	float     ecc = instance.channel2.y;
	float2 domain = instance.channel2.zw;

	float  theta[2] = 
	{
		(domain.y - domain.x) * vertex.parameters.x + domain.x,
		(domain.y - domain.x) * vertex.parameters.y + domain.x,
	};

	float4 worldPosition[2] =
	{
		mul(worldMatrix, float4(computeConicPoint(hu, ecc, theta[0]), 0, 1)),
		mul(worldMatrix, float4(computeConicPoint(hu, ecc, theta[1]), 0, 1)),
	};

	float4 ppsPosition[2] = 
	{
		mul(viewProjMatrix, worldPosition[0]),
		mul(viewProjMatrix, worldPosition[1]),
	};

	ppsPosition[0].xyz = ppsPosition[0].xyz / ppsPosition[0].w;
	ppsPosition[1].xyz = ppsPosition[1].xyz / ppsPosition[1].w;

	float2 tangent = normalize(ppsPosition[1].xy - ppsPosition[0].xy);
	float2 normal = float2(-tangent.y, tangent.x);

	float2 width = vertex.parameters.z * scale * texelSize;

	ppsPosition[0].xy = width * normal + ppsPosition[0].xy;
	ppsPosition[0]. z = min(ppsPosition[0].z, 0.999);

	ppsPosition[0].xyz = ppsPosition[0].xyz * ppsPosition[0].w;

	float2 texcoord = float2(0.5, -0.5) * ppsPosition[0].xy / ppsPosition[0].w + float2(0.5, 0.5);
	float depth = instance.channel1.w * length(worldPosition[0].xyz);

	output.ppsPosition = ppsPosition[0];
	output.id = instance.id;
	output.texcoord = float3(texcoord, depth);
	output.color = instance.channel0;
	output.blend = vertex.parameters.z;

	return output;
}
