#include "ForwardShaderCommon.hlsl"


//////////////
// TYPEDEFS //
//////////////

struct PS_OUTPUT
{
	float4 AlbedoRough : SV_Target0;
	float4 Normal : SV_Target1;
	float4 F0Metal : SV_Target2;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
VS_OUTPUT FillGbufferVertexShader(VS_INPUT VertexShaderInput)
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


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
PS_OUTPUT PackGBuffer(Material material)
{

	PS_OUTPUT output;

	// Pack all the data into the GBuffer structure

	output.AlbedoRough = float4(material.diffuseColor.rgb, material.roughness);

	output.Normal = float4(material.normal.xyz * 0.5 + 0.5, 0.0);

	output.F0Metal = float4(material.f0, material.metalness, 0.0, 0.0);

	return output;

}

PS_OUTPUT FillGbufferPixelShader(VS_OUTPUT VertexShaderOutput) : SV_TARGET
{

	// Prepare the material structure
	Material material = PrepareMaterial(VertexShaderOutput);

	return PackGBuffer(material);
}

