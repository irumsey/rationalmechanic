#include "screen.header.hlsl"

Texture2D colorTarget;
Texture2D  glowTarget;

SamplerState theSampler;

SinglePixel main(InputPixel input)
{
	SinglePixel output = (SinglePixel)0;

	//
	//	adapted from an ogl version seen on stack overflow
	//

	float FXAA_SPAN_MAX = 8.0;
	float FXAA_REDUCE_MUL = 1.0 / 8.0;
	float FXAA_REDUCE_MIN = 1.0 / 128.0;

	float3 rgbNW = colorTarget.Sample(theSampler, input.texcoord + float2(-texelSize.x, -texelSize.y)).rgb;
	float3 rgbNE = colorTarget.Sample(theSampler, input.texcoord + float2( texelSize.x, -texelSize.y)).rgb;
	float3 rgbSW = colorTarget.Sample(theSampler, input.texcoord + float2(-texelSize.x,  texelSize.y)).rgb;
	float3 rgbSE = colorTarget.Sample(theSampler, input.texcoord + float2( texelSize.x,  texelSize.y)).rgb;
	float3 rgbM  = colorTarget.Sample(theSampler, input.texcoord).rgb;

	float3 luma = float3(0.299, 0.587, 0.114);
	float lumaNW = dot(rgbNW, luma);
	float lumaNE = dot(rgbNE, luma);
	float lumaSW = dot(rgbSW, luma);
	float lumaSE = dot(rgbSE, luma);
	float lumaM  = dot( rgbM, luma);

	float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
	float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

	float2 dir;
	dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
	dir.y = ((lumaNW + lumaSW) - (lumaNE + lumaSE));

	float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
	float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);

	dir = min(float2(FXAA_SPAN_MAX, FXAA_SPAN_MAX), max(float2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), dir * rcpDirMin)) * texelSize;

	float3 rgbA = (1.0 / 2.0) * (
		colorTarget.Sample(theSampler, input.texcoord + dir * (1.0 / 3.0 - 0.5)).rgb +
		colorTarget.Sample(theSampler, input.texcoord + dir * (2.0 / 3.0 - 0.5)).rgb);
	float3 rgbB = rgbA * (1.0 / 2.0) + (1.0 / 4.0) * (
		colorTarget.Sample(theSampler, input.texcoord + dir * (0.0 / 3.0 - 0.5)).rgb +
		colorTarget.Sample(theSampler, input.texcoord + dir * (3.0 / 3.0 - 0.5)).rgb);
	float lumaB = dot(rgbB, luma);


	float4 color = (lumaB < lumaMin) ? float4(rgbA, 1) : float4(rgbB, 1);
	float4 glow = glowTarget.Sample(theSampler, input.texcoord);

	output.color = float4(color.rgb + glow.rgb, 1);

	return output;
}

