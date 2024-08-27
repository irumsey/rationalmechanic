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
	float4    diffuse : LOCATION6;
	float4    ambient : LOCATION7;
	float4 parameters : LOCATION8;	// ( scaleFactor, thickness, x, x )
};

struct OutputVertex
{
	float4     ppsPosition : SV_POSITION;
	float3    planetCenter : POSITION0;
	float2           radii : POSITION1;	// ( radius of planet, radius of atmosphere )
	float3          normal : POSITION2;
	float3   viewDirection : POSITION3;	// from vertex position to view position (0, 0, 0)
	float3  lightDirection : POSITION4;	// from vertex position to light position
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	float4 color : SV_TARGET0;
	float4  glow : SV_TARGET1;
};

float rayleighScatter(float cosTheta)
{
	return 0.75 * (1 + cosTheta * cosTheta);
}

float rayleighScatter(float3 viewDirection, float3 lightDirection)
{
	return rayleighScatter(dot(viewDirection, lightDirection));
}

float phase(float cosTheta)
{
 	return 0.5 * ((1 + cosTheta * cosTheta) * pow(0.75 / (1.25 - 0.5 * cosTheta), 1.5));
}
