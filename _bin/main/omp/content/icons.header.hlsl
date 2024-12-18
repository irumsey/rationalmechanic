float2 texelSize;

float4x4 viewProjMatrix;

struct InputVertex
{
	float2     vertex : LOCATION0;
	uint1          id : LOCATION1;
	float4   position : LOCATION2;	// screen position, depth scale factor
	float4   texcoord : LOCATION3;	// texcoord origin, width, height
	float2      scale : LOCATION4;	// screen size
	float4      color : LOCATION5;
};
   
struct OutputVertex
{
	float4     ppsPosition : SV_POSITION;
	uint1               id : POSITION0;
	float3 depthParameters : POSITION1;
	float4           color : COLOR0;
	float2        texcoord : TEXCOORD0;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	uint1     id : SV_TARGET2;
	float4 color : SV_TARGET0;
	float4  glow : SV_TARGET1;
};
