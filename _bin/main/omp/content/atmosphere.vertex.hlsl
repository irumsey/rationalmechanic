#include "atmosphere.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 position = input.position.xyz;
	float scale = input.position.w;
	float3 vertex = 1.05 * scale * input.vertex;

	float3 e1 = normalize(viewPosition - position);
	float3 e0 = normalize(cross(e1, viewUp));
	float3 e2 = normalize(cross(e0, e1));

	position = e0 * vertex.x + e2 * vertex.y + e1 * vertex.z + position;

	float3x3 R = float3x3(e0, e2, e1);

	output.clipSpace = mul(viewProjMatrix, float4(position, 1));
	output.ppsPosition = output.clipSpace;
	output.center = input.position.xyz;
	output.radii = float2( scale, 1.05 * scale);
	output.lightDirection = mul(R, normalize(lightPosition - position));
	output.diffuse = input.diffuse;
	output.ambient = input.ambient;
	output.texcoord = input.texcoord;

	return output; 
}
