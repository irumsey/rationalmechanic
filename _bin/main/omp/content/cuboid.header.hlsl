float3    lightPosition;
float3     viewPosition;
float4x4 viewProjMatrix;

struct InputVertex
{
	float3     vertex : LOCATION0;
	float3     normal : LOCATION1;
	uint1          id : LOCATION2;
	float3   position : LOCATION3;
	float       scale : LOCATION4;
	float4   rotation : LOCATION5;
	float4      color : LOCATION6;
	float4 parameters : LOCATION7;
};

struct OutputVertex
{
	float4     ppsPosition : SV_POSITION;
	float            shade : POSITION0;
	uint1               id : POSITION1;
	float4         diffuse : COLOR0;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	float4 color : SV_TARGET0;
	float4  glow : SV_TARGET1;
	uint1     id : SV_TARGET2;
};
