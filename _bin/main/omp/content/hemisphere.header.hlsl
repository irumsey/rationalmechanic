float3    lightPosition;
float3     viewPosition;
float3           viewUp;
float4x4 viewProjMatrix;

struct InputVertex
{
	float3     vertex : LOCATION0;
	float2   texcoord : LOCATION1;
	float3   position : LOCATION2;
	float       scale : LOCATION3;
	float4   rotation : LOCATION4;
	float4      color : LOCATION5;
	float4 parameters : LOCATION6;
};

struct OutputVertex
{
	float4     ppsPosition : SV_POSITION;
	float3          normal : POSITION0;
	float3  lightDirection : POSITION1;
	float4         diffuse : COLOR0;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	float4 color : SV_TARGET0;
	float4  glow : SV_TARGET1;
};
