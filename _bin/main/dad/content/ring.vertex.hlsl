float radius;
float width;

float4x4 viewProjMatrix;

struct InputVertex
{
	float3  position : LOCATION0;
};

struct OutputVertex
{
	float2 localPosition :   POSITION0;
	float4 ppsPosition   : SV_POSITION;
};

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	output.localPosition = radius * input.position.xy;
	output.ppsPosition = mul(viewProjMatrix, float4(radius * input.position, 1));

	return output;
}
