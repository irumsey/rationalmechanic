float3 viewUp;
float3 viewPosition;
float4x4 viewProjMatrix;
float1 segCoeff;
float2 fadeDistance;

struct InputVertex
{
	float4     vertex : LOCATION0;
	float3      focus : LOCATION1;
	float       scale : LOCATION2;
	float4   rotation : LOCATION3;
	float4      color : LOCATION4;
	float4 parameters : LOCATION5;
};

struct OutputVertex
{
	float4 ppsPosition : SV_POSITION;
	float     distance :   TEXCOORD0;
	float        width :   TEXCOORD1;
	float4       color :      COLOR0;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	float4 color : SV_TARGET0;
	float4  glow : SV_TARGET1;
};

float2 computeConicPoint(float hu, float e, float theta)
{
	float c = cos(theta);
	float s = sin(theta);
	float r = hu / (1 + e * c);

	return r * float2(c, s);
}
