#include "instance.header.hlsl"

float4x4     viewProjMatrix;

struct Vertex
{
	float3 position : LOCATION0;
	float3   normal : LOCATION1;
};

struct InputVS
{
	Vertex      vertex;
	Instance  instance;
};

struct OutputVS
{
	float4     ppsPosition : SV_POSITION;
	float            shade : POSITION0;
	uint1               id : POSITION1;
	float        specCoeff : COLOR0;
	float4         ambient : COLOR1;
	float            depth : POSITION2;
};

typedef OutputVS InputPS;

struct OutputPixel
{
	uint1     id : SV_TARGET2;
	float4 color : SV_TARGET0;
	float4  glow : SV_TARGET1;
	float  depth : SV_TARGET3;
};
