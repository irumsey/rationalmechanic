Texture2D diffuseTexture;
Texture2D lookupTexture;

SamplerState theSampler;

struct InputPixel
{
	float4 ppsPosition : SV_POSITION;
	float2    texcoord : TEXCOORD0;
	float     lifetime : TEXCOORD1;
};

struct OutputPixel
{
	float4 color : SV_TARGET0;
};
 
OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float4 diffuse = diffuseTexture.Sample(theSampler, input.texcoord);
	float4 lookup = lookupTexture.Sample(theSampler, float2(input.lifetime, 0));

	output.color.rgb = diffuse.rgb + lookup.rgb;
	output.color.a = diffuse.a * lookup.a;

	return output;
}
