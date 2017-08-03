/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "DeferredShaderCommon.hlsl"
#include "PBRCommon.hlsl"

/////////////////////////////////////////////////////////////////////////////
// Constant Buffers
/////////////////////////////////////////////////////////////////////////////
cbuffer PointLightScaleMatrix				: register(b0)
{
	float4x4 worldViewProjection			: packoffset(c0);
}

cbuffer LightBufferType						: register(b1)
{
	float3 lightPosition					: packoffset(c0);
	float lightRange						: packoffset(c0.w);
	float3 lightColor						: packoffset(c1);
}

/////////////////////////////////////////////////////////////////////////////
// Vertex shader
/////////////////////////////////////////////////////////////////////////////
float4 DeferredPointLightVertexShader() : SV_Position
{
    return float4(0.0, 0.0, 0.0, 1.0); 
}

/////////////////////////////////////////////////////////////////////////////
// Hull shader
/////////////////////////////////////////////////////////////////////////////
struct HS_CONSTANT_DATA_OUTPUT
{
	float Edges[4] : SV_TessFactor;
	float Inside[2] : SV_InsideTessFactor;
};

HS_CONSTANT_DATA_OUTPUT PointLightConstantHS()
{
	HS_CONSTANT_DATA_OUTPUT Output;
	
	float tessFactor = 18.0;
	Output.Edges[0] = Output.Edges[1] = Output.Edges[2] = Output.Edges[3] = tessFactor;
	Output.Inside[0] = Output.Inside[1] = tessFactor;

	return Output;
}

struct HS_OUTPUT
{
	float3 HemiDir : POSITION;
};

static const float3 HemilDir[2] = {
	float3(1.0, 1.0,1.0),
	float3(-1.0, 1.0, -1.0)
};

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PointLightConstantHS")]
HS_OUTPUT DeferredPointLightHullShader(uint PatchID : SV_PrimitiveID)
{
	HS_OUTPUT Output;

	Output.HemiDir = HemilDir[PatchID];

	return Output;
}

/////////////////////////////////////////////////////////////////////////////
// Domain Shader shader
/////////////////////////////////////////////////////////////////////////////
struct DS_OUTPUT
{
	float4 Position : SV_POSITION;
	float2 cpPos	: TEXCOORD0;
};

[domain("quad")]
DS_OUTPUT DeferredPointLightDomainShader(HS_CONSTANT_DATA_OUTPUT input, float2 UV : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 4> quad)
{
	// Transform the UV's into clip-space
	float2 posClipSpace = UV.xy * 2.0 - 1.0;

	// Find the absulate maximum distance from the center
	float2 posClipSpaceAbs = abs(posClipSpace.xy);
	float maxLen = max(posClipSpaceAbs.x, posClipSpaceAbs.y);

	// Generate the final position in clip-space
	float3 normDir = normalize(float3(posClipSpace.xy, (maxLen - 1.0)) * quad[0].HemiDir);
	float4 posLS = float4(normDir.xyz, 1.0);
	
	// Transform all the way to projected space
	DS_OUTPUT Output;
	Output.Position = mul( posLS, worldViewProjection );

	// Store the clip space position
	Output.cpPos = Output.Position.xy / Output.Position.w;

	return Output;
}

/////////////////////////////////////////////////////////////////////////////
// Pixel shader
/////////////////////////////////////////////////////////////////////////////

float3 CalcPoint(float3 position, Material material, bool bUseShadow)
{
	float3 L = lightPosition - position;
	float DistToLight = length(L);
	L /= DistToLight;									// Normalize
	
	float3 V = EyePosition - position;
	V = normalize(V);

	float3 H = normalize(V + L);
	
	// Calculate Lighning Dot Products 
	float NDotL = saturate(dot(material.normal, L));
	float NDotH = saturate(dot(material.normal, H));
	float NDotV = saturate(dot(material.normal, V));
	float VDotH = saturate(dot(V, H));

	//Calculate Cook Torrance Lightning
	float3 finalColor = CalcCookTorrance(material, lightColor, NDotL, NDotH, NDotV, VDotH);

	// Attenuation
	float DistToLightNorm = 1.0 - saturate(DistToLight * lightRange);
	float Attn = DistToLightNorm * DistToLightNorm;
	finalColor *= Attn ;
   
	return finalColor;
}

float4 DeferredPointLightPixelShader(DS_OUTPUT In) : SV_TARGET
{

	bool bUseShadow = false;

	// Unpack the GBuffer
	SURFACE_DATA gbd = UnpackGBuffer_Loc(In.Position.xy);
	
	// Convert the data into the material structure
	Material mat;
	MaterialFromGBuffer(gbd, mat);

	// Reconstruct the world position
	float3 position = CalcWorldPos(In.cpPos, gbd.LinearDepth);

	// Calculate the light contribution
	float3 finalColor = CalcPoint(position, mat, bUseShadow);

	float3 finalColorDepth = float4(1.0 - saturate(gbd.LinearDepth / 75.0), 1.0 - saturate(gbd.LinearDepth / 125.0), 1.0 - saturate(gbd.LinearDepth / 200.0), 0.0);
	
	// return the final color
	return float4(finalColor, 1.0);
}

