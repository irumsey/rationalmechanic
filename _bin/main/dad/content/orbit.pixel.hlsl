float         hu;
float          e;
float  lineWidth;
float4 lineColor;

struct InputPixel
{
	float2    position :   POSITION0;
	float2      normal :   POSITION1;
	float4 ppsPosition : SV_POSITION;
};

struct OutputPixel
{
	float4 color : SV_TARGET0;
};

float2 computeConicPoint(float theta)
{
	float c = cos(theta);
	float s = sin(theta);
	float r = hu / (1 + e * c);

	return r * float2(c, s);
}

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	output.color = float4(lineColor.rgb, 1);

	return output;
}

