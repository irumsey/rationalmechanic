#include "utility.header.hlsl"
#include "orbit.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float4x4 worldMatrix = matrixFromQuaternion(input.rotation, input.focus);

	float  hu = input.parameters.x;
	float  eccentricity = input.parameters.y;
	float2 domain = input.parameters.zw;

	float r = hu * dot(input.vertex.yzw, float3(1, 1 - input.scale, 1 + input.scale));

	float1 theta = (domain.y - domain.x) * input.vertex.x + domain.x;
	float4 curvePosition = mul(worldMatrix, float4(computeConicPoint(r, eccentricity, theta), 0, 1));

	output.parameters.xy = input.parameters.xy;
	output.parameters. z = theta; 
	output.parameters. w = input.scale;
	output.        color = input.color;
	output.     position = input.focus;
	output.     rotation = input.rotation;
	output.       vertex = curvePosition.xyz;

	output.ppsPosition = mul(viewProjMatrix, curvePosition);

	return output;
}
