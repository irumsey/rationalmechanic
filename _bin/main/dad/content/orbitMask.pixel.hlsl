struct InputPixel
{
	float4 ppsPosition : SV_POSITION;
};

struct OutputPixel
{
	float4 color : SV_TARGET0;
};

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;
	output.color = float4(1, 1, 1, 1);
	return output;
}
