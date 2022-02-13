float              time;
float       interpolant;
float3    lightPosition;
float3     viewPosition;
float3           viewUp;
float4x4 viewProjMatrix;

struct InputVertex
{
	float2     vertex : LOCATION0;
	float3  position0 : LOCATION1;
	float3  position1 : LOCATION2;
	float2      scale : LOCATION3;
	float4    diffuse : LOCATION4;
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
