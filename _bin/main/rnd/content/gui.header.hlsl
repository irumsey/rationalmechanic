float2 screenSize;
float2 texelSize;

struct InputVertex
{
	float2      vertex : LOCATION0;
	float4    position : LOCATION1;	// (xmin, ymin), (xmax, ymax)
	float4    texcoord : LOCATION2;	// (umin, vmin), (umax, ymax)
	float4       color : LOCATION3;
};
   
struct OutputVertex
{
	float4 ppsPosition : SV_POSITION;
	float2    texcoord : TEXCOORD0;
	float4       color : COLOR0;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	float4       color : SV_TARGET0;
	float4        glow : SV_TARGET1;
};
