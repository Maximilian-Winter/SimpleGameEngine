/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "ForwardShaderCommon.hlsl"
#include "PBRCommon.hlsl"

/////////////////////////////////////////////////////////////////////////////
// Constant Buffers
/////////////////////////////////////////////////////////////////////////////

cbuffer LightBuffer					: register(b2)
{
	float3 SpotLightPos				: packoffset(c0);
	float SpotLightRangeRcp			: packoffset(c0.w);
	float3 SpotDirToLight			: packoffset(c1);
	float SpotCosOuterCone			: packoffset(c1.w);
	float3 SpotColor				: packoffset(c2);
	float SpotCosConeAttRange		: packoffset(c2.w);
}

/////////////////////////////////////////////////////////////////////////////
// Vertex shader
/////////////////////////////////////////////////////////////////////////////

VS_OUTPUT ForwardSpotLightVertexShader(VS_INPUT VertexShaderInput)
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

// Spot light calculation helper function
float3 CalcSpot(float3 position, Material material)
{

	float3 ToLight = SpotLightPos - position;
	float3 ToEye = EyePosition.xyz - position;
	ToEye = normalize(ToEye);
	float DistToLight = length(ToLight);

	// Calculate Lightning Dot Products 
	ToLight /= DistToLight;										// Normalize
	float NDotL = saturate(dot(material.normal, ToLight));
	float3 HalfWay = normalize(ToEye + ToLight);

	float NDotH = saturate(dot(material.normal, HalfWay));
	float NDotV = saturate(dot(material.normal, ToEye));
	float VDotH = saturate(dot(ToEye, HalfWay));

	float3 finalColor = CalcCookTorrance(material, SpotColor, NDotL, NDotH, NDotV, VDotH);
   
	// Cone attenuation
	float cosAng = dot(SpotDirToLight, ToLight);
	float conAtt = saturate((cosAng - SpotCosOuterCone) / SpotCosConeAttRange);
	conAtt *= conAtt;
   
	// Attenuation
	float DistToLightNorm = 1.0 - saturate(DistToLight * SpotLightRangeRcp);
	float Attn = DistToLightNorm * DistToLightNorm;
	finalColor *= Attn * conAtt;

   
	return finalColor;
}

float4 ForwardSpotLightPixelShader( VS_OUTPUT In ) : SV_TARGET0
{
	// Prepare the material structure
	Material material = PrepareMaterial(In);

	// Calculate the spot light color
	float3 finalColor = CalcSpot(In.PositionInWorld, material);

	// Return the final color
	return float4(finalColor, material.transparency);
}