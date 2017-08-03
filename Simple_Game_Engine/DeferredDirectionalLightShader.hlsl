/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "DeferredShaderCommon.hlsl"
#include "PBRCommon.hlsl"

// TYPEDEFS //

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 Position : SV_Position; // vertex position 
	float2 cpPos	: TEXCOORD0;
};


//Corners of Screen//

static const float2 arrBasePos[4] = {
	float2(-1.0, 1.0),
	float2(1.0, 1.0),
	float2(-1.0, -1.0),
	float2(1.0, -1.0),
};


// CONSTANT BUFFERS //

cbuffer LightBuffer				: register(b1)
{
	float3 lightDirection		:packoffset(c0);
	float3 lightColor			:packoffset(c1);
	float3 AmbientDown			:packoffset(c2);
	float3 AmbientRange			:packoffset(c3);
};


//Deferred Ambient Light Vertex Shader//

PixelInputType DeferredDirectionalLightVertexShader(uint VertexID : SV_VertexID)
{
	PixelInputType Output;

	Output.Position = float4( arrBasePos[VertexID].xy, 0.0, 1.0);
	Output.cpPos = Output.Position.xy;

	return Output;    
}


// Ambient light calculation helper function//

float3 CalcAmbient(float3 normal, float3 color)
{
	// Convert from [-1, 1] to [0, 1]
	float up = normal.y * 0.5 + 0.5;

	// Calculate the ambient value
	float3 ambient = AmbientDown + up * AmbientRange;

	// Apply the ambient value to the color
	return ambient * color;
}


// Directional light help function//

float3 CalcDirectional(float3 position, Material material)
{
	//Calculate Vectors for Directional Light
	float3 ToEye = EyePosition.xyz - position;
	ToEye = normalize(ToEye);
	float3 HalfWay = normalize(ToEye + lightDirection);

	// Calculate Lighning Dot Products 

	float NDotL = saturate(dot(material.normal, lightDirection));
	float NDotH = saturate(dot(material.normal, HalfWay));
	float NDotV = saturate(dot(material.normal, ToEye));
	float VDotH = saturate(dot(ToEye, HalfWay));

	//Calculate Cook Torrance Lightning
	float3 finalColor = CalcCookTorrance(material, lightColor, NDotL, NDotH, NDotV, VDotH);

	return finalColor;

}


//Deferred Ambient Light Pixel Shader//

float4 DeferredDirectionalLightPixelShader(PixelInputType In) : SV_TARGET
{
	// Unpack the GBuffer
	SURFACE_DATA gbd = UnpackGBuffer_Loc(In.Position.xy);
	
	Material mat;

	// Convert the data into the material structure
	MaterialFromGBuffer(gbd, mat);

	// Reconstruct the world position
	float3 position = CalcWorldPos(In.cpPos, gbd.LinearDepth);

	// Calculate the ambient color
	float3 finalColor = CalcAmbient(mat.normal, mat.diffuseColor.rgb);

	// Calculate the directional light
	finalColor += CalcDirectional(position, mat);

	// Return the final color
	return float4(finalColor, 1.0);

}