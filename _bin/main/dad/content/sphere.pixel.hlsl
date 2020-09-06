#include "sphere.header.hlsl"

Texture2D normalsTexture;
SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	///	TBD: swizzling and flipping because of the way the texture was made, fix it.
	float4  texel = normalsTexture.Sample(theSampler, input.texcoord).gbra;
	float3 normal = normalize(2 * texel.xyz - 1);
	       normal = viewRight * normal.xxx + viewUp * normal.yyy - viewForward * normal.zzz;
		 normal.z = -normal.z;
	float   shade = clamp(dot(input.lightDirection, normal), 0, 1) + input.diffuse.a;

	output.color = float4(shade * input.diffuse.rgb, texel.a);

	return output;
}
