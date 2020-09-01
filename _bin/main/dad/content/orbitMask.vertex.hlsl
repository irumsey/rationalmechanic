float4x4 viewProjMatrix;

struct InputVertex
{
	float3     vertex : LOCATION0;

	float3   position : LOCATION1;
	float       scale : LOCATION2;
	float4   rotation : LOCATION3;
	float4      color : LOCATION4;
	float4 parameters : LOCATION5;
};

struct OutputVertex
{
	float4 ppsPosition : SV_POSITION;
};

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 position = input.scale * input.vertex + input.position.xyz;
	output.ppsPosition = mul(viewProjMatrix, float4(position, 1));

	return output;
}
