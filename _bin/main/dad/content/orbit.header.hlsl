float            hu;
float  eccentricity;
float2       domain;
float     lineWidth;
float4    lineColor;

float4x4 worldMatrix;
float4x4 viewProjMatrix;

struct InputVertex
{
	float2 position : LOCATION0;
	float2  adapted : LOCATION1;
	float2   select : LOCATION2;
};

struct OutputVertex
{
	float2    position :   POSITION0;
	float2      normal :   POSITION1;
	float2      vertex :   POSITION2;
	float4 ppsPosition : SV_POSITION;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	float4 color : SV_TARGET0;
};

float2 computeConicPoint(float theta)
{
	float c = cos(theta);
	float s = sin(theta);
	float r = hu / (1 + eccentricity * c);

	return r * float2(c, s);
}
