/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "ForwardShaderCommon.hlsl"
#include "PBRCommon.hlsl"



/////////////////////////////////////////////////////////////////////////////
// Constant Buffers
/////////////////////////////////////////////////////////////////////////////
cbuffer LightBuffer					: register(b2) // Point light constants
{
	float3 PointLightPosition		: packoffset(c0);
	float PointLightRangeRcp		: packoffset(c0.w);
	float3 PointLightColor			: packoffset(c1);
}


/////////////////////////////////////////////////////////////////////////////
// Pixel shaders
/////////////////////////////////////////////////////////////////////////////

// Point light calculation helper function
float3 CalcPoint(float3 position, Material material)
{
	float3 L = PointLightPosition - position;
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
	float3 finalColor = CalcCookTorrance(material, PointLightColor, NDotL, NDotH, NDotV, VDotH);
   
	// Attenuation
	float DistToLightNorm = 1.0 - saturate(DistToLight * PointLightRangeRcp);
	float Attn = DistToLightNorm * DistToLightNorm;
	finalColor *= Attn;
   
	return finalColor;
}


float4 ForwardPointLightPixelShader(VS_OUTPUT In) : SV_TARGET0
{
	// Prepare the material structure
	Material material = PrepareMaterial(In);

	// Calculate the point light color
	float3 finalColor = CalcPoint(In.PositionInWorld, material);

	// Return the final color
	return float4(finalColor, material.transparency);
}