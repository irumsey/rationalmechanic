float3    lightPosition;
float3     viewPosition;
float3           viewUp;
float4x4 viewProjMatrix;

struct InputVertex
{
	float2     vertex : LOCATION0;
	uint1          id : LOCATION1;
	float3   position : LOCATION2;
	float       scale : LOCATION3;
	float4   rotation : LOCATION4;
	float4      color : LOCATION5;
	float4 parameters : LOCATION6;
};

struct OutputVertex
{
	float4     ppsPosition : SV_POSITION;
	float3  lightDirection : POSITION0;
	uint1               id : POSITION1;
	float4         diffuse : COLOR0;
	float2        texcoord : TEXCOORD0;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	uint1     id : SV_TARGET2;
	float4 color : SV_TARGET0;
	float4  glow : SV_TARGET1;
};
