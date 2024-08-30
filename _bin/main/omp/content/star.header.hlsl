float   starFieldRadius;
float        starScale;

float3     viewPosition;
float3           viewUp;
float4x4 viewProjMatrix;

struct InputVertex
{
	float2     vertex : LOCATION0;
	uint1          id : LOCATION1;
	float4 parameters : LOCATION2;
	float4      color : LOCATION3;
};

struct OutputVertex
{
	float4 ppsPosition : SV_POSITION;
	uint1           id :   POSITION0;
	float4       color :      COLOR0;
	float2    texcoord :   TEXCOORD1;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	uint       id : SV_TARGET2;
	float4  color : SV_TARGET0;
	float4   glow : SV_TARGET1;
};

float3 computeRelativeDirection(float ra, float dec)
{
	float cos_dec = cos(dec);
	return float3(cos(ra) * cos_dec, sin(ra) * cos_dec, sin(dec));
}