float3       lightPosition;
float3        viewPosition;
float3              viewUp;
float4x4    viewProjMatrix;
float4x4 invViewProjMatrix;

struct InputVertex
{
	float2     vertex : LOCATION0;
	float3   position : LOCATION1;
	float       scale : LOCATION2;
	float4   rotation : LOCATION3;
	float4      color : LOCATION4;
	float4 parameters : LOCATION5;
};

struct OutputVertex
{
	float4     ppsPosition : SV_POSITION;		///	in pixel shader this results in screen space (requiriing screen dimensions)
	float4       clipSpace :   POSITION0;		///	making a copy of what is written into ppsPosition (will remain -1, 1)
	float3  lightDirection :   POSITION1;		///	in world space
	float3          center :   POSITION4;		///	center of planet and atmosphere in world space
 	float3           radii :   POSITION5;		///	x,y are the radii of planet and atmosphere respectively. z is maximum distance a ray can travel through atmos
	float2        texcoord :   TEXCOORD0;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	float4 color : SV_TARGET0;
	float4  glow : SV_TARGET1;
};

float phase(float cos_theta)
{
 	return 0.5 * ((1 + cos_theta * cos_theta) * pow(0.75 / (1.25 - 0.5 * cos_theta), 1.5));
}
