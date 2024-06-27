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
	float eccentricity = input.parameters.y;
	
	float2 domain = input.parameters.zw;

	float theta0 = (domain.y - domain.x) * input.vertex.x + domain.x;
	float theta1 = (domain.y - domain.x) * input.vertex.y + domain.x;

	float4x4 worldMatrix = matrixFromQuaternion(input.rotation, input.focus);

	float4 worldPosition0 = mul(worldMatrix, float4(computeConicPoint(hu, eccentricity, theta0), 0, 1));
	float4 worldPosition1 = mul(worldMatrix, float4(computeConicPoint(hu, eccentricity, theta1), 0, 1));

	float4 ppsPosition0 = mul(viewProjMatrix, worldPosition0);
	float4 ppsPosition1 = mul(viewProjMatrix, worldPosition1);

	ppsPosition0 = ppsPosition0 / exclude(ppsPosition0.w, -znear, znear);
	ppsPosition1 = ppsPosition1 / exclude(ppsPosition1.w, -znear, znear);

	float2 tangent = ppsPosition1.xy - ppsPosition0.xy;
	float2 normal = float2(-tangent.y, tangent.x);

	float N = length(normal);
	N = (N > 0) ? N : 0.001;

	normal = input.vertex.z * input.scale * texelSize.x * normal / N;

	output.ppsPosition = float4(ppsPosition0.xy + normal, ppsPosition0.zw);
	output.         id = input.id;
	output.      color = input.color;
	 
	return output;
}
