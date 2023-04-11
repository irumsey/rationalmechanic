float3 viewUp;
float3 viewPosition;
float4x4 viewProjMatrix;
float1 segCoeff;
float2 fadeDistance;

struct InputVertex
{
	float4     vertex : LOCATION0;
	uint1          id : LOCATION1;
	float3      focus : LOCATION2;
	float       scale : LOCATION3;
	float4   rotation : LOCATION4;
	float4      color : LOCATION5;
	float4 parameters : LOCATION6;
};

struct OutputVertex
{
	float4 ppsPosition : SV_POSITION;
	uint1           id :   POSITION0;
	float     distance :   TEXCOORD0;
	float        width :   TEXCOORD1;
	float4       color :      COLOR0;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	float4 color : SV_TARGET0;
	float4  glow : SV_TARGET1;
	uint1     id : SV_TARGET2;
};

float2 computeConicPoint(float hu, float e, float theta)
{
	float c = cos(theta);
	float s = sin(theta);
	float r = hu / (1 + e * c);

	return r * float2(c, s);
}
