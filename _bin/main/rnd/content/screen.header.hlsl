float2 texelSize;

struct InputVertex
{
	float2   vertex : LOCATION0;
	float2 texcoord : LOCATION1;
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

struct DoublePixel
{
	float4  color : SV_TARGET0;
	float4   glow : SV_TARGET1;
};
