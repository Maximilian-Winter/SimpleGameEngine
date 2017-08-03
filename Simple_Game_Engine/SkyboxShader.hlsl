//IBL Shader
#include "ForwardShaderCommon.hlsl"
TextureCube skybox						: register(t4);

struct SKYMAP_VS_OUTPUT	//output structure for skymap vertex shader
{
	float4 Pos : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

SKYMAP_VS_OUTPUT SkyboxVertexShader(VS_INPUT VertexShaderInput)
{
	SKYMAP_VS_OUTPUT output = (SKYMAP_VS_OUTPUT)0;

	//Set Pos to xyww instead of xyzw, so that z will always be 1 (furthest from camera)
	output.Pos = mul(float4(VertexShaderInput.Position.xyz, 1.0f), WorldViewProjection).xyww;

	output.texCoord = VertexShaderInput.Position.xyz;

	return output;
}

float4 SkyboxPixelShader(SKYMAP_VS_OUTPUT vertexShaderOut) : SV_TARGET0
{
	return float4(skybox.Sample(LinearSampler, vertexShaderOut.texCoord));
}