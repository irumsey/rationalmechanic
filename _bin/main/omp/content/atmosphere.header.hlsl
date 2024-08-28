float3      lightDirection;	// unit direction from view position to light position
float4x4    viewProjMatrix;

struct InputVertex
{
	float3     vertex : LOCATION0;
	float3    tangent : LOCATION1;
	float2   texcoord : LOCATION2;
	uint1          id : LOCATION3;
	float4   position : LOCATION4;	// ( position(3), planet radius )
	float4   rotation : LOCATION5;
	float4   channel0 : LOCATION6;
	float4   channel1 : LOCATION7;
	float4   channel2 : LOCATION8;	// ( light distance, thickness, x, x )
};

struct OutputVertex
{
	float4     ppsPosition : SV_POSITION;
	float3    planetCenter : POSITION0;
	float2           radii : POSITION1;	// ( radius of planet, radius of atmosphere )
	float3          normal : POSITION2;
	float3			  beta : POSITION3;
	float3   viewDirection : POSITION4;	// from vertex position to view position (0, 0, 0)
	float3  lightDirection : POSITION5;	// from vertex position to light position
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	float4 color : SV_TARGET0;
	float4  glow : SV_TARGET1;
};
