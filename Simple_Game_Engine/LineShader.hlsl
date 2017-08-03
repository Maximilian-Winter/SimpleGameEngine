//Line Shader

#include "ForwardShaderCommon.hlsl"


struct LineShaderInput	//output structure for skymap vertex shader
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
};


struct LineShaderOutput	//output structure for skymap vertex shader
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

LineShaderOutput LineVertexShader(LineShaderInput VertexShaderInput)
{
	LineShaderOutput output = (LineShaderOutput)0;

	output.Pos = mul(float4(VertexShaderInput.Pos.xyz, 1.0f), WorldViewProjection).xyww;

	output.Pos.z = 0.01f;

	output.Color = VertexShaderInput.Color;

	return output;
}

float4 LinePixelShader(LineShaderOutput vertexShaderOut) : SV_TARGET0
{
	return vertexShaderOut.Color;
}