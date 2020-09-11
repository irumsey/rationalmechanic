float      sphereRadius;

float3     viewPosition;
float3           viewUp;
float4x4 viewProjMatrix;

struct InputVertex
{
	float2     vertex : LOCATION0;
	float4 parameters : LOCATION1;
};

struct OutputVertex
{
	float2    texcoord :   TEXCOORD0;
	float     specoord :   TEXCOORD1;
	float4 ppsPosition : SV_POSITION;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	float4 color : SV_TARGET0;
};

float3 computeRelativeDirection(float ra, float dec)
{
	float cos_dec = cos(dec);
	return float3(cos(ra) * cos_dec, sin(ra) * cos_dec, sin(dec));
}