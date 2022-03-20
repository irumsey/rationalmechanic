float1 screenWidth;
float1 screenHeight;

struct InputVertex
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
	float4       color :   TEXCOORD1;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	float4  color : SV_TARGET0;
	float4   glow : SV_TARGET1;
	uint1      id : SV_TARGET2;	//	the select target's format is UINT_R32
};
