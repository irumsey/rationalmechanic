#include "orbit.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float2 adapted = (input.adapted - input.position) * clamp(eccentricity, 0, 1) + input.position;
	float2 theta = (domain.yy - domain.xx) * adapted.xy + domain.xx;

	float2 curvePosition = computeConicPoint(theta.x);
	float2 curveTangent = computeConicPoint(theta.y) - curvePosition;
	float2 curveNormal = -normalize(float2(-curveTangent.y, curveTangent.x));

	float2 vertexDelta = 0.5 * lineWidth * curveNormal;
	float2 innerVertex = curvePosition - vertexDelta;
	float2 outerVertex = curvePosition + vertexDelta;

	float2 meshVertex = input.select.x * innerVertex + input.select.y * outerVertex;

	output.position = curvePosition;
	output.normal = curveNormal;
	output.vertex = meshVertex;
	output.ppsPosition = mul(viewProjMatrix, float4(meshVertex, 0, 1));

	return output;
}
