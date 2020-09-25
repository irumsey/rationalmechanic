float4x4 viewProjMatrix;

struct InputVertex
{
	float3     vertex : LOCATION0;
	float3   position : LOCATION1;
	float       scale : LOCATION2;
	float4   rotation : LOCATION3;
};

struct OutputVertex
{
	float4     ppsPosition : SV_POSITION;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	float4 color : SV_TARGET0;
	float4  glow : SV_TARGET1;
};
