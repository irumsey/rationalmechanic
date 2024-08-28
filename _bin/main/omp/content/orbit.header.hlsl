float2 texelSize;

float4x4 viewMatrix;
float4x4 projMatrix;

struct InputVertex
{
	float3     vertex : LOCATION0;
	uint1          id : LOCATION1;
	float4   position : LOCATION2;	// focus.w is scale
	float4   rotation : LOCATION3;
	float4   channel0 : LOCATION4;
	float4   channel1 : LOCATION5;
	float4   channel2 : LOCATION6;
};

struct OutputVertex
{
	float4 ppsPosition : SV_POSITION;
	uint1           id :   POSITION0;
	float4       color :      COLOR0;
	float        blend :   TEXCOORD0;
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

float3 computeConics(float hu, float e, float theta)
{
	float c = cos(theta);
	float s = sin(theta);
	float r = hu / (1 + e * c);

	return float3(c, s, r);
}
