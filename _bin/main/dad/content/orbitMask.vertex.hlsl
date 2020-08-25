float4x4 viewProjMatrix;

struct InputVertex
{
	float3   vertex : LOCATION0;
	float4 position : LOCATION1;
};

struct OutputVertex
{
	float4 ppsPosition : SV_POSITION;
};

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 position = input.position.www * input.vertex + input.position.xyz;
	output.ppsPosition = mul(viewProjMatrix, float4(position, 1));

	return output;
}
