#include "utility.header.hlsl"
#include "orbit.header.hlsl"

float exclude(float x, float a, float b)
{
	if ((x < a) || (b < x))
		return x;

	if ((b - x) > (x - a))
		return a;

	return b;
}

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float hu = input.parameters.x;
	float ecc = input.parameters.y;
	
	float2 domain = input.parameters.zw;
	float theta = (domain.y - domain.x) * input.vertex.x + domain.x;

	float4x4 worldMatrix = matrixFromQuaternion(input.rotation, input.focus);
	float4 worldPosition = mul(worldMatrix, float4(computeConicPoint(hu, ecc, theta), 0, 1));

	float3 normal = normalize(worldPosition.xyz - input.focus);

	worldPosition.xyz = worldPosition.xyz + input.scale * input.vertex.y * normal;

	output.ppsPosition = mul(viewProjMatrix, worldPosition);
	output.         id = input.id;
	output.      color = input.color;
	 
	return output;
}
