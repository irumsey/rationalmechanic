#include "instance.header.hlsl"

float4x4    viewProjMatrix;

struct Vertex
{
	float3   normal : LOCATION0;
	float3  tangent : LOCATION1;
	float2 texcoord : LOCATION2;
};

struct InputVS
{
	Vertex      vertex;
	Instance  instance;
};

struct OutputVS
{
	float4     ppsPosition : SV_POSITION;
	float3    planetCenter : POSITION0;
	float4           radii : POSITION1;	// ( radius of planet, radius of atmosphere, H, D^-1 )
	float3          normal : POSITION2;
	float3			  beta : POSITION3;
	float3   viewDirection : POSITION4;	// from vertex position to view position (0, 0, 0)
	float3  lightDirection : POSITION5;	// from vertex position to light position
};

typedef OutputVS InputPS;

struct OutputPixel
{
	float4 color : SV_TARGET0;
	float4  glow : SV_TARGET1;
};
