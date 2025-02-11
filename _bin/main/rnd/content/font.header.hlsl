float2 texelSize;

struct InputVertex
{
	float4      vertex : LOCATION0;
	float4    position : LOCATION1;
	float4    texcoord : LOCATION2;
	float4     channel : LOCATION3;
	float4       color : LOCATION4;
};

struct OutputVertex
{
	float4 ppsPosition : SV_POSITION;
	float2    texcoord : TEXCOORD0;
	float4     channel : COLOR0;
	float4       color : COLOR1;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	uint1      id : SV_TARGET0;
	float4  color : SV_TARGET1;
	float4   glow : SV_TARGET2;
};
