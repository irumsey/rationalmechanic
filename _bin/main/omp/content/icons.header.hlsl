float2 texelSize;

float4x4 viewProjMatrix;

struct InputVertex
{
	float2     vertex : LOCATION0;
	uint1          id : LOCATION1;
	float3   position : LOCATION2;	// screen position
	float4   texcoord : LOCATION3;	// texcoord origin, width, height
	float2      scale : LOCATION4;
	float4      color : LOCATION5;
};
   
struct OutputVertex
{
	float4     ppsPosition : SV_POSITION;
	uint1               id : POSITION1;
	float4           color : COLOR0;
	float2        texcoord : TEXCOORD0;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	float4 color : SV_TARGET0;
	float4  glow : SV_TARGET1;
	uint1     id : SV_TARGET2;
};
