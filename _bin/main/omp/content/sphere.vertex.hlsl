#include "utility.header.hlsl"
#include "sphere.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 e2 = input.vertex;				/// normal
	float3 e0 = input.tangent;				/// tangent
	float3 e1 = normalize(cross(e2, e0));	///	bi-tangent

	float3x3 surfaceMatrix = float3x3(e0, e1, e2);

	float3 sphereCenter = input.position.xyz;
	float radius = input.position.w;

	float3x3 worldMatrix = rotationFromQuaterion(input.rotation);
	float3 worldPosition = mul(worldMatrix, radius * e2) + sphereCenter;

	float  lightDistance = input.parameters.x;
	float3 lightPosition = lightDistance * lightDirection;
	float3 lightDirection2 = normalize(lightPosition - sphereCenter);

	float3 viewDirection = -normalize(worldPosition);

	output.ppsPosition = mul(viewProjMatrix, float4(worldPosition, 1));
	output.texcoord = input.texcoord;
	output.lightDirection = mul(surfaceMatrix, mul(lightDirection2, worldMatrix));
	output.viewDirection = mul(surfaceMatrix, mul(viewDirection, worldMatrix));
	output.diffuse = input.diffuse;
	output.ambient = input.ambient;
	output.id = input.id;

	return output;
}
  