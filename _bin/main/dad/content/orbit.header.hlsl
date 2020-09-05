float3 viewPosition;
float4x4 viewProjMatrix;

struct InputVertex
{
	float2     vertex : LOCATION0;
	float2    adapted : LOCATION1;
	float2     select : LOCATION2;
	float3      focus : LOCATION3;
	float       scale : LOCATION4;
	float4   rotation : LOCATION5;
	float4      color : LOCATION6;
	float4 parameters : LOCATION7;
};

struct OutputVertex
{
	float4  parameters :   POSITION0;
	float3    position :   POSITION1;
	float4    rotation :   POSITION2;
	float3      vertex :   POSITION3;
	float4       color :      COLOR0;
	float4 ppsPosition : SV_POSITION;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	float4 color : SV_TARGET0;
};

float2 computeConicPoint(float hu, float e, float theta)
{
	float c = cos(theta);
	float s = sin(theta);
	float r = hu / (1 + e * c);

	return r * float2(c, s);
}
