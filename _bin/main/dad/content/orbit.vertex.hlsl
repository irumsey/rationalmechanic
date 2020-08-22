#include "orbit.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	/// test {
	///	constants
	float      pi = 3.1415926;
	float half_pi = 0.5 * pi;
	float  two_pi = 2.0 * pi;
	//// } test

	float2 adapted = (input.adapted - input.position) * e + input.position;

	float2 position = computeConicPoint(two_pi * adapted.x);
	float2 tangent = computeConicPoint(two_pi * adapted.y) - position;
	float2 normal = -normalize(float2(-tangent.y, tangent.x));

	float2 delta = lineWidth * normal;
	float2 inner = position - delta;
	float2 outer = position + delta;

	output.position = input.select.x * inner + input.select.y * outer;
	output.normal = normal;
	output.ppsPosition = mul(viewProjMatrix, float4(output.position, 0, 1));

	return output;
}
