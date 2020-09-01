#include "utility.header.hlsl"
#include "orbit.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float  hu = input.parameters.x;
	float  eccentricity = input.parameters.y;
	float2 domain = input.parameters.zw;

	float2 adapted = (input.adapted - input.position) * clamp(eccentricity, 0, 1) + input.position;
	float2 theta = (domain.yy - domain.xx) * adapted.xy + domain.xx;

	float2 curvePosition = computeConicPoint(hu, eccentricity, theta.x);
	float2 curveTangent = computeConicPoint(hu, eccentricity, theta.y) - curvePosition;
	float2 curveNormal = -normalize(float2(-curveTangent.y, curveTangent.x));

	float2 vertexDelta = input.scale * curveNormal;
	float2 innerVertex = curvePosition - vertexDelta;
	float2 outerVertex = curvePosition + vertexDelta;

	float2 meshVertex = input.select.x * innerVertex + input.select.y * outerVertex;

	output.parameters = input.parameters;
	output.position = meshVertex;
	output.theta = theta.x;
	output.lineWidth = input.scale;
	output.lineColor = input.color;

	float4x4 worldMatrix = matrixFromQuaternion(input.rotation);
	float4 worldPosition = mul(worldMatrix, float4(meshVertex, 0, 1));
	
	output.ppsPosition = mul(viewProjMatrix, worldPosition);

	return output;
}
