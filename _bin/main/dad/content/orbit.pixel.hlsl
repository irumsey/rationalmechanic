#include "utility.header.hlsl"
#include "orbit.header.hlsl"

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float4x4    worldMatrix = matrixFromQuaternion(input.rotation, input.position);
	float4    curvePosition = mul(worldMatrix, float4(computeConicPoint(input.parameters.x, input.parameters.y, input.parameters.z), 0, 1));

	float delta = length(input.vertex - curvePosition.xyz);
	float     u = clamp(delta / input.parameters.w, 0, 1);

	output.color = float4(input.color.rgb, 1 - u * u);

	return output;
}
