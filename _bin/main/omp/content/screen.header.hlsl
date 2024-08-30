float2 texelSize;

struct InputVertex
{
	float2   vertex : LOCATION0;
	float2 texcoord : LOCATION1;
};

struct NineSliceVertex
{
	float4    select : LOCATION0;
	float2  position : LOCATION1;
	float4 dimension : LOCATOIN2;
	float4     color : LOCATION3;
};

struct OutputVertex
{
	float4 ppsPosition : SV_POSITION;
	float2    texcoord :   TEXCOORD0;
};

typedef OutputVertex InputPixel;

struct SinglePixel
{
	float4 color : SV_TARGET0;
};

struct TriplePixel
{
	uint1      id : SV_TARGET2;	//	the select target's format is UINT_R32
	float4  color : SV_TARGET0;
	float4   glow : SV_TARGET1;
};

struct QuadPixel
{
	uint1     id : SV_TARGET2;
	float4 color : SV_TARGET0;
	float4  glow : SV_TARGET1;
	float  depth : SV_TARGET3;
};