float3   lightDirection;	// unit direction from view position to light position
float4x4 viewProjMatrix;

struct InputVertex
{
	float3     vertex : LOCATION0;
	float3     normal : LOCATION1;
	uint1          id : LOCATION2;
	float4   position : LOCATION3;	// (position(3), x)
	float4   rotation : LOCATION4;
	float4   channel0 : LOCATION5;	// (scale(3), x)
	float4   channel1 : LOCATION6;	// ambient
	float4   channel2 : LOCATION7;	// (light distance, x, x, x)
};

struct OutputVertex
{
	float4     ppsPosition : SV_POSITION;
	float            shade : POSITION0;
	uint1               id : POSITION1;
	float        specCoeff : COLOR0;
	float4         ambient : COLOR1;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	float4 color : SV_TARGET0;
	float4  glow : SV_TARGET1;
	uint1     id : SV_TARGET2;
};
