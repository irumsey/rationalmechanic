float2 texelSize;

struct InputVertex
{
	float2      vertex : LOCATION0;
	uint1           id : LOCATION1;
	float4    position : LOCATION2;	// (xmin, ymin), (xmax, ymax)
	float4    texcoord : LOCATION3;	// (umin, vmin), (umax, ymax)
	float4       color : LOCATION4;
};
   
struct OutputVertex
{
	float4 ppsPosition : SV_POSITION;
	uint1           id : POSITION0;
	float2    texcoord : TEXCOORD0;
	float4       color : COLOR0;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	uint1      id : SV_TARGET0;
	float4  color : SV_TARGET1;
	float4   glow : SV_TARGET2;
};
