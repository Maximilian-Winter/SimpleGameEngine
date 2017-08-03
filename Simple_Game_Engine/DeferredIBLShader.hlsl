//IBL Shader
#include "DeferredShaderCommon.hlsl"

//Texture for IBL Lightning
TextureCube diffuseProbe						: register(t4);
TextureCube specularProbe						: register(t5);
Texture2D specularIntegration					: register(t6);

//Sample States for IBL Lightning
SamplerState anisotropicSampler					: register(s1);
SamplerState clampSampler						: register(s2);

//Typedefs and quad definiton
struct VS_OUTPUT
{
	float4 Position : SV_Position; // vertex position 
	float2 cpPos	: TEXCOORD0;
};

static const float2 arrBasePos[4] = {
	float2(-1.0, 1.0),
	float2(1.0, 1.0),
	float2(-1.0, -1.0),
	float2(1.0, -1.0),
};

VS_OUTPUT DeferredIBLVertexShader(uint VertexID : SV_VertexID)
{
	VS_OUTPUT Output;

	Output.Position = float4(arrBasePos[VertexID].xy, 0.0, 1.0);
	Output.cpPos = Output.Position.xy;

	return Output;
}

float RoughnessToMipMapLevel(float roughness)
{
	// Calculate Mip Level
	int mipLevels, width, height;
	specularProbe.GetDimensions(0, width, height, mipLevels);

	float MipMapLevel = mipLevels * roughness;

	return MipMapLevel;

}

float4 shade(
float vdotn,
float roughness,
float metalness,
float4 diffuseColor,
float4 diffuseIBL,
float4 specularIBL,
float3 normal)
{

	float4 brdfTerm = specularIntegration.SampleLevel(clampSampler, float2(vdotn,  roughness), 0);
	float3 metalSpecularIBL = specularIBL.rgb;

	float3 dielectricColor = float3(0.04, 0.04, 0.04);
	float3 diffColor = diffuseColor.rgb * (1.0 - metalness);
	float3 specColor = lerp(dielectricColor.rgb, diffuseColor.rgb, metalness);

	float3 albedoByDiffuse = diffColor.rgb * diffuseIBL.rgb;

	float4 litColor;
	litColor.rgb = (albedoByDiffuse.rgb + (metalSpecularIBL * (specColor * brdfTerm.x + (brdfTerm.y))));
	litColor.a = 1.0;

	return litColor;
}

float4 DeferredIBLPixelShader(VS_OUTPUT vertexShaderOut) : SV_TARGET0
{
	// Unpack the GBuffer
	SURFACE_DATA gbd = UnpackGBuffer_Loc(vertexShaderOut.Position.xy);

	Material material;

	// Convert the data into the material structure
	MaterialFromGBuffer(gbd, material);

	if(gbd.Color.r == 0.0f && gbd.Color.g == 0.0f && gbd.Color.b == 0.0f && gbd.roughness == 0.0f && gbd.metalness == 0.0f)
	{ 
		return (0.0f, 0.0f, 0.0f, 0.0f);
	}
	else
	{
		// Reconstruct the world position
		float3 position = CalcWorldPos(vertexShaderOut.cpPos, gbd.LinearDepth);

		float4 diffuseColor = material.diffuseColor;

		float3 normal = material.normal;

		// view, vdotn and reflection.
		float3 view = normalize(EyePosition.xyz - position.xyz);
		float3 reflection = normalize(reflect(-view, normalize(normal)));
		float  vdotn = (dot(view, normal));

		float roughness = material.roughness;
		float metalness = material.metalness;

		// Diffuse Probe. Convert Irradiance to Radiance.
		float4 diffuseIBL = ((diffuseProbe.SampleLevel(anisotropicSampler, normal, 0.0f) / 3.14159f));

		// Specular Probe.
		float specMipLevel = RoughnessToMipMapLevel(roughness);

		float4 specularIBL = specularProbe.SampleLevel(anisotropicSampler, reflection, specMipLevel);

		float4 output = shade(vdotn, roughness, metalness, diffuseColor, diffuseIBL, specularIBL, normal);

		output.a = 1.0f;

		return output;
	}

}