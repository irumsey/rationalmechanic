#include "instance.header.hlsl"

float2 texelSize;

float4x4 viewProjMatrix;

struct Vertex
{
	float3 parameters : LOCATION0;
};

struct InputVS
{
	Vertex     vertex;
	Instance instance;
};

struct OutputVS
{
	float4 ppsPosition : SV_POSITION;
	uint1           id :   POSITION0;
	float4       color :      COLOR0;
	float        blend :   TEXCOORD0;
};

typedef OutputVS InputPS;

struct OutputPixel
{
	uint1     id : SV_TARGET2;
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

float3 computeConics(float hu, float e, float theta)
{
	float c = cos(theta);
	float s = sin(theta);
	float r = hu / (1 + e * c);

	return float3(c, s, r);
}
