#include "orbit.header.hlsl"

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float           hu = input.parameters.x;
	float eccentricity = input.parameters.y;
	float        theta = input.theta;
	float4   lineColor = input.lineColor;

	float u = clamp(2 * length(computeConicPoint(hu, eccentricity, theta) - input.position) / input.lineWidth, 0, 1);
	output.color = float4(lineColor.rgb, 1 - u * u);

	return output;
}
