float3    lightPosition;
float3     viewPosition;
float3           viewUp;
float4x4 viewProjMatrix;

struct InputVertex
{
	float2     vertex : LOCATION0;
	float3   position : LOCATION1;
	float       scale : LOCATION2;
	float4   rotation : LOCATION3;
	float4      color : LOCATION4;
	float4 parameters : LOCATION5;
};

struct OutputVertex
{
	float4     ppsPosition : SV_POSITION;
	float3  lightDirection : POSITION0;
	float3   viewDirection : POSITION1;
	float2        texcoord : TEXCOORD0;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	float4 color : SV_TARGET0;
	float4  glow : SV_TARGET1;
};
