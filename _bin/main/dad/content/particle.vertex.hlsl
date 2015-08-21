float time;
float interpolant;

float3 viewRight;
float3 viewUp;
float4x4 viewProjMatrix;

struct InputVertex
{
	float4     corner : LOCATION0;
	float2       time : LOCATION1;
	float3  position0 : LOCATION2;
	float3  position1 : LOCATION3;
	float4 parameters : LOCATION4;
	float4   texcoord : LOCATION5;
};
 
struct OutputVertex
{
	float4 ppsPosition : SV_POSITION;
	float2    texcoord : TEXCOORD0;
	float     lifetime : TEXCOORD1;
};

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 position = lerp(input.position0, input.position1, interpolant);

	float theta = lerp(input.parameters.x, input.parameters.y, interpolant);
	float c = cos(theta);
	float s = sin(theta);
	float2x2 R = float2x2(c, s, -s, c);

	float scale = lerp(input.parameters.z, input.parameters.w, interpolant);
	float2 corner = scale.xx * mul(R, input.corner.xy);

	position = position + corner.xxx * viewRight + corner.yyy * viewUp;
	output.ppsPosition = mul(viewProjMatrix, float4(position, 1));

	output.texcoord = input.texcoord.xy + input.texcoord.zw * input.corner.zw;
	output.lifetime = (time - input.time.x) / input.time.y;

	return output;
}
