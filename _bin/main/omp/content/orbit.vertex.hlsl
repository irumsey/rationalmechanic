#include "utility.header.hlsl"
#include "orbit.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float4x4 worldMatrix = matrixFromQuaternion(input.rotation, input.focus);
	 
	float  hu = input.parameters.x;
	float  eccentricity = input.parameters.y;
	float2 domain = input.parameters.zw;

	float2 adapted = (input.adapted - input.vertex) * clamp(eccentricity, 0, 1) + input.vertex;
	float2 theta = (domain.yy - domain.xx) * adapted.xy + domain.xx;

	float4 curvePosition0 = mul(worldMatrix, float4(computeConicPoint(hu, eccentricity, theta.x), 0, 1));
	float4 curvePosition1 = mul(worldMatrix, float4(computeConicPoint(hu, eccentricity, theta.y), 0, 1));
	float3 curveTangent = normalize(curvePosition1.xyz - curvePosition0.xyz);
	float3 viewDirection = normalize(viewPosition - curvePosition0.xyz);
	float3 curveNormal = normalize(cross(curveTangent, viewDirection));

	float3 vertexDelta = input.scale.xxx * curveNormal;
	float3 innerVertex = curvePosition0.xyz - vertexDelta;
	float3 outerVertex = curvePosition0.xyz + vertexDelta;

	float3 meshVertex = input.select.xxx * innerVertex + input.select.yyy * outerVertex;

	output.parameters.xy = input.parameters.xy;
	output.parameters. z = theta.x; 
	output.parameters. w = input.scale;
	output.        color = input.color;
	output.     position = input.focus;
	output.     rotation = input.rotation;
	output.       vertex = meshVertex;

	output.ppsPosition = mul(viewProjMatrix, float4(meshVertex, 1));

	return output;
}
