float radius;
float width;
float innerEdge; /// = radius - 0.5 * width

Texture2D lookupTexture;

SamplerState theSampler;

struct InputPixel
{
	float2 localPosition :   POSITION0;
	float4   ppsPosition : SV_POSITION;
};

struct OutputPixel
{
	float4 color : SV_TARGET0;
};

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float u = clamp((length(input.localPosition) - innerEdge) / width, 0, 1);
	float alpha = lookupTexture.Sample(theSampler, float2(u, 0)).r;

	output.color = float4(0, 0, 0.5, alpha);

	return output;
}

