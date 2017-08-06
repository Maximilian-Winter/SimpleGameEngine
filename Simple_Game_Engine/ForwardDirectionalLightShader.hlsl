/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "ForwardShaderCommon.hlsl"
#include "PBRCommon.hlsl"


/////////////////////////////////////////////////////////////////////////////
// Constant Buffers
/////////////////////////////////////////////////////////////////////////////
cbuffer LightBuffer					: register(b2)  // Directional and ambient light constants
{
	float3 TolightDirection			: packoffset(c0);
	float3 lightColor				: packoffset(c1);
	float3 AmbientDown				: packoffset(c2);
	float3 AmbientRange				: packoffset(c3);
}

/////////////////////////////////////////////////////////////////////////////
// Pixel shaders
/////////////////////////////////////////////////////////////////////////////

// Ambient light calculation helper function
float3 CalcAmbient(float3 normal, float3 diffuseColor)
{
	// Convert from [-1, 1] to [0, 1]
	float up = normal.y * 0.5 + 0.5;

	// Calculate the ambient value
	float3 ambient = AmbientDown + up * AmbientRange;

	// Apply the ambient value to the color
	return ambient * diffuseColor;
}


// Directional light calculation helper function
float3 CalcDirectional(float3 position, Material material)
{
	//Calculate Vectors for Directional Light
	float3 ToEye = EyePosition.xyz - position;
	ToEye = normalize(ToEye);
	float3 HalfWay = normalize(ToEye + TolightDirection);

	// Calculate Lightning Dot Products 

	float NDotL = saturate(dot(material.normal, TolightDirection));
	float NDotH = saturate(dot(material.normal, HalfWay));
	float NDotV = saturate(dot(material.normal, ToEye));
	float VDotH = saturate(dot(ToEye, HalfWay));

	//Calculate Cook Torrance Lightning
	float3 finalColor = CalcCookTorrance(material, lightColor, NDotL, NDotH, NDotV, VDotH);

	return finalColor;
}

float4 ForwardDirectionalLightPixelShader(VS_OUTPUT In) : SV_TARGET0
{
	// Prepare the material structure
	Material material = PrepareMaterial(In);

	// Calculate the ambient color
	float3 finalColor = CalcAmbient(material.normal, material.diffuseColor.rgb);
	
	// Calculate the directional light
	finalColor += CalcDirectional(In.PositionInWorld, material);

	// Return the final color
	return float4(finalColor, material.transparency);
}