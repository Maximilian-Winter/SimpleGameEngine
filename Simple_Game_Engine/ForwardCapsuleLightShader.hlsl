/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "ForwardShaderCommon.hlsl"
#include "PBRCommon.hlsl"



/////////////////////////////////////////////////////////////////////////////
// Constant Buffers
/////////////////////////////////////////////////////////////////////////////

cbuffer CapsuleLightConstants : register( b2 )
{
	float3 CapsuleLightPos		: packoffset( c0 );
	float CapsuleLightRangeRcp	: packoffset( c0.w );
	float3 CapsuleLightDir		: packoffset( c1 );
	float CapsuleLightLen		: packoffset( c1.w );
	float3 CapsuleLightColor	: packoffset( c2 );
}

/////////////////////////////////////////////////////////////////////////////
// Vertex shader
/////////////////////////////////////////////////////////////////////////////

VS_OUTPUT ForwardCapsuleLightVertexShader(VS_INPUT VertexShaderInput)
{
	VS_OUTPUT output;

	output.Position = mul(VertexShaderInput.Position, WorldViewProjection);
	float3 position = VertexShaderInput.Position.xyz;
	float2 texCoord = VertexShaderInput.UV;

	float3 normal = normalize(mul(VertexShaderInput.Normal, (float3x3)World));
	float3 worldPos = mul(float4(position.xyz, 1), World).xyz;
	output.PositionInWorld = worldPos;
	output.PositionInWorldViewProj = mul(float4(position.xyz, 1), WorldViewProjection);
	output.UV = texCoord;
	output.Normal = normal;

	return output;
}

/////////////////////////////////////////////////////////////////////////////
// Pixel shaders
/////////////////////////////////////////////////////////////////////////////

// Capsule light calculation helper function
float3 CalcCapsule(float3 position, Material material)
{	
	float3 ToEye = EyePosition.xyz - position;
	
	// Find the shortest distance between the pixel and capsules segment
	float3 ToCapsuleStart = position - CapsuleLightPos;
	float DistOnLine = dot(ToCapsuleStart, CapsuleLightDir) / CapsuleLightLen;
	DistOnLine = saturate(DistOnLine) * CapsuleLightLen;
	float3 PointOnLine = CapsuleLightPos + CapsuleLightDir * DistOnLine;
	float3 ToLight = PointOnLine - position;
	float DistToLight = length(ToLight);
	
	// Calculate Lightning Dot Products 
	ToLight /= DistToLight; // Normalize
	float NDotL = saturate(dot(ToLight, material.normal));
	ToEye = normalize(ToEye);
	
	float3 HalfWay = normalize(ToEye + ToLight);
	float NDotH = saturate(dot(HalfWay, material.normal));
	float NDotV = saturate(dot(material.normal, ToEye));
	float VDotH = saturate(dot(ToEye, HalfWay));
	
	//Calculate Cook Torrance Lightning
	float3 finalColor = CalcCookTorrance(material, CapsuleLightColor, NDotL, NDotH, NDotV, VDotH);
	
	// Attenuation
	float DistToLightNorm = 1.0 - saturate(DistToLight * CapsuleLightRangeRcp);
	float Attn = DistToLightNorm * DistToLightNorm;
	finalColor *= Attn;
   
	return finalColor;
}

float4 ForwardCapsuleLightPixelShader( VS_OUTPUT In ) : SV_TARGET0
{
	// Prepare the material structure
	Material material = PrepareMaterial(In);

	// Calculate the spot light color
	float3 finalColor = CalcCapsule(In.PositionInWorld, material);

	// Return the final color
	return float4(finalColor, material.transparency);
}