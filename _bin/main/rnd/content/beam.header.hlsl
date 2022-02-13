float3     viewPosition;
float4x4 viewProjMatrix;

struct InputVertex
{
	float3    select0 : LOCATION0;
	float3    select1 : LOCATION1;
	float2   texcoord : LOCATION2;
	float3  position0 : LOCATION3;
	float3  position1 : LOCATION4;
	float4    diffuse : LOCATION5;
};

struct OutputVertex
{
	float4     ppsPosition : SV_POSITION;
	float4         diffuse : COLOR0;
	float2        texcoord : TEXCOORD0;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	float4 color : SV_TARGET0;
};
